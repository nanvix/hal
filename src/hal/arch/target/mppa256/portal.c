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

#include <arch/target/kalray/mppa256/portal.h>
#include <nanvix/hal/hal.h>
#include <nanvix/hal/resource.h>
#include <nanvix/klib.h>
#include <errno.h>

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
#define UNDERLYING_CREATE_DTAG(remoteid)      (bostan_node_portal_tag(remoteid))                                    /**< Receiver Dtag RX ID.     */
#define UNDERLYING_CREATE_INTERFACE(portalid) (portalid / BOSTAN_PORTAL_CREATE_PER_DMA)                             /**< Receiver DMA channel ID. */
#define UNDERLYING_OPEN_CTAG(remoteid)        (bostan_node_portal_tag(remoteid))
#define UNDERLYING_OPEN_DTAG(portalid)        ((portalid % BOSTAN_PORTAL_OPEN_PER_DMA) + PORTAL_DATA_TAG_BASE)
#define UNDERLYING_OPEN_INTERFACE(portalid)   (portalid / BOSTAN_PORTAL_OPEN_PER_DMA)
/**@}*/

/**
 * @brief portal Table
 */
PRIVATE struct portal
{
	/**
	 * @brief portal receiver
	 */
	struct rx
	{
		struct resource resource; /**< Control flags.                                */
		
		/* Control parameters */
		int remoteid;             /**< ID of the remote node allowed to communicate. */

		/* Receiver requisition on hold. */
		k1b_spinlock_t lock;      /**< Receiver request barrier.                     */
	} rxs[MPPA256_PORTAL_CREATE_MAX];

	/**
	 * @brief portal sender
	 */
	struct tx
	{
		struct resource resource; /**< Control flags.                          */

		/* Control parameters */
		k1b_byte_t is_allowed;    /**< Permission signal to sends the message. */
		int remoteid;             /**< ID of the remote node allowed to communicate. */
		
		/* Sender requisition on hold. */
		const void * buffer;
		uint64_t size;
		k1b_spinlock_t lock;      /**< Transfer request barrier.               */
	} txs[MPPA256_PORTAL_OPEN_MAX];
} portaltab = {
	.rxs[0 ... MPPA256_PORTAL_CREATE_MAX-1] = { {0}, 0, K1B_SPINLOCK_UNLOCKED },
	.txs[0 ... MPPA256_PORTAL_OPEN_MAX-1]   = { {0}, 0, -1, NULL, 0, K1B_SPINLOCK_UNLOCKED }
};

/**
 * @brief Pools of Portals
 */
PRIVATE struct portalpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} portalpools = {
	.rx_pool = {portaltab.rxs, MPPA256_PORTAL_CREATE_MAX, sizeof(struct rx)},
	.tx_pool = {portaltab.txs, MPPA256_PORTAL_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Gets portal table parameters.
 * 
 * @param nodenum Logic number of the node.
 * 
 * @returns Portal ID on RX arrays elements, -1 if has no one available.
 */
PRIVATE int get_free_resource_rx(int nodenum)
{
	unsigned base;

	UNUSED(portalpools);

	base = (nodenum % BOSTAN_NR_INTERFACES) * BOSTAN_PORTAL_CREATE_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_CREATE_PER_DMA); ++i)
		if (resource_is_used(&portaltab.rxs[i].resource))
			return (i);

	return (-1);
}

/**
 * @brief Gets portal table parameters.
 * 
 * @param nodenum Logic number of the node.
 * 
 * @returns Portal ID on RX arrays elements, -1 if has no one available.
 */
PRIVATE int get_free_resource_tx(int nodenum)
{
	unsigned base;

	base = (nodenum % BOSTAN_NR_INTERFACES) * BOSTAN_PORTAL_OPEN_PER_DMA;

	for (unsigned i = base; i < (base + BOSTAN_PORTAL_OPEN_PER_DMA); ++i)
		if (resource_is_used(&portaltab.txs[i].resource))
			return (i);

	return (-1);
}

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

	if (portalid == -1)
	{
		clusternum = bostan_node_get_num(bostan_cluster_get_id());

		return WITHIN(nodenum, clusternum, clusternum + BOSTAN_NR_INTERFACES);
	}
	else
	{
		clusternum = bostan_node_get_num(bostan_cluster_get_id() + UNDERLYING_CREATE_INTERFACE(portalid));

		return (clusternum == nodenum);
	}
}

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
	return WITHIN(portalid, 0, MPPA256_PORTAL_CREATE_MAX);
}

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
	return WITHIN(portalid, 0, MPPA256_PORTAL_OPEN_MAX);
}

/**
 * @brief Portal Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_portal_receiver_handler(int interface, int tag)
{
	unsigned base;

	kprintf("[hal][target][portal] Receiver Handler triggered (%d, %d).", interface, tag);

	base = (interface * BOSTAN_PORTAL_CREATE_PER_DMA);

	for (unsigned i = base; i < (base + MPPA256_PORTAL_CREATE_MAX); ++i)
	{
		if (!resource_is_used(&portaltab.rxs[i].resource))
			continue;

		if (UNDERLYING_CREATE_DTAG(portaltab.rxs[i].remoteid) != tag)
			continue;

		resource_set_notbusy(&portaltab.rxs[i].resource);

		/* Releases the wait condition. */
		spinlock_unlock(&portaltab.rxs[i].lock);

		break;
	}
}

PRIVATE int mppa256_portal_send_data(int portalid);

/**
 * @brief Portal Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_portal_sender_handler(int interface, int tag)
{
	unsigned base;

	kprintf("[hal][target][portal] Sender Handler triggered (%d, %d).", interface, tag);

	base = (interface * BOSTAN_PORTAL_OPEN_PER_DMA);

	for (unsigned i = base; i < (base + MPPA256_PORTAL_OPEN_MAX); ++i)
	{
		if (!resource_is_used(&portaltab.txs[i].resource))
			continue;

		if (UNDERLYING_OPEN_CTAG(portaltab.txs[i].remoteid) != tag)
			continue;

		portaltab.txs[i].is_allowed = 1;

		/* Sends requested message. */
		if (!resource_is_busy(&portaltab.txs[i].resource))
		{
			if (mppa256_portal_send_data(i) != 0)
				kpanic("[hal][target][portal] Sender Handler failed!");
		}

		break;
	}
}

/**
 * @brief Creates a portal.
 *
 * @param local ID of the local NoC node.
 *
 * @returns Upon successful completion, the ID of a newly created
 * portal is returned. Upon failure, a negative error code is returned
 * instead.
 */
int mppa256_portal_create(int nodenum)
{
	int ctag;
	int portalid;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_node_is_local(-1, nodenum))
		return (-EINVAL);

	/* Gets portal index not used. */
	if ((portalid = get_free_resource_rx(nodenum)) < 0)
		return (-EBUSY);

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	/* Opens control sender point. */
	if (bostan_dma_control_open(interface, ctag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	k1b_spinlock_lock(&portaltab.rxs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remoteid = -1;
	resource_set_used(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	return (portalid);
}

/**
 * @brief Enables read operations from a remote.
 *
 * @param portalid ID of the target portal.
 * @param remote   NoC node ID of target remote.
 *
 * @returns Upons successful completion zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
int mppa256_portal_allow(int portalid, int remote)
{
	int dtag;
	int remoteid;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EINVAL);

	/* Gets portal index not used. */
	if (mppa256_portal_node_is_local(portalid, remote))
		return (-EINVAL);

	interface = UNDERLYING_CREATE_INTERFACE(portalid);
	
	if (portaltab.rxs[portalid].remoteid != -1)
	{
		dtag = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remoteid);

		if (bostan_dma_data_unlink(interface, dtag) != 0)
			return (-EINVAL);
	}

	/* Gets underlying parameters. */
	remoteid = bostan_node_convert_id(remote);
	dtag     = UNDERLYING_CREATE_DTAG(remoteid);

	/* Creates data reciever point. */
	if (bostan_dma_data_create(interface, dtag) != 0)
		return (-EINVAL);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remoteid = remoteid;
	resource_set_used(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	return (0);
}

/**
 * @brief Opens a portal.
 *
 * @param nodeid Logic ID of the target NoC node.
 *
 * @returns Upon successful completion, the ID of the target portal
 * is returned. Upon failure, a negative error code is returned
 * instead.
 */
int mppa256_portal_open(int local, int remote)
{
	int ctag;
	int dtag;
	int remoteid;
	int portalid;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_node_is_local(-1, local))
		return (-EINVAL);
	
	/* Invalid NoC node ID. */
	if (local == remote)
		return (-EINVAL);

	/* Gets portal index not used. */
	if ((portalid = get_free_resource_tx(local)) < 0)
		return (-EBUSY);

	/* Gets underlying parameters. */
	remoteid  = bostan_node_convert_id(remote);
	ctag      = UNDERLYING_OPEN_CTAG(remoteid);
	dtag      = UNDERLYING_OPEN_DTAG(portalid);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Opens control sender point. */
	if (bostan_dma_control_create(interface, ctag, (1), mppa256_portal_sender_handler) != 0)
		return (-EAGAIN);
	
	/* Opens data sender point. */
	if (bostan_dma_data_open(interface, dtag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	k1b_spinlock_lock(&portaltab.txs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.txs[portalid].remoteid = remoteid;
	resource_set_used(&portaltab.txs[portalid].resource);
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	return (portalid);
}

/**
 * @brief Destroys a portal.
 *
 * @param portalid ID of the target portal.
 *
 * @returns Upon successful completion, zero is returned. Upon failure,
 * a negative error code is returned instead.
 */
int mppa256_portal_unlink(int portalid)
{
	int ctag;
	int dtag;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EINVAL);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		return (-EINVAL);

	/* Gets underlying parameters. */
	ctag = UNDERLYING_CREATE_CTAG(portalid);
	interface = UNDERLYING_CREATE_INTERFACE(portalid);

	if (portaltab.rxs[portalid].remoteid != -1)
	{
		dtag = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remoteid);

		if (bostan_dma_data_unlink(interface, dtag) != 0)
			return (-EINVAL);	
	}

	/* Opens control sender point. */
	if (bostan_dma_control_close(interface, ctag) != 0)
		return (-EAGAIN);

	/* Configures lock from asynchronous operations. */
	k1b_spinlock_unlock(&portaltab.rxs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.rxs[portalid].remoteid = -1;
	resource_set_unused(&portaltab.rxs[portalid].resource);
	resource_set_notbusy(&portaltab.rxs[portalid].resource);

	return (0);
}

/**
 * @brief Closes a portal.
 *
 * @param portalid ID of the target portal.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
int mppa256_portal_close(int portalid)
{
	int ctag;
	int dtag;
	int interface;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_tx_is_valid(portalid))
		return (-EINVAL);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.txs[portalid].resource))
		return (-EINVAL);

	/* Gets underlying parameters. */
	ctag      = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remoteid);
	dtag      = UNDERLYING_OPEN_DTAG(portalid);
	interface = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Opens data sender point. */
	if (bostan_dma_data_close(interface, dtag) != 0)
		return (-EAGAIN);

	/* Opens control sender point. */
	if (bostan_dma_control_unlink(interface, ctag) != 0)
		return (-EAGAIN);
	
	/* Configures lock from asynchronous operations. */
	k1b_spinlock_unlock(&portaltab.txs[portalid].lock);

	/* Allocates associated resource. */
	portaltab.txs[portalid].remoteid = -1;
	resource_set_unused(&portaltab.txs[portalid].resource);
	resource_set_notbusy(&portaltab.txs[portalid].resource);

	return (0);
}

PRIVATE int mppa256_portal_send_data(int portalid)
{
	int ret;
	int ctag;
	int dtag;
	int localid;
	int interface;
	int target_dtag;

	/* Local Parameters. */
	ctag        = UNDERLYING_OPEN_CTAG(portaltab.txs[portalid].remoteid);
	dtag        = UNDERLYING_OPEN_DTAG(portalid);
	interface   = UNDERLYING_OPEN_INTERFACE(portalid);

	/* Target parameters*/
	localid     = bostan_node_convert_id(cluster_get_id() + interface);
	target_dtag = UNDERLYING_CREATE_DTAG(localid);

	/* Reconfigures signal receiver. */
	if (bostan_dma_control_config(interface, ctag, (1), mppa256_portal_sender_handler) < 0)
		return (-EINVAL);

	/* Sends data. */
	ret = bostan_dma_data_write(
		interface,
		dtag,
		portaltab.txs[portalid].remoteid,
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

	k1b_spinlock_unlock(&portaltab.txs[portalid].lock);

	return (0);
}

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
int mppa256_portal_write(int portalid, const void * buffer, uint64_t size, struct aiocb * aiocb)
{
	/* Invalid NoC node ID. */
	if (!mppa256_portal_tx_is_valid(portalid))
		return (-EINVAL);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.txs[portalid].resource))
		return (-EINVAL);

	/* Busy portal. */
	if (!resource_is_busy(&portaltab.txs[portalid].resource))
		return (-EBUSY);
	
	/* Bad aiocb. */
	if (aiocb == NULL || size > MPPA256_PORTAL_MAX_SIZE)
		return (-EINVAL);

	portaltab.txs[portalid].buffer = buffer;
	portaltab.txs[portalid].size   = size;
	resource_set_busy(&portaltab.txs[portalid].resource);
	
	if (portaltab.txs[portalid].is_allowed)
		mppa256_portal_send_data(portalid);

	return (0);
}

/**
 * @brief Reads data from a portal.
 *
 * @param portalid  ID of the target portal.
 * @param buffer Buffer where the data should be written to.
 * @param size   Number of bytes to read.
 * @param aiocb  Asynchronous operation control.
 *
 * @returns Upon successful completion, 0 is returned
 * and non zero otherwise.
 */
int mppa256_portal_aread(int portalid, void * buffer, uint64_t size, struct aiocb * aiocb)
{
	int ret;
	int ctag;
	int dtag;
	int interface;
	int target_ctag;

	/* Invalid NoC node ID. */
	if (!mppa256_portal_rx_is_valid(portalid))
		return (-EINVAL);

	/* Bad portal. */
	if (!resource_is_used(&portaltab.rxs[portalid].resource))
		return (-EINVAL);

	/* Busy portal. */
	if (!resource_is_busy(&portaltab.rxs[portalid].resource))
		return (-EBUSY);
	
	/* Bad aiocb. */
	if (aiocb == NULL || size > MPPA256_PORTAL_MAX_SIZE)
		return (-EINVAL);

	/* Data parameters. */
	interface = UNDERLYING_CREATE_INTERFACE(portalid);
	dtag      = UNDERLYING_CREATE_DTAG(portaltab.rxs[portalid].remoteid);

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
	target_ctag = UNDERLYING_OPEN_CTAG((bostan_cluster_get_id() + interface));

	/* Sends permission to transmit the data. */
	ret = bostan_dma_control_signal(
		interface,
		ctag,
		&portaltab.rxs[portalid].remoteid,
		1,
		target_ctag,
		(~0)
	);

	/* Sent successful? */
	if (ret < 0)
		return (ret);

	/* Configures controller for asynchronous operation. */
	aiocb->fd   = portalid;
	aiocb->type = BOSTAN_NOC_RX_TYPE;
	aiocb->lock = &portaltab.rxs[portalid].lock;

	resource_set_busy(&portaltab.rxs[portalid].resource);

	return (0);
}

/**
 * @brief Waits asynchronous operation.
 *
 * @param aiocb  Asynchronous operation control.
 *
 * @return Zero if wait read correctly and non zero otherwise.
 */
int mppa256_portal_wait(struct aiocb * aiocb)
{
	/* Bad aiocb. */
	if (aiocb == NULL)
		return (-EINVAL);

	/* Is it a rx operation? */
	if (aiocb->type == BOSTAN_NOC_RX_TYPE)
	{
		if (!mppa256_portal_rx_is_valid(aiocb->fd))
			return (-EBADF);
	}

	/* Is it a tx operation? */
	else if (aiocb->type == BOSTAN_NOC_RX_TYPE)
	{
		if (!mppa256_portal_tx_is_valid(aiocb->fd))
			return (-EBADF);
	}

	/* Invalid operation type. */
	else
		return (-EINVAL);

	/* Bad lock. */
	if (aiocb->lock == NULL)
		return (-EINVAL);

	/* Waits for the handler release the lock. */
	k1b_spinlock_lock(aiocb->lock);

	return (0);
}
