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

#ifndef ARCH_CLUSTER_K1B_CLUSTER_CORES_H_
#define ARCH_CLUSTER_K1B_CLUSTER_CORES_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/k1b-cluster/_k1b-cluster.h>

/**
 * @addtogroup k1b-cluster-cpu Cores
 * @ingroup k1b-cluster
 *
 * @brief Cores
 */
/**@{*/

	/**
	 * @brief Number of cores in an I/O Cluster.
	 */
	#define K1BIO_CORES_NUM 4

	/**
	 * @brief Number of cores in an Compute Cluster.
	 */
	#define K1BDP_CORES_NUM 16

	/**
	 * @brief Number of cores in the underlying cluster.
	 */
	#ifdef __k1io__
		#define K1B_CLUSTER_NUM_CORES K1BIO_CORES_NUM
	#else
		#define K1B_CLUSTER_NUM_CORES K1BDP_CORES_NUM
	#endif

	/**
	 * @brief ID of the master core.
	 */
	#define K1B_CLUSTER_COREID_MASTER 0

	/**
	 * @brief Gets the number of cores.
	 *
	 * The k1b_cluster_get_num_cores() gets the number of cores in the
	 * underlying k1b processor.
	 *
	 * @returns The the number of cores in the underlying processor.
	 */
	static inline int k1b_cluster_get_num_cores(void)
	{
		return (K1B_CLUSTER_NUM_CORES);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __cluster_get_num_cores /**< cluster_get_num_cores() */
	/**@}*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define CORES_NUM K1B_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER K1B_CLUSTER_COREID_MASTER

	/**
	 * @see k1b_cluster_get_num_cores().
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (k1b_cluster_get_num_cores());
	}

/**@endcond*/

#endif /* ARCH_CLUSTER_K1B_CLUSTER_CORES_H_ */
