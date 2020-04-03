/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
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
 * @name File descriptor offset.
 */
/**@{*/
#define MPPA256_SYNC_CREATE_OFFSET 0                       /**< Initial File Descriptor ID for Creates. */
#define MPPA256_SYNC_OPEN_OFFSET   MPPA256_SYNC_CREATE_MAX /**< Initial File Descriptor ID for Opens.   */
/**@}*/

/**
 * @name Gets syncid.
 */
/**@{*/
#define RESOURCEID_RX(interface, tag) ((interface * BOSTAN_SYNC_CREATE_PER_DMA) + (tag - BOSTAN_SYNC_RX_OFF)) /**< synctab.rxs' index. */
#define RESOURCEID_TX(interface)      (interface * BOSTAN_SYNC_OPEN_PER_DMA)                                  /**< synctab.txs' index. */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define UNDERLYING_RX_TAG(syncid)       ((syncid % BOSTAN_SYNC_CREATE_PER_DMA) + BOSTAN_SYNC_RX_OFF) /**< Receiver tag ID.         */
#define UNDERLYING_RX_INTERFACE(syncid) (syncid / BOSTAN_SYNC_CREATE_PER_DMA)                        /**< Receiver DMA channel ID. */
#define UNDERLYING_TX_TAG(syncid)       (BOSTAN_SYNC_CNOC_TX_BASE)                                   /**< Transfer tag ID.         */
#define UNDERLYING_TX_INTERFACE(syncid) (syncid)                                                     /**< Transfer DMA channel ID. */
/**@}*/

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

/**
 * @brief Synchronization Points Table
 */
PRIVATE struct sync
{
	/**
	 * @brief Receiver Synchronization Points
	 */
	struct rx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource; /**< Generic resource information. */

		uint64_t mask;            /**< Initial value.                */
		k1b_spinlock_t lock;      /**< Wait condition.               */
	} ALIGN(sizeof(dword_t)) rxs[MPPA256_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;             /**< Generic resource information. */

		int remotes[PROCESSOR_NOC_NODES_NUM]; /**< Targets Logic IDs.            */
		int nremotes;                         /**< Number of remotes.            */
		int remote_tag;                       /**< Master tag.                   */
		uint64_t mask;                        /**< Signal mask.                  */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_SYNC_OPEN_MAX];
} synctab = {
	.rxs[0 ... MPPA256_SYNC_CREATE_MAX-1] = { {0}, 0, K1B_SPINLOCK_UNLOCKED },
	.txs[0 ... MPPA256_SYNC_OPEN_MAX-1]   = { {0}, {0, }, 0, 0, 0ULL }
};

/**
 * @brief Pools of Synchronization Resource
 */
PRIVATE struct syncpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} ALIGN(sizeof(dword_t)) syncpools = {
	.rx_pool = {synctab.rxs, MPPA256_SYNC_CREATE_MAX, sizeof(struct rx)},
	.tx_pool = {synctab.txs, MPPA256_SYNC_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Global lock
 */
PRIVATE k1b_spinlock_t synctab_lock = K1B_SPINLOCK_UNLOCKED;

/*============================================================================*
 * mppa256_sync_lock()                                                        *
 *============================================================================*/

PRIVATE void mppa256_sync_lock(void)
{
	k1b_spinlock_lock(&synctab_lock);
}

/*============================================================================*
 * mppa256_sync_unlock()                                                      *
 *============================================================================*/


PRIVATE void mppa256_sync_unlock(void)
{
	k1b_spinlock_unlock(&synctab_lock);
}

/*============================================================================*
 * mppa256_sync_rx_is_valid()                                                 *
 *============================================================================*/

/**
 * @brief Assess limits of the receiver resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_rx_is_valid(int syncid)
{
	return (
		WITHIN(
			syncid,
			MPPA256_SYNC_CREATE_OFFSET,
			MPPA256_SYNC_CREATE_OFFSET + MPPA256_SYNC_CREATE_MAX
		)
	);
}

/*============================================================================*
 * mppa256_sync_tx_is_valid()                                                 *
 *============================================================================*/

/**
 * @brief Assess limits of the sender resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_tx_is_valid(int syncid)
{
	return (
		WITHIN(
			syncid,
			MPPA256_SYNC_OPEN_OFFSET,
			MPPA256_SYNC_OPEN_OFFSET + MPPA256_SYNC_OPEN_MAX
		)
	);
}

/*============================================================================*
 * mppa256_sync_node_list_is_valid()                                          *
 *============================================================================*/

/**
 * @brief Node list validation.
 *
 * @param local    Logic ID of local node.
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 *
 * @return Non zero if node list is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_node_list_is_valid(int local, const int *nodenums, int nnodes)
{
	uint64_t checks; /* Bit-stream of nodenums. */

	checks = 0ULL;

	/* Build list of RX NoC nodes. */
	for (int i = 0; i < nnodes; ++i)
	{
		if (!WITHIN(nodenums[i], 0, PROCESSOR_NOC_NODES_NUM))
			return (0);

		/* Does a node appear twice? */
		if (checks & (1ULL << nodenums[i]))
			return (0);

		checks |= (1ULL << nodenums[i]);
	}

	/* Local Node found. */
	return (checks & (1ULL << local));
}

/*============================================================================*
 * mppa256_sync_is_local()                                                    *
 *============================================================================*/

/**
 * @brief Sync local point validation.
 *
 * @param nodenum  Logic ID of local node.
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 *
 * @return Non zero if local point is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_is_local(int nodenum, const int *nodenums, int nnodes)
{
	/* Underlying NoC node SHOULD be here. */
	if (nodenum != nodenums[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodenum == nodenums[i])
			return (0);

	return (1);
}

/*============================================================================*
 * mppa256_sync_is_remote()                                                   *
 *============================================================================*/

/**
 * @brief Sync remote point validation.
 *
 * @param nodenum  Logic ID of local node.
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 *
 * @return Zero if remote point is valid and non zero otherwise.
 */
PRIVATE int mppa256_sync_is_remote(int nodenum, const int *nodenums, int nnodes)
{
	int found; /* Bool variable. */

	found = 0;

	/* Underlying NoC node SHOULD NOT be here. */
	if (nodenum == nodenums[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodenum == nodenums[i])
			found++;

	if (found != 1)
		return (0);

	return (1);
}

/*============================================================================*
 * mppa256_sync_it_handler()                                                  *
 *============================================================================*/

/**
 * @brief Sync Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_sync_it_handler(int interface, int tag)
{
	int ret;    /* Return value. */
	int syncid; /* Sync ID.      */

	UNUSED(interface);
	UNUSED(tag);

	syncid = RESOURCEID_RX(interface, tag);

	if (resource_is_used(&synctab.rxs[syncid].resource))
	{
		/* Reconfigure sync point. */
		ret = bostan_dma_control_config(
			interface,
			tag,
			synctab.rxs[syncid].mask,
			mppa256_sync_it_handler
		);

		if (ret < 0)
			kpanic("[hal][sync][handler] Reconfiguration of the sync falied!");

		/* Releases slave. */
		k1b_spinlock_unlock(&synctab.rxs[syncid].lock);
	}
}

/*============================================================================*
 * mppa256_sync_select_rx_interface()                                         *
 *============================================================================*/

/**
 * @brief Associate a underliyng DMA RX channel.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of the synchronization.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_rx_interface(const int *nodenums, int nnodes, int type)
{
	int i;          /* Iterator value.         */
	int tag;        /* Underlying control tag. */
	int syncid;     /* Sync ID.                */
	int interface;  /* Underlying interface.   */
	int clusternum; /* Local node number.      */

	clusternum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	/* Searchs on local IDs? */
	if (type == MPPA256_SYNC_ALL_TO_ONE)
	{
		i = 0;
		nnodes = 1;
	}

	/* Searchs on Remote IDs? */
	else
		i = 1;

	for (; i < nnodes; i++)
	{
		if (WITHIN(nodenums[i], clusternum, clusternum + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM))
		{
			tag       = bostan_processor_node_sync_tag(nodenums[i]);
			interface = (nodenums[i] - clusternum);
			syncid    = RESOURCEID_RX(interface, tag);

			if (!resource_is_used(&synctab.rxs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_sync_create()                                                      *
 *============================================================================*/

/**
 * @brief Allocates and configures the receiving side of the synchronization point.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of synchronization point.
 *
 * @return The tag of underlying resource ID.
 */
PRIVATE int do_mppa256_sync_create(const int *nodenums, int nnodes, int type)
{
	int ret;       /* Return value.           */
	int tag;       /* Underlying control tag. */
	int syncid;    /* Sync ID.                */
	int nodenum;   /* Local nodenum.          */
	int interface; /* Underlying interface.   */
	uint64_t mask; /* Sync mask.              */

	mppa256_sync_lock();

	ret = (-EINVAL);

	if ((interface = mppa256_sync_select_rx_interface(nodenums, nnodes, type)) < 0)
		goto error;

	nodenum = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	if (!mppa256_sync_node_list_is_valid(nodenum, nodenums, nnodes))
		goto error;

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		if (!mppa256_sync_is_remote(nodenum, nodenums, nnodes))
			goto error;

		mask = (1ULL);
	}

	/* Gather. */
	else
	{
		if (!mppa256_sync_is_local(nodenum, nodenums, nnodes))
			goto error;

		mask = 0ULL;
		for (int i = 1; i < nnodes; i++)
			mask |= (1ULL << nodenums[i]);
		mask = (~mask);
	}

	tag = bostan_processor_node_sync_tag(nodenums[0]);
	syncid = RESOURCEID_RX(interface, tag);

	ret = (-EBADF);

	if (resource_is_used(&synctab.rxs[syncid].resource))
		goto error;

	ret = bostan_dma_control_create(
		interface,
		tag,
		mask,
		mppa256_sync_it_handler
	);

	if (ret < 0)
	{
		ret = (-EINVAL);
		goto error;
	}

	synctab.rxs[syncid].mask = mask;
	k1b_spinlock_init(&synctab.rxs[syncid].lock);
	k1b_spinlock_lock(&synctab.rxs[syncid].lock);

	resource_set_used(&synctab.rxs[syncid].resource);
	resource_set_async(&synctab.rxs[syncid].resource);
	resource_set_notbusy(&synctab.rxs[syncid].resource);

	ret = (MPPA256_SYNC_CREATE_OFFSET + syncid);

error:
	mppa256_sync_unlock();

	return (ret);
}

/**
 * @see do_mppa256_sync_create().
 */
PUBLIC int mppa256_sync_create(const int *nodenums, int nnodes, int type)
{
	if (nodenums == NULL)
		return (-EINVAL);

	if (!WITHIN(nnodes, 2, PROCESSOR_NOC_NODES_NUM + 1))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	return (do_mppa256_sync_create(nodenums, nnodes, type));
}

/*============================================================================*
 * mppa256_sync_select_tx_interface()                                         *
 *============================================================================*/

/**
 * @brief Associate a underliyng DMA TX channel.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of the synchronization.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_tx_interface(const int *nodenums, int nnodes, int type)
{
	int i;          /* Iterator value.         */
	int syncid;     /* Sync ID.                */
	int interface;  /* Underlying interface.   */
	int clusternum; /* Local node number.      */

	clusternum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	/* Searchs on local IDs? */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		i = 0;
		nnodes = 1;
	}

	/* Searchs on Remote IDs? */
	else
		i = 1;

	for (; i < nnodes; i++)
	{
		if (WITHIN(nodenums[i], clusternum, clusternum + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM))
		{
			interface = (nodenums[i] - clusternum);
			syncid    = RESOURCEID_TX(interface);

			if (!resource_is_used(&synctab.txs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_sync_open()                                                        *
 *============================================================================*/

/**
 * @brief Allocates and configures the sending side of the synchronization point.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of synchronization point.
 *
 * @return The tag of underlying resource ID.
 */
PRIVATE int do_mppa256_sync_open(const int *nodenums, int nnodes, int type)
{
	int ret;       /* Return value.           */
	int tag;       /* Underlying control tag. */
	int syncid;    /* Sync ID.                */
	int localnum;  /* Local nodenum.          */
	int interface; /* Underlying interface.   */

	mppa256_sync_lock();

	ret = (-EINVAL);

	if ((interface = mppa256_sync_select_tx_interface(nodenums, nnodes, type)) < 0)
		goto error;

	syncid   = RESOURCEID_TX(interface);
	localnum = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	if (!mppa256_sync_node_list_is_valid(localnum, nodenums, nnodes))
		goto error;

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		if (nodenums[0] != localnum)
			goto error;

		kmemcpy(synctab.txs[syncid].remotes, &nodenums[1], sizeof(int) * (nnodes - 1));
		synctab.txs[syncid].nremotes = (nnodes - 1);
		synctab.txs[syncid].mask     = ~(1ULL);
	}

	/* Unicast. */
	else
	{
		if (!mppa256_sync_is_remote(localnum, nodenums, nnodes))
			goto error;

		synctab.txs[syncid].remotes[0] = nodenums[0];
		synctab.txs[syncid].nremotes   = 1;
		synctab.txs[syncid].mask       = (1ULL << (localnum));
	}

	synctab.txs[syncid].remote_tag = bostan_processor_node_sync_tag(nodenums[0]);
	tag = UNDERLYING_TX_TAG(syncid);

	if (bostan_dma_control_open(interface, tag) < 0)
		goto error;

	resource_set_used(&synctab.txs[syncid].resource);
	resource_set_sync(&synctab.txs[syncid].resource);
	resource_set_notbusy(&synctab.txs[syncid].resource);

	ret = (MPPA256_SYNC_OPEN_OFFSET + syncid);

error:
	mppa256_sync_unlock();

	return (ret);
}

/**
 * @see do_mppa256_sync_open().
 */
PUBLIC int mppa256_sync_open(const int *nodenums, int nnodes, int type)
{
	if (nodenums == NULL)
		return (-EINVAL);

	if (!WITHIN(nnodes, 2, PROCESSOR_NOC_NODES_NUM + 1))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	/* Are nodenums valid? */
	for (int i = 0; i < nnodes; ++i)
		if (!WITHIN(nodenums[i], 0, PROCESSOR_NOC_NODES_NUM))
			return (-EINVAL);

	return (do_mppa256_sync_open(nodenums, nnodes, type));
}

/*============================================================================*
 * mppa256_sync_unlink()                                                      *
 *============================================================================*/

/**
 * @brief Releases and cleans receiver buffer.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if free the resource and non zero otherwise.
 */
PRIVATE int do_mppa256_sync_unlink(int syncid)
{
	int tag;       /* Underlying control tag. */
	int interface; /* Underlying interface.   */

	tag       = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	if (bostan_dma_control_unlink(interface, tag) != 0)
		return (-EINVAL);

	mppa256_sync_lock();
		resource_free(&syncpools.rx_pool, syncid);
	mppa256_sync_unlock();

	return (0);
}

/**
 * @see do_mppa256_sync_unlink().
 */
PUBLIC int mppa256_sync_unlink(int syncid)
{
	if (!mppa256_sync_rx_is_valid(syncid))
		return (-EBADF);

	syncid -= MPPA256_SYNC_CREATE_OFFSET;

	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

	mppa256_sync_unlock();

	return (do_mppa256_sync_unlink(syncid));
}

/*============================================================================*
 * mppa256_sync_close()                                                       *
 *============================================================================*/

/**
 * @brief Free the sender resources on a specific DMA channel.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if free the resource and non zero otherwise.
 */
PRIVATE int do_mppa256_sync_close(int syncid)
{
	int tag;       /* Underlying control tag. */
	int interface; /* Underlying interface.   */

	tag       = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

	if (bostan_dma_control_close(interface, tag) != 0)
		return (-EINVAL);

	mppa256_sync_lock();
		resource_free(&syncpools.tx_pool, syncid);
	mppa256_sync_unlock();

	return (0);
}

/**
 * @see do_mppa256_sync_close().
 */
PUBLIC int mppa256_sync_close(int syncid)
{
	if (!mppa256_sync_tx_is_valid(syncid))
		return (-EBADF);

	syncid -= MPPA256_SYNC_OPEN_OFFSET;

	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

	mppa256_sync_unlock();

	return (do_mppa256_sync_close(syncid));
}

/*============================================================================*
 * mppa256_sync_wait()                                                        *
 *============================================================================*/

/**
 * @brief Wait signal on a specific synchronization point.
 *
 * @param syncid ID of the synchronization point.
 *
 * @return Zero if wait signal correctly and non zero otherwise.
 *
 * @todo How make the slave wait on lock without the need to
 * access the master's private structures? (microkernel approach)
 */
PUBLIC int mppa256_sync_wait(int syncid)
{
	if (!mppa256_sync_rx_is_valid(syncid))
		return (-EBADF);

	syncid -= MPPA256_SYNC_CREATE_OFFSET;

	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

	mppa256_sync_unlock();

	/* Waits for the handler release the lock. */
	k1b_spinlock_lock(&synctab.rxs[syncid].lock);

	return (0);
}

/*============================================================================*
 * mppa256_sync_signal()                                                      *
 *============================================================================*/

/**
 * @brief Send signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if send signal correctly and non zero otherwise.
 */
PRIVATE int do_mppa256_sync_signal(int syncid)
{
	int tag;       /* Underlying control tag. */
	int interface; /* Underlying interface.   */

	tag       = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

	return bostan_dma_control_signal(
		interface,
		tag,
		synctab.txs[syncid].remotes,
		synctab.txs[syncid].nremotes,
		synctab.txs[syncid].remote_tag,
		synctab.txs[syncid].mask
	);
}

/**
 * @see do_mppa256_sync_signal().
 */
PUBLIC int mppa256_sync_signal(int syncid)
{
	if (!mppa256_sync_tx_is_valid(syncid))
		return (-EBADF);

	syncid -= MPPA256_SYNC_OPEN_OFFSET;

	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

	mppa256_sync_unlock();

	return (do_mppa256_sync_signal(syncid));
}

