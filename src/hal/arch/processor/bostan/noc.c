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

/* Must come fist. */
#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>

/**
 * @brief Macro to get the number of NoC nodes inside an IO Cluster.
 */
#define BOSTAN_PROCESSOR_NOC_NODES_PER_IOCLUSTER (PROCESSOR_NOC_IONODES_NUM / PROCESSOR_IOCLUSTERS_NUM)

/**
 * @brief Map of logical IDs to physical IDs of Nodes.
 */
PRIVATE const int bostan_processor_nodeids[PROCESSOR_NOC_NODES_NUM] = {
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
		return (clusternum * BOSTAN_PROCESSOR_NOC_NODES_PER_IOCLUSTER);

	else if (cluster_is_ccluster(clusternum))
		return (clusternum + PROCESSOR_NOC_IONODES_NUM - PROCESSOR_IOCLUSTERS_NUM);

	return (-EINVAL);
}


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
		return (nodenum / BOSTAN_PROCESSOR_NOC_NODES_PER_IOCLUSTER);

	else if (processor_noc_is_cnode(nodenum))
		return (nodenum - PROCESSOR_NOC_IONODES_NUM + PROCESSOR_IOCLUSTERS_NUM);

	return (-EINVAL);
}

/**
 * @brief Gets the logic number of a NoC node.
 *
 * @returns The logic number of the target NoC node.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_processor_node_get_num(void)
{
	int coreid;
	int clusternum;

	coreid     = core_get_id();
	clusternum = cluster_get_num();

	return (bostan_processor_noc_cluster_to_node_num(clusternum) + coreid);
}

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
		
	return bostan_processor_nodeids[nodenum];
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
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return (BOSTAN_SYNC_RX_OFF + nodenum);
}
