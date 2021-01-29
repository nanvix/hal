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

#ifndef ARCH_CLUSTER_LINUX64_CLUSTER_CORES_H_
#define ARCH_CLUSTER_LINUX64_CLUSTER_CORES_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/linux64-cluster/_linux64-cluster.h>

/**
 * @addtogroup linux64-cluster-cpu Cores
 * @ingroup linux64-cluster
 *
 * @brief Cores
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Number of cores in the cluster.
	 */
    #define LINUX64_CLUSTER_NUM_CORES 5

	/**
	 * @brief ID of the master core.
	 */
	#define LINUX64_CLUSTER_COREID_MASTER 0

	/**
	 * @brief Array of the cores.
	 */
	EXTERN pthread_t linux64_cores_tab[LINUX64_CLUSTER_NUM_CORES];

	/**
	 * @brief Lock for the array of the cores.
	 */
	EXTERN linux64_spinlock_t linux64_cores_lock;

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN void linux64_core_setup(void);

	/**
	 * @brief Gets the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core or -1 if the thread
	 * calling thread is not attached to the underlying cluster.
	 */
	EXTERN int linux64_core_get_id(void);

	/**
	 * @brief Resets the underlying core.
	 */
	EXTERN NORETURN void _linux64_core_reset(void);

	/**
	 * @brief Gets the number of cores.
	 *
	 * The linux64_cluster_get_num_cores() gets the number of cores in the
	 * underlying linux64 processor.
	 *
	 * @returns The the number of cores in the underlying processor.
	 */
	static inline int linux64_cluster_get_num_cores(void)
	{
		return (LINUX64_CLUSTER_NUM_CORES);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define ___core_reset_fn        /**< core_reset()            */
	#define __core_get_id_fn        /**< core_get_id()           */
	#define __core_setup_fn         /**< core_setup()            */
	#define __cluster_get_num_cores /**< cluster_get_num_cores() */
	/**@}*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define CORES_NUM LINUX64_CLUSTER_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER LINUX64_CLUSTER_COREID_MASTER


	/**
	 * @see _linux_core_reset().
	 */
	static NORETURN inline void _core_reset(void)
	{
		_linux64_core_reset();
	}

	/**
	 * @see linux64_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (linux64_core_get_id());
	}

	/**
	 * @see linux64_cluster_get_num_cores().
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (linux64_cluster_get_num_cores());
	}

/**@endcond*/

#endif /* ARCH_CLUSTER_LINUX64_CLUSTER_CORES_H_ */
