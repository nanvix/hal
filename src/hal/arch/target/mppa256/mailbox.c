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

#include <arch/target/kalray/mppa256/mailbox.h>
#include <nanvix/hal/hal.h>
#include <nanvix/hal/resource.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * @name File descriptor offset.
 */
/**@{*/
#define MPPA256_MAILBOX_CREATE_OFFSET 0                          /**< Initial File Descriptor ID for Creates. */
#define MPPA256_MAILBOX_OPEN_OFFSET   MPPA256_MAILBOX_CREATE_MAX /**< Initial File Descriptor ID for Opens.   */
/**@}*/

/**
 * @name Message Queue constants.
 */
/**@{*/
#define MQUEUE_MSG_SIZE   (sizeof(struct underliyng_message))        /**< Size of the underlying message.      */
#define MQUEUE_MIN_SIZE   (MQUEUE_MSG_SIZE)                          /**< Minimum size of the Message Queue.   */
#define MQUEUE_MAX_SIZE   (PROCESSOR_NOC_NODES_NUM* MQUEUE_MIN_SIZE) /**< Maximum size of the Message Queue.   */
#define MQUEUE_MSG_AMOUNT (MQUEUE_MAX_SIZE / MQUEUE_MIN_SIZE)        /**< Message amount in the Message Queue. */
/**@}*/

/**
 * @name Gets Mailbox table parameters.
 */
/**@{*/
#define RESOURCEID_RX(nodenum)       (nodenum % BOSTAN_NR_INTERFACES) /**< mbxid.rxs' index. */
#define MAILBOXID_RX(interface, tag) (interface)                      /**< mbxid.rxs' index. */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define MAILBOX_CONTROL_TAG                (BOSTAN_MAILBOX_CNOC_TX_BASE)         /**< Associated Control Tag.  */
#define UNDERLYING_OPEN_INTERFACE(mbxid)   (mbxid / BOSTAN_MAILBOX_OPEN_PER_DMA) /**< Transfer interface.      */
#define UNDERLYING_OPEN_TAG(mbxid)         (BOSTAN_MAILBOX_DNOC_TX_BASE                      \
                                              + (mbxid % BOSTAN_NR_DNOC_TX_PER_COMM_SERVICE) \
                                           )                                    /**< Transfer tag.             */
#define UNDERLYING_CREATE_INTERFACE(mbxid) (mbxid)                              /**< Receiver tag ID.          */
#define UNDERLYING_CREATE_TAG(mbxid)       (bostan_node_mailbox_tag(                                     \
                                            bostan_cluster_get_id() + UNDERLYING_CREATE_INTERFACE(mbxid) \
                                           ))                                   /**< Receiver DMA channel ID.  */
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
		k1b_byte_t target_node;            /**< Target NoC Node ID.                                       */
		k1b_byte_t is_first_msg : 1;       /**< Indicates whether it is the first message.                */
		k1b_byte_t ack          : 1;       /**< Indicates that the receiver cosumes the previous message. */

		/* Sender requisition on hold. */
		k1b_byte_t commit       : 1;       /**< Indicates whether it is need send the message.            */
		k1b_byte_t unused       : 5;       /**< Unused.                                                   */
		k1b_spinlock_t lock;               /**< Receiver request barrier.                                 */
		struct underliyng_message message; /**< Underlying buffer for sending asynchronously.             */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_MAILBOX_OPEN_MAX];
} mbxtab = {
	.rxs[0 ... MPPA256_MAILBOX_CREATE_MAX-1] = { {0}, {{0}}, 0, 0, K1B_SPINLOCK_UNLOCKED, NULL },
	.txs[0 ... MPPA256_MAILBOX_OPEN_MAX-1]   = { {0}, 0, 0, 0, 0, 0, 0, K1B_SPINLOCK_UNLOCKED, {0} }
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
 * @brief Assess if the nodenum in on local cluster.
 *
 * @param nodenum Logic ID of a NoC node.
 *
 * @return No zero if nodenum is on local cluster and zero otherwise.
 */
PRIVATE int mppa256_mailbox_node_is_local(int nodenum)
{
	int clusternum;

	clusternum = bostan_node_get_num(bostan_cluster_get_id());

	return WITHIN(nodenum, clusternum, clusternum + BOSTAN_NR_INTERFACES);
}

/**
 * @brief Asserts whether or not a receiver mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mppa256_mailbox_rx_is_valid(int mbxid)
{
	return WITHIN(mbxid, MPPA256_MAILBOX_CREATE_OFFSET, (MPPA256_MAILBOX_CREATE_OFFSET + MPPA256_MAILBOX_CREATE_MAX));
}

/**
 * @brief Asserts whether or not a sender mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mppa256_mailbox_tx_is_valid(int mbxid)
{
	return WITHIN(mbxid, MPPA256_MAILBOX_OPEN_OFFSET, (MPPA256_MAILBOX_OPEN_OFFSET + MPPA256_MAILBOX_OPEN_MAX));
}

/**
 * @brief Asserts whether or not a sender mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mppa256_node_is_valid(int nodenum)
{
	return WITHIN(nodenum, 0, PROCESSOR_CLUSTERS_NUM);
}

/**
 * @see mppa256_mailbox_msg_copy prototype.
 */
PRIVATE int mppa256_mailbox_msg_copy(int mbxid);

/**
 * @brief Mailbox Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_mailbox_rx_handler(int interface, int tag)
{
	int mbxid;

	UNUSED(interface);
	UNUSED(tag);

	mbxid = MAILBOXID_RX(interface, tag);

	++mbxtab.rxs[mbxid].message_count;

	if (mbxtab.rxs[mbxid].buffer != NULL)
		if (mppa256_mailbox_msg_copy(mbxid) != 0)
			kpanic("[hal][mailbox] Handler failed on copy the message or send the ack to transfer node!");
}

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

/**
 * @brief Creates a mailbox.
 *
 * @param nodenum Logic ID of the underlying NoC node.
 *
 * @returns Upon successful completion, the ID of the newly created
 * mailbox is returned. Upon failure, a negative error code is
 * returned instead.
 */
PUBLIC int mppa256_mailbox_create(int nodenum)
{
	int ret;
	int tag;
	int mbxid;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_mailbox_node_is_local(nodenum))
		return (-EINVAL);
	
	if (!mppa256_node_is_valid(nodenum))
		return (-EINVAL);

	/* Gets Mailbox index. */
	mbxid = RESOURCEID_RX(nodenum);

	/* Mailbox is used. */
	if (resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EINVAL);

	/* Gets underlying parameters. */
	tag       = UNDERLYING_CREATE_TAG(mbxid);
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Creates data reciever point. */
	if (bostan_dma_data_create(interface, tag) != 0)
		return (-EINVAL);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, MAILBOX_CONTROL_TAG) != 0)
	{
		bostan_dma_data_unlink(interface, tag);
		return (-EINVAL);
	}

	/* Configures underlying message queue. */
	for (unsigned int i = 0; i < MQUEUE_MSG_AMOUNT; i++)
	{
		mbxtab.rxs[mbxid].messages[i].source  = -1;
		mbxtab.rxs[mbxid].messages[i].confirm = -1;
	}

	mbxtab.rxs[mbxid].initial_message = 0;
	mbxtab.rxs[mbxid].message_count   = 0;

	k1b_spinlock_lock(&mbxtab.rxs[mbxid].lock);

	ret = bostan_dma_data_aread(
		interface,
		tag,
		mbxtab.rxs[mbxid].messages,
		MQUEUE_MIN_SIZE,
		MQUEUE_MAX_SIZE,
		0,
		mppa256_mailbox_rx_handler
	);

	if (ret < 0)
	{
		k1b_spinlock_unlock(&mbxtab.rxs[mbxid].lock);
		return (ret);
	}

	/* Allocates associated resource. */
	resource_set_used(&mbxtab.rxs[mbxid].resource);
	resource_set_notbusy(&mbxtab.rxs[mbxid].resource);

	return (mbxid + MPPA256_MAILBOX_CREATE_OFFSET);
}

/**
 * @brief Opens a mailbox.
 *
 * @param nodeid Logic ID of the target NoC node.
 *
 * @returns Upon successful completion, the ID of the target mailbox
 * is returned. Upon failure, a negative error code is returned
 * instead.
 */
PUBLIC int mppa256_mailbox_open(int nodenum)
{
	int dtag;
	int ctag;
	int mbxid;
	int nodeid;
	int target_node;
	int interface;

	/* Is nodenum valid? */
	if (!mppa256_node_is_valid(nodenum))
		return (-EINVAL);

	/* Is nodenum in the local cluster? */
	if (mppa256_mailbox_node_is_local(nodenum))
	{
		/* Is it a compute cluster? */
		if (bostan_cluster_is_ccluster(bostan_cluster_get_id()))
			return (-EINVAL);

		/* Explanation (IO cluster):
		 * It is allowed to open locally in the IO cluster because
		 * it is as if it would be sent to a neighboring node (core).
		 */
	}

	/* Discovers underlying target node id. */
	if ((target_node = bostan_node_convert_id(nodenum)) < 0)
		return (-EINVAL);

	/* Allocates a resource. */
	if ((mbxid = resource_alloc(&mbxpools.tx_pool)) < 0)
		return (-EINVAL);

	/* Data arguments. */
	dtag      = UNDERLYING_OPEN_TAG(mbxid);
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	/* Control arguments. */
	ctag   = bostan_node_mailbox_tag(target_node);
	nodeid = (bostan_cluster_get_id() + interface);

	/* Opens data sender point. */
	if (bostan_dma_data_open(interface, dtag) != 0)
		return (-EINVAL);

	/* Creates control reciever point. */
	if (bostan_dma_control_create(interface, ctag, (1), mppa256_mailbox_tx_handler) != 0)
	{
		bostan_dma_data_close(interface, dtag);
		return (-EINVAL);
	}

	/* Configures associated resource. */
	mbxtab.txs[mbxid].commit          = 0;
	mbxtab.txs[mbxid].is_first_msg    = 1;
	mbxtab.txs[mbxid].source_ctag     = ctag;
	mbxtab.txs[mbxid].target_node     = target_node;
	mbxtab.txs[mbxid].message.source  = nodeid;
	mbxtab.txs[mbxid].message.confirm = nodeid;
	k1b_spinlock_lock(&mbxtab.txs[mbxid].lock);

	resource_set_notbusy(&mbxtab.txs[mbxid].resource);

	return (mbxid + MPPA256_MAILBOX_OPEN_OFFSET);
}

/**
 * @brief Destroys a mailbox.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns Upon successful completion, zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
PUBLIC int mppa256_mailbox_unlink(int mbxid)
{
	int tag;
	int interface;

	/* Invalid mailbox. */
	if (!mppa256_mailbox_rx_is_valid(mbxid))
		return (-EBADF);
	
	mbxid = mbxid - MPPA256_MAILBOX_CREATE_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EINVAL);

	/* Gets underlying parameters. */
	tag       = UNDERLYING_CREATE_TAG(mbxid);
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Close control sender point. */
	if (bostan_dma_control_close(interface, MAILBOX_CONTROL_TAG) != 0)
		return (-EINVAL);

	/* Unlink data reciever point. */
	if (bostan_dma_data_unlink(interface, tag) != 0)
		return (-EINVAL);

	/* Releases associated resource. */
	resource_free(&mbxpools.rx_pool, mbxid);
	mbxtab.rxs[mbxid].buffer = NULL;
	k1b_spinlock_unlock(&mbxtab.rxs[mbxid].lock);

	return (0);
}

/**
 * @brief Closes a mailbox.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int mppa256_mailbox_close(int mbxid)
{
	int dtag;
	int ctag;
	int interface;

	/* Invalid mailbox. */
	if (!mppa256_mailbox_tx_is_valid(mbxid))
		return (-EBADF);
	
	mbxid = mbxid - MPPA256_MAILBOX_OPEN_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		return (-EINVAL);

	/* Data parameters. */
	dtag      = UNDERLYING_OPEN_TAG(mbxid);
	interface = UNDERLYING_OPEN_INTERFACE(mbxid);

	/* Control parameters. */
	ctag = bostan_node_mailbox_tag(mbxtab.txs[mbxid].target_node);

	/* Unlink control reciever point. */
	if (bostan_dma_control_unlink(interface, ctag) != 0)
		return (-EINVAL);

	/* Close data sender point. */
	if (bostan_dma_data_close(interface, dtag) != 0)
		return (-EINVAL);

	/* Releases associated resource. */
	resource_free(&mbxpools.tx_pool, mbxid);
	mbxtab.txs[mbxid].commit = 0;
	k1b_spinlock_unlock(&mbxtab.txs[mbxid].lock);

	return (0);
}

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
	int nodenum;
	int interface;
	int target_tag;
	int target_node;

	/* Data parameters. */
	dtag        = UNDERLYING_OPEN_TAG(mbxid);
	interface   = UNDERLYING_OPEN_INTERFACE(mbxid);
	target_node = mbxtab.txs[mbxid].target_node;
	target_tag  = bostan_node_mailbox_tag(target_node);
	nodenum     = bostan_node_get_num(bostan_cluster_get_id() + interface);

	/* Control parameters. */
	ctag    = target_tag;

	if (bostan_dma_control_config(interface, ctag, (1), mppa256_mailbox_tx_handler) < 0)
		return (-EINVAL);

	/* Send message. */
	ret = bostan_dma_data_write(
		interface,
		dtag,
		target_node,
		target_tag,
		&mbxtab.txs[mbxid].message,
		MQUEUE_MSG_SIZE,
		(nodenum * MQUEUE_MSG_SIZE)
	);

	if (ret != 0)
		return (ret);

	mbxtab.txs[mbxid].commit = 0;
	resource_set_notbusy(&mbxtab.txs[mbxid].resource);

	k1b_spinlock_unlock(&mbxtab.txs[mbxid].lock);

	return (0);
}

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
PUBLIC int mppa256_mailbox_awrite(int mbxid, const void * buffer, uint64_t size)
{
	/* Invalid mailbox. */
	if (!mppa256_mailbox_tx_is_valid(mbxid))
		return (-EBADF);

	/* Invalid buffer. */
	if (buffer == NULL)
		return (-EINVAL);

	/* Invalid write size. */
	if (size != MPPA256_MAILBOX_MSG_SIZE)
		return (-EINVAL);
	
	mbxid = mbxid - MPPA256_MAILBOX_OPEN_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.txs[mbxid].resource))
		return (-EINVAL);

	/* Busy mailbox */
	if (resource_is_busy(&mbxtab.txs[mbxid].resource))
		return (-EBUSY);

/* Begins underlying write */

	k1b_dcache_inval();

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
		return (0);

	/* Sends the message and configurates the control receiver buffet. */
	if (mppa256_mailbox_send_msg(mbxid) != 0)
		return (-EINVAL);

/* Ends underlying write */

	return (0);
}

/**
 * @brief Copy the incoming message to the buffer and emit
 * the ack signal to the sender.
 *
 * @param mbxid Resource ID.
 *
 * @return Zero if copy and sends ack correctly and non zero otherwise.
 */
PRIVATE int mppa256_mailbox_msg_copy(int mbxid)
{
	int interface;
	int target;
	int target_tag;
	struct rx * mqueue;
	struct underliyng_message * msg;

	mqueue = &mbxtab.rxs[mbxid];

	/* Default assertions. */
	if((mqueue->buffer == NULL) || (mqueue->message_count == 0))
		return (-EINVAL);

	/* While not find a valid message. */
	do
	{
		/* Updates message queue. */
		k1b_dcache_inval(); //! Is this need?

		/* Searches for a received message. */
		for (k1b_byte_t i = 0, j = mqueue->initial_message; i < MQUEUE_MSG_AMOUNT; i++)
		{
			msg = &mqueue->messages[j];

			if (msg->source != -1 && msg->source == msg->confirm)
				break;

			j = ((j + 1) % MQUEUE_MSG_AMOUNT);
		}
	} while (msg->source == -1 || msg->source != msg->confirm);

	/* Copies message data. */
	kmemcpy(mqueue->buffer, msg->buffer, MPPA256_MAILBOX_MSG_SIZE);

	/* Cleans underlying message. */
	target = msg->source;
	msg->source = msg->confirm = -1;

	/* Updates message queue parameters. */
	--mqueue->message_count;
	mqueue->buffer          = NULL;
	mqueue->initial_message = (msg - mqueue->messages);

	/* Releases to be read again. */
	resource_set_notbusy(&mbxtab.rxs[mbxid].resource);

	/* Updates message queue. */
	k1b_dcache_inval();

	/* Unlock to slave read the data. */
	k1b_spinlock_unlock(&mqueue->lock);

	/* Gets underlying parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(mbxid);

	/* Confirms receiving with message source. */
	target_tag = bostan_node_mailbox_tag(bostan_cluster_get_id() + interface);

	return bostan_dma_control_signal(
		interface,
		MAILBOX_CONTROL_TAG,
		&target,
		1,
		target_tag,
		(~0)
	);
}

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
PUBLIC int mppa256_mailbox_aread(int mbxid, void * buffer, uint64_t size)
{
	/* Invalid mailbox. */
	if (!mppa256_mailbox_rx_is_valid(mbxid))
		return (-EBADF);

	/* Invalid buffer. */
	if (buffer == NULL)
		return (-EINVAL);

	/* Invalid read size. */
	if (size != MPPA256_MAILBOX_MSG_SIZE)
		return (-EINVAL);
	
	mbxid = mbxid - MPPA256_MAILBOX_CREATE_OFFSET;

	/* Bad mailbox. */
	if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
		return (-EINVAL);

	/* Busy mailbox. */
	if (resource_is_busy(&mbxtab.rxs[mbxid].resource))
		return (-EBUSY);

/* Begins underlying read */

	mbxtab.rxs[mbxid].buffer = buffer;
	resource_set_busy(&mbxtab.rxs[mbxid].resource);

	/* Is the message queue not empty? */
	if (mbxtab.rxs[mbxid].message_count > 0)
		mppa256_mailbox_msg_copy(mbxid);

/* Ends underlying read */

	return (0);
}

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
		if (!mppa256_mailbox_rx_is_valid(mbxid))
			return (-EBADF);
		
		mbxid = mbxid - MPPA256_MAILBOX_CREATE_OFFSET;

		#if 1 /* Is the slave with correct data cached? */
			/* Bad sync. */
			if (!resource_is_used(&mbxtab.rxs[mbxid].resource))
				return (-EACCES);
		#endif

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&mbxtab.rxs[mbxid].lock);
	}

	/* Is it a tx operation? */
	else
	{
		if (!mppa256_mailbox_tx_is_valid(mbxid))
			return (-EBADF);
		
		mbxid = mbxid - MPPA256_MAILBOX_OPEN_OFFSET;

		#if 1 /* Is the slave with correct data cached? */
			/* Bad sync. */
			if (!resource_is_used(&mbxtab.txs[mbxid].resource))
				return (-EACCES);
		#endif

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&mbxtab.txs[mbxid].lock);
	}

	return (0);
}
