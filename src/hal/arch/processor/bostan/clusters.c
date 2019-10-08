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
 * @brief Map of logical IDs to physical IDs of Clusters.
 */
PRIVATE const int bostan_processor_clusterids[PROCESSOR_CLUSTERS_NUM] = {
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0,
	BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1,
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

/*============================================================================*
 * bostan_cluster_get_num()                                                   *
 *============================================================================*/

/**
 * @brief Retrieves the logical number of the underlying cluster.
 */
PUBLIC int bostan_cluster_get_num(void)
{
    int clusterid;

    clusterid = __k1_get_cluster_id();

    /* IO Cluster 0 */
    if (
        WITHIN(
            clusterid,
            BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0,
            BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 + BOSTAN_PROCESSOR_NOC_NODES_PER_IOCLUSTER
        )
    )
        return (BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER0);

    /* IO Cluster 1 */
    if (
        WITHIN(
            clusterid,
            BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1,
            BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 + BOSTAN_PROCESSOR_NOC_NODES_PER_IOCLUSTER
        )
    )
        return (BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER1);

    /* Compute Cluster */
    return (PROCESSOR_IOCLUSTERS_NUM + clusterid);
}

/*============================================================================*
 * bostan_cluster_is_compute()                                                *
 *============================================================================*/

/**
 * @brief Asserts whether or not the target cluster is a compute cluster.
 *
 * @param clusternum Logical ID of the target cluster.
 *
 * @return Non zero if the target cluster is a compute cluster and
 * zero otherwise.
 */
PUBLIC int bostan_cluster_is_compute(int clusternum)
{
    return (
        WITHIN(
            clusternum,
            BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER0,
            BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER0 + PROCESSOR_CCLUSTERS_NUM
        )
    );
}

/*============================================================================*
 * bostan_cluster_is_io()                                                     *
 *============================================================================*/

/**
 * @brief Asserts whether or not the target cluster is an IO cluster.
 *
 * @param clusternum Logical ID of the target cluster.
 *
 * @return Non zero if the target cluster is an IO cluster and
 * zero otherwise.
 */
PUBLIC int bostan_cluster_is_io(int clusternum)
{
    return (
        (clusternum == BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER0) ||
        (clusternum == BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER1)
    );
}

/*============================================================================*
 * bostan_processor_poweroff()                                                *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC NORETURN void bostan_processor_poweroff(void)
{
	if (cluster_get_num() == PROCESSOR_CLUSTERNUM_MASTER)
	{
		kprintf("[hal][processor] powering off...");
		k1b_core_halt = 0;
	}

	cluster_poweroff();
}
