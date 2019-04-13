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

	/* Must come first. */
	#define __NEED_CLUSTER_MEMMAP

	#include <nanvix/const.h>
	#include <arch/cluster/or1k-cluster/memmap.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define OR1K_CLUSTER_KERNEL_BASE_PHYS OR1K_CLUSTER_DRAM_BASE_PHYS                       /**< Kernel Code and Data */
	#define OR1K_CLUSTER_KERNEL_END_PHYS  (OR1K_CLUSTER_KERNEL_BASE_PHYS + OR1K_PGTAB_SIZE) /**< Kernel End           */
	#define OR1K_CLUSTER_KPOOL_BASE_PHYS  (OR1K_CLUSTER_KERNEL_END_PHYS + OR1K_PGTAB_SIZE)  /**< Kernel Page Pool     */
	#define OR1K_CLUSTER_KPOOL_END_PHYS   (OR1K_CLUSTER_KPOOL_BASE_PHYS + OR1K_PGTAB_SIZE)  /**< Kernel Pool End      */
	#define OR1K_CLUSTER_USER_BASE_PHYS   OR1K_CLUSTER_KPOOL_END_PHYS                       /**< User Base            */
	#define OR1K_CLUSTER_USER_END_PHYS    OR1K_CLUSTER_DRAM_END_PHYS                        /**< User End             */
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define OR1K_CLUSTER_UART_BASE_VIRT   OR1K_CLUSTER_UART_BASE_PHYS   /**< UART Base            */
	#define OR1K_CLUSTER_UART_END_VIRT    OR1K_CLUSTER_UART_END_PHYS    /**< UART End             */
	#define OR1K_CLUSTER_OMPIC_BASE_VIRT  OR1K_CLUSTER_OMPIC_BASE_PHYS  /**< PIC Base             */
	#define OR1K_CLUSTER_OMPIC_END_VIRT   OR1K_CLUSTER_OMPIC_END_PHYS   /**< PIC End              */
	#define OR1K_CLUSTER_KERNEL_BASE_VIRT OR1K_CLUSTER_KERNEL_BASE_PHYS /**< Kernel Code and Data */
	#define OR1K_CLUSTER_KERNEL_END_VIRT  OR1K_CLUSTER_KERNEL_END_PHYS  /**< Kernel End           */
	#define OR1K_CLUSTER_KPOOL_BASE_VIRT  OR1K_CLUSTER_KPOOL_BASE_PHYS  /**< Kernel Page Pool     */
	#define OR1K_CLUSTER_KPOOL_END_VIRT   OR1K_CLUSTER_KPOOL_END_PHYS   /**< Kernel Pool End      */
	#define OR1K_CLUSTER_USER_BASE_VIRT   0xa0000000                    /**< User Base            */
	#define OR1K_CLUSTER_USER_END_VIRT    0xc0000000                    /**< User End             */
	#define OR1K_CLUSTER_USTACK_BASE_VIRT 0xc0000000                    /**< User Stack Base      */
	#define OR1K_CLUSTER_USTACK_END_VIRT  0xb0000000                    /**< User Stack End       */
	/**@}*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define OR1K_CLUSTER_MEM_SIZE \
		OR1K_CLUSTER_DRAM_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define OR1K_CLUSTER_KMEM_SIZE \
		(OR1K_CLUSTER_KERNEL_END_VIRT - OR1K_CLUSTER_KERNEL_BASE_VIRT)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define OR1K_CLUSTER_KPOOL_SIZE \
		(OR1K_CLUSTER_KPOOL_END_VIRT - OR1K_CLUSTER_KPOOL_BASE_VIRT)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define OR1K_CLUSTER_UMEM_SIZE \
		(OR1K_CLUSTER_USER_END_VIRT - OR1K_CLUSTER_USER_BASE_VIRT)

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define OR1K_CLUSTER_KSTACK_SIZE OR1K_PAGE_SIZE

	/**
	 * OMPIC Registers and flags.
	 */
	/**@{*/
	#define OR1K_OMPIC_CPUBYTES	        8
	#define OR1K_OMPIC_CTRL(cpu)        (OR1K_CLUSTER_OMPIC_BASE_VIRT + (0x0 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_STAT(cpu)        (OR1K_CLUSTER_OMPIC_BASE_VIRT + (0x4 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_CTRL_IRQ_ACK	    (1 << 31)
	#define OR1K_OMPIC_CTRL_IRQ_GEN	    (1 << 30)
	#define OR1K_OMPIC_CTRL_DST(cpu)    (((cpu) & 0x3fff) << 16)
	#define OR1K_OMPIC_STAT_IRQ_PENDING (1 << 30)
	#define OR1K_OMPIC_DATA(x)          ((x) & 0xffff)
	#define OR1K_OMPIC_STAT_SRC(x)      (((x) >> 16) & 0x3fff)
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the Memory Interface.
	 */
	EXTERN void or1k_cluster_mem_setup(void);

#endif /* _ASM_FILE_ */

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
	#define MEMORY_SIZE  OR1K_CLUSTER_MEM_SIZE         /**< @see OR1K_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE    OR1K_CLUSTER_KMEM_SIZE        /**< @see OR1K_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE    OR1K_CLUSTER_UMEM_SIZE        /**< @see OR1K_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE  OR1K_CLUSTER_KSTACK_SIZE      /**< @see OR1K_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE   OR1K_CLUSTER_KPOOL_SIZE       /**< @see OR1K_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS   OR1K_CLUSTER_KERNEL_BASE_PHYS /**< @see OR1K_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS   OR1K_CLUSTER_KPOOL_BASE_PHYS  /**< @see OR1K_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS   OR1K_CLUSTER_USER_BASE_PHYS   /**< @see OR1K_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT  OR1K_CLUSTER_USTACK_BASE_VIRT /**< @see OR1K_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT   OR1K_CLUSTER_USER_BASE_VIRT   /**< @see OR1K_CLUSTER_USER_BASE_VIRT   */
	#define KBASE_VIRT   OR1K_CLUSTER_KERNEL_BASE_VIRT /**< @see OR1K_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT   OR1K_CLUSTER_KPOOL_BASE_VIRT  /**< @see OR1K_CLUSTER_KPOOL_BASE_VIRT  */
	#define _UART_ADDR   OR1K_CLUSTER_UART_BASE_PHYS   /**< @see OR1K_CLUSTER_UART_BASE_PHYS   */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_OR1K_CLUSTER_MEMORY_H_ */
