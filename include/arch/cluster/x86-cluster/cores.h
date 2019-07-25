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

#ifndef ARCH_CLUSTER_X86_CLUSTER_CORES_H_
#define ARCH_CLUSTER_X86_CLUSTER_CORES_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/x86-cluster/_x86-cluster.h>

/**
 * @addtogroup x86-cluster-cpu Cores
 * @ingroup x86-cluster
 *
 * @brief Cores
 */
/**@{*/

	/**
	 * @brief Number of cores.
	 */
	#define X86_CLUSTER_NUM_CORES 1

	/**
	 * @brief ID of the master core.
	 */
	#define X86_CLUSTER_COREID_MASTER 0

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the underlying cluster.
	 */
	EXTERN void x86_cluster_setup(void);

	/**
	 * @brief Gets the number of cores.
	 *
	 * The x86_cluster_cluster_get_num_cores() gets the number of cores in
	 * the underlying x86 cluster.
	 *
	 * @returns The the number of cores in the underlying cluster.
	 */
	static inline int x86_cluster_cluster_get_num_cores(void)
	{
		return (X86_CLUSTER_NUM_CORES);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

/**
 * @cond x86_cluster
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
	#define CORES_NUM X86_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER X86_CLUSTER_COREID_MASTER

#ifndef _ASM_FILE_
#ifdef __NANVIX_HAL

	/**
	 * @see x86_cluster_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (x86_cluster_cluster_get_num_cores());
	}

#endif /* __NANVIX_HAL */
#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_X86_CLUSTER_CORES_H_ */
