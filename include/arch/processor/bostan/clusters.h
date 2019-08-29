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

#ifndef PROCESSOR_BOSTAN_CLUSTERS_H_
#define PROCESSOR_BOSTAN_CLUSTERS_H_

	/* Processor Interface Implementation */
	#include <arch/processor/bostan/_bostan.h>

/**
 * @addtogroup processor-bostan-clusters Clusters
 * @ingroup processor-bostan
 *
 * @brief Clusters
 */
/**@*/

	#include <arch/processor/bostan/noc/dma.h>
	#include <nanvix/klib.h>

	/**
	 * @brief Number of IO Clusters in the platform.
	 */
	#define BOSTAN_PROCESSOR_IOCLUSTERS_NUM 2

	/**
	 * @brief Number of Compute Clusters in the platform.
	 */
	#define BOSTAN_PROCESSOR_CCLUSTERS_NUM 16

	/**
	 * @name Clusters' Physical ID
	 */
	/**@{*/
	#define BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER0 128 /**< IO Cluster 0       */
	#define BOSTAN_PROCESSOR_CLUSTERID_IOCLUSTER1 192 /**< IO Cluster 1       */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER0    0 /**< Compute Cluster  0 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER1    1 /**< Compute Cluster  1 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER2    2 /**< Compute Cluster  2 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER3    3 /**< Compute Cluster  3 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER4    4 /**< Compute Cluster  4 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER5    5 /**< Compute Cluster  5 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER6    6 /**< Compute Cluster  6 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER7    7 /**< Compute Cluster  7 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER8    8 /**< Compute Cluster  8 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER9    9 /**< Compute Cluster  9 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER10  10 /**< Compute Cluster 10 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER11  11 /**< Compute Cluster 11 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER12  12 /**< Compute Cluster 12 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER13  13 /**< Compute Cluster 13 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER14  14 /**< Compute Cluster 14 */
	#define BOSTAN_PROCESSOR_CLUSTERID_CCLUSTER15  15 /**< Compute Cluster 15 */
	/**@}*/

	/**
	 * @name Clusters' Logical ID
	 */
	/**@{*/
	#define BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER0  0 /**< IO Cluster 0       */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER1  1 /**< IO Cluster 1       */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER0   2 /**< Compute Cluster  0 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER1   3 /**< Compute Cluster  1 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER2   4 /**< Compute Cluster  2 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER3   5 /**< Compute Cluster  3 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER4   6 /**< Compute Cluster  4 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER5   7 /**< Compute Cluster  5 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER6   8 /**< Compute Cluster  6 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER7   9 /**< Compute Cluster  7 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER8  10 /**< Compute Cluster  8 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER9  11 /**< Compute Cluster  9 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER10 12 /**< Compute Cluster 10 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER11 13 /**< Compute Cluster 11 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER12 14 /**< Compute Cluster 12 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER13 15 /**< Compute Cluster 13 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER14 16 /**< Compute Cluster 14 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_CCLUSTER15 17 /**< Compute Cluster 15 */
	/**@}*/

	/**
	 * @brief Logical ID of master cluster.
	 */
	#define BOSTAN_PROCESSOR_CLUSTERNUM_MASTER BOSTAN_PROCESSOR_CLUSTERNUM_IOCLUSTER0

	/**
	 * @brief Retrieves the logical number of the underlying cluster.
	 */
	EXTERN int bostan_cluster_get_num(void);

	/**
	 * @brief Asserts whether or not the target cluster is a compute cluster.
	 *
	 * @param clusternum Logical ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is a compute cluster and
	 * zero otherwise.
	 */
	EXTERN int bostan_cluster_is_compute(int clusternum);

	/**
	 * @brief Asserts whether or not the target cluster is an IO cluster.
	 *
	 * @param clusternum Logical ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is an IO cluster and
	 * zero otherwise.
	 */
	EXTERN int bostan_cluster_is_io(int clusternum);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond bostan
 */

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define PROCESSOR_CCLUSTERS_NUM     BOSTAN_PROCESSOR_CCLUSTERS_NUM     /**< @see BOSTAN_PROCESSOR_CCLUSTERS_NUM     */
	#define PROCESSOR_IOCLUSTERS_NUM    BOSTAN_PROCESSOR_IOCLUSTERS_NUM    /**< @see BOSTAN_PROCESSOR_IOCLUSTERS_NUM    */
	#define PROCESSOR_CLUSTERNUM_MASTER BOSTAN_PROCESSOR_CLUSTERNUM_MASTER /**< @see BOSTAN_PROCESSOR_CLUSTERNUM_MASTER */
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __cluster_get_num_fn      /**< bostan_cluster_get_num()    */
	#define __cluster_is_ccluster_fn  /**< bostan_cluster_is_compute() */
	#define __cluster_is_iocluster_fn /**< bostan_cluster_is_io()      */
	/**@}*/

	/**
	 * @see bostan_cluster_get_num().
	 */
	static inline int cluster_get_num(void)
	{
		return (bostan_cluster_get_num());
	}

	/**
	 * @see bostan_cluster_is_compute().
	 */
	static inline int cluster_is_ccluster(int clusternum)
	{
		return (bostan_cluster_is_compute(clusternum));
	}

	/**
	 * @see bostan_cluster_is_io().
	 */
	static inline int cluster_is_iocluster(int clusternum)
	{
		return (bostan_cluster_is_io(clusternum));
	}

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_CLUSTERS_H_ */

