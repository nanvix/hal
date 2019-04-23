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

#ifndef ARCH_CLUSTER_OPTIMSOC_CLUSTER_CORES_H_
#define ARCH_CLUSTER_OPTIMSOC_CLUSTER_CORES_H_

	#ifndef __optimsoc_cluster__
		#error "wrong cluter included!"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

/**
 * @addtogroup optimsoc-cluster-cpu Cores
 * @ingroup optimsoc-cluster
 *
 * @brief Cores
 */
/**@{*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define OPTIMSOC_CLUSTER_NUM_CORES 4
	#define OR1K_CLUSTER_NUM_CORES OPTIMSOC_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define OPTIMSOC_CLUSTER_COREID_MASTER 0

#ifndef _ASM_FILE_

	/**
	 * @brief Resets the underlying cluster
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN NORETURN void _optimsoc_cluster_core_reset(int coreid);

	/**
	 * @brief Initializes the underlying cluster.
	 */
	EXTERN void optimsoc_cluster_setup(void);

	/**
	 * @brief Gets the number of cores.
	 *
	 * The optimsoc_cluster_cluster_get_num_cores() gets the number of cores in the
	 * underlying optimsoc processor.
	 *
	 * @returns The the number of cores in the underlying processor.
	 */
	static inline int optimsoc_cluster_cluster_get_num_cores(void)
	{
		return (OPTIMSOC_CLUSTER_NUM_CORES);
	}

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond optimsoc_cluster
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
	#define CORES_NUM OPTIMSOC_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER OPTIMSOC_CLUSTER_COREID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @see _or1k_cluster_core_reset().
	 */
	static inline void _core_reset(void)
	{
		_optimsoc_cluster_core_reset(or1k_core_get_id());
	}

	/**
	 * @see optimsoc_cluster_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (optimsoc_cluster_cluster_get_num_cores());
	}

#ifdef __NANVIX_HAL

	/**
	 * @see optimsoc_cluster_setup().
	 */
	static inline void core_setup(void)
	{
		optimsoc_cluster_setup();
	}

#endif /* __NANVIX_HAL */

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_OPTIMSOC_CLUSTER_CORES_H_ */
