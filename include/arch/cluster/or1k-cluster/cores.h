/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
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

#ifndef ARCH_CLUSTER_OR1K_CLUSTER_CORES_H_
#define ARCH_CLUSTER_OR1K_CLUSTER_CORES_H_

	#ifndef __or1k_cluster__
		#error "wrong cluter included!"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/or1k-cluster/_or1k-cluster.h>

/**
 * @addtogroup or1k-cluster-cpu Cores
 * @ingroup or1k-cluster
 *
 * @brief Cores
 */
/**@{*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define OR1K_CLUSTER_NUM_CORES 2

	/**
	 * @brief ID of the master core.
	 */
	#define OR1K_CLUSTER_COREID_MASTER 0

#ifndef _ASM_FILE_

	/**
	 * @brief Resets the underlying cluster
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN NORETURN void _or1k_cluster_core_reset(int coreid);

	/**
	 * @brief Initializes the underlying cluster.
	 */
	EXTERN void or1k_cluster_setup(void);

	/**
	 * @brief Gets the number of cores.
	 *
	 * The or1k_cluster_cluster_get_num_cores() gets the number of cores in the
	 * underlying or1k processor.
	 *
	 * @returns The the number of cores in the underlying processor.
	 */
	static inline int or1k_cluster_cluster_get_num_cores(void)
	{
		return (OR1K_CLUSTER_NUM_CORES);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k_cluster
 */

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __core_get_id_fn        /**< core_get_id()           */
	#define __core_setup_fn         /**< core_setup()            */
	#define __cluster_get_num_cores /**< cluster_get_num_cores() */
	/**@}*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define CORES_NUM OR1K_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER OR1K_CLUSTER_COREID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @see _rv32i_core_reset().
	 */
	static inline void _core_reset(void)
	{
		_or1k_cluster_core_reset(or1k_core_get_id());
	}

	/**
	 * @see or1k_cluster_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (or1k_cluster_cluster_get_num_cores());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_OR1K_CLUSTER_CORES_H_ */
