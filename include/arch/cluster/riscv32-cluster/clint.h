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

#ifndef ARCH_CLUSTER_RISCV32_CLUSTER_CLINT_H_
#define ARCH_CLUSTER_RISCV32_CLUSTER_CLINT_H_

	#ifndef __NEED_CLUSTER_CLINT
		#error "do not include this file"
	#endif

/**
 * @addtogroup riscv32-cluster-clint CLINT
 * @ingroup riscv32-cluster
 *
 * @brief Core-Local Interruptor
 */
/**@{*/

	#include <arch/cluster/riscv32-cluster/memory.h>
	#include <arch/cluster/riscv32-cluster/cores.h>
	#include <nanvix/const.h>
	#include <posix/errno.h>

	/**
	 * @brief CLINT base address
	 */
	#define RISCV32_CLUSTER_CLINT_BASE RISCV32_CLUSTER_PIC_BASE_VIRT

	/**
	 * @name Offset to Registers
	 */
	/**@{*/
	#define RISCV32_CLUSTER_CLINT_MSIP_OFFSET     0x00000000 /**< Offset for MSIP     */
	#define RISCV32_CLUSTER_CLINT_MTIMECMP_OFFSET 0x00004000 /**< Offset for MTIMECMP */
	#define RISCV32_CLUSTER_CLINT_MTIME_OFFSET    0x0000bff8 /**< Offset for MTIME    */
	/**@}*/

	/**
	 * @name CLINT Memory Map
	 */
	/**@{*/

		/**
		 * @brief Machine Software Interrupt Pending Register (MSIP)
		 */
		#define RISCV32_CLUSTER_CLINT_MSIP_BASE \
			(RISCV32_CLUSTER_CLINT_BASE + RISCV32_CLUSTER_CLINT_MSIP_OFFSET)

		/**
		 * @brief Machine Time Compare Register (MTIMECMP)
		 */
		#define RISCV32_CLUSTER_CLINT_MTIMECMP_BASE \
			(RISCV32_CLUSTER_CLINT_BASE + RISCV32_CLUSTER_CLINT_MTIMECMP_OFFSET)

		/**
		 * @brief Machine Time Register (MTIME)
		 */
		#define RISCV32_CLUSTER_CLINT_MTIME_BASE \
			(RISCV32_CLUSTER_CLINT_BASE + RISCV32_CLUSTER_CLINT_MTIME_OFFSET)

	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Machine Software Interrupt Pending Register (msip).
	 */
	EXTERN volatile uint32_t *riscv32_cluster_msip;

	/**
	 * @brief Sends an IPI.
	 *
	 * @param coreid ID of the target core.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	static inline int riscv32_cluster_ipi_send(int coreid)
	{
		/* Invalid core. */
		if (UNLIKELY((coreid < 0) || (coreid >= RISCV32_CLUSTER_NUM_CORES)))
			return (-EINVAL);

		/* Raise IPI. */
		riscv32_cluster_msip[coreid] = 1;
		rv32gc_dcache_inval();

		return (0);
	}

	/**
	 * @brief Acknowledges an IPI.
	 *
	 * @param coreid ID of the target core.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	static inline int riscv32_cluster_ipi_ack(int coreid)
	{
		/* Invalid core. */
		if (UNLIKELY((coreid < 0) || (coreid >= RISCV32_CLUSTER_NUM_CORES)))
			return (-EINVAL);

		/* Raise IPI. */
		riscv32_cluster_msip[coreid] = 0;
		rv32gc_mcall_ipi_ack();
		rv32gc_dcache_inval();

		return (0);
	}

#endif

/**@}*/

#endif /* ARCH_CLUSTER_RISCV32_CLUSTER_CLINT_H_ */
