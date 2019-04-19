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

#ifndef CLUSTER_OPTIMSOC_CLUSTER_MEMMAP_H_
#define CLUSTER_OPTIMSOC_CLUSTER_MEMMAP_H_

	#ifndef __NEED_CLUSTER_MEMMAP
		#error "do not include this file"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

/**
 * @addtogroup optimsoc_cluster-cluster-memmap Memory Map
 * @ingroup optimsoc_cluster-cluster
 *
 * @brief Physical memory map.
 */
/**@{*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define OPTIMSOC_CLUSTER_OMPIC_BASE_PHYS 0x98000000 /**< OMPIC Base */
	#define OPTIMSOC_CLUSTER_OMPIC_END_PHYS  0x98010000 /**< OMPIC End */
	#define OPTIMSOC_CLUSTER_DRAM_BASE_PHYS  0x00000000 /**< DRAM Base */
	#define OPTIMSOC_CLUSTER_DRAM_END_PHYS   0x08000000 /**< DRAM End  */
	/**@}*/

	/**
	 * @brief DRAM brief (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_DRAM_SIZE \
		(OPTIMSOC_CLUSTER_DRAM_END_PHYS - OPTIMSOC_CLUSTER_DRAM_BASE_PHYS)

	/**
	 * @brief OMPIC brief (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_OMPIC_MEM_SIZE \
		(OPTIMSOC_CLUSTER_OMPIC_END_PHYS - OPTIMSOC_CLUSTER_OMPIC_BASE_PHYS)

/**@}*/

#endif /* CLUSTER_OPTIMSOC_CLUSTER_MEMMAP_H_ */
