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

#ifndef ARCH_CLUSTER_WOLFE_CORES_H_
#define ARCH_CLUSTER_WOLFE_CORES_H_


	/* Cluster Interface Implementation */
	#include <arch/cluster/wolfe/_wolfe.h>

/**
 * @addtogroup riscv-cluster-cores Cores
 * @ingroup riscv-cluster
 *
 * @brief Wolfe Cluster Cores
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define WOLFE_NUM_CORES 9

	/**
	 * @brief ID of the master core.
	 */
	#define WOLFE_COREID_MASTER RI5CY_COREID_MASTER

	/**
	 * @brief ID of the master cluster.
	 */
	#define WOLFE_CLUSTERID_MASTER RI5CY_CLUSTERID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @brief Gets the number of cores.
	 *
	 * THe wolfe_cluster_get_num_cores() function returns the number
	 * of core in the underlying cores.
	 *
	 * @returns The the number of cores in the underlying cluster.
	 */
	static inline int wolfe_cluster_get_num_cores(void)
	{
		return (WOLFE_NUM_CORES);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond wolfe_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __cluster_get_num_cores_fn /**< cluster_get_num_cores() */
	/**@}*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define CORES_NUM WOLFE_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER WOLFE_COREID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @see wolfe_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (wolfe_cluster_get_num_cores());
	}

	/**
	 * @see wolfe_get_cluster_id
	 */
	static inline int cluster_get_id(void)
	{
		return (ri5cy_cluster_get_id());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_WOLFE_CORES*/
