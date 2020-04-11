/*
 * MIT License
 *
 * Copyright(c) 2018 Pedro Henrique Penna <pedrohenriquepenna@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SK1BL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Must come fist. */
#define __NEED_HAL_TARGET
#define __NEED_RESOURCE

#include <nanvix/hal/target.h>
#include <nanvix/hal/resource.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/*============================================================================*
 * Definitions                                                                *
 *============================================================================*/

/**
 * @name Message Queue constants.
 */
/**@{*/
#define MQUEUE_MSG_SIZE   (sizeof(struct underliyng_message))         /**< Size of the underlying message.      */
#define MQUEUE_MIN_SIZE   (MQUEUE_MSG_SIZE)                           /**< Minimum size of the Message Queue.   */
#define MQUEUE_MAX_SIZE   (PROCESSOR_NOC_NODES_NUM * MQUEUE_MIN_SIZE) /**< Maximum size of the Message Queue.   */
#define MQUEUE_MSG_AMOUNT (MQUEUE_MAX_SIZE / MQUEUE_MIN_SIZE)         /**< Message amount in the Message Queue. */
/**@}*/

/**
 * @name Gets Mailbox table parameters.
 */
/**@{*/
#define RESOURCEID_RX(nodenum)       (nodenum % BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) /**< mbxid.rxs' index. */
#define MAILBOXID_RX(interface, tag) (interface)                                     /**< mbxid.rxs' index. */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define MAILBOX_CONTROL_TAG   (BOSTAN_MAILBOX_CNOC_TX_BASE) /**< Associated Control Tag. */
#define MAILBOX_DATA_TAG_BASE (BOSTAN_PORTAL_DNOC_TX_BASE)  /**< Associated Data Tag.    */

#define UNDERLYING_OPEN_INTERFACE(mbxid)   (mbxid / BOSTAN_MAILBOX_OPEN_PER_DMA) /**< Transfer interface.      */
#define UNDERLYING_OPEN_TAG(mbxid)         (BOSTAN_MAILBOX_DNOC_TX_BASE                  \
                                            + (mbxid % BOSTAN_DNOC_TXS_PER_COMM_SERVICE) \
                                           )                                     /**< Transfer tag.            */
#define UNDERLYING_CREATE_INTERFACE(mbxid) (mbxid)                               /**< Receiver tag ID.         */
#define UNDERLYING_CREATE_TAG(mbxid)       (bostan_processor_node_mailbox_tag(                             \
                                             bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + \
                                             UNDERLYING_CREATE_INTERFACE(mbxid)                            \
                                           ))                                    /**< Receiver DMA channel ID. */
/**@}*/

/**
 * @name Status of the underlying message.
 */
/**@{*/
#define MPPA256_MAILBOX_MESSAGE_INVALID (254)
#define MPPA256_MAILBOX_MESSAGE_VALID   (253)
/**@}*/

/**
 * @brief Underliyng message of the Mailbox
 */
struct underliyng_message
{
	int source;
	k1b_byte_t buffer[MPPA256_MAILBOX_MSG_SIZE];
	int confirm;
};

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

/**
 * @brief Mailbox Table
 */
PRIVATE struct mailbox
{
	/**
	 * @brief Mailbox receiver
	 */
	struct rx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;                              /**< Generic resource information. */

		/* Control parameters. */
		int ret;                                               /**< Returned value on a copy.     */
		k1b_byte_t initial_message;                            /**< Last position read.           */
		k1b_byte_t message_count;                              /**< Received messages.            */

		/* Receiver requisition on hold. */
		k1b_spinlock_t lock;                                   /**< Receiver request barrier.     */
		void * buffer;                                         /**< Receiver buffer.              */

		/* Underlying buffers. */
		struct underliyng_message messages[MQUEUE_MSG_AMOUNT]; /**< Message Queue.                */
	} ALIGN(sizeof(dword_t)) rxs[MPPA256_MAILBOX_CREATE_MAX];

	/**
	 * @brief Mailbox sender
	 */
	struct tx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;          /**< Generic resource information.                             */

		/* Control parameters */
		int ret;                           /**< Returned value on a send.                                 */
		k1b_byte_t source_ctag;            /**< Source Control Tag ID.                                    */
		k1b_byte_t remote;                 /**< Logical ID of the Target NoC Node.                        */
		k1b_byte_t ack                : 1; /**< Indicates that the receiver cosumes the previous message. */

		/* Sender requisition on hold. */
		k1b_byte_t commit             : 1; /**< Indicates whether it is need send the message.            */
		k1b_byte_t unused             : 6; /**< Unused.                                                   */
		k1b_spinlock_t lock;               /**< Receiver request barrier.                                 */
		struct underliyng_message message; /**< Underlying buffer for sending asynchronously.             */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_MAILBOX_OPEN_MAX];
} mbxtab = {
	.rxs[0 ... MPPA256_MAILBOX_CREATE_MAX - 1] = {
		.resource        = {0, },
		.ret             = -1,
		.initial_message = 0,
		.message_count   = 0,
		.lock            = K1B_SPINLOCK_UNLOCKED,
		.buffer          = NULL,
		.messages[0 ... MQUEUE_MSG_AMOUNT - 1] = {
			.source  = MPPA256_MAILBOX_MESSAGE_INVALID,
			.buffer  = {0, },
			.confirm = MPPA256_MAILBOX_MESSAGE_INVALID
		}
	},
	.txs[0 ... MPPA256_MAILBOX_OPEN_MAX - 1]   = {
		.resource    = {0, },
		.ret         = -1,
		.source_ctag = 0,
		.remote      = 0,
		.ack         = 0,
		.commit      = 0,
		.unused      = 0,
		.lock        = K1B_SPINLOCK_UNLOCKED,
		.message = {
			.source  = MPPA256_MAILBOX_MESSAGE_INVALID,
			.buffer  = {0, },
			.confirm = MPPA256_MAILBOX_MESSAGE_INVALID
		}
	}
};

/**
 * @brief Pools of Mailbox
 */
PRIVATE struct mbxpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} ALIGN(sizeof(dword_t)) mbxpools = {
	.rx_pool = {mbxtab.rxs, MPPA256_MAILBOX_CREATE_MAX, sizeof(struct rx)},
	.tx_pool = {mbxtab.txs, MPPA256_MAILBOX_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Global lock
 */
PRIVATE k1b_spinlock_t mbxtab_lock = K1B_SPINLOCK_UNLOCKED;

/*============================================================================*
 * mppa256_mailbox_lock()                                                     *
 *============================================================================*/

PRIVATE void mppa256_mailbox_lock(void)
{
	k1b_spinlock_lock(&mbxtab_lock);
}

/*============================================================================*
 * mppa256_mailbox_unlock()                                                   *
 *============================================================================*/


PRIVATE void mppa256_mailbox_unlock(void)
{
	k1b_spinlock_unlock(&mbxtab_lock);
}

/*============================================================================*
 * mppa256_mailbox_rx_handler()                                               *
 *============================================================================*/

/**
 * @brief Counts new incoming underlying messages.
 *
 * @param mqueue Message queue involved.
 */
PRIVATE void mppa256_mailbox_count_messages(struct rx * mqueue)
{
	k1b_byte_t total;                /* Count total of messages.   */
	struct underliyng_message * msg; /* Auxiliar message variable. */

	total = 0;

	for (unsigned i = 0; i < MQUEUE_MSG_AMOUNT; ++i)
	{
		msg = &mqueue->messages[i];

		if (msg->source == MPPA256_MAILBOX_MESSAGE_INVALID)
			continue;

		if (msg->source != msg->confirm)
		{
			if (msg->confirm == MPPA256_MAILBOX_MESSAGE_VALID)
				total++;

			continue;
		}

		total++;
		msg->confirm = MPPA256_MAILBOX_MESSAGE_VALID;
	}

	mqueue->message_count = total;
}

/**
 * @see mppa256_mailbox_msg_copy prototype.
 */
PRIVATE int mppa256_mailbox_msg_copy(int mbxid, void * buffer);

/**
 * @brief Mailbox Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_mailbox_rx_handler(int interface, int tag)
{
	int mbxid;     /* Mailbox ID.  */
	void * buffer; /* User buffer. */

	UNUSED(interface);
	UNUSED(tag);

	mbxid = MAILBOXID_RX(interface, tag);

	mppa256_mailbox_count_messages(&mbxtab.rxs[mbxid]);

	if (mbxtab.rxs[mbxid].buffer != NULL)
	{
		buffer = mbxtab.rxs[mbxid].buffer;

		mbxtab.rxs[mbxid].buffer = NULL;

		/* The failure does not cause kpanic because it is recoverable. */
		if (mppa256_mailbox_msg_copy(mbxid, buffer) != 0)
			kprintf("[hal][mailbox][handler] Handler failed on copy the message or send the ack to transfer node!");
	}
}

/*============================================================================*
 * mppa256_mailbox_tx_handler()                                               *
 *============================================================================*/

/**
 * @see mppa256_mailbox_send_msg prototype.
 */
PRIVATE int mppa256_mailbox_send_msg(int mbxid);

/**
 * @brief Mailbox Sender Handler.
 *
 * @param interface Number of the transfer interface.
 * @param tag       Number of the underlying transfer resource.
 */
PRIVATE void mppa256_mailbox_tx_handler(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	for (k1b_byte_t mbxid = 0; mbxid < MPPA256_MAILBOX_OPEN_MAX; ++mbxid)
	{
		/**
		 * This verification cannot be protected by the global lock
		 * because the handler can interrupt the master core when
		 * it's inside a critical region. That's create a deadlock
		 * situation.
		 */
		if (!resource_is_used(&mbxtab.txs[mbxid].resource))
			continue;

		if (mbxtab.txs[mbxid].source_ctag != tag)
			continue;

		mbxtab.txs[mbxid].ack = 1;

		/* Sends requested message. */
		if (mbxtab.txs[mbxid].commit != 0)
		{
			mbxtab.txs[mbxid].ack    = 0;
			mbxtab.txs[mbxid].commit = 0;

			if (mppa256_mailbox_send_msg(mbxid) != 0)
			{
				mbxtab.txs[mbxid].ack = 1;

				/* The failure does not cause kpanic because it is recoverable. */
				kprintf("[hal][mailbox][handler] Sender Handler failed!");
			}
		}

		break;
	}
}

/*============================================================================*
 * mppa256_mailbox_create()                                                   *
 *============================================================================*/

/**
 * @brief Creates a mailbox.
 *
 * @param nodenum Logic ID of the underlying NoC node.
 *
 * @returns Upon successful completion, the ID of the newly created
 * mailbox is returned. Upon failure, a negative error code is
 * returned instead.
 */
PRIVATE int do_mppa256_mailbox_create(int nodenum)
{
	int ret;       /* Return value.         */
	int tag;       /* Underlying data tag.  */
	int mbxid;     /* Mailbox ID.           */
	int target;    /* Target nodenum.       */
	int interface; /* Underlying interface. */

	/* Gets Mailbox index. */
	mbxid = RESOURCEID_RX(nodenum);

	target = -1;
	ret = (-EBADF);

	mppa256_mailbox_lock();

	/* Mailbox is used. */
	if (resource_is_used(&mbxtab.rxs[mbxid].resource))
		goto release_global_lock;

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	ret = (-EINVAL);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, MAILBOX_CONTROL_TAG) != 0)
		goto release_global_lock;

	/* Configures underlying message queue. */
	for (unsigned i = 0; i < MQUEUE_MSG_AMOUNT; i++)
	{
		mbxtab.rxs[mbxid].messages[i].source  = MPPA256_MAILBOX_MESSAGE_INVALID;
		mbxtab.rxs[mbxid].messages[i].confirm = MPPA256_MAILBOX_MESSAGE_INVALID;
	}

	mbxtab.rxs[mbxid].initial_message = 0;
	mbxtab.rxs[mbxid].message_count   = 0;
	mbxtab.rxs[mbxid].ret             = (-EAGAIN);
	k1b_spinlock_trylock(&mbxtab.rxs[mbxid].lock);

	/* Configures a message slot for each node available. */
	for (target = 0; target < ((int) PROCESSOR_NOC_NODES_NUM); target++)
	{
		tag = bostan_processor_node_mailbox_tag(target);

		/* Creates data reciever point. */
		if (bostan_dma_data_create(interface, tag) != 0)
		{
			/* We were unable to create for this target, ignore it. */
			target--;
			goto release_dma_resources;
		}

		ret = bostan_dma_data_aread(
			interface,
			tag,
			&mbxtab.rxs[mbxid].messages[target],
			MQUEUE_MIN_SIZE,
			MQUEUE_MIN_SIZE,
			0,
			mppa256_mailbox_rx_handler
		);

		if (ret < 0)
			goto release_dma_resources;
	}

	/* Allocates associated resource. */
	resource_set_used(&mbxtab.rxs[mbxid].resource);
	resource_set_notbusy(&mbxtab.rxs[mbxid].resource);

	mppa256_mailbox_unlock();

	return (MPPA256_MAILBOX_CREATE_OFFSET + mbxid);

release_dma_resources:
	if (bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG) != 0)
		kpanic("[hal][mailbox][create] Failed on releasing control dma emitter.");

	for (target--; target >= 0; target--)
	{
		tag = bostan_processor_node_mailbox_tag(target);

		if (bostan_dma_data_unlink(interface, tag) != 0)
			kpanic("[hal][mailbox][create] Failed on releasing data dma receiver.");
	}

	ret = (-EINVAL);

release_global_lock:
	mppa256_mailbox_unlock();

	return (ret);

}

/**
 * @see do_mppa256_mailbox_create().
 */
PUBLIC int mppa256_mailbox_create(int nodenum)
{
	return (do_mppa256_mailbox_create(nodenum));
}

/*============================================================================*
 * mppa256_mailbox_open()                                                     *
 *============================================================================*/

/**
 * @brief Opens a mailbox.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns Upon successful completion, the ID of the target mailbox
 * is returned. Upon failure, a negative error code is returned
 * instead.
 */
PRIVATE int do_mppa256_mailbox_open(int nodenum)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int mbxid;     /* Mailbox ID.             */
	int localnum;  /* Local nodenum.          */
	int interface; /* Underlying interface.   */

	mppa256_mailbox_lock();

	ret = (-EINVAL);

	/* Allocates a resource. */
	if ((mbxid = resource_alloc(&mbxpools.tx_pool)) < 0)
		goto error;

	/* Control arguments. */
	ctag      = bostan_processor_node_mailbox_tag(nodenum);
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	localnum = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	/* Creates control reciever point. */
	if (bostan_dma_control_create(interface, ctag, (1), mppa256_mailbox_tx_handler) != 0)
	{
		resource_free(&mbxpools.tx_pool, mbxid);
		goto error;
	}

	/* Configures associated resource. */
	mbxtab.rxs[mbxid].ret             = (-EAGAIN);
	mbxtab.txs[mbxid].commit          = 0;
	mbxtab.txs[mbxid].ack             = 1;
	mbxtab.txs[mbxid].source_ctag     = ctag;
	mbxtab.txs[mbxid].remote          = nodenum;
	mbxtab.txs[mbxid].message.source  = localnum;
	mbxtab.txs[mbxid].message.confirm = localnum;
	k1b_spinlock_trylock(&mbxtab.txs[mbxid].lock);

	resource_set_notbusy(&mbxtab.txs[mbxid].resource);

	ret = (MPPA256_MAILBOX_OPEN_OFFSET + mbxid);

error:
	mppa256_mailbox_unlock();

	return (ret);
}

/**
 * @see do_mppa256_mailbox_open().
 */
PUBLIC int mppa256_mailbox_open(int nodenum)
{
	return (do_mppa256_mailbox_open(nodenum));
}

/*============================================================================*
 * mppa256_mailbox_unlink()                                                   *
 *============================================================================*/

/**
 * @brief Destroys a mailbox.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns Upon successful completion, zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
PRIVATE int do_mppa256_mailbox_unlink(int mbxid)
{
	int ret;       /* Return value.         */
	int tag;       /* Underlying data tag.  */
	int interface; /* Underlying interface. */

	mppa256_mailbox_lock();

	ret = (-EBADF);

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		goto error;

	ret = (-EBUSY);

	/* Bad mailbox. */
	if (resource_is_busy(&mbxtab.rxs[mbxid].resource))
		goto error;

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	ret = (-EINVAL);

	/* Close control sender point. */
	if (bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG) != 0)
		goto error;

	/* Configures a message slot for each node available. */
	for (unsigned int target = 0; target < PROCESSOR_NOC_NODES_NUM; target++)
	{
		tag = bostan_processor_node_mailbox_tag(target);

		/* Unlink data reciever point. */
		if (bostan_dma_data_unlink(interface, tag) != 0)
			kpanic("[hal][mailbox][unlink] Failed on closing a mailbox.");
	}

	/* Releases associated resource. */
	resource_free(&mbxpools.rx_pool, mbxid);
	mbxtab.rxs[mbxid].buffer = NULL;

	ret = 0;

error:
	mppa256_mailbox_unlock();

	return (ret);
}

/**
 * @see do_mppa256_mailbox_unlink().
 */
PUBLIC int mppa256_mailbox_unlink(int mbxid)
{
	mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

	return (do_mppa256_mailbox_unlink(mbxid));
}

/*============================================================================*
 * mppa256_mailbox_close()                                                    *
 *============================================================================*/

/**
 * @brief Closes a mailbox.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE int do_mppa256_mailbox_close(int mbxid)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int interface; /* Underlying interface.   */

	mppa256_mailbox_lock();

	ret = (-EBADF);

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		goto error;

	/* Data parameters. */
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	/* Control parameters. */
	ctag = bostan_processor_node_mailbox_tag(mbxtab.txs[mbxid].remote);

	/* Unlink control reciever point. */
	if (bostan_dma_control_unlink(interface, ctag) == 0)
	{
		/* Releases associated resource. */
		resource_free(&mbxpools.tx_pool, mbxid);
		mbxtab.txs[mbxid].commit = 0;
		mbxtab.txs[mbxid].ack    = 1;

		ret = 0;
	}
	else
		ret = (-EINVAL);

error:
	mppa256_mailbox_unlock();

	return (ret);
}

/**
 * @see do_mppa256_mailbox_close().
 */
PUBLIC int mppa256_mailbox_close(int mbxid)
{
	mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

	return (do_mppa256_mailbox_close(mbxid));
}

/*============================================================================*
 * mppa256_mailbox_send_msg()                                                 *
 *============================================================================*/

/**
 * @brief Sends the underliyng message.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns Upon successful completion, 0 is retuned
 * and non zero oterwise.
 */
PRIVATE int mppa256_mailbox_send_msg(int mbxid)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int dtag;      /* Underlying data tag.    */
	int localnum;  /* Local nodenum.          */
	int remotenum; /* Remote nodenum.         */
	int interface; /* Underlying interface.   */

	/* Defines parameters. */
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);
	remotenum = mbxtab.txs[mbxid].remote;
	ctag      = bostan_processor_node_mailbox_tag(remotenum);
	localnum  = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	/* Opens data sender point. */
	dtag = UNDERLYING_OPEN_TAG(mbxid);

	ret = (-EBUSY);

	/* Tries to open DMA Data Channel. */
	if (bostan_dma_data_open(interface, dtag) != 0)
		goto error;

	ret = (-EAGAIN);

	if (bostan_dma_control_config(interface, ctag, (1), mppa256_mailbox_tx_handler) != 0)
		goto error;

	/* Send message. */
	ret = bostan_dma_data_write(
		interface,
		dtag,
		remotenum,
		bostan_processor_node_mailbox_tag(localnum),
		&mbxtab.txs[mbxid].message,
		MQUEUE_MSG_SIZE,
		0
	);

error:
	/* Close data sender point. */
	if (bostan_dma_data_close(interface, dtag) != 0)
		kpanic("[hal][mailbox][send] Failed to close the data transmission channel.");

	mbxtab.txs[mbxid].ret = (ret < 0) ? (-EAGAIN) : 0;

	k1b_spinlock_unlock(&mbxtab.txs[mbxid].lock);

	return (ret);
}

/*============================================================================*
 * mppa256_mailbox_awrite()                                                   *
 *============================================================================*/

/**
 * @brief Writes data to a mailbox.
 *
 * @param mbxid  ID of the target mailbox.
 * @param buffer Buffer where the data should be read from.
 * @param size   Number of bytes to write.
 *
 * @returns Upon successful completion, 0 is retuned
 * and non zero oterwise.
 */
PRIVATE ssize_t do_mppa256_mailbox_awrite(int mbxid, const void * buffer, uint64_t size)
{
	ssize_t ret; /* Return value. */

	ret = size;

	mppa256_mailbox_lock();
		resource_set_busy(&mbxtab.txs[mbxid].resource);
	mppa256_mailbox_unlock();

	interrupt_mask(K1B_INT_CNOC);

		/* Programs the next write. */
		kmemcpy(&mbxtab.txs[mbxid].message.buffer, buffer, MPPA256_MAILBOX_MSG_SIZE);
		mbxtab.txs[mbxid].commit = 1;

		/**
		* Do we not receive the acknowledge signal from the receiver yet?
		* Then don't send the message.
		*/
		if (mbxtab.txs[mbxid].ack != 0)
		{
			mbxtab.txs[mbxid].ack = 0;
			mbxtab.txs[mbxid].commit = 0;

			/* Sends the message and configurates the control receiver buffet. */
			if (mppa256_mailbox_send_msg(mbxid) != 0)
			{
				mbxtab.txs[mbxid].ack = 1;
				ret = -EAGAIN;
			}
		}

		/* Double check. */
		bostan_cnoc_it_verify();

	interrupt_unmask(K1B_INT_CNOC);

	if (ret < 0)
	{
		mppa256_mailbox_lock();
			resource_set_notbusy(&mbxtab.txs[mbxid].resource);
		mppa256_mailbox_unlock();
	}

	return (ret);
}

/**
 * @see do_mppa256_mailbox_awrite().
 */
PUBLIC ssize_t mppa256_mailbox_awrite(int mbxid, const void * buffer, uint64_t size)
{
	mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

	mppa256_mailbox_lock();

		/* Bad mailbox. */
		if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		{
			mppa256_mailbox_unlock();
			return (-EBADF);
		}

		/* Busy mailbox */
		if (resource_is_busy(&mbxtab.txs[mbxid].resource))
		{
			mppa256_mailbox_unlock();
			return (-EBUSY);
		}

	mppa256_mailbox_unlock();

	return (do_mppa256_mailbox_awrite(mbxid, buffer, size));
}

/*============================================================================*
 * mppa256_mailbox_msg_copy()                                                 *
 *============================================================================*/

/**
 * @brief Copy the incoming message to the buffer and emit
 * the ack signal to the sender.
 *
 * @param mbxid Resource ID.
 *
 * @return Zero if copy and sends ack correctly and non zero otherwise.
 */
PRIVATE int mppa256_mailbox_msg_copy(int mbxid, void * buffer)
{
	int ret;                         /* Return value.             */
	int tag;                         /* Underlying control tag.   */
	int interface;                   /* Underlying interface.     */
	int remotenum;                   /* Remote nodenum.           */
	struct rx * mqueue;              /* Auxiliar mqueue variable. */
	struct underliyng_message * msg; /* Auxiliar msg variable.    */

	/* Current mailbox. */
	mqueue = &mbxtab.rxs[mbxid];

	ret = (-EAGAIN);

	/* Is the buffer valid or mqueue doesn't have a message? */
	if((buffer == NULL) || (mqueue->message_count == 0))
		goto error;

	/* While not find a valid message. */
	do
	{
		/* Updates message queue. */
		dcache_invalidate();

		/* Searches for a received message. */
		remotenum = mqueue->initial_message;
		for (unsigned i = 0; i < MQUEUE_MSG_AMOUNT; ++i)
		{
			msg = &mqueue->messages[remotenum];

			if (msg->source != MPPA256_MAILBOX_MESSAGE_INVALID && msg->confirm == MPPA256_MAILBOX_MESSAGE_VALID)
				break;

			remotenum = (remotenum + 1) % MQUEUE_MSG_AMOUNT;
		}
	} while (msg->confirm != MPPA256_MAILBOX_MESSAGE_VALID);

	/* Cleans underlying message. */
	remotenum = msg->source;

	/* Assert that remotenum is a valid node. */
	if (remotenum == MPPA256_MAILBOX_MESSAGE_INVALID)
		kpanic("[hal][mailbox][copy] The chosen message is invalid.");

	/* Take the message off the queue. */
	msg->source  = MPPA256_MAILBOX_MESSAGE_INVALID;
	msg->confirm = MPPA256_MAILBOX_MESSAGE_INVALID;

	/* Copies message data. */
	kmemcpy(buffer, msg->buffer, MPPA256_MAILBOX_MSG_SIZE);

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);
	tag = bostan_processor_node_mailbox_tag(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	/* Confirms receiving with message source. */
	ret = bostan_dma_control_signal(
		interface,
		MAILBOX_CONTROL_TAG,
		&remotenum,
		1,
		tag,
		(~0)
	);

	/* Wasn't there an error sending the signal? */
	if (ret < 0)
	{
		/* Return the message to the queue. */
		msg->source  = remotenum;
		msg->confirm = MPPA256_MAILBOX_MESSAGE_VALID;

		ret = (-EAGAIN);
	}
	else
	{
		/* Updates message queue parameters. */
		mqueue->message_count--;
		mqueue->initial_message = ((msg - mqueue->messages) + 1) % MQUEUE_MSG_AMOUNT;

		ret = 0;
	}

error:
	/* Releases to be read again. */
	mbxtab.rxs[mbxid].ret = ret;

	k1b_spinlock_unlock(&mbxtab.rxs[mbxid].lock);

	return (ret);
}

/*============================================================================*
 * mppa256_mailbox_aread()                                                    *
 *============================================================================*/

/**
 * @brief Reads data from a mailbox.
 *
 * @param mbxid  ID of the target mailbox.
 * @param buffer Buffer where the data should be written to.
 * @param size   Number of bytes to read.
 *
 * @returns Upon successful completion, 0 is retuned
 * and non zero oterwise.
 */
PRIVATE ssize_t do_mppa256_mailbox_aread(int mbxid, void * buffer, uint64_t size)
{
	ssize_t ret; /* Return value. */

	ret = size;

	mppa256_mailbox_lock();
		resource_set_busy(&mbxtab.rxs[mbxid].resource);
	mppa256_mailbox_unlock();

	interrupt_mask(K1B_INT_DNOC);

		/* Check on underlying messages. */
		mppa256_mailbox_count_messages(&mbxtab.rxs[mbxid]);

		/* Is the message queue not empty? */
		if (mbxtab.rxs[mbxid].message_count > 0)
		{
			if (mppa256_mailbox_msg_copy(mbxid, buffer) != 0)
				ret = (-EAGAIN);
		}
		else
			ret = (-ENOMSG);

		/* Double check. */
		bostan_dnoc_it_verify();

	interrupt_unmask(K1B_INT_DNOC);

	if (ret < 0)
	{
		mppa256_mailbox_lock();
			resource_set_notbusy(&mbxtab.rxs[mbxid].resource);
		mppa256_mailbox_unlock();
	}

	return (ret);
}

/**
 * @see do_mppa256_mailbox_aread().
 */
PUBLIC ssize_t mppa256_mailbox_aread(int mbxid, void * buffer, uint64_t size)
{
	mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

	mppa256_mailbox_lock();

		/* Bad mailbox. */
		if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		{
			mppa256_mailbox_unlock();
			return (-EBADF);
		}

		/* Busy mailbox. */
		if (resource_is_busy(&mbxtab.rxs[mbxid].resource))
		{
			mppa256_mailbox_unlock();
			return (-EBUSY);
		}

	mppa256_mailbox_unlock();

	return (do_mppa256_mailbox_aread(mbxid, buffer, size));
}

/*============================================================================*
 * mppa256_mailbox_wait()                                                     *
 *============================================================================*/

/**
 * @brief Waits asynchronous operation.
 *
 * @param mbxid ID of the Target Mailbox.
 *
 * @return Zero if wait read correctly and non zero otherwise.
 */
PUBLIC int mppa256_mailbox_wait(int mbxid)
{
	int ret; /* Return value. */

	/* Is it a rx operation? */
	if (mbxid < MPPA256_MAILBOX_OPEN_OFFSET)
	{
		mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

		mppa256_mailbox_lock();

			/* Bad sync. */
			if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
				goto error;

			/* Bad sync. */
			if (!resource_is_busy(&mbxtab.rxs[mbxid].resource))
				goto error;

		mppa256_mailbox_unlock();

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&mbxtab.rxs[mbxid].lock);

		/**
		 * The attribution of the return value is allowed on the slave
		 * side because it will be protected by the busy flag release
		 * by the own slave core.
		 **/
		ret = mbxtab.rxs[mbxid].ret;
		mbxtab.rxs[mbxid].ret = (-EAGAIN);

		mppa256_mailbox_lock();
			resource_set_notbusy(&mbxtab.rxs[mbxid].resource);
		mppa256_mailbox_unlock();
	}

	/* Is it a tx operation? */
	else
	{
		mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

		mppa256_mailbox_lock();

			/* Bad sync. */
			if (!resource_is_used(&mbxtab.txs[mbxid].resource))
				goto error;

			/* Bad sync. */
			if (!resource_is_busy(&mbxtab.txs[mbxid].resource))
				goto error;

		mppa256_mailbox_unlock();

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&mbxtab.txs[mbxid].lock);

		/**
		 * The attribution of the return value is allowed on the slave
		 * side because it will be protected by the busy flag release
		 * by the own slave core.
		 **/
		ret = mbxtab.txs[mbxid].ret;
		mbxtab.txs[mbxid].ret = (-EAGAIN);

		mppa256_mailbox_lock();
			resource_set_notbusy(&mbxtab.txs[mbxid].resource);
		mppa256_mailbox_unlock();
	}

	return (ret);

error:
	mppa256_mailbox_unlock();

	return (-EBADF);
}
