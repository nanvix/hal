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

#ifndef PROCESSOR_LINUX64_PROCESSOR_CLUSTERS_H_
#define PROCESSOR_LINUX64_PROCESSOR_CLUSTERS_H_

	/* Processor Interface Implementation */
	#include <arch/processor/linux64/_linux64.h>

/**
 * @addtogroup processor-linux64-clusters Clusters
 * @ingroup processor-linux64
 *
 * @brief Clusters
 */
/**@*/

	/**
	 * @brief Number of IO Clusters.
	 */
	#define LINUX64_PROCESSOR_IOCLUSTERS_NUM 2

	/**
	 * @brief Number of compute clusters.
	 */
	#define LINUX64_PROCESSOR_CCLUSTERS_NUM 16

	/**
	 * @brief Number of Clusters
	 */
	#define LINUX64_PROCESSOR_CLUSTERS_NUM \
		(LINUX64_PROCESSOR_IOCLUSTERS_NUM + LINUX64_PROCESSOR_CCLUSTERS_NUM)

	/**
	 * @name Types of Clusters
	 */
	/**@{*/
	#define LINUX64_PROCESSOR_IOCLUSTER 1 /**< IO Cluster      */
	#define LINUX64_PROCESSOR_CCLUSTER  2 /**< Compute Cluster */
	/**@}*/

	/**
	 * @brief ID of master cluster.
	 */
	EXTERN int LINUX64_PROCESSOR_CLUSTERID_MASTER;

#ifdef __NANVIX_HAL

	/**
	 * @brief Powers on clusters of the underlying processor.
	 */
	EXTERN void linux64_processor_clusters_boot(void);

	/**
	 * @brief Powers off clusters of the underlying processor.
	 */
	EXTERN void linux64_processor_clusters_shutdown(void);

	/**
	 * @brief Initialize clusters of the underlying processor.
	 */
	EXTERN void linux64_processor_clusters_setup(void);

	/**
	 * @brief Retrieves the ID of the underlying cluster.
	 */
	EXTERN int linux64_cluster_get_id(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Retrieves the logical number of the underlying cluster.
	 */
	EXTERN int linux64_cluster_get_num(void);

	/**
	 * @brief Asserts whether or not the target cluster is a compute cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is a compute cluster and
	 * zero otherwise.
	 */
	EXTERN int linux64_cluster_is_compute(int clusternum);

	/**
	 * @brief Asserts whether or not the target cluster is an IO cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @return Non zero if the target cluster is an IO cluster and
	 * zero otherwise.
	 */
	EXTERN int linux64_cluster_is_io(int clusternum);

/**@}*/
/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_processor
 */

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define PROCESSOR_CCLUSTERS_NUM  LINUX64_PROCESSOR_CCLUSTERS_NUM  /**< @see LINUX64_PROCESSOR_CCLUSTERS_NUM  */
	#define PROCESSOR_IOCLUSTERS_NUM LINUX64_PROCESSOR_IOCLUSTERS_NUM /**< @see LINUX64_PROCESSOR_IOCLUSTERS_NUM */
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __cluster_get_id_fn       /**< linux64_cluster_cluster_get_id() */
	#define __cluster_is_ccluster_fn  /**< linux64_cluster_is_compute()     */
	#define __cluster_is_iocluster_fn /**< linux64_cluster_is_io()          */
	/**@}*/

	/**
	 * @see linux64_processor_cluster_get_num().
	 */
	static inline int cluster_get_num(void)
	{
		return (linux64_cluster_get_num());
	}

	/**
	 * @see linux64_processor_cluster_is_ccluster().
	 */
	static inline int cluster_is_ccluster(int clusternum)
	{
		return (linux64_cluster_is_compute(clusternum));
	}

	/**
	 * @see linux64_processor_cluster_is_iocluster().
	 */
	static inline int cluster_is_iocluster(int clusternum)
	{
		return (linux64_cluster_is_io(clusternum));
	}

/**@endcond*/

#endif /* PROCESSOR_LINUX64_PROCESSOR_CLUSTERS_H_ */

