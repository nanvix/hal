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
 * @brief Default C-NoC TX buffer
 */
#define DEFAULT_TX_TAG 0

/**
 * @brief Auxiliar macros
 */
#define RESOURCEID_RX(interface, tag) ((interface * MPPA256_NR_SYNC_RX_DMA) + tag)
#define RESOURCEID_TX(interface, tag) ((interface * MPPA256_NR_SYNC_TX_DMA) + tag)
#define UNDERLYING_RX_TAG(syncid) (syncid % MPPA256_NR_SYNC_RX_DMA)
#define UNDERLYING_TX_TAG(syncid) (DEFAULT_TX_TAG)
#define UNDERLYING_RX_INTERFACE(syncid) (syncid / MPPA256_NR_SYNC_RX_DMA)
#define UNDERLYING_TX_INTERFACE(syncid) (synctab.txs[syncid].interface)

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
		uint64_t mask;            /**< Initial value. */
		uint8_t type;             /**< Type.          */
	} rxs[MPPA256_NR_SYNC_RX_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		struct resource resource; /**< Control flags.          */
		uint32_t targets    : 24; /**< Target node IDs.        */
		uint32_t target_tag :  8; /**< Target buffer ID.       */
		uint8_t  ntargets   :  5; /**< Amount of target nodes. */
		uint8_t  type       :  1; /**< Type.                   */
		uint8_t  interface  :  2; /**< Associate interface.    */
	} txs[MPPA256_NR_SYNC_TX_MAX];
} synctab;

/**
 * @brief Pools of Synchronization Resource
 */
PRIVATE struct syncpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} syncpools = {
	.rx_pool = {synctab.rxs, MPPA256_NR_SYNC_RX_MAX, sizeof(struct rx)},
	.tx_pool = {synctab.txs, MPPA256_NR_SYNC_TX_MAX, sizeof(struct tx)},
};

/**
 * @brief Assess limits of the receiver resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_rx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, MPPA256_NR_SYNC_RX_MAX);
}

/**
 * @brief Assess limits of the sender resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int mppa256_sync_tx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, MPPA256_NR_SYNC_TX_MAX);
}

/**
 * @brief Builds the list of RX NoC nodes.
 *
 * @param ranks  Target list of RX NoC nodes.
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE void mppa256_sync_ranks(int *ranks, const int *nodes, int nnodes)
{
	int j, tmp;
	int nodeid;

	j = 1;
	nodeid = processor_node_get_id();

	/* Build list of RX NoC nodes. */
	for (int i = 0; i < nnodes; i++)
	{
		if (nodes[i] == nodeid)
			j = i;

		ranks[i] = nodes[i];
	}

	tmp = ranks[1];
	ranks[1] = ranks[j];
	ranks[j] = tmp;
}

/**
 * @brief Associate a underliyng DMA channel.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * @param class  Class of synchronization point.
 * 
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_interface(const int *nodes, int nnodes, int mode)
{
	int clusterid;

	clusterid = bostan_cluster_get_id();

	if (bostan_cluster_is_ccluster(clusterid))
		return 0;

	for (int i = 0; i < nnodes; i++)
	{
		if (WITHIN(nodes[i], clusterid, clusterid+K1BIO_CORES_NUM))
		{
			int resourceid;
			int interface = nodes[i] - clusterid;
			int tag = bostan_node_sync_tag(nodes[i]);

			if (mode == MPPA256_SYNC_RX)
				resourceid = RESOURCEID_RX(interface, tag);
			else
				resourceid = RESOURCEID_TX(interface, tag);
		
			if (!resource_is_used(&synctab.rxs[resourceid].resource))
				return (nodes[i] - clusterid);
		}
	}

	return -EINVAL;
}

/**
 * @brief Sync local point validation.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * 
 * @return Zero if local point is valid and non zero otherwise.
 */
PRIVATE int mppa256_sync_local_point_check(int nodeid, const int *nodes, int nnodes)
{
	/* Underlying NoC node SHOULD be here. */
	if (nodeid != nodes[0])
		return -EINVAL;

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodeid == nodes[i])
			return -EINVAL;

	return 0;
}

/**
 * @brief Sync remote point validation.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * 
 * @return Zero if remote point is valid and non zero otherwise.
 */
PRIVATE int mppa256_sync_remote_point_check(int nodeid, const int *nodes, int nnodes)
{
	int found = 0;

	/* Underlying NoC node SHOULD NOT be here. */
	if (nodeid == nodes[0])
		return -EINVAL;

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
		if (nodeid == nodes[i])
			found++;

	if (found != 1)
		return -EINVAL;
	
	return 0;
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
	int tag;
	int nodeid;
	int interface;
	int resourceid;
	int ranks[BOSTAN_NR_NOC_NODES];
	int _nodes[BOSTAN_NR_NOC_NODES];
	uint64_t mask;

	if (nodes == NULL || !WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		goto error0;

	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		goto error0;

	/* Convert NoC node numbers into IDs. */
	if (bostan_nodes_convert(_nodes, nodes, nnodes) < 0)
		goto error0;

	if((interface = mppa256_sync_select_interface(_nodes, nnodes, MPPA256_SYNC_RX)) < 0)
		goto error0;

	nodeid = bostan_cluster_get_id() + interface;

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{

		if (mppa256_sync_remote_point_check(nodeid, _nodes, nnodes) != 0)
			goto error0;
	}
	/* Gather. */
	else if (mppa256_sync_local_point_check(nodeid, _nodes, nnodes) != 0)
		goto error0;

	/* Note: Study the need for sync_ranks */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
		mppa256_sync_ranks(ranks, _nodes, nnodes);
	else
		memcpy(ranks, _nodes, nnodes*sizeof(int));

	tag = bostan_node_sync_tag(ranks[0]);
	resourceid = RESOURCEID_RX(interface, tag);

	if (resource_is_used(&synctab.rxs[resourceid].resource))
		goto error0;

    /* Gather. */
    mask = 0;

     /* Broadcast. */
    if (type == MPPA256_SYNC_ALL_TO_ONE)
	{
        for (int i = 1; i < nnodes; i++)
            mask |= 1 << bostan_node_get_num(ranks[i]);

		mask = ~mask;
	}

	if (bostan_dma_control_create(interface, tag, mask) != 0)
		goto error0;

	synctab.rxs[resourceid].mask = mask;
	synctab.rxs[resourceid].type = type;

	resource_set_used(&synctab.rxs[resourceid].resource);
	resource_set_async(&synctab.rxs[resourceid].resource);
	resource_set_notbusy(&synctab.rxs[resourceid].resource);

	return resourceid;

error0:
	return (-EAGAIN);
}

/**
 * @brief Allocates and configures the sending side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * @param type   Type of synchronization point.
 * 
 * @return The tag of underlying resource ID.
 */
PUBLIC int mppa256_sync_open(const int *nodes, int nnodes, int type)
{
	int nodeid;
	int interface;
	int resourceid;
	int _nodes[BOSTAN_NR_NOC_NODES];

	if (nodes == NULL || !WITHIN(nnodes, 2, BOSTAN_NR_NOC_NODES))
		goto error0;
	
	if (type != MPPA256_SYNC_ALL_TO_ONE && type != MPPA256_SYNC_ONE_TO_ALL)
		goto error0;

	/* Convert NoC node numbers into IDs. */
	if (processor_nodes_convert(_nodes, nodes, nnodes) < 0)
		goto error0;
	
	if((interface = mppa256_sync_select_interface(_nodes, nnodes, MPPA256_SYNC_TX)) < 0)
		goto error0;
	
	nodeid = bostan_cluster_get_id() + interface;

	/* Broadcast. */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		if (mppa256_sync_local_point_check(nodeid, _nodes, nnodes) != 0)
			goto error0;
	}
	/* Gather. */
	else if (mppa256_sync_remote_point_check(nodeid, _nodes, nnodes) != 0)
		goto error0;

	/* Allocate a synchronization point. */
	if ((resourceid = resource_alloc(&syncpools.tx_pool)) < 0)
		goto error0;

	synctab.txs[resourceid].targets = 0;
	synctab.txs[resourceid].target_tag = bostan_node_sync_tag(_nodes[0]);
	synctab.txs[resourceid].type       = type;
	synctab.txs[resourceid].interface  = interface;

    /* Broadcast. */
    if (type == MPPA256_SYNC_ONE_TO_ALL)
    {
		for (int i = 1; i < (nnodes - 1); i++)
			synctab.txs[resourceid].targets |= 1 << bostan_node_get_num(_nodes[i]);
        synctab.txs[resourceid].ntargets = (nnodes - 1);
    }

    /* Gather. */
    else
    {
		synctab.txs[resourceid].targets |= 1 << bostan_node_get_num(_nodes[0]);
        synctab.txs[resourceid].ntargets = 1;
    }

	resource_set_used(&synctab.txs[resourceid].resource);
	resource_set_sync(&synctab.txs[resourceid].resource);
	resource_set_notbusy(&synctab.txs[resourceid].resource);

	return resourceid;

error0:
	return (-EAGAIN);
}

/**
 * @brief Free the receiver resources.
 *
 * @param  tag Resource ID.
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int mppa256_sync_unlink(int syncid)
{
	int tag;
	int interface;

	if (!mppa256_sync_rx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		goto error0;

	tag = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	if (bostan_dma_control_unlink(interface, tag) != 0)
		goto error0;

	resource_free(&syncpools.rx_pool, syncid);

	return (0);

error0:
	return (-EAGAIN);
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
	if (!mppa256_sync_tx_is_valid(syncid))
		goto error0;

	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		goto error0;

	resource_free(&syncpools.tx_pool, syncid);

	return (0);

error0:
	return (-EAGAIN);
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
	int tag;
	int interface;

	if (!mppa256_sync_rx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		goto error0;

	tag = UNDERLYING_RX_TAG(syncid);
	interface = UNDERLYING_RX_INTERFACE(syncid);

	k1b_dcache_inval();

	if (bostan_dma_control_wait(interface, tag) != 0)
		goto error0;

	if (bostan_dma_control_config(interface, tag, synctab.rxs[syncid].mask) != 0)
		goto error0;

	return (0);

error0:
	return (-EAGAIN);
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
	int nodeid;
	int interface;
	int target_ids[BOSTAN_NR_NOC_NODES];
	int logic_ids[BOSTAN_NR_NOC_NODES];
	uint64_t mask;

	if (!mppa256_sync_tx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		goto error0;

	tag = UNDERLYING_TX_TAG(syncid);
	interface = UNDERLYING_TX_INTERFACE(syncid);
	nodeid = bostan_cluster_get_id() + interface;

	if (bostan_dma_control_open(interface, tag) != 0)
		goto error1;
	
	/* Broadcast. */
    if (synctab.txs[syncid].type == MPPA256_SYNC_ONE_TO_ALL)
        mask = ~0;

    /* Unicast. */
    else
        mask = 1 << processor_node_get_num(nodeid);

	struct tx *tx = &synctab.txs[syncid];

	for (unsigned int i = 0, j = 0; i < BOSTAN_NR_NOC_NODES && j < tx->ntargets; i++)
		if (tx->targets & (1 << i))
			logic_ids[j++] = i;
	
	bostan_nodes_convert(target_ids, logic_ids, tx->ntargets);

	if (bostan_dma_control_signal(interface, nodeid, tag, target_ids, tx->ntargets, tx->target_tag, mask) != 0)
		goto error1;
	
	if (bostan_dma_control_close(interface, tag) != 0)
		goto error1;

	return (0);

error1:
	kpanic("[PANIC][target][sync] failed to send signal");
error0:
	return (-EAGAIN);
}
