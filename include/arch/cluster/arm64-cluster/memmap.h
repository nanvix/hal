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

#ifndef CLUSTER_ARM64_CLUSTER_MEMMAP_H_
#define CLUSTER_ARM64_CLUSTER_MEMMAP_H_

	#ifndef __NEED_CLUSTER_MEMMAP
		#error "do not include this file"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/arm64-cluster/_arm64-cluster.h>

/**
 * @addtogroup arm64_cluster-cluster-memmap Memory Map
 * @ingroup arm64_cluster-cluster
 *
 * @brief Physical memory map.
 */
/**@{*/
/**
	 * @name Physical Memory Layout
	 */
	#define ARM64_CLUSTER_DRAM_BASE_PHYS		(0x40000000UL)	/**< DRAM Base */
	#define ARM64_CLUSTER_DRAM_END_PHYS			(0x48000000UL)	/**< DRAM End  */
	#define ARM64_CLUSTER_GIC_BASE_PHYS			(0x08000000UL)	/**< GIC Base */
	#define ARM64_CLUSTER_GIC_END_PHYS			(0x09000000UL)	/**< GIC Base */
	#define ARM64_CLUSTER_UART_BASE_PHYS        (0x09000000UL)	/**< UART Base */
	#define ARM64_CLUSTER_UART_END_PHYS  		(0x09010000UL) 	/**< UART End  */
	/**@}*/

	/**
	 * @brief DRAM brief (in bytes).
	 */
	#define ARM64_CLUSTER_DRAM_SIZE \
		(ARM64_CLUSTER_DRAM_END_PHYS - ARM64_CLUSTER_DRAM_BASE_PHYS)

	/**
	 * @brief UART brief (in bytes).
	 */
	#define ARM64_CLUSTER_UART_MEM_SIZE \
		(ARM64_CLUSTER_UART_END_PHYS - ARM64_CLUSTER_UART_BASE_PHYS)

	/**
	 * @brief GIC brief (in bytes).
	 */
	#define ARM64_CLUSTER_GIC_MEM_SIZE \
		(ARM64_CLUSTER_GIC_END_PHYS - ARM64_CLUSTER_GIC_BASE_PHYS)

/**@}*/

#endif /* CLUSTER_AMR64_CLUSTER_MEMMAP_H_ */

