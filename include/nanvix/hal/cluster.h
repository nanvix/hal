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

#ifndef NANVIX_HAL_CLUSTER_H_
#define NANVIX_HAL_CLUSTER_H_

	#ifndef __NEED_HAL_CLUSTER
		#error "include <nanvix/hal/processor.h> instead"
	#endif

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

	#include <nanvix/hal/cluster/memory.h>
	#include <nanvix/hal/cluster/mmio.h>
	#include <nanvix/const.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Feature Checking */
	#ifndef CLUSTER_IS_MULTICORE
	#error "is this a multicore cluster?"
	#endif
	#ifndef CLUSTER_IS_IO
	#error "is this an i/o cluster?"
	#endif
	#ifndef CLUSTER_IS_COMPUTE
	#error "is this a compute cluster?"
	#endif
	#if (CLUSTER_IS_IO == CLUSTER_IS_COMPUTE)
	#error "ambiguous cluster type!"
	#endif

	/* Constants */
	#ifndef CORES_NUM
	#error "CORES_NUM not defined"
	#endif
	#ifndef COREID_MASTER
	#error "COREID_MASTER not defined"
	#endif

	/* Functions */
	#ifndef __cluster_get_num_cores
	#error "cluster_get_num_cores() not defined?"
	#endif

#endif

/*============================================================================*
 * Cluster Interface                                                          *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-cluster Cluster
 * @ingroup kernel-hal
 *
 * @brief Cluster HAL Interface
 */
/**@{*/

	/**
	 * @name Types of Cluster
	 */
	/**@{*/
	#define CLUSTER_IO      1 /**< I/O Cluster     */
	#define CLUSTER_COMPUTE 2 /**< Compute Cluster */
	/**@}*/

	/**
	 * @brief Gets the number of cores.
	 *
	 * @returns The number of cores in the underlying cluster
	 */
	EXTERN int cluster_get_num_cores(void);

	/**
	 * @brief Asserts whether the cluster is an I/O Cluster.
	 *
	 * @returns Non-zero if the underlying cluster is an I/O cluster
	 * and zero otherwise.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_is_io(void)
	{
		return (CLUSTER_IS_IO);
	}

	/**
	 * @brief Asserts whether the cluster is an Compute Cluster.
	 *
	 * @returns Non-zero if the underlying cluster is an Compute
	 * cluster and zero otherwise.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_is_compute(void)
	{
		return (CLUSTER_IS_COMPUTE);
	}

	/**
	 * @brief Gets the type of cluster.
	 *
	 * @returns CLUSTER_IO if the underlying cluster is an I/O cluster
	 * and CLUSTER_COMPUTE if it is a compute cluster.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_get_type(void)
	{
		return ((CLUSTER_IS_IO) ? CLUSTER_IO : CLUSTER_COMPUTE);
	}

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_H_ */
