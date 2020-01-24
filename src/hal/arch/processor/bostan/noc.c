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
#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>

/*=======================================================================*
 * Private defines and Control variables                                 *
 *=======================================================================*/

/**
 * @brief Macro to get the number of NoC nodes inside an IO Cluster.
 */
#define BOSTAN_NODES_PER_IOCLUSTER \
	(PROCESSOR_NOC_IONODES_NUM / PROCESSOR_IOCLUSTERS_NUM)

/**
 * @brief Macro to get the number of NoC nodes inside an Compute Cluster.
 */
#define BOSTAN_NODES_PER_CCLUSTER \
	(PROCESSOR_NOC_CNODES_NUM / PROCESSOR_CCLUSTERS_NUM)

/**
 * @brief Map of logical IDs to physical IDs of Nodes.
 */
PRIVATE const int bostan_nodeids[PROCESSOR_NOC_NODES_NUM] = {
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 + 0,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 + 1,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 + 2,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 + 3,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 + 0,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 + 1,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 + 2,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 + 3,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER0,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER1,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER2,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER3,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER4,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER5,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER6,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER7,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER8,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER9,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER10,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER11,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER12,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER13,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER14,
	BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER15
};

/**
 * @brief Auxiliar macros for Core Numbering with Node numbers.
 *
* @details Since only IO Clusters have more than one valid node number,
 * Compute Clusters only simulate the behavior of being able to configure
 * other numbers.
 */
#ifdef __node__
	#define BOSTAN_CORENUMS_NUM           (1)                          /**< Size of corenum array.           */
	#define BOSTAN_CORENUMS_INDEX(coreid) (0)                          /**< Core index in corenum array.     */
	#define BOSTAN_NODES_AVAILABLE        (BOSTAN_NODES_PER_CCLUSTER)  /**< Amount of node numbers avaiable. */
#else
	#define BOSTAN_CORENUMS_NUM           (K1BIO_CORES_NUM)            /**< Size of corenum array.           */
	#define BOSTAN_CORENUMS_INDEX(coreid) (coreid)                     /**< Core index in corenum array.     */
	#define BOSTAN_NODES_AVAILABLE        (BOSTAN_NODES_PER_IOCLUSTER) /**< Amount of node numbers avaiable. */
#endif

/**
 * @brief Map of core ids to nodenums.
 */
PRIVATE int bostan_corenums[BOSTAN_CORENUMS_NUM];

/*=======================================================================*
 * bostan_processor_noc_setup()                                          *
 *=======================================================================*/

/**
 * @brief Initializes the noc interface.
 */
PUBLIC void bostan_processor_noc_setup(void)
{
	int nodenum;

	bostan_dma_init();

	nodenum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	for (int i = 0; i < BOSTAN_CORENUMS_NUM; ++i)
		bostan_corenums[i] = nodenum;
}

/*=======================================================================*
 * bostan_processor_noc_cluster_to_node_num()                            *
 *=======================================================================*/

/**
 * @brief Converts a cluster number to NoC node number.
 *
 * @param clusternum Target node number.
 *
 * @returns The logical node number in which the cluster number @p
 * clusternum is located.
 */
PUBLIC int bostan_processor_noc_cluster_to_node_num(int clusternum)
{
	if (cluster_is_iocluster(clusternum))
		return (clusternum * BOSTAN_NODES_PER_IOCLUSTER);

	else if (cluster_is_ccluster(clusternum))
		return (clusternum + PROCESSOR_NOC_IONODES_NUM - PROCESSOR_IOCLUSTERS_NUM);

	return (-EINVAL);
}

/*=======================================================================*
 * bostan_processor_noc_is_ionode()                                      *
 *=======================================================================*/

/**
 * @brief Asserts whether a NoC node is attached to an IO cluster.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns One if the target NoC node is attached to an IO cluster,
 * and zero otherwise.
 */
PUBLIC int bostan_processor_noc_is_ionode(int nodenum)
{
	return (WITHIN(nodenum, 0, PROCESSOR_NOC_IONODES_NUM));
}

/*=======================================================================*
 * bostan_processor_noc_is_cnode()                                       *
 *=======================================================================*/

/**
 * @brief Asserts whether a NoC node is attached to a compute cluster.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns One if the target NoC node is attached to a compute
 * cluster, and zero otherwise.
 */
PUBLIC int bostan_processor_noc_is_cnode(int nodenum)
{
	return (
		WITHIN(
			nodenum,
			PROCESSOR_NOC_IONODES_NUM,
			PROCESSOR_NOC_IONODES_NUM + PROCESSOR_NOC_CNODES_NUM
		)
	);
}

/*=======================================================================*
 * bostan_processor_noc_node_to_cluster_num()                            *
 *=======================================================================*/

/**
 * @brief Converts a NoC node number to cluster number.
 *
 * @param nodenum Target node number.
 *
 * @returns The logical cluster number in which the node number @p
 * nodenum is located.
 */
PUBLIC int bostan_processor_noc_node_to_cluster_num(int nodenum)
{
	if (processor_noc_is_ionode(nodenum))
		return (nodenum / BOSTAN_NODES_PER_IOCLUSTER);

	else if (processor_noc_is_cnode(nodenum))
		return (nodenum - PROCESSOR_NOC_IONODES_NUM + PROCESSOR_IOCLUSTERS_NUM);

	return (-EINVAL);
}

/*=======================================================================*
 * bostan_processor_node_get_num()                                       *
 *=======================================================================*/

/**
 * @brief Gets the logic number of the target NoC node
 * attached with a core.
 *
 * @param coreid Attached core ID.
 *
 * @returns The logic number of the target NoC node attached
 * with the @p coreid.
 */
PUBLIC int bostan_processor_node_get_num(int coreid)
{
	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	return (bostan_corenums[BOSTAN_CORENUMS_INDEX(coreid)]);
}

/*=======================================================================*
 * bostan_processor_node_set_num()                                       *
 *=======================================================================*/

/**
 * @brief Exchange the logic number of the target NoC node
 * attached with a core.
 *
 * @param coreid  Attached core ID.
 * @param nodenum Logic ID of the target NoC node.
 * 
 * @returns Zero if the target NoC node is successfully attached
 * to the requested @p coreid, and non zero otherwise.
 */
PUBLIC int bostan_processor_node_set_num(int coreid, int nodenum)
{
	int cluster_nodenum;

	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	cluster_nodenum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	/* Invalid nodenum. */
	if (!WITHIN(nodenum, cluster_nodenum, cluster_nodenum + BOSTAN_NODES_AVAILABLE))
		return (-EINVAL);

	bostan_corenums[BOSTAN_CORENUMS_INDEX(coreid)] = nodenum;

	return (0);
}

/*=======================================================================*
 * bostan_processor_noc_node_num_to_id()                                 *
 *=======================================================================*/

/**
 * @brief Converts a nodes list.
 *
 * @param nodenum Logic ID of the Target node.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int bostan_processor_noc_node_num_to_id(int nodenum)
{
	/* Invalid nodenum. */
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return bostan_nodeids[nodenum];
}

/*=======================================================================*
 * bostan_processor_node_mailbox_tag()                                   *
 *=======================================================================*/

/**
 * @brief Returns the mailbox NoC tag for a target NoC node ID.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns The NoC tag attached to the underlying node ID is
 * returned.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_processor_node_mailbox_tag(int nodenum)
{
	/* Invalid nodenum. */
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return (BOSTAN_MAILBOX_RX_OFF + nodenum);
}

/*=======================================================================*
 * bostan_processor_node_portal_tag()                                    *
 *=======================================================================*/

/**
 * @brief Returns the portal NoC tag for a target NoC node ID.
 *
 * @param nodenum     ID of the target NoC node.
 *
 * @returns The NoC tag attached to the underlying node ID is
 * returned.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_processor_node_portal_tag(int nodenum)
{
	/* Invalid nodenum. */
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return (BOSTAN_PORTAL_RX_OFF + nodenum);
}

/*=======================================================================*
 * bostan_processor_node_sync_tag()                                      *
 *=======================================================================*/

/**
 * @brief Returns the synchronization NoC tag for a target NoC node ID.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_processor_node_sync_tag(int nodenum)
{
	/* Invalid nodenum. */
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return (BOSTAN_SYNC_RX_OFF + nodenum);
}
