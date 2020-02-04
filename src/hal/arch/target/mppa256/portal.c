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
 * @name File descriptor offset.
 */
/**@{*/
#define MPPA256_PORTAL_CREATE_OFFSET 0                         /**< Initial File Descriptor ID for Creates. */
#define MPPA256_PORTAL_OPEN_OFFSET   MPPA256_PORTAL_CREATE_MAX /**< Initial File Descriptor ID for Opens.   */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define PORTAL_CONTROL_TAG_BASE (BOSTAN_PORTAL_CNOC_TX_BASE) /**< Associated Control Tag.  */
#define PORTAL_DATA_TAG_BASE    (BOSTAN_PORTAL_DNOC_TX_BASE) /**< Associated Data Tag.  */
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
		struct resource resource; /**< Control flags.                                      */

		/* Control parameters */
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
		struct resource resource; /**< Control flags.                                      */

		/* Control parameters */
		int remote;               /**< Logic ID of the remote node allowed to communicate. */
		k1b_byte_t is_allowed;    /**< Permission signal to sends the message.             */

		/* Sender requisition on hold. */
		const void * buffer;      /**< Buffer of the data.                                 */
		uint64_t size;            /**< Size of the buffer.                                 */
		k1b_spinlock_t lock;      /**< Transfer request barrier.                           */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_PORTAL_OPEN_MAX];
} ALIGN(sizeof(dword_t)) portaltab = {
	.rxs[0 ... MPPA256_PORTAL_CREATE_MAX-1] = { {0}, -1, 0, K1B_SPINLOCK_UNLOCKED },
	.txs[0 ... MPPA256_PORTAL_OPEN_MAX-1]   = { {0}, -1, 0, NULL, 0, K1B_SPINLOCK_UNLOCKED }
};

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
	unsigned base;

	UNUSED(portalpools);

	base = (nodenum % BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) * BOSTAN_PORTAL_CREATE_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_CREATE_PER_DMA); ++i)
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
	unsigned base;

	base = (nodenum % BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) * BOSTAN_PORTAL_OPEN_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_OPEN_PER_DMA); ++i)
		if (!resource_is_used(&portaltab.txs[i].resource))
			return (i);

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_portal_node_is_local()                                             *
 *============================================================================*/

/**
 * @brief Assess if the nodenum in on local cluster.
 *
 * @param nodenum Logic ID of a NoC node.
 *
 * @return No zero if nodenum is on local cluster and zero otherwise.
 */
PRIVATE int mppa256_portal_node_is_local(int portalid, int nodenum)
{
	int clusternum;

	clusternum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	if (portalid == -1)
		return WITHIN(nodenum, clusternum, clusternum + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM);

	clusternum += UNDERLYING_CREATE_INTERFACE(portalid);

	return (clusternum == nodenum);
}

/*============================================================================*
 * mppa256_portal_rx_is_valid()                                               *
 *============================================================================*/

/**
 * @brief Asserts whether or not a receiver portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mppa256_portal_rx_is_valid(int portalid)
{
	return (
		WITHIN(
			portalid,
			MPPA256_PORTAL_CREATE_OFFSET,
			MPPA256_PORTAL_CREATE_OFFSET + MPPA256_PORTAL_CREATE_MAX
		)
	);
}

/*============================================================================*
 * mppa256_portal_tx_is_valid()                                               *
 *============================================================================*/

/**
 * @brief Asserts whether or not a sender portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mppa256_portal_tx_is_valid(int portalid)
{
	return (
		WITHIN(
			portalid,
			MPPA256_PORTAL_OPEN_OFFSET,
			MPPA256_PORTAL_OPEN_OFFSET + MPPA256_PORTAL_OPEN_MAX
		)
	);
}

/*============================================================================*
 * mppa256_node_is_valid()                                                    *
 *============================================================================*/

/**
 * @brief Asserts whether or not a sender mailbox is valid.
 *
 * @param portalid ID of the target mailbox.
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
	return (WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM));
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
	unsigned begin;
	unsigned end;

	begin = (interface * BOSTAN_PORTAL_CREATE_PER_DMA);
	end   = (begin + MPPA256_PORTAL_CREATE_MAX);

	for (unsigned i = begin; i < end; ++i)
	{
		if (!resource_is_used(&portaltab.rxs[i].resource))
			continue;

		if (UNDERLYING_CREATE_DTAG(portaltab.rxs[i].remote) != tag)
			continue;

		portaltab.rxs[i].is_allowed = 0;
		resource_set_notbusy(&portaltab.rxs[i].resource);

		/* Releases the wait condition. */
		spinlock_unlock(&portaltab.rxs[i].lock);

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
	unsigned begin;
	unsigned end;

	begin = (interface * BOSTAN_PORTAL_OPEN_PER_DMA);
	end   = (begin + MPPA256_PORTAL_OPEN_MAX);

	for (unsigned i = begin; i < end; ++i)
	{
		if (!resource_is_used(&portaltab.txs[i].resource))
			continue;

		if (UNDERLYING_OPEN_CTAG(portaltab.txs[i].remote) != tag)
			continue;

		portaltab.txs[i].is_allowed = 1;
		dcache_invalidate();

		/* Sends requested message. */
		if (resource_is_busy(&portaltab.txs[i].resource))
		{
			if (mppa256_portal_send_data(i) != 0)
				kpanic("[hal][target][portal] Sender Handler failed!");
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
	int ctag;
	int portalid;
	int interface;

	/* Gets portal index not used. */
	if ((portalid = mppa256_get_free_resource_rx(nodenum)) < 0)
		return (-EBUSY);

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, ctag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	spinlock_lock(&portaltab.rxs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remote     = -1;
	portaltab.rxs[portalid].is_allowed = 0;
	resource_set_used(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	return (MPPA256_PORTAL_CREATE_OFFSET + portalid);
}

/**
 * @see do_mppa256_portal_create().
 */
PUBLIC int mppa256_portal_create(int nodenum)
{
	/* Is local valid? */
	if (!mppa256_node_is_valid(nodenum))
		return (-EINVAL);

	/* Invalid NoC node ID. */
	if (!mppa256_portal_node_is_local(-1, nodenum))
		return (-EINVAL);

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
	int dtag;
	int interface;

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
	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EBADF);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		return (-EBADF);

	/* Bad portal. */
	if (resource_is_busy(&portaltab.rxs[portalid].resource))
		return (-EBUSY);

	/* Is nodenum valid? */
	if (!mppa256_node_is_valid(remotenum))
		return (-EINVAL);

	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	/* Gets portal index not used. */
	if (mppa256_portal_node_is_local(portalid, remotenum))
		return (-EINVAL);

	/* Read already allowed. */
	if (portaltab.rxs[portalid].is_allowed)
		return (-EBUSY);

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
	int ctag;
	int dtag;
	int portalid;
	int interface;

	/* Gets portal index not used. */
	if ((portalid = mppa256_get_free_resource_tx(localnum)) < 0)
		return (-EBUSY);

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(remotenum);
	dtag      = UNDERLYING_OPEN_DTAG(portalid);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Opens control sender point. */
	if (bostan_dma_control_create(interface, ctag, (1), mppa256_portal_sender_handler) != 0)
		return (-EAGAIN);

	/* Opens data sender point. */
	if (bostan_dma_data_open(interface, dtag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	spinlock_lock(&portaltab.txs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.txs[portalid].remote     = remotenum;
	portaltab.txs[portalid].buffer     = NULL;
	portaltab.txs[portalid].size       = 0;
	portaltab.txs[portalid].is_allowed = 0;
	resource_set_used(&portaltab.txs[portalid].resource);
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	return (MPPA256_PORTAL_OPEN_OFFSET + portalid);
}

/**
 * @see do_mppa256_portal_open().
 */
PUBLIC int mppa256_portal_open(int localnum, int remotenum)
{
	/* Is nodenum valid? */
	if (!mppa256_node_is_valid(localnum) || !mppa256_node_is_valid(remotenum))
		return (-EINVAL);

	/* Invalid NoC node ID. */
	if (!mppa256_portal_node_is_local(-1, localnum))
		return (-EINVAL);

	/* Invalid NoC node ID. */
	if (localnum == remotenum)
		return (-EINVAL);

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
	int ctag;
	int dtag;
	int interface;

	/* Gets underlying parameters. */
	ctag = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	if (portaltab.rxs[portalid].remote != -1)
	{
		dtag = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remote);

		if (bostan_dma_data_unlink(interface, dtag) != 0)
			return (-EINVAL);
	}

	/* Opens control sender point. */
	if (bostan_dma_control_close(interface, ctag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	spinlock_unlock(&portaltab.rxs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remote     = -1;
	portaltab.rxs[portalid].is_allowed = 0;
	resource_set_unused(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	return (0);
}

/**
 * @see do_mppa256_portal_unlink().
 */
PUBLIC int mppa256_portal_unlink(int portalid)
{
	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EBADF);

	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		return (-EBADF);

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
	int ctag;
	int dtag;
	int interface;

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remote);
	dtag      = UNDERLYING_OPEN_DTAG(portalid);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Opens data sender point. */
	if (bostan_dma_data_close(interface, dtag) != 0)
		return (-EAGAIN);

	/* Opens control sender point. */
	if (bostan_dma_control_unlink(interface, ctag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	spinlock_unlock(&portaltab.txs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.txs[portalid].remote     = -1;
	portaltab.txs[portalid].is_allowed = 0;
	resource_set_unused(&portaltab.txs[portalid].resource);
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	return (0);
}

/**
 * @see do_mppa256_portal_close().
 */
PUBLIC int mppa256_portal_close(int portalid)
{
	/* Invalid NoC node ID. */
	if (!mppa256_portal_tx_is_valid(portalid))
		return (-EBADF);

	portalid -= MPPA256_PORTAL_OPEN_OFFSET;

	/* Bad portal. */
	if (!resource_is_used(&portaltab.txs[portalid].resource))
		return (-EBADF);

	return (do_mppa256_portal_close(portalid));
}

/*============================================================================*
 * mppa256_portal_send_data()                                                 *
 *============================================================================*/

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
	int ret;
	int ctag;
	int dtag;
	int interface;
	int target_dtag;

	/* Local Parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remote);
	dtag      = UNDERLYING_OPEN_DTAG(portalid);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Target parameters*/
	target_dtag = UNDERLYING_CREATE_DTAG(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	/* Reconfigures signal receiver. */
	if (bostan_dma_control_config(interface, ctag, (1), mppa256_portal_sender_handler) < 0)
		return (-EINVAL);

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

	if (ret < 0)
		return (ret);

	portaltab.txs[portalid].buffer     = NULL;
	portaltab.txs[portalid].size       = 0;
	portaltab.txs[portalid].is_allowed = 0;
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	spinlock_unlock(&portaltab.txs[portalid].lock);

	return (0);
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
	if (!portaltab.txs[portalid].is_allowed)
		return (-EAGAIN);

	portaltab.txs[portalid].buffer = buffer;
	portaltab.txs[portalid].size   = size;
	resource_set_busy(&portaltab.txs[portalid].resource);

	if (mppa256_portal_send_data(portalid) != 0)
		return (-EAGAIN);

	return (size);
}

/**
 * @see do_mppa256_portal_awrite().
 *
 * @todo Check fixed size from microkernel.
 */
PUBLIC ssize_t mppa256_portal_awrite(int portalid, const void * buffer, uint64_t size)
{
	/* Invalid NoC node ID. */
	if (!mppa256_portal_tx_is_valid(portalid))
		return (-EBADF);

	portalid -= MPPA256_PORTAL_OPEN_OFFSET;

	/* Bad portal. */
	if (!resource_is_used(&portaltab.txs[portalid].resource))
		return (-EBADF);

	/* Busy portal. */
	if (resource_is_busy(&portaltab.txs[portalid].resource))
		return (-EAGAIN);

	/* Bad buffer*/
	if (buffer == NULL)
		return (-EINVAL);

	/* Bad size. */
	if (size == 0)
		return (-EINVAL);

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
	int ret;
	int ctag;
	int dtag;
	int interface;
	int target_ctag;

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
		return (ret);

	/* Control parameters. */
	ctag        = UNDERLYING_CREATE_CTAG(portalid);
	target_ctag = UNDERLYING_OPEN_CTAG(
		bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface
	);

	/* Marks resource as busy. */
	resource_set_busy(&portaltab.rxs[portalid].resource);

	/* Sends permission to transmit the data. */
	ret = bostan_dma_control_signal(
		interface,
		ctag,
		&portaltab.rxs[portalid].remote,
		1,
		target_ctag,
		(~0)
	);

	/* Sent successful? */
	if (ret < 0)
	{
		resource_set_notbusy(&portaltab.rxs[portalid].resource);
		return (ret);
	}

	return (size);
}

/**
 * @see do_mppa256_portal_aread().
 *
 * @todo Check fixed size from microkernel.
 */
PUBLIC ssize_t mppa256_portal_aread(int portalid, void * buffer, uint64_t size)
{
	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EBADF);

	portalid -= MPPA256_PORTAL_CREATE_OFFSET;

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		return (-EBADF);

	/* Busy portal. */
	if (resource_is_busy(&portaltab.rxs[portalid].resource))
		return (-EBUSY);

	/* Bad buffer. */
	if (buffer == NULL)
		return (-EINVAL);

	/* Bad size. */
	if (size == 0)
		return (-EINVAL);

	/* Read not allowed. */
	if (!portaltab.rxs[portalid].is_allowed)
		return (-EINVAL);

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
	/* Is it a rx operation? */
	if (portalid < MPPA256_PORTAL_OPEN_OFFSET)
	{
		if (!mppa256_portal_rx_is_valid(portalid))
			return (-EBADF);

		portalid -= MPPA256_PORTAL_CREATE_OFFSET;

		#if 1 /* Is the slave with correct data cached? */
			/* Bad sync. */
			if (!resource_is_used(&portaltab.rxs[portalid].resource))
				return (-EBADF);
		#endif

		/* Waits for the handler release the lock. */
		spinlock_lock(&portaltab.rxs[portalid].lock);
	}

	/* Is it a tx operation? */
	else
	{
		if (!mppa256_portal_tx_is_valid(portalid))
			return (-EBADF);

		portalid -= MPPA256_PORTAL_OPEN_OFFSET;

		#if 1 /* Is the slave with correct data cached? */
			/* Bad sync. */
			if (!resource_is_used(&portaltab.txs[portalid].resource))
				return (-EBADF);
		#endif

		/* Waits for the handler release the lock. */
		spinlock_lock(&portaltab.txs[portalid].lock);
	}

	return (0);
}
