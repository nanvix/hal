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
	/**@{*/
	#define ARM64_CLUSTER_DRAM_BASE_PHY			(0x40000000)                    // DRAM BASE
	#define ARM64_CLUSTER_DRAM_SIZE				(1 << 30)                       // DRAM SIZE
	#define ARM64_CLUSTER_DRAM_END_PHY			(ARM64_CLUSTER_DRAM_BASE_PHY + ARM64_CLUSTER_DRAM_SIZE)
	/**@}*/

	#define KERNEL_VA_START         (0xffffff8000000000)
	#define IMAGE_START             (ARM64_CLUSTER_DRAM_BASE_PHY + 0x80000)            // QEMU自动将内核镜像搬到该位置，前面的空就不要了
	#define LOW_MEM                 (IMAGE_START + (1<<20) )        // 内存低端(1MB)，该部分用于内核镜像
	#define HIGH_MEM                (IMAGE_START + PAGING_MEMORY - 1)           // 内存高端(16MB)，超过该地址就越界了
	#define UART_PA_BASE        	(0x09000000)

/**@}*/

#endif /* CLUSTER_AMR64_CLUSTER_MEMMAP_H_ */

