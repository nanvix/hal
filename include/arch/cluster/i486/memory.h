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
	 * @brief User memory size (in bytes).
	 */
	#define I486_UMEM_SIZE (I486_MEM_SIZE - I486_KMEM_SIZE - I486_KPOOL_SIZE)

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define I486_KSTACK_SIZE I486_PAGE_SIZE

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define I486_USER_BASE_VIRT   0x02000000 /**< User Base             */
	#define I486_USTACK_BASE_VIRT 0xc0000000 /**< User Stack Base       */
	#define I486_KERNEL_BASE_VIRT 0xc0000000 /**< Kernel Base           */
	#define I486_KPOOL_BASE_VIRT  0xc1000000 /**< Kernel Page Pool Base */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define I486_KERNEL_BASE_PHYS 0x00000000 /**< Kernel Base           */
	#define I486_KPOOL_BASE_PHYS  0x01000000 /**< Kernel Page Pool Base */
	#define I486_USER_BASE_PHYS   0x02000000 /**< User Base             */
	/**@}*/

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond x86_smp
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE  I486_MEM_SIZE          /**< @see I486_MEM_SIZE         */
	#define KMEM_SIZE    I486_KMEM_SIZE         /**< @see I486_KMEM_SIZE        */
	#define UMEM_SIZE    I486_UMEM_SIZE         /**< @see I486_UMEM_SIZE        */
	#define KSTACK_SIZE  I486_KSTACK_SIZE       /**< @see I486_KSTACK_SIZE      */
	#define KPOOL_SIZE   I486_KPOOL_SIZE        /**< @see I486_KPOOL_SIZE       */
	#define KBASE_PHYS   I486_KERNEL_BASE_PHYS  /**< @see I486_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS   I486_KPOOL_BASE_PHYS   /**< @see I486_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS   I486_USER_BASE_PHYS    /**< @see I486_USER_BASE_PHYS   */
	#define USTACK_VIRT  I486_USTACK_BASE_VIRT  /**< @see I486_USTACK_BASE_VIRT */
	#define UBASE_VIRT   I486_USER_BASE_VIRT    /**< @see I486_USER_BASE_VIRT   */
	#define KBASE_VIRT   I486_KERNEL_BASE_VIRT  /**< @see I486_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT   I486_KPOOL_BASE_VIRT   /**< @see I486_KPOOL_BASE_VIRT  */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_I486_MEMORY_H_ */
