/*
 * MIT License
 *
 * Copyright(c) 2011-2019 The Maintainers of Nanvix
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

#include <arch/target/kalray/mppa256/sync.h>
#include <nanvix/hal/resource.h>
#include <nanvix/klib.h>
#include <errno.h>

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
		struct resource resource; /**< Control flags.  */
		uint64_t mask;            /**< Initial value.  */
		k1b_spinlock_t lock;      /**< Wait condition. */
	} rxs[MPPA256_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		struct resource resource;         /**< Control flags.      */
		int targets[BOSTAN_NR_NOC_NODES]; /**< Targets Physic IDs. */
		int ntargets;                     /**< Number of targets.  */
		int target_tag;                   /**< Master tag.         */
		uint64_t mask;                    /**< Signal mask.        */
	} txs[MPPA256_SYNC_OPEN_MAX];
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
} syncpools = {
	.rx_pool = {synctab.rxs, MPPA256_SYNC_CREATE_MAX, sizeof(struct rx)},
	.tx_pool = {synctab.txs, MPPA256_SYNC_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @name Gets syncid.
 */
#define RESOURCEID_RX(interface, tag) ((interface * BOSTAN_SYNC_CREATE_PER_DMA) + (tag - BOSTAN_SYNC_RX_OFF)) /**< synctab.rxs' index. */
#define RESOURCEID_TX(interface)      (interface * BOSTAN_SYNC_OPEN_PER_DMA)                                /**< synctab.txs' index. */

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define UNDERLYING_RX_TAG(syncid)       ((syncid % BOSTAN_SYNC_CREATE_PER_DMA) + BOSTAN_SYNC_RX_OFF) /**< Receiver tag ID.         */
#define UNDERLYING_RX_INTERFACE(syncid) (syncid / BOSTAN_SYNC_CREATE_PER_DMA)                        /**< Receiver DMA channel ID. */
#define UNDERLYING_TX_TAG(syncid)       (BOSTAN_SYNC_CNOC_TX_BASE)                                   /**< Transfer tag ID.         */
#define UNDERLYING_TX_INTERFACE(syncid) (syncid)                                                     /**< Transfer DMA channel ID. */
/**@}*/

/**
 * @brief Assess limits of the receiver resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_rx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, MPPA256_SYNC_CREATE_MAX);
}

/**
 * @brief Assess limits of the sender resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_tx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, MPPA256_SYNC_OPEN_MAX);
}

/**
 * @brief Sync local point validation.
 *
 * @param nodenum Logic ID of local node.
 * @param nodes   IDs of target NoC nodes.
 * @param nnodes  Number of target NoC nodes.
 *
 * @return Non zero if local point is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_is_local(int nodenum, const int *nodes, int nnodes)
{
	/* Underlying NoC node SHOULD be here. */
	if (nodenum != nodes[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodenum == nodes[i])
			return (0);

	return (1);
}

/**
 * @brief Sync remote point validation.
 *
 * @param nodenum Logic ID of local node.
 * @param nodes   IDs of target NoC nodes.
 * @param nnodes  Number of target NoC nodes.
 *
 * @return Zero if remote point is valid and non zero otherwise.
 */
PRIVATE int mppa256_sync_is_remote(int nodenum, const int *nodes, int nnodes)
{
	int found = 0;

	/* Underlying NoC node SHOULD NOT be here. */
	if (nodenum == nodes[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodenum == nodes[i])
			found++;

	if (found != 1)
		return (0);

	return (1);
}

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PRIVATE void mppa256_sync_it_handler(int interface, int tag)
{
	int syncid;

	UNUSED(interface);
	UNUSED(tag);

	syncid = RESOURCEID_RX(interface, tag);

	if (resource_is_used(&synctab.rxs[syncid].resource))
		k1b_spinlock_unlock(&synctab.rxs[syncid].lock);
	
	if (bostan_dma_control_config(interface, tag, synctab.rxs[syncid].mask, mppa256_sync_it_handler) < 0)
		kpanic("[hal][sync] Reconfiguration of the sync falied!");
}

/**
 * @brief Associate a underliyng DMA RX channel.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_rx_interface(const int *nodes, int nnodes)
{
	int clusterid;
	int _nodes[BOSTAN_NR_NOC_NODES];

	clusterid = bostan_cluster_get_id();

	/* Logic IDs are correctly. */
	if (bostan_nodes_convert(_nodes, nodes, nnodes) < 0)
		return (-EINVAL);

	for (int i = 0; i < nnodes; i++)
	{
		if (WITHIN(_nodes[i], clusterid, clusterid + BOSTAN_NR_INTERFACES))
		{
			int interface = _nodes[i] - clusterid;
			int tag = bostan_node_sync_tag(_nodes[i]);
			int syncid = RESOURCEID_RX(interface, tag);

			if (!resource_is_used(&synctab.rxs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

/**
 * @brief Allocates and configures the receiving side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 * @param type   Type of synchronization point.
 * @param aiocb  Asynchronous operation control.
 *
 * @return The tag of underlying resource ID.
 */
PUBLIC int mppa256_sync_create(const int *nodes, int nnodes, int type, struct aiocb * aiocb)
{
	int ret;
	int tag;
	int syncid;
	int nodenum;
	int interface;
	uint64_t mask;

	if (nodes == NULL || aiocb == NULL)
		return (-EINVAL);
	
	if (!WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	if ((interface = mppa256_sync_select_rx_interface(nodes, nnodes)) < 0)
		return (-EBUSY);

	nodenum = bostan_node_get_num(bostan_cluster_get_id() + interface);

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		if (!mppa256_sync_is_remote(nodenum, nodes, nnodes))
			return (-ECONNREFUSED);

		mask = 0;
	}

	/* Gather. */
	else
	{
		if (!mppa256_sync_is_local(nodenum, nodes, nnodes))
			return (-ECONNREFUSED);

		mask = 0;
		for (int i = 1; i < nnodes; i++)
			mask |= 1 << nodes[i];
		mask = ~mask;
	}

	tag = bostan_node_sync_tag(bostan_node_convert_id(nodes[0]));
	syncid = RESOURCEID_RX(interface, tag);

	if (resource_is_used(&synctab.rxs[syncid].resource))
		return (-EALREADY);

	if ((ret = bostan_dma_control_create(interface, tag, mask, mppa256_sync_it_handler)) < 0)
		return (ret);

	synctab.rxs[syncid].mask = mask;
	k1b_spinlock_lock(&synctab.rxs[syncid].lock);

	resource_set_used(&synctab.rxs[syncid].resource);
	resource_set_async(&synctab.rxs[syncid].resource);
	resource_set_notbusy(&synctab.rxs[syncid].resource);

	aiocb->fd   = syncid;
	aiocb->type = BOSTAN_NOC_RX_TYPE;
	aiocb->lock = &synctab.rxs[syncid].lock;

	return (syncid);
}

/**
 * @brief Associate a underliyng DMA TX channel.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_tx_interface(const int *_nodes, int nnodes)
{
	int clusterid;

	clusterid = bostan_cluster_get_id();

	for (int i = 0; i < nnodes; i++)
	{
		if (WITHIN(_nodes[i], clusterid, clusterid + BOSTAN_NR_INTERFACES))
		{
			int interface = _nodes[i] - clusterid;
			int syncid = RESOURCEID_TX(interface);

			if (!resource_is_used(&synctab.txs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

/**
 * @todo Comment this function.
 */
PUBLIC int mppa256_sync_open(const int *nodes, int nnodes, int type)
{
	int ret;
	int tag;
	int syncid;
	int nodenum;
	int interface;
	int clusterid;
	int _nodes[BOSTAN_NR_NOC_NODES];

	if (nodes == NULL || !WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	if (bostan_nodes_convert(_nodes, nodes, nnodes) < 0)
		return (-EINVAL);

	if ((interface = mppa256_sync_select_tx_interface(_nodes, nnodes)) < 0)
		return (-EBUSY);

	syncid    = RESOURCEID_TX(interface);
	clusterid = bostan_cluster_get_id();
	nodenum   = bostan_node_get_num(clusterid + interface);

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
#if 1 /* First alternative of the check. */
		if (nodes[0] != nodenum)
			return (-ECONNREFUSED);
#else
		if (!WITHIN(_nodes[0], clusterid, clusterid + BOSTAN_NR_INTERFACES))
			return (-EINVAL);

		if (!mppa256_sync_is_local(nodeid, nodes, nnodes))
			return (-ECONNREFUSED);
#endif
		kmemcpy(synctab.txs[syncid].targets, &_nodes[1], (nnodes - 1));
		synctab.txs[syncid].ntargets = (nnodes - 1);
		synctab.txs[syncid].mask     = ~(0ULL);
	}

	/* Unicast. */
	else
	{
		if (!mppa256_sync_is_remote(nodenum, nodes, nnodes))
			return (-ECONNREFUSED);

		kmemcpy(synctab.txs[syncid].targets, &_nodes[0], 1);
		synctab.txs[syncid].ntargets = 1;
		synctab.txs[syncid].mask     = 1ULL << (nodenum);
	}

	synctab.txs[syncid].target_tag = bostan_node_sync_tag(_nodes[0]);
	
	tag = UNDERLYING_TX_TAG(syncid);

	if ((ret = bostan_dma_control_open(interface, tag)) < 0)
		return (ret);

	resource_set_used(&synctab.txs[syncid].resource);
	resource_set_sync(&synctab.txs[syncid].resource);
	resource_set_notbusy(&synctab.txs[syncid].resource);

	return (syncid);
}

/**
 * @todo Document this function.
 */
PUBLIC int mppa256_sync_unlink(int syncid)
{
	int ret;
	int tag;
	int interface;

	if (!mppa256_sync_rx_is_valid(syncid))
		return (-EBADF);

	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		return (-EACCES);

	tag       = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	if ((ret = bostan_dma_control_unlink(interface, tag)) < 0)
		return (ret);

	resource_free(&syncpools.rx_pool, syncid);
	k1b_spinlock_unlock(&synctab.rxs[syncid].lock);

	return (0);
}

/**
 * @brief Free the sender resources on a specific DMA channel.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int mppa256_sync_close(int syncid)
{
	int ret;
	int tag;
	int interface;

	if (!mppa256_sync_tx_is_valid(syncid))
		return (-EBADF);

	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		return (-EACCES);
	tag       = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

	if ((ret = bostan_dma_control_close(interface, tag)) < 0)
		return (ret);

	resource_free(&syncpools.tx_pool, syncid);

	return (0);
}

/**
 * @brief Wait signal on a specific synchronization point.
 *
 * @param aiocb Asynchronous operation control.
 *
 * @return Zero if wait signal correctly and non zero otherwise.
 * 
 * @todo How make the slave wait on lock without the need to
 * access the master's private structures? (microkernel approach)
 */
PUBLIC int mppa256_sync_wait(struct aiocb * aiocb)
{
	/* Bad aiocb. */
	if (aiocb == NULL)
		return (-EINVAL);
	
	k1b_dcache_inval();

	if (!mppa256_sync_rx_is_valid(aiocb->fd))
		return (-EBADF);
	
	if (aiocb->type != BOSTAN_NOC_RX_TYPE)
		return (-EINVAL);
	
	/* Bad lock. */
	if (aiocb->lock == NULL)
		return (-EINVAL);

	/* Waits for the handler release the lock. */
	k1b_spinlock_lock(aiocb->lock);

	return (0);
}

/**
 * @brief Send signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if send signal correctly and non zero otherwise.
 */
PUBLIC int mppa256_sync_signal(int syncid)
{
	int tag;
	int interface;

	if (!mppa256_sync_tx_is_valid(syncid))
		return (-EBADF);

	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		return (-EACCES);

	tag       = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

	return bostan_dma_control_signal(
		interface,
		tag,
		synctab.txs[syncid].targets,
		synctab.txs[syncid].ntargets,
		synctab.txs[syncid].target_tag,
		synctab.txs[syncid].mask
	);
}
