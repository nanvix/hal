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

#ifndef CLUSTER_I486_MEMORY_H_
#define CLUSTER_I486_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/i486/_i486.h>

/**
 * @addtogroup i486-cluster-mem Memory
 * @ingroup i486-cluster
 *
 * @brief Memory System
 */
/**@{*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define I486_MEM_SIZE (32*1024*1024)

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define I486_KMEM_SIZE (16*1024*1024)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define I486_KPOOL_SIZE (4*1024*1024)

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define I486_UBASE_VIRT  0x02000000 /**< User base.        */
	#define I486_USTACK_ADDR 0xc0000000 /**< User stack.       */
	#define I486_KBASE_VIRT  0xc0000000 /**< Kernel base.      */
	#define I486_KPOOL_VIRT  0xc1000000 /**< Kernel page pool. */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define I486_KBASE_PHYS 0x00000000 /**< Kernel base.      */
	#define I486_KPOOL_PHYS 0x01000000 /**< Kernel page pool. */
	#define I486_UBASE_PHYS 0x02000000 /**< User base.        */
	/**@}*/

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @brief Memory size (in bytes).
	 */
	#define _MEMORY_SIZE I486_MEM_SIZE

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define _KSTACK_SIZE I486_PAGE_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define _KMEM_SIZE I486_KMEM_SIZE

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define _KPOOL_SIZE I486_KPOOL_SIZE

	/**
	 * @brief User memory size (in bytes).
	 */
	#define _UMEM_SIZE (I486_MEM_SIZE - I486_KMEM_SIZE - I486_KPOOL_SIZE)

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define _UBASE_VIRT  I486_UBASE_VIRT  /**< User Base        */
	#define _USTACK_ADDR I486_USTACK_ADDR /**< User Stack       */
	#define _KBASE_VIRT  I486_KBASE_VIRT  /**< Kernel Base      */
	#define _KPOOL_VIRT  I486_KPOOL_VIRT  /**< Kernel Page Pool */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define _KBASE_PHYS I486_KBASE_PHYS /**< Kernel Base      */
	#define _KPOOL_PHYS I486_KPOOL_PHYS /**< Kernel Page Pool */
	#define _UBASE_PHYS I486_UBASE_PHYS /**< User Base        */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_I486_MEMORY_H_ */
