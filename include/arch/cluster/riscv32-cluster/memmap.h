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

#ifndef CLUSTER_RISCV32_CLUSTER_MEMMAP_H_
#define CLUSTER_RISCV32_CLUSTER_MEMMAP_H_

	#ifndef __NEED_CLUSTER_MEMMAP
		#error "do not include this file"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/riscv32-cluster/_riscv32-cluster.h>

/**
 * @addtogroup riscv_cluster-cluster-memmap Memory Map
 * @ingroup riscv_cluster-cluster
 *
 * @brief Physical memory map.
 */
/**@{*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define RISCV32_CLUSTER_PIC_BASE_PHYS  0x02000000 /**< PIC Base  */
	#define RISCV32_CLUSTER_PIC_END_PHYS   0x02010000 /**< PIC End   */
	#define RISCV32_CLUSTER_UART_BASE_PHYS 0x10000000 /**< UART Base */
	#define RISCV32_CLUSTER_UART_END_PHYS  0x10010000 /**< UART End  */
	#define RISCV32_CLUSTER_DRAM_BASE_PHYS 0x80000000 /**< DRAM Base */
	#define RISCV32_CLUSTER_DRAM_END_PHYS  0x88000000 /**< DRAM End  */
	/**@}*/

	/**
	 * @brief DRAM brief (in bytes).
	 */
	#define RISCV32_CLUSTER_DRAM_SIZE \
		(RISCV32_CLUSTER_DRAM_END_PHYS - RISCV32_CLUSTER_DRAM_BASE_PHYS)

	/**
	 * @brief PIC brief (in bytes).
	 */
	#define RISCV32_CLUSTER_PIC_MEM_SIZE \
		(RISCV32_CLUSTER_PIC_END_PHYS - RISCV32_CLUSTER_PIC_BASE_PHYS)

	/**
	 * @brief UART brief (in bytes).
	 */
	#define RISCV32_CLUSTER_UART_MEM_SIZE \
		(RISCV32_CLUSTER_UART_END_PHYS - RISCV32_CLUSTER_UART_BASE_PHYS)

/**@}*/

#endif /* CLUSTER_RISCV32_CLUSTER_MEMMAP_H_ */

