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

#ifndef NANVIX_HAL_PROCESSOR_CLUSTERS_H_
#define NANVIX_HAL_PROCESSOR_CLUSTERS_H_

	/* Processor Interface Implementation */
	#include <nanvix/hal/processor/_processor.h>

	#include <nanvix/klib.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Feature Checking */
	#ifndef PROCESSOR_IS_MULTICLUSTER
	#error "Is this processor multicluster?"
	#endif

	/* Multicluster Processor*/
	#if (PROCESSOR_IS_MULTICLUSTER)

		/* Constants */
		#ifndef _PROCESSOR_CCLUSTERS_NUM
		#error "_PROCESSOR_CCLUSTERS_NUM not defined"
		#endif
		#ifndef _PROCESSOR_IOCLUSTERS_NUM
		#error "_PROCESSOR_IOCLUSTERS_NUM not defined"
		#endif

		/* Functions */
		#ifndef __cluster_get_id_fn
		#error "cluster_get_id() not defined?"
		#endif
		#ifndef __cluster_is_ccluster_fn
		#error "cluster_is_ccluster() not defined?"
		#endif
		#ifndef __cluster_is_iocluster_fn
		#error "cluster_is_iocluster() not defined?"
		#endif

	#else

		/* Constants */
		#define _PROCESSOR_CCLUSTERS_NUM  0
		#define _PROCESSOR_IOCLUSTERS_NUM 1

	#endif

#endif

/*============================================================================*
 * Cluster Interface                                                          *
 *============================================================================*/

/**
 * @defgroup kernel-hal-processor-clusters Clusters
 * @ingroup kernel-hal-processor
 *
 * @brief Processor Clusters HAL Interface
 */
/**@{*/

	/**
	 * @brief Gets the ID of the underlying cluster.
	 *
	 * @returns The ID of the underlying cluster.
	 */
#if (PROCESSOR_IS_MULTICLUSTER)
	EXTERN int cluster_get_id(void);
#else
	static inline int cluster_get_id(void)
	{
		return (0);
	}
#endif

	/**
	 * @brief Asserts if a cluster is a compute cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @returns Non-zero if the target cluster @p clusterid is a
	 * compute cluster and zero otherwise.
	 */
#if (PROCESSOR_IS_MULTICLUSTER)
	EXTERN int cluster_is_ccluster(int clusterid);
#else
	static inline int cluster_is_ccluster(int clusterid)
	{
		UNUSED(clusterid);

		return (0);
	}
#endif

	/**
	 * @brief Asserts if a cluster is an I/O cluster.
	 *
	 * @param clusterid ID of the target cluster.
	 *
	 * @returns Non-zero if the target cluster @p clusterid is an I/O
	 * cluster and zero otherwise.
	 */
#if (PROCESSOR_IS_MULTICLUSTER)
	EXTERN int cluster_is_iocluster(int clusterid);
#else
	static inline int cluster_is_iocluster(int clusterid)
	{
		UNUSED(clusterid);

		return (1);
	}
#endif

/**@}*/

#endif /* NANVIX_HAL_PROCESSOR_CLUSTERS_H_ */

