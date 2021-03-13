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

#ifndef ARCH_CLUSTER_ARM64_CLUSTER_CORES_H_
#define ARCH_CLUSTER_ARM64_CLUSTER_CORES_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/arm64-cluster/_arm64-cluster.h>

/**
 * @addtogroup arm64-cluster-cpu Cores
 * @ingroup arm64-cluster
 *
 * @brief Cores
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define ARM64_CLUSTER_NUM_CORES 4

	/**
	 * @brief ID of the master core.
	 */
	#define ARM64_CLUSTER_COREID_MASTER 0

#ifndef _ASM_FILE_

	/**
	 * @brief Resets the underlying cluster
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN NORETURN void _arm64_cluster_core_reset(int coreid);

	/**
	 * @brief Initializes the underlying cluster.
	 */
	EXTERN void arm64_cluster_setup(void);

	/**
	 * @brief Gets the number of cores.
	 *
	 * The arm64_cluster_cluster_get_num_cores() gets the number of
	 * cores in the underlying arm64 cluster.
	 *
	 * @returns The the number of cores in the underlying cluster.
	 */
	static inline int arm64_cluster_get_num_cores(void)
	{
		return (ARM64_CLUSTER_NUM_CORES);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define ___core_reset_fn        /**< core_get_id()           */
	#define __core_get_id_fn        /**< core_get_id()           */
	#define __core_setup_fn         /**< core_setup()            */
	#define __cluster_get_num_cores /**< cluster_get_num_cores() */
	/**@}*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define CORES_NUM ARM64_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER ARM64_CLUSTER_COREID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @see _arm64_core_reset().
	 */
	static inline void _core_reset(void)
	{
		_arm64_cluster_core_reset(arm64_core_get_id());
	}

	/**
	 * @see arm64_cluster_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (arm64_cluster_get_num_cores());
	}

#ifdef __NANVIX_HAL

	/**
	 * @see arm64_cluster_setup().
	 */
	static inline void cluster_setup(void)
	{
		arm64_cluster_setup();
	}

#endif /* __NANVIX_HAL */

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_ARM64_CORES_H_ */
