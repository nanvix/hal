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

#ifndef NANVIX_HAL_CLUSTER_MMIO_H_
#define NANVIX_HAL_CLUSTER_MMIO_H_

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * MMIO Interface                                                             *
 *============================================================================*/

/**
 * @defgroup kernel-hal-cluster-mmio MMIO
 * @ingroup kernel-hal-cluster
 *
 * @brief MMIO Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Gets the equivalent address accordingly with
	 * the current state of the MMU.
	 *
	 * @param paddr Target virtual physical address.
	 *
	 * @returns If mmu is enabled, returns the equivalent
	 * virtual address, otherwise, returns the same physical
	 * address.
	 */
	static inline void* mmio_get(paddr_t paddr)
	{
		if (mmu_is_enabled())
			return (mmu_page_walk(paddr));
		else
			return ((void*)paddr);
	}

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_MMIO_H_ */
