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

#ifndef CLUSTER_OR1K_CLUSTER_MEMORY_H_
#define CLUSTER_OR1K_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/or1k-cluster/_or1k-cluster.h>

/**
 * @addtogroup or1k-cluster-mem Memory
 * @ingroup or1k-cluster
 *
 * @brief Memory System
 */
/**@{*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define OR1K_MEM_SIZE (32*1024*1024)

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define OR1K_KMEM_SIZE (16*1024*1024)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define OR1K_KPOOL_SIZE (4*1024*1024)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define OR1K_UMEM_SIZE (OR1K_MEM_SIZE - OR1K_KMEM_SIZE - OR1K_KPOOL_SIZE)

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define OR1K_KSTACK_SIZE OR1K_PAGE_SIZE

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define OR1K_USER_BASE_VIRT    0x02000000 /**< User Base Base        */
	#define OR1K_USTACK_BASE_VIRT  0xc0000000 /**< User Stack Base       */
	#define OR1K_KERNEL_BASE_VIRT  0xc0000000 /**< Kernel Base           */
	#define OR1K_KPOOL_BASE_VIRT   0xc1000000 /**< Kernel Page Pool Base */
	#define OR1K_UART_BASE_VIRT    0xc4000000 /**< Kernel UART Base      */
	#define OR1K_OMPIC_BASE_VIRT   0xc5000000 /**< Kernel OMPIC Base     */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define OR1K_KERNEL_BASE_PHYS 0x00000000 /**< Kernel Base           */
	#define OR1K_KPOOL_BASE_PHYS  0x01000000 /**< Kernel Page Pool Base */
	#define OR1K_USER_BASE_PHYS   0x02000000 /**< User Base             */
	#define OR1K_UART_BASE_PHYS   0x90000000 /**< UART Base             */
	#define OR1K_OMPIC_BASE_PHYS  0x98000000 /**< OMPIC Base            */
	/**@}*/

	/**
	 * OMPIC Registers and flags.
	 */
	/**@{*/
	#define OR1K_OMPIC_CPUBYTES	        8
	#define OR1K_OMPIC_CTRL(cpu)        (OR1K_OMPIC_BASE_VIRT + (0x0 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_STAT(cpu)        (OR1K_OMPIC_BASE_VIRT + (0x4 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_CTRL_IRQ_ACK	    (1 << 31)
	#define OR1K_OMPIC_CTRL_IRQ_GEN	    (1 << 30)
	#define OR1K_OMPIC_CTRL_DST(cpu)    (((cpu) & 0x3fff) << 16)
	#define OR1K_OMPIC_STAT_IRQ_PENDING (1 << 30)
	#define OR1K_OMPIC_DATA(x)          ((x) & 0xffff)
	#define OR1K_OMPIC_STAT_SRC(x)      (((x) >> 16) & 0x3fff)
	/**@}*/

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE  OR1K_MEM_SIZE         /**< @see OR1K_MEM_SIZE         */
	#define KMEM_SIZE    OR1K_KMEM_SIZE        /**< @see OR1K_KMEM_SIZE        */
	#define UMEM_SIZE    OR1K_UMEM_SIZE        /**< @see OR1K_UMEM_SIZE        */
	#define KSTACK_SIZE  OR1K_KSTACK_SIZE      /**< @see OR1K_KSTACK_SIZE      */
	#define KPOOL_SIZE   OR1K_KPOOL_SIZE       /**< @see OR1K_KPOOL_SIZE       */
	#define KBASE_PHYS   OR1K_KERNEL_BASE_PHYS /**< @see OR1K_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS   OR1K_KPOOL_BASE_PHYS  /**< @see OR1K_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS   OR1K_USER_BASE_PHYS   /**< @see OR1K_USER_BASE_PHYS   */
	#define USTACK_VIRT  OR1K_USTACK_BASE_VIRT /**< @see OR1K_USTACK_BASE_VIRT */
	#define _UART_ADDR   OR1K_UART_BASE_PHYS   /**< @see OR1K_UART_BASE_PHYS   */
	#define UBASE_VIRT   OR1K_USER_BASE_VIRT   /**< @see OR1K_USER_BASE_VIRT   */
	#define KBASE_VIRT   OR1K_KERNEL_BASE_VIRT /**< @see OR1K_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT   OR1K_KPOOL_BASE_VIRT  /**< @see OR1K_KPOOL_BASE_VIRT  */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_OR1K_CLUSTER_MEMORY_H_ */
