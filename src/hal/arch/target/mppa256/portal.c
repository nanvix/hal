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
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define PORTAL_CONTROL_TAG_BASE (BOSTAN_PORTAL_CNOC_TX_BASE) /**< Associated Control Tag.  */
#define PORTAL_DATA_TAG_BASE    (BOSTAN_PORTAL_DNOC_TX_BASE) /**< Associated Data Tag.     */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define UNDERLYING_CREATE_CTAG(portalid)      ((portalid % BOSTAN_PORTAL_CREATE_PER_DMA) + PORTAL_CONTROL_TAG_BASE) /**< Receiver Ctag TX ID.     */
#define UNDERLYING_CREATE_DTAG(remote_num)    (bostan_processor_node_portal_tag(remote_num))                        /**< Receiver Dtag RX ID.     */
#define UNDERLYING_CREATE_INTERFACE(portalid) (portalid / BOSTAN_PORTAL_CREATE_PER_DMA)                             /**< Receiver DMA channel ID. */
#define UNDERLYING_OPEN_CTAG(remote_num)      (bostan_processor_node_portal_tag(remote_num))                        /**< Transfer Ctag TX ID.     */
#define UNDERLYING_OPEN_DTAG(portalid)        ((portalid % BOSTAN_PORTAL_OPEN_PER_DMA) + PORTAL_DATA_TAG_BASE)      /**< Transfer Dtag RX ID.     */
#define UNDERLYING_OPEN_INTERFACE(portalid)   (portalid / BOSTAN_PORTAL_OPEN_PER_DMA)                               /**< Transfer DMA channel ID. */
/**@}*/

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

/**
 * @brief portal Table
 */
PRIVATE struct portal
{
	/**
	 * @brief Portal receiver
	 */
	struct rx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource; /**< Generic resource information.                       */

		/* Control parameters */
		int ret;                  /**< Returned value on a copy.                           */
		int remote;               /**< Logic ID of the remote node allowed to communicate. */
		k1b_byte_t is_allowed;    /**< One read allowed bu portal_allow() call.            */

		/* Receiver requisition on hold. */
		k1b_spinlock_t lock;      /**< Receiver request barrier.                           */
	} ALIGN(sizeof(dword_t)) rxs[MPPA256_PORTAL_CREATE_MAX];

	/**
	 * @brief Portal sender
	 */
	struct tx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource; /**< Generic resource information.                       */

		/* Control parameters */
		int ret;                  /**< Returned value on a send.                           */
		int remote;               /**< Logic ID of the remote node allowed to communicate. */
		k1b_byte_t is_allowed;    /**< Permission signal to sends the message.             */
		k1b_byte_t commit;        /**< Indicates whether it is need send the data.         */

		/* Sender requisition on hold. */
		const void * buffer;      /**< Buffer of the data.                                 */
		uint64_t size;            /**< Size of the buffer.                                 */
		k1b_spinlock_t lock;      /**< Transfer request barrier.                           */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_PORTAL_OPEN_MAX];
} ALIGN(sizeof(dword_t)) portaltab;

/**
 * @brief Pools of Portals
 */
PRIVATE struct portalpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} ALIGN(sizeof(dword_t)) portalpools = {
	.rx_pool = {portaltab.rxs, MPPA256_PORTAL_CREATE_MAX, sizeof(struct rx)},
	.tx_pool = {portaltab.txs, MPPA256_PORTAL_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Global lock
 */
PRIVATE k1b_spinlock_t portaltab_lock = K1B_SPINLOCK_UNLOCKED;

/*============================================================================*
 * mppa256_portal_lock()                                                      *
 *============================================================================*/

PRIVATE void mppa256_portal_lock(void)
{
	k1b_spinlock_lock(&portaltab_lock);
}

/*============================================================================*
 * mppa256_portal_unlock()                                                    *
 *============================================================================*/


PRIVATE void mppa256_portal_unlock(void)
{
	k1b_spinlock_unlock(&portaltab_lock);
}

/*============================================================================*
 * mppa256_get_free_resource_rx()                                             *
 *============================================================================*/

/**
 * @brief Gets portal table parameters.
 *
 * @param nodenum Logic number of the node.
 *
 * @returns Portal ID on RX arrays elements, -1 if has no one available.
 */
PRIVATE int mppa256_get_free_resource_rx(int nodenum)
{
	unsigned base; /* Initial index. */

	UNUSED(portalpools);

	base = (nodenum % BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) * BOSTAN_PORTAL_CREATE_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_CREATE_PER_DMA); i++)
		if (!resource_is_used(&portaltab.rxs[i].resource))
			return (i);

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_get_free_resource_tx()                                             *
 *============================================================================*/

/**
 * @brief Gets portal table parameters.
 *
 * @param nodenum Logic number of the node.
 *
 * @returns Portal ID on RX arrays elements, -1 if has no one available.
 */
PRIVATE int mppa256_get_free_resource_tx(int nodenum)
{
	unsigned base; /* Initial index. */

	base = (nodenum % BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) * BOSTAN_PORTAL_OPEN_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_OPEN_PER_DMA); i++)
		if (!resource_is_used(&portaltab.txs[i].resource))
			return (i);

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_portal_receiver_handler()                                          *
 *============================================================================*/

/**
 * @brief Portal Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_portal_receiver_handler(int interface, int tag)
{
	unsigned begin; /* Initial index. */
	unsigned end;   /* Limit index.   */

	begin = (interface * BOSTAN_PORTAL_CREATE_PER_DMA);
	end   = (begin + MPPA256_PORTAL_CREATE_MAX);

	for (unsigned i = begin; i < end; i++)
	{
		/**
		 * This verification cannot be protected by the global lock
		 * because the handler can interrupt the master core when
		 * it's inside a critical region. That's create a deadlock
		 * situation.
		 */
		if (!resource_is_used(&portaltab.rxs[i].resource))
			continue;

		if (UNDERLYING_CREATE_DTAG(portaltab.rxs[i].remote) != tag)
			continue;

		portaltab.rxs[i].is_allowed = 0;
		portaltab.rxs[i].ret        = 0;

		/* Releases the wait condition. */
		k1b_spinlock_unlock(&portaltab.rxs[i].lock);

		break;
	}
}

/*============================================================================*
 * mppa256_portal_sender_handler()                                            *
 *============================================================================*/

/**
 * @see mppa256_portal_send_data().
 */
PRIVATE int mppa256_portal_send_data(int portalid);

/**
 * @brief Portal Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_portal_sender_handler(int interface, int tag)
{
	unsigned begin; /* Initial index. */
	unsigned end;   /* Limit index.   */

	begin = (interface * BOSTAN_PORTAL_OPEN_PER_DMA);
	end   = (begin + MPPA256_PORTAL_OPEN_MAX);

	for (unsigned i = begin; i < end; i++)
	{
		/**
		 * This verification cannot be protected by the global lock
		 * because the handler can interrupt the master core when
		 * it's inside a critical region. That's create a deadlock
		 * situation.
		 */
		if (!resource_is_used(&portaltab.txs[i].resource))
			continue;

		if (UNDERLYING_OPEN_CTAG(portaltab.txs[i].remote) != tag)
			continue;

		portaltab.txs[i].is_allowed = 1;

		dcache_invalidate();

		/* Sends requested message. */
		if (portaltab.txs[i].commit != 0)
		{
			portaltab.txs[i].commit     = 0;
			portaltab.txs[i].is_allowed = 0;

			if (mppa256_portal_send_data(i) != 0)
			{
				portaltab.txs[i].is_allowed = 1;

				/* The failure does not cause kpanic because it is recoverable. */
				kprintf("[hal][portal][handler] Sender Handler failed!");
			}
		}

		break;
	}
}

/*============================================================================*
 * mppa256_portal_create()                                                    *
 *============================================================================*/

/**
 * @brief Creates a portal.
 *
 * @param nodenum Logical ID of the local NoC node.
 *
 * @returns Upon successful completion, the ID of a newly created
 * portal is returned. Upon failure, a negative error code is returned
 * instead.
 */
PRIVATE int do_mppa256_portal_create(int nodenum)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int portalid;  /* Portal ID.              */
	int interface; /* Underlying interface.   */

	mppa256_portal_lock();

	ret = (-EBUSY);

	/* Gets portal index not used. */
	if ((portalid = mppa256_get_free_resource_rx(nodenum)) < 0)
		goto error;

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	ret = (-EAGAIN);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, ctag) != 0)
		goto error;

	/* Configures lock from asynchronous operations. */
	k1b_spinlock_trylock(&portaltab.rxs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remote     = -1;
	portaltab.rxs[portalid].is_allowed = 0;
	portaltab.rxs[portalid].ret        = (-EAGAIN);
	resource_set_used(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	ret = (MPPA256_PORTAL_CREATE_OFFSET + portalid);

error:
	mppa256_portal_unlock();

	return (ret);
}

/**
 * @see do_mppa256_portal_create().
 */
PUBLIC int mppa256_portal_create(int nodenum)
{
	return (do_mppa256_portal_create(nodenum));
}

/*============================================================================*
 * mppa256_portal_allow()                                                     *
 *============================================================================*/

/**
 * @brief Enables read operations from a remote.
 *
 * @param portalid  ID of the target portal.
 * @param remotenum Logic ID of target NoC node.
 *
 * @returns Upons successful completion zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
PRIVATE int do_mppa256_portal_allow(int portalid, int remotenum)
{
	int dtag;      /* Underlying data tag.    */
	int interface; /* Underlying interface.   */

	/* Gets dma interface. */
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	/* Is the remote already configured with another remote? */
	if (portaltab.rxs[portalid].remote != -1 && portaltab.rxs[portalid].remote != remotenum)
	{
		dtag = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remote);

		/* Unlink previous allowed resources. */
		if (bostan_dma_data_unlink(interface, dtag) != 0)
			return (-EINVAL);

		portaltab.rxs[portalid].remote = -1;
	}

	/* Does it need to be configured? */
	if (portaltab.rxs[portalid].remote == -1)
	{
		/* Gets resource tag. */
		dtag = UNDERLYING_CREATE_DTAG(remotenum);

		/* Creates data reciever point. */
		if (bostan_dma_data_create(interface, dtag) != 0)
			return (-EINVAL);

		portaltab.rxs[portalid].remote = remotenum;
	}

	/* Allow one read. */
	portaltab.rxs[portalid].is_allowed = 1;

	dcache_invalidate();

	return (0);
}

/**
 * @see do_mppa256_portal_allow().
 */
PUBLIC int mppa256_portal_allow(int portalid, int remotenum)
{
	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	mppa256_portal_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.rxs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBADF);
		}

		/* Bad portal. */
		if (resource_is_busy(&portaltab.rxs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBUSY);
		}

		/* Read already allowed. */
		if (portaltab.rxs[portalid].is_allowed)
		{
			mppa256_portal_unlock();
			return (-EBUSY);
		}

	mppa256_portal_unlock();

	return (do_mppa256_portal_allow(portalid, remotenum));
}

/*============================================================================*
 * mppa256_portal_open()                                                      *
 *============================================================================*/

/**
 * @brief Opens a portal.
 *
 * @param localnum Logic ID of the local NoC node.
 * @param remote   Logic ID of the target NoC node.
 *
 * @returns Upon successful completion, the ID of the target portal
 * is returned. Upon failure, a negative error code is returned
 * instead.
 */
PRIVATE int do_mppa256_portal_open(int localnum, int remotenum)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int portalid;  /* Portal ID.              */
	int interface; /* Underlying interface.   */

	mppa256_portal_lock();

	ret = (-EBUSY);

	/* Gets portal index not used. */
	if ((portalid = mppa256_get_free_resource_tx(localnum)) < 0)
		goto error;

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(remotenum);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	ret = bostan_dma_control_create(
		interface,
		ctag,
		BOSTAN_CNOC_BARRIER_MODE,
		(1),
		mppa256_portal_sender_handler
	);

	/* Opens control sender point. */
	if (ret != 0)
	{
		ret = (-EAGAIN);
		goto error;
	}

	/* Configures lock from asynchronous operations. */
	k1b_spinlock_trylock(&portaltab.txs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.txs[portalid].ret        = (-EAGAIN);
	portaltab.txs[portalid].remote     = remotenum;
	portaltab.txs[portalid].buffer     = NULL;
	portaltab.txs[portalid].commit     = 0;
	portaltab.txs[portalid].size       = 0;
	portaltab.txs[portalid].is_allowed = 0;
	resource_set_used(&portaltab.txs[portalid].resource);
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	ret = (MPPA256_PORTAL_OPEN_OFFSET + portalid);

error:
	mppa256_portal_unlock();

	return (ret);
}

/**
 * @see do_mppa256_portal_open().
 */
PUBLIC int mppa256_portal_open(int localnum, int remotenum)
{
	return (do_mppa256_portal_open(localnum, remotenum));
}

/*============================================================================*
 * mppa256_portal_unlink()                                                    *
 *============================================================================*/

/**
 * @brief Destroys a portal.
 *
 * @param portalid ID of the target portal.
 *
 * @returns Upon successful completion, zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
PRIVATE int do_mppa256_portal_unlink(int portalid)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int dtag;      /* Underlying data tag.    */
	int interface; /* Underlying interface.   */

	mppa256_portal_lock();

	ret = (-EBADF);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		goto error;

	ret = (-EBUSY);

	/* Bad portal. */
	if (resource_is_busy(&portaltab.rxs[portalid].resource))
		goto error;

	ret = (-EINVAL);

	/* Gets underlying parameters. */
	ctag = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	if (portaltab.rxs[portalid].remote != -1)
	{
		dtag = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remote);

		if (bostan_dma_data_unlink(interface, dtag) != 0)
			goto error;
	}

	/* Opens control sender point. */
	if (bostan_dma_control_close(interface, ctag) != 0)
		kpanic("[hal][portal][unlink] Failed to release signal emitter after unlinking the data receiver.");

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remote     = -1;
	portaltab.rxs[portalid].is_allowed = 0;
	resource_free(&portalpools.rx_pool, portalid);

	ret = 0;

error:
	mppa256_portal_unlock();

	return (ret);
}

/**
 * @see do_mppa256_portal_unlink().
 */
PUBLIC int mppa256_portal_unlink(int portalid)
{
	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	return (do_mppa256_portal_unlink(portalid));
}

/*============================================================================*
 * mppa256_portal_close()                                                     *
 *============================================================================*/

/**
 * @brief Closes a portal.
 *
 * @param portalid ID of the target portal.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE int do_mppa256_portal_close(int portalid)
{
	int ret;       /* Return value.           */
	int ctag;      /* Underlying control tag. */
	int interface; /* Underlying interface.   */

	mppa256_portal_lock();

	ret = (-EBADF);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.txs[portalid].resource))
		goto error;

	ret = (-EBUSY);

	/* Bad portal. */
	if (resource_is_busy(&portaltab.txs[portalid].resource))
		goto error;

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remote);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	ret = (-EINVAL);

	/* Unlinks control sender point. */
	if (bostan_dma_control_unlink(interface, ctag) != 0)
		goto error;

	/* Allocates associated resource. */
	portaltab.txs[portalid].remote     = -1;
	portaltab.txs[portalid].is_allowed = 0;
	resource_free(&portalpools.tx_pool, portalid);

	ret = 0;

error:
	mppa256_portal_unlock();

	return (ret);
}

/**
 * @see do_mppa256_portal_close().
 */
PUBLIC int mppa256_portal_close(int portalid)
{
	portalid -= MPPA256_PORTAL_OPEN_OFFSET;

	return (do_mppa256_portal_close(portalid));
}

/*============================================================================*
 * mppa256_portal_send_data()                                                 *
 *============================================================================*/

PRIVATE struct resource bostan_portal_tx_channels[BOSTAN_DNOC_TXS_PER_COMM_SERVICE];

/**
 * @brief Underlying sender funtion.
 *
 * @param portalid ID of the target portal.
 *
 * @returns Upon successful completion, 0 is returned
 * and non zero otherwise.
 */
PRIVATE int mppa256_portal_send_data(int portalid)
{
	int ret;         /* Return value.               */
	int ctag;        /* Underlying control tag.     */
	int dtag;        /* Underlying data tag.        */
	int interface;   /* Underlying interface.       */
	int target_dtag; /* Underlying target data tag. */

	/* Local Parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remote);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Opens data sender point. */
	ret  = (dtag = (-EBUSY));

	/* Try to find not busy dtag. */
	for (int i = 0; i < BOSTAN_DNOC_TXS_PER_COMM_SERVICE; i++)
	{
		if (!resource_is_busy(&bostan_portal_tx_channels[i]))
		{
			dtag = (PORTAL_DATA_TAG_BASE + i);
			resource_set_busy(&bostan_portal_tx_channels[i]);
			break;
		}
	}

	/* Checks if succesfully allocated a DTAG. */
	if (dtag < 0)
		goto error1;

	/* Target parameters*/
	target_dtag = UNDERLYING_CREATE_DTAG(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	ret = bostan_dma_control_config(
		interface,
		ctag,
		BOSTAN_CNOC_BARRIER_MODE,
		(1),
		mppa256_portal_sender_handler
	);

	/* Reconfigures signal receiver. */
	if (ret < 0)
	{
		ret = (-EAGAIN);
		goto error2;
	}

	/* Sends data. */
	ret = bostan_dma_data_write(
		interface,
		dtag,
		portaltab.txs[portalid].remote,
		target_dtag,
		portaltab.txs[portalid].buffer,
		portaltab.txs[portalid].size,
		0
	);

error2:
	resource_set_notbusy(&bostan_portal_tx_channels[(dtag - PORTAL_DATA_TAG_BASE)]);

error1:
	portaltab.txs[portalid].buffer = NULL;
	portaltab.txs[portalid].size   = 0;
	portaltab.txs[portalid].ret    = (ret < 0) ? (-EAGAIN) : 0;

	k1b_spinlock_unlock(&portaltab.txs[portalid].lock);

	return (ret);
}

/*============================================================================*
 * mppa256_portal_awrite()                                                    *
 *============================================================================*/

/**
 * @brief Writes data to a portal.
 *
 * @param portalid  ID of the target portal.
 * @param buffer Buffer where the data should be read from.
 * @param size   Number of bytes to write.
 *
 * @returns Upon successful completion, 0 is returned
 * and non zero otherwise.
 */
PRIVATE ssize_t do_mppa256_portal_awrite(int portalid, const void * buffer, uint64_t size)
{
	ssize_t ret; /* Return value. */

	ret = size;

	mppa256_portal_lock();
		resource_set_busy(&portaltab.txs[portalid].resource);
	mppa256_portal_unlock();

	interrupt_mask(K1B_INT_CNOC);

		portaltab.txs[portalid].buffer = buffer;
		portaltab.txs[portalid].size   = size;
		portaltab.txs[portalid].commit = 1;

		if (portaltab.txs[portalid].is_allowed)
		{
			portaltab.txs[portalid].commit     = 0;
			portaltab.txs[portalid].is_allowed = 0;

			if (mppa256_portal_send_data(portalid) != 0)
			{
				portaltab.txs[portalid].is_allowed = 1;
				ret = (-EACCES);
			}
		}

		/* Double check. */
		bostan_cnoc_it_verify();

	interrupt_unmask(K1B_INT_CNOC);

	if (ret < 0)
	{
		mppa256_portal_lock();
			portaltab.txs[portalid].buffer = NULL;
			portaltab.txs[portalid].size   = 0;
			resource_set_notbusy(&portaltab.txs[portalid].resource);
		mppa256_portal_unlock();
	}

	return (ret);
}

/**
 * @see do_mppa256_portal_awrite().
 *
 * @todo Check fixed size from microkernel.
 */
PUBLIC ssize_t mppa256_portal_awrite(int portalid, const void * buffer, uint64_t size)
{
	portalid -= MPPA256_PORTAL_OPEN_OFFSET;

	mppa256_portal_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.txs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBADF);
		}

		/* Busy portal. */
		if (resource_is_busy(&portaltab.txs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBUSY);
		}

	mppa256_portal_unlock();

	return (do_mppa256_portal_awrite(portalid, buffer, size));
}

/*============================================================================*
 * mppa256_portal_aread()                                                     *
 *============================================================================*/

/**
 * @brief Reads data from a portal.
 *
 * @param portalid  ID of the target portal.
 * @param buffer Buffer where the data should be written to.
 * @param size   Number of bytes to read.
 *
 * @returns Upon successful completion, 0 is returned
 * and non zero otherwise.
 */
PUBLIC ssize_t do_mppa256_portal_aread(int portalid, void * buffer, uint64_t size)
{
	ssize_t ret;     /* Return value.                  */
	int ctag;        /* Underlying control tag.        */
	int dtag;        /* Underlying data tag.           */
	int interface;   /* Underlying interface.          */
	int target_ctag; /* Underlying target control tag. */

	mppa256_portal_lock();
		resource_set_busy(&portaltab.rxs[portalid].resource);
	mppa256_portal_unlock();

	/* Data parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(portalid);
	dtag      = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remote);

	/* Configures the underlying resources to receive the data. */
	ret = bostan_dma_data_aread(
		interface,
		dtag,
		buffer,
		size,
		size,
		0,
		mppa256_portal_receiver_handler
	);

	/* Setup successful? */
	if (ret < 0)
		goto error;

	/* Control parameters. */
	ctag        = UNDERLYING_CREATE_CTAG(portalid);
	target_ctag = UNDERLYING_OPEN_CTAG(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	/* Sends permission to transmit the data. */
	ret = bostan_dma_control_signal(
		interface,
		ctag,
		&portaltab.rxs[portalid].remote,
		1,
		target_ctag,
		(~0)
	);

error:
	/* Sent successful? */
	if (ret >= 0)
		ret = size;
	else
	{
		mppa256_portal_lock();
			resource_set_notbusy(&portaltab.rxs[portalid].resource);
		mppa256_portal_unlock();
	}

	return (ret);
}

/**
 * @see do_mppa256_portal_aread().
 *
 * @todo Check fixed size from microkernel.
 */
PUBLIC ssize_t mppa256_portal_aread(int portalid, void * buffer, uint64_t size)
{
	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	mppa256_portal_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.rxs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBADF);
		}

		/* Busy portal. */
		if (resource_is_busy(&portaltab.rxs[portalid].resource))
		{
			mppa256_portal_unlock();
			return (-EBUSY);
		}

		/* Read not allowed. */
		if (!portaltab.rxs[portalid].is_allowed)
		{
			mppa256_portal_unlock();
			return (-EINVAL);
		}

	mppa256_portal_unlock();

	return (do_mppa256_portal_aread(portalid, buffer, size));
}

/*============================================================================*
 * mppa256_portal_wait()                                                      *
 *============================================================================*/

/**
 * @brief Waits asynchronous operation.
 *
 * @param portaid ID of the target portal.
 *
 * @return Zero if wait read correctly and non zero otherwise.
 */
PUBLIC int mppa256_portal_wait(int portalid)
{
	int ret; /* Return value. */

	/* Is it a rx operation? */
	if (portalid < MPPA256_PORTAL_OPEN_OFFSET)
	{
		portalid -= MPPA256_PORTAL_CREATE_OFFSET;

		mppa256_portal_lock();

			/* Bad sync. */
			if (!resource_is_used(&portaltab.rxs[portalid].resource))
				goto error;

			/* Bad sync. */
			if (!resource_is_busy(&portaltab.rxs[portalid].resource))
				goto error;

		mppa256_portal_unlock();

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&portaltab.rxs[portalid].lock);

		/**
		 * The attribution of the return value is allowed on the slave
		 * side because it will be protected by the busy flag release
		 * by the own slave core.
		 **/
		ret = portaltab.rxs[portalid].ret;
		portaltab.rxs[portalid].ret = (-EAGAIN);

		mppa256_portal_lock();
			resource_set_notbusy(&portaltab.rxs[portalid].resource);
		mppa256_portal_unlock();
	}

	/* Is it a tx operation? */
	else
	{
		portalid -= MPPA256_PORTAL_OPEN_OFFSET;

		mppa256_portal_lock();

			/* Bad sync. */
			if (!resource_is_used(&portaltab.txs[portalid].resource))
				goto error;

			/* Bad sync. */
			if (!resource_is_busy(&portaltab.txs[portalid].resource))
				goto error;

		mppa256_portal_unlock();

		/* Waits for the handler release the lock. */
		k1b_spinlock_lock(&portaltab.txs[portalid].lock);

		/**
		 * The attribution of the return value is allowed on the slave
		 * side because it will be protected by the busy flag release
		 * by the own slave core.
		 **/
		ret = portaltab.txs[portalid].ret;
		portaltab.txs[portalid].ret = (-EAGAIN);

		mppa256_portal_lock();
			resource_set_notbusy(&portaltab.txs[portalid].resource);
		mppa256_portal_unlock();
	}

	return (ret);

error:
	mppa256_portal_unlock();

	return (-EBADF);
}

/*============================================================================*
 * mppa256_portal_setup()                                                     *
 *============================================================================*/

/**
 * @see mppa256_portal_setup().
 */
PUBLIC void mppa256_portal_setup(void)
{
	int dtag;

	kprintf("[hal][portal] Portal Initialization.");

	for (unsigned i = 0; i < BOSTAN_DNOC_TXS_PER_COMM_SERVICE; i++)
	{
		bostan_portal_tx_channels[i] = RESOURCE_INITIALIZER;
		resource_set_used(&bostan_portal_tx_channels[i]);

		/* Opens data sender point. */
		dtag = (PORTAL_DATA_TAG_BASE + i);
		if (bostan_dma_data_open(0, dtag) < 0)
			kpanic("[hal][portal] Cannot open data channel.");
	}

	for (unsigned i = 0; i < MPPA256_PORTAL_CREATE_MAX; i++)
	{
		portaltab.rxs[i].resource   = RESOURCE_INITIALIZER;
		portaltab.rxs[i].ret        = (-EAGAIN);
		portaltab.rxs[i].remote     = (-1);
		portaltab.rxs[i].is_allowed = 0;
		k1b_spinlock_init(&portaltab.rxs[i].lock);
		k1b_spinlock_lock(&portaltab.rxs[i].lock);
	}

	for (unsigned i = 0; i < MPPA256_PORTAL_OPEN_MAX; i++)
	{
		portaltab.txs[i].resource   = RESOURCE_INITIALIZER;
		portaltab.txs[i].ret        = (-EAGAIN);
		portaltab.txs[i].remote     = (-1);
		portaltab.txs[i].is_allowed = 0;
		portaltab.txs[i].commit     = 0;
		portaltab.txs[i].buffer     = NULL;
		portaltab.txs[i].size       = 0ULL;
		k1b_spinlock_init(&portaltab.txs[i].lock);
		k1b_spinlock_lock(&portaltab.txs[i].lock);
	}
}

/*============================================================================*
 * mppa256_portal_shutdown()                                                     *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void mppa256_portal_shutdown(void)
{
	int dtag;

	for (unsigned i = 0; i < BOSTAN_DNOC_TXS_PER_COMM_SERVICE; i++)
	{
		/* Opens data sender point. */
		dtag = (PORTAL_DATA_TAG_BASE + i);
		if (bostan_dma_data_close(0, dtag) < 0)
			kpanic("[hal][portal] Cannot close data channel.");
	}
}
