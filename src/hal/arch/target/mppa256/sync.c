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
		struct resource resource; /**< Control flags. */
		uint64_t mask;			  /**< Initial value. */
	} rxs[MPPA256_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		struct resource resource; /**< Control flags. */
	} txs[MPPA256_SYNC_OPEN_MAX];
} synctab;

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
#define RESOURCEID_TX(interface, tag) (interface * BOSTAN_SYNC_CREATE_PER_DMA)                                /**< synctab.txs' index. */

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define UNDERLYING_RX_TAG(syncid) ((syncid % BOSTAN_SYNC_CREATE_PER_DMA) + BOSTAN_SYNC_RX_OFF) /**< Receiver tag ID.         */
#define UNDERLYING_RX_INTERFACE(syncid) (syncid / BOSTAN_SYNC_CREATE_PER_DMA)                  /**< Receiver DMA channel ID. */
#define UNDERLYING_TX_TAG(syncid) (BOSTAN_SYNC_CNOC_TX_TAG)                                    /**< Transfer tag ID.         */
#define UNDERLYING_TX_INTERFACE(syncid) (syncid)                                               /**< Transfer DMA channel ID. */
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
 * @brief Associate a underliyng DMA channel.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_interface(const int *nodes, int nnodes)
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
 * @brief Allocates and configures the receiving side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 * @param type   Type of synchronization point.
 *
 * @return The tag of underlying resource ID.
 */
PUBLIC int mppa256_sync_create(const int *nodes, int nnodes, int type)
{
	int ret;
	int tag;
	int syncid;
	int nodenum;
	int interface;
	uint64_t mask;

	if (nodes == NULL || !WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	if ((interface = mppa256_sync_select_interface(nodes, nnodes)) < 0)
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

	if ((ret = bostan_dma_control_create(interface, tag, mask)) < 0)
		return (ret);

	synctab.rxs[syncid].mask = mask;

	resource_set_used(&synctab.rxs[syncid].resource);
	resource_set_async(&synctab.rxs[syncid].resource);
	resource_set_notbusy(&synctab.rxs[syncid].resource);

	return (syncid);
}

/**
 * @todo Comment this function.
 */
PUBLIC int mppa256_sync_open(void)
{
	int ret;
	int tag;
	int syncid;
	int interface;

	/* Allocate a synchronization point. */
	if ((syncid = resource_alloc(&syncpools.tx_pool)) < 0)
		return (-EMLINK);

	tag = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

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

	tag = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	if ((ret = bostan_dma_control_unlink(interface, tag)) < 0)
		return (ret);

	resource_free(&syncpools.rx_pool, syncid);

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

	tag = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);

	if ((ret = bostan_dma_control_close(interface, tag)) < 0)
		return (ret);

	resource_free(&syncpools.tx_pool, syncid);

	return (0);
}

/**
 * @brief Wait signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if wait signal correctly and non zero otherwise.
 */
PUBLIC int mppa256_sync_wait(int syncid)
{
	int ret;
	int tag;
	int interface;

	if (!mppa256_sync_rx_is_valid(syncid))
		return (-EBADF);

	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		return (-EACCES);

	tag = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	k1b_dcache_inval();

	if ((ret = bostan_dma_control_wait(interface, tag)) < 0)
		return (ret);

	if ((ret = bostan_dma_control_config(interface, tag, synctab.rxs[syncid].mask)) < 0)
		return (ret);

	return (0);
}

/**
 * @brief Send signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if send signal correctly and non zero otherwise.
 */
PUBLIC int mppa256_sync_signal(int syncid, const int *nodes, int nnodes, int type)
{
	int tag;
	int interface;
	int clusterid;
	const int *targets;
	int target_tag;
	int ntargets;
	int _nodes[BOSTAN_NR_NOC_NODES];
	k1b_dword_t mask;

	if (!mppa256_sync_tx_is_valid(syncid))
		return (-EBADF);

	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		return (-EACCES);

	if (nodes == NULL || !WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		return (-EINVAL);

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		return (-EINVAL);

	if (bostan_nodes_convert(_nodes, nodes, nnodes) < 0)
		return (-EINVAL);

	interface = UNDERLYING_TX_INTERFACE(syncid);
	clusterid = bostan_cluster_get_id();

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		if (!WITHIN(_nodes[0], clusterid, clusterid + BOSTAN_NR_INTERFACES))
			return (-EINVAL);

		if (!mppa256_sync_is_local(nodes[0], nodes, nnodes))
			return (-ECONNREFUSED);

		targets = &_nodes[1];
		ntargets = nnodes - 1;
		mask = ~0;
	}

	/* Unicast. */
	else
	{
		if (!WITHIN(_nodes[1], clusterid, clusterid + BOSTAN_NR_INTERFACES))
			return (-EINVAL);

		if (!mppa256_sync_is_remote(nodes[1], nodes, nnodes))
			return (-ECONNREFUSED);

		targets = &_nodes[0];
		ntargets = 1;
		mask = 1 << nodes[1];
	}

	tag = UNDERLYING_TX_TAG(syncid);
	target_tag = bostan_node_sync_tag(_nodes[0]);

	return bostan_dma_control_signal(
		interface,
		(clusterid + interface),
		tag,
		targets,
		ntargets,
		target_tag,
		mask
	);
}
