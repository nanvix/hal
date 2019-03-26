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

#ifndef ARCH_CLUSTER_RISCV32_SMP_CORES_H_
#define ARCH_CLUSTER_RISCV32_SMP_CORES_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/riscv32-smp/_riscv32-smp.h>

/**
 * @addtogroup riscv-cluster-cpu Cores
 * @ingroup riscv-cluster
 *
 * @brief Cores
 */
/**@{*/

	/**
	 * @brief Number of cores in a cluster.
	 */
	#define RISCV32_SMP_NUM_CORES 4

	/**
	 * @brief ID of the master core.
	 */
	#define RISCV32_SMP_COREID_MASTER 0

#ifndef _ASM_FILE_

	/**
	 * @brief Gets the number of cores.
	 *
	 * The riscv32_smp_cluster_get_num_cores() gets the number of
	 * cores in the underlying riscv32 cluster.
	 *
	 * @returns The the number of cores in the underlying cluster.
	 */
	static inline int riscv32_smp_cluster_get_num_cores(void)
	{
		return (RISCV32_SMP_NUM_CORES);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond riscv32_smp
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
	#define CORES_NUM RISCV32_SMP_NUM_CORES

	/**
	 * @brief ID of the master core.
	 */
	#define COREID_MASTER RISCV32_SMP_COREID_MASTER

#ifndef _ASM_FILE_

	/**
	 * @see riscv32_smp_cluster_get_num_cores()
	 */
	static inline int cluster_get_num_cores(void)
	{
		return (riscv32_smp_cluster_get_num_cores());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_RISCV32_CORES_H_ */
