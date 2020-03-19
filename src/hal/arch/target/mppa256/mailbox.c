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
#define MAILBOXID_RX(interface, tag) (interface)                                /**< mbxid.rxs' index. */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define MAILBOX_CONTROL_TAG   (BOSTAN_MAILBOX_CNOC_TX_BASE) /**< Associated Control Tag. */
#define MAILBOX_DATA_TAG_BASE (BOSTAN_PORTAL_DNOC_TX_BASE)  /**< Associated Data Tag.    */

#define UNDERLYING_OPEN_INTERFACE(mbxid)   (mbxid / BOSTAN_MAILBOX_OPEN_PER_DMA) /**< Transfer interface.      */
#define UNDERLYING_OPEN_TAG(mbxid)         (BOSTAN_MAILBOX_DNOC_TX_BASE                                \
                                              + (mbxid % BOSTAN_DNOC_TXS_PER_COMM_SERVICE) \
                                           )                                     /**< Transfer tag.            */
#define UNDERLYING_CREATE_INTERFACE(mbxid) (mbxid)                               /**< Receiver tag ID.         */
#define UNDERLYING_CREATE_TAG(mbxid)       (bostan_processor_node_mailbox_tag(                         \
                                                bostan_processor_noc_cluster_to_node_num(cluster_get_num()) +  \
												UNDERLYING_CREATE_INTERFACE(mbxid)                     \
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
		struct resource resource;                              /**< Control flags.            */

		/* Control parameters */
		struct underliyng_message messages[MQUEUE_MSG_AMOUNT]; /**< Message Queue.            */
		k1b_byte_t initial_message;                            /**< Last position read.       */
		k1b_byte_t message_count;                              /**< Received messages.        */

		/* Receiver requisition on hold. */
		k1b_spinlock_t lock;                                   /**< Receiver request barrier. */
		void * buffer;                                         /**< Receiver buffer.          */
	} ALIGN(sizeof(dword_t)) rxs[MPPA256_MAILBOX_CREATE_MAX];

	/**
	 * @brief Mailbox sender
	 */
	struct tx
	{
		struct resource resource;          /**< Control flags.                                            */

		/* Control parameters */
		k1b_byte_t source_ctag;            /**< Source Control Tag ID.                                    */
		k1b_byte_t remote;                 /**< Logical ID of the Target NoC Node.                        */
		k1b_byte_t is_first_msg : 1;       /**< Indicates whether it is the first message.                */
		k1b_byte_t ack          : 1;       /**< Indicates that the receiver cosumes the previous message. */

		/* Sender requisition on hold. */
		k1b_byte_t commit       : 1;       /**< Indicates whether it is need send the message.            */
		k1b_byte_t unused       : 5;       /**< Unused.                                                   */
		k1b_spinlock_t lock;               /**< Receiver request barrier.                                 */
		struct underliyng_message message; /**< Underlying buffer for sending asynchronously.             */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_MAILBOX_OPEN_MAX];
} mbxtab = {
	.rxs[0 ... MPPA256_MAILBOX_CREATE_MAX-1] = {
		.resource = {0},
		.messages[0 ... MQUEUE_MSG_AMOUNT-1] = {
			.source  = MPPA256_MAILBOX_MESSAGE_INVALID,
			.buffer = {0, },
			.confirm = MPPA256_MAILBOX_MESSAGE_INVALID
		},
		.initial_message = 0,
		.message_count   = 0,
		.lock            = K1B_SPINLOCK_UNLOCKED,
		.buffer          = NULL
	},
	.txs[0 ... MPPA256_MAILBOX_OPEN_MAX-1]   = { {0}, 0, 0, 0, 0, 0, 0, K1B_SPINLOCK_UNLOCKED, {MPPA256_MAILBOX_MESSAGE_INVALID, {0}, MPPA256_MAILBOX_MESSAGE_INVALID} }
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

/*============================================================================*
 * mppa256_mailbox_node_is_local()                                            *
 *============================================================================*/

/**
 * @brief Assess if the nodenum in on local cluster.
 *
 * @param nodenum Logic ID of a NoC node.
 *
 * @return No zero if nodenum is on local cluster and zero otherwise.
 */
PRIVATE int mppa256_mailbox_node_is_local(int nodenum)
{
	int local;

	local = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	return (WITHIN(nodenum, local, local + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM));
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
	k1b_byte_t total;
	struct underliyng_message * msg;

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
	int mbxid;
	void * buffer;

	UNUSED(interface);
	UNUSED(tag);

	mbxid = MAILBOXID_RX(interface, tag);

	mppa256_mailbox_count_messages(&mbxtab.rxs[mbxid]);

	if (mbxtab.rxs[mbxid].buffer != NULL)
	{
		buffer = mbxtab.rxs[mbxid].buffer;

		mbxtab.rxs[mbxid].buffer = NULL;

		if (mppa256_mailbox_msg_copy(mbxid, buffer) != 0)
			kpanic("[hal][mailbox] Handler failed on copy the message or send the ack to transfer node!");
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
		if (!resource_is_used(&mbxtab.txs[mbxid].resource))
			continue;

		if (mbxtab.txs[mbxid].source_ctag != tag)
			continue;

		mbxtab.txs[mbxid].ack = 1;

		/* Sends requested message. */
		if (mbxtab.txs[mbxid].commit != 0)
		{
			if (mppa256_mailbox_send_msg(mbxid) != 0)
				kpanic("[hal][target][mailbox] Sender Handler failed!");
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
	int ret;
	int tag;
	int mbxid;
	int interface;

	/* Gets Mailbox index. */
	mbxid = RESOURCEID_RX(nodenum);

	/* Mailbox is used. */
	if (resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EBADF);

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, MAILBOX_CONTROL_TAG) != 0)
		return (-EINVAL);

	/* Configures underlying message queue. */
	for (unsigned i = 0; i < MQUEUE_MSG_AMOUNT; i++)
	{
		mbxtab.rxs[mbxid].messages[i].source  = MPPA256_MAILBOX_MESSAGE_INVALID;
		mbxtab.rxs[mbxid].messages[i].confirm = MPPA256_MAILBOX_MESSAGE_INVALID;
	}

	mbxtab.rxs[mbxid].initial_message = 0;
	mbxtab.rxs[mbxid].message_count   = 0;
	spinlock_lock(&mbxtab.rxs[mbxid].lock);

	/* Configures a message slot for each node available. */
	for (int target = 0; target < ((int) PROCESSOR_NOC_NODES_NUM); target++)
	{
		tag = bostan_processor_node_mailbox_tag(target);

		/* Creates data reciever point. */
		if (bostan_dma_data_create(interface, tag) != 0)
		{
			bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG);

			for (target--; target >= 0; target--)
			{
				tag = bostan_processor_node_mailbox_tag(target);
				bostan_dma_control_unlink(interface, tag);
			}

			return (-EINVAL);
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
		{
			bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG);

			for (target--; target >= 0; target--)
			{
				tag = bostan_processor_node_mailbox_tag(target);
				bostan_dma_control_unlink(interface, tag);
			}

			return (ret);
		}
	}

	/* Allocates associated resource. */
	resource_set_used(&mbxtab.rxs[mbxid].resource);
	resource_set_notbusy(&mbxtab.rxs[mbxid].resource);

	return (MPPA256_MAILBOX_CREATE_OFFSET + mbxid);
}

/**
 * @see do_mppa256_mailbox_create().
 */
PUBLIC int mppa256_mailbox_create(int nodenum)
{
	/* Invalid NoC node is local. */
	if (!mppa256_mailbox_node_is_local(nodenum))
		return (-EINVAL);

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
	int ctag;
	int localnum;
	int mbxid;
	int interface;

	/* Allocates a resource. */
	if ((mbxid = resource_alloc(&mbxpools.tx_pool)) < 0)
		return (-EINVAL);

	/* Control arguments. */
	ctag      = bostan_processor_node_mailbox_tag(nodenum);
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	localnum = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	/* Creates control reciever point. */
	if (bostan_dma_control_create(interface, ctag, (1), mppa256_mailbox_tx_handler) != 0)
		return (-EINVAL);

	/* Configures associated resource. */
	mbxtab.txs[mbxid].commit          = 0;
	mbxtab.txs[mbxid].ack             = 1;
	mbxtab.txs[mbxid].is_first_msg    = 1;
	mbxtab.txs[mbxid].source_ctag     = ctag;
	mbxtab.txs[mbxid].remote          = nodenum;
	mbxtab.txs[mbxid].message.source  = localnum;
	mbxtab.txs[mbxid].message.confirm = localnum;
	spinlock_lock(&mbxtab.txs[mbxid].lock);

	resource_set_notbusy(&mbxtab.txs[mbxid].resource);

	return (MPPA256_MAILBOX_OPEN_OFFSET + mbxid);
}

/**
 * @see do_mppa256_mailbox_open().
 */
PUBLIC int mppa256_mailbox_open(int nodenum)
{
	/* Is remote in the local cluster? */
	if (mppa256_mailbox_node_is_local(nodenum))
	{
		/* Is it a compute cluster? */
		if (cluster_is_ccluster(cluster_get_num()))
			return (-EINVAL);

		/* Explanation (IO cluster):
		 * It is allowed to open locally in the IO cluster because
		 * it is as if it would be sent to a neighboring node (core).
		 */
	}

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
	int tag;
	int interface;

	mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EBADF);

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Close control sender point. */
	if (bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG) != 0)
		return (-EINVAL);

	/* Configures a message slot for each node available. */
	for (unsigned int target = 0; target < PROCESSOR_NOC_NODES_NUM; target++)
	{
		tag = bostan_processor_node_mailbox_tag(target);

		/* Unlink data reciever point. */
		if (bostan_dma_data_unlink(interface, tag) != 0)
			return (-EINVAL);
	}

	/* Releases associated resource. */
	resource_free(&mbxpools.rx_pool, mbxid);
	mbxtab.rxs[mbxid].buffer = NULL;
	spinlock_unlock(&mbxtab.rxs[mbxid].lock);

	return (0);
}

/**
 * @see do_mppa256_mailbox_unlink().
 */
PUBLIC int mppa256_mailbox_unlink(int mbxid)
{
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
	int ctag;
	int interface;

	/* Data parameters. */
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	/* Control parameters. */
	ctag = bostan_processor_node_mailbox_tag(mbxtab.txs[mbxid].remote);

	/* Unlink control reciever point. */
	if (bostan_dma_control_unlink(interface, ctag) != 0)
		return (-EINVAL);

	/* Releases associated resource. */
	resource_free(&mbxpools.tx_pool, mbxid);
	mbxtab.txs[mbxid].commit = 0;
	mbxtab.txs[mbxid].ack    = 1;
	spinlock_unlock(&mbxtab.txs[mbxid].lock);

	return (0);
}

/**
 * @see do_mppa256_mailbox_close().
 */
PUBLIC int mppa256_mailbox_close(int mbxid)
{
	mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		return (-EBADF);

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
	int ret;
	int ctag;
	int dtag;
	int remotenum;
	int localnum;
	int interface;

	/* Defines parameters. */
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);
	remotenum = mbxtab.txs[mbxid].remote;
	ctag      = bostan_processor_node_mailbox_tag(remotenum);
	localnum  = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	/* Opens data sender point. */
	dtag = MAILBOX_DATA_TAG_BASE;
	ret = -1;

	/* Try to find not busy dtag. */
	for (int i = 0; i < BOSTAN_DNOC_TXS_PER_COMM_SERVICE; ++i)
	{
		/* Tries to open DMA Data Channel. */
		if ((ret = bostan_dma_data_open(interface, dtag)) != -EBUSY)
			break;

		dtag++;
	}

	/* Checks if succesfully allocated a DTAG. */
	if (ret != 0)
		return (-EAGAIN);

	if (bostan_dma_control_config(interface, ctag, (1), mppa256_mailbox_tx_handler) < 0)
		return (-EINVAL);

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

	/* Close data sender point. */
	if (bostan_dma_data_close(interface, dtag) != 0)
		return (-EINVAL);

	if (ret != 0)
		return (ret);

	mbxtab.txs[mbxid].commit = 0;
	mbxtab.txs[mbxid].ack    = 0;
	resource_set_notbusy(&mbxtab.txs[mbxid].resource);

	spinlock_unlock(&mbxtab.txs[mbxid].lock);

	return (0);
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
	dcache_invalidate();

	/* Programs the next write. */
	kmemcpy(&mbxtab.txs[mbxid].message.buffer, buffer, MPPA256_MAILBOX_MSG_SIZE);
	resource_set_busy(&mbxtab.txs[mbxid].resource);
	mbxtab.txs[mbxid].commit = 1;

	/* Is this the first message? Then we send the message. */
	if (mbxtab.txs[mbxid].is_first_msg)
		mbxtab.txs[mbxid].is_first_msg = 0;

	/**
	 * Do we not receive the acknowledge signal from the receiver yet?
	 * Then don't send the message.
	 */
	else if (mbxtab.txs[mbxid].ack == 0)
		return (size);

	/* Sends the message and configurates the control receiver buffet. */
	if (mppa256_mailbox_send_msg(mbxid) != 0)
		return (-EAGAIN);

	return (size);
}

/**
 * @see do_mppa256_mailbox_awrite().
 */
PUBLIC ssize_t mppa256_mailbox_awrite(int mbxid, const void * buffer, uint64_t size)
{
	mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		return (-EBADF);

	/* Busy mailbox */
	if (resource_is_busy(&mbxtab.txs[mbxid].resource))
		return (-EBUSY);

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
	int ret;
	int tag;
	int interface;
	int remotenum;
	struct rx * mqueue;
	struct underliyng_message * msg;

	/* Current mailbox. */
	mqueue = &mbxtab.rxs[mbxid];

	/* Is the buffer valid or mqueue doesn't have a message? */
	if((buffer == NULL) || (mqueue->message_count == 0))
		return (-EINVAL);

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

			remotenum = (remotenum + 1) < ((int) MQUEUE_MSG_AMOUNT) ? (remotenum + 1) : 0;
		}
	} while (msg->confirm != MPPA256_MAILBOX_MESSAGE_VALID);

	/* Cleans underlying message. */
	remotenum    = msg->source;
	msg->source  = MPPA256_MAILBOX_MESSAGE_INVALID;
	msg->confirm = MPPA256_MAILBOX_MESSAGE_INVALID;

	/* Updates message queue parameters. */
	--mqueue->message_count;
	mqueue->initial_message = (msg - mqueue->messages);

	KASSERT(remotenum != MPPA256_MAILBOX_MESSAGE_INVALID);

	/* Copies message data. */
	kmemcpy(buffer, msg->buffer, MPPA256_MAILBOX_MSG_SIZE);

	/* Releases to be read again. */
	resource_set_notbusy(&mbxtab.rxs[mbxid].resource);

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Confirms receiving with message source. */
	tag = bostan_processor_node_mailbox_tag(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	ret = bostan_dma_control_signal(
		interface,
		MAILBOX_CONTROL_TAG,
		&remotenum,
		1,
		tag,
		(~0)
	);

	if (ret < 0)
		return (-EINVAL);

	/* Updates message queue. */
	dcache_invalidate();

	/* Releases reads. */
	spinlock_unlock(&mbxtab.rxs[mbxid].lock);

	return (0);
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
	resource_set_busy(&mbxtab.rxs[mbxid].resource);

	interrupt_mask(K1B_INT_DNOC);

		dcache_invalidate();

		/* Check on underlying messages. */
		mppa256_mailbox_count_messages(&mbxtab.rxs[mbxid]);

		/* Is the message queue not empty? */
		if (mbxtab.rxs[mbxid].message_count > 0)
		{
			mbxtab.rxs[mbxid].buffer = NULL;

			if (mppa256_mailbox_msg_copy(mbxid, buffer) != 0)
				return (-EAGAIN);
		}
		else
			mbxtab.rxs[mbxid].buffer = buffer;

		/* Double check. */
		bostan_dnoc_it_verify();

	interrupt_unmask(K1B_INT_DNOC);

	return (size);
}

/**
 * @see do_mppa256_mailbox_aread().
 */
PUBLIC ssize_t mppa256_mailbox_aread(int mbxid, void * buffer, uint64_t size)
{
	mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EBADF);

	/* Busy mailbox. */
	if (resource_is_busy(&mbxtab.rxs[mbxid].resource))
		return (-EBUSY);

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
	/* Is it a rx operation? */
	if (mbxid < MPPA256_MAILBOX_OPEN_OFFSET)
	{
		mbxid -= MPPA256_MAILBOX_CREATE_OFFSET;

			/* Bad sync. */
			if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
				return (-EBADF);

		/* Waits for the handler release the lock. */
		spinlock_lock(&mbxtab.rxs[mbxid].lock);
	}

	/* Is it a tx operation? */
	else
	{
		mbxid -= MPPA256_MAILBOX_OPEN_OFFSET;

			/* Bad sync. */
			if (!resource_is_used(&mbxtab.txs[mbxid].resource))
				return (-EBADF);

		/* Waits for the handler release the lock. */
		spinlock_lock(&mbxtab.txs[mbxid].lock);
	}

	return (0);
}
