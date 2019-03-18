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

#ifndef PROCESSOR_I486_QEMU_CLUSTERS_H_
#define PROCESSOR_I486_QEMU_CLUSTERS_H_

	/* Processor Interface Implementation */
	#include <arch/processor/i486-qemu/_i486-qemu.h>

/**
 * @addtogroup processor-i486-qemu-clusters Clusters
 * @ingroup processor-i486-qemu
 *
 * @brief Clusters
 */
/**@*/

	#include <arch/processor/i486-qemu/noc/node.h>
	#include <nanvix/klib.h>

	/**
	 * @brief Number of IO Clusters in the platform.
	 */
	#define I486_QEMU_IOCLUSTERS_NUM 2

	/**
	 * @brief Number of Compute Clusters in the platform.
	 */
	#define I486_QEMU_CCLUSTERS_NUM 16

	/**
	 * @name Cluster IDs
	 */
	/**@{*/
	#define I486_QEMU_CCLUSTER0    0 /**< Compute Cluster  0 */
	#define I486_QEMU_CCLUSTER1    1 /**< Compute Cluster  1 */
	#define I486_QEMU_CCLUSTER2    2 /**< Compute Cluster  2 */
	#define I486_QEMU_CCLUSTER3    3 /**< Compute Cluster  3 */
	#define I486_QEMU_CCLUSTER4    4 /**< Compute Cluster  4 */
	#define I486_QEMU_CCLUSTER5    5 /**< Compute Cluster  5 */
	#define I486_QEMU_CCLUSTER6    6 /**< Compute Cluster  6 */
	#define I486_QEMU_CCLUSTER7    7 /**< Compute Cluster  7 */
	#define I486_QEMU_CCLUSTER8    8 /**< Compute Cluster  8 */
	#define I486_QEMU_CCLUSTER9    9 /**< Compute Cluster  9 */
	#define I486_QEMU_CCLUSTER10  10 /**< Compute Cluster 10 */
	#define I486_QEMU_CCLUSTER11  11 /**< Compute Cluster 11 */
	#define I486_QEMU_CCLUSTER12  12 /**< Compute Cluster 12 */
	#define I486_QEMU_CCLUSTER13  13 /**< Compute Cluster 13 */
	#define I486_QEMU_CCLUSTER14  14 /**< Compute Cluster 14 */
	#define I486_QEMU_CCLUSTER15  15 /**< Compute Cluster 15 */
	#define I486_QEMU_IOCLUSTER0 128 /**< IO Cluster 0       */
	#define I486_QEMU_IOCLUSTER1 192 /**< IO Cluster 1       */
	/**@}*/

	/**
	 * @brief Gets the ID of the underling cluster.
	 *
	 * @returns The ID of the underlying cluster.
	 */
	static inline int i486_qemu_cluster_get_id(void)
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
	static inline int i486_qemu_cluster_is_ccluster(int clusterid)
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
	static inline int i486_qemu_cluster_is_iocluster(int clusterid)
	{
		UNUSED(clusterid);

		return (0);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486-qemu
 */

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define _PROCESSOR_CCLUSTERS_NUM  I486_QEMU_CCLUSTERS_NUM  /**< @see I486_QEMU_CCLUSTERS_NUM  */
	#define _PROCESSOR_IOCLUSTERS_NUM I486_QEMU_IOCLUSTERS_NUM /**< @see I486_QEMU_IOCLUSTERS_NUM */
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __cluster_get_id_fn       /**< i486_qemu_cluster_get_id()       */
	#define __cluster_is_ccluster_fn  /**< i486_qemu_cluster_is_ccluster()  */
	#define __cluster_is_iocluster_fn /**< i486_qemu_cluster_is_iocluster() */
	/**@}*/

	/**
	 * @see i486_qemu_cluster_get_id().
	 */
	static inline int cluster_get_id(void)
	{
		return (i486_qemu_cluster_get_id());
	}

	/**
	 * @see i486_qemu_cluster_is_ccluster().
	 */
	static inline int cluster_is_ccluster(int clusterid)
	{
		return (i486_qemu_cluster_is_ccluster(clusterid));
	}

	/**
	 * @see i486_qemu_cluster_is_iocluster().
	 */
	static inline int cluster_is_iocluster(int clusterid)
	{
		return (i486_qemu_cluster_is_iocluster(clusterid));
	}

/**@endcond*/

#endif /* PROCESSOR_I486_QEMU_CLUSTERS_H_ */

