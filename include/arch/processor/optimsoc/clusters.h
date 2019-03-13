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

#ifndef PROCESSOR_OR1K_OPTIMSOC_CLUSTERS_H_
#define PROCESSOR_OR1K_OPTIMSOC_CLUSTERS_H_

	/* Processor Interface Implementation */
	#include <arch/processor/or1k-optimsoc/_or1k-optimsoc.h>

/**
 * @addtogroup processor-or1k-optimsoc-clusters Clusters
 * @ingroup processor-or1k-optimsoc
 *
 * @brief Clusters
 */
/**@*/

	#include <arch/processor/or1k-optimsoc/noc/node.h>
	#include <nanvix/klib.h>

	/**
	 * @brief Number of IO Clusters in the platform.
	 */
	#define OR1K_OPTIMSOC_IOCLUSTERS_NUM 2

	/**
	 * @brief Number of Compute Clusters in the platform.
	 */
	#define OR1K_OPTIMSOC_CCLUSTERS_NUM 16

	/**
	 * @name Cluster IDs
	 */
	/**@{*/
	#define OR1K_OPTIMSOC_CCLUSTER0    0 /**< Compute Cluster  0 */
	#define OR1K_OPTIMSOC_CCLUSTER1    1 /**< Compute Cluster  1 */
	#define OR1K_OPTIMSOC_CCLUSTER2    2 /**< Compute Cluster  2 */
	#define OR1K_OPTIMSOC_CCLUSTER3    3 /**< Compute Cluster  3 */
	#define OR1K_OPTIMSOC_CCLUSTER4    4 /**< Compute Cluster  4 */
	#define OR1K_OPTIMSOC_CCLUSTER5    5 /**< Compute Cluster  5 */
	#define OR1K_OPTIMSOC_CCLUSTER6    6 /**< Compute Cluster  6 */
	#define OR1K_OPTIMSOC_CCLUSTER7    7 /**< Compute Cluster  7 */
	#define OR1K_OPTIMSOC_CCLUSTER8    8 /**< Compute Cluster  8 */
	#define OR1K_OPTIMSOC_CCLUSTER9    9 /**< Compute Cluster  9 */
	#define OR1K_OPTIMSOC_CCLUSTER10  10 /**< Compute Cluster 10 */
	#define OR1K_OPTIMSOC_CCLUSTER11  11 /**< Compute Cluster 11 */
	#define OR1K_OPTIMSOC_CCLUSTER12  12 /**< Compute Cluster 12 */
	#define OR1K_OPTIMSOC_CCLUSTER13  13 /**< Compute Cluster 13 */
	#define OR1K_OPTIMSOC_CCLUSTER14  14 /**< Compute Cluster 14 */
	#define OR1K_OPTIMSOC_CCLUSTER15  15 /**< Compute Cluster 15 */
	#define OR1K_OPTIMSOC_IOCLUSTER0 128 /**< IO Cluster 0       */
	#define OR1K_OPTIMSOC_IOCLUSTER1 192 /**< IO Cluster 1       */
	/**@}*/

	/**
	 * @brief Gets the ID of the underling cluster.
	 *
	 * @returns The ID of the underlying cluster.
	 */
	static inline int or1k_optimsoc_cluster_get_id(void)
	{
		return (0);
	}

	/**
	 * @brief Asserts whether or not the target cluster is a Compute Cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is a Compute Cluster and
	 * zero otherwise.
	 */
	static inline int or1k_optimsoc_cluster_is_ccluster(int clusterid)
	{
		UNUSED(clusterid);

		return (0);
	}

	/**
	 * @brief Asserts whether or not the target cluster is an IO cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is an IO cluster and
	 * zero otherwise.
	 */
	static inline int or1k_optimsoc_cluster_is_iocluster(int clusterid)
	{
		UNUSED(clusterid);

		return (0);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k-optimsoc
 */

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define _PROCESSOR_CCLUSTERS_NUM  OR1K_OPTIMSOC_CCLUSTERS_NUM  /**< @see OR1K_OPTIMSOC_CCLUSTERS_NUM  */
	#define _PROCESSOR_IOCLUSTERS_NUM OR1K_OPTIMSOC_IOCLUSTERS_NUM /**< @see OR1K_OPTIMSOC_IOCLUSTERS_NUM */
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __cluster_get_id_fn       /**< or1k_optimsoc_cluster_get_id()       */
	#define __cluster_is_ccluster_fn  /**< or1k_optimsoc_cluster_is_ccluster()  */
	#define __cluster_is_iocluster_fn /**< or1k_optimsoc_cluster_is_iocluster() */
	/**@}*/

	/**
	 * @see or1k_optimsoc_cluster_get_id().
	 */
	static inline int cluster_get_id(void)
	{
		return (or1k_optimsoc_cluster_get_id());
	}

	/**
	 * @see or1k_optimsoc_cluster_is_ccluster().
	 */
	static inline int cluster_is_ccluster(int clusterid)
	{
		return (or1k_optimsoc_cluster_is_ccluster(clusterid));
	}

	/**
	 * @see or1k_optimsoc_cluster_is_iocluster().
	 */
	static inline int cluster_is_iocluster(int clusterid)
	{
		return (or1k_optimsoc_cluster_is_iocluster(clusterid));
	}

/**@endcond*/

#endif /* PROCESSOR_OR1K_OPTIMSOC_CLUSTERS_H_ */

