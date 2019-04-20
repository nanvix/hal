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

#ifndef ARCH_CLUSTER_OPTIMSOC_CLUSTER_MEMORY_H_
#define ARCH_CLUSTER_OPTIMSOC_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

	/* Must come first. */
	#define __NEED_CLUSTER_MEMMAP

/**
 * @addtogroup optimsoc-cluster-mem Memory
 * @ingroup optimsoc-cluster
 *
 * @brief Memory Interface
 */
/**@{*/

	#include <arch/cluster/optimsoc-cluster/memmap.h>
	#include <nanvix/const.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS OPTIMSOC_CLUSTER_DRAM_BASE_PHYS                       /**< Kernel Code and Data */
	#define OPTIMSOC_CLUSTER_KERNEL_END_PHYS  (OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS + OR1K_PGTAB_SIZE) /**< Kernel End           */
	#define OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  (OPTIMSOC_CLUSTER_KERNEL_END_PHYS + OR1K_PGTAB_SIZE)  /**< Kernel Page Pool     */
	#define OPTIMSOC_CLUSTER_KPOOL_END_PHYS   (OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS + OR1K_PGTAB_SIZE)  /**< Kernel Pool End      */
	#define OPTIMSOC_CLUSTER_USER_BASE_PHYS   OPTIMSOC_CLUSTER_KPOOL_END_PHYS                       /**< User Base            */
	#define OPTIMSOC_CLUSTER_USER_END_PHYS    OPTIMSOC_CLUSTER_DRAM_END_PHYS                        /**< User End             */
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define OPTIMSOC_CLUSTER_OMPIC_BASE_VIRT  OPTIMSOC_CLUSTER_OMPIC_BASE_PHYS  /**< PIC Base             */
	#define OPTIMSOC_CLUSTER_OMPIC_END_VIRT   OPTIMSOC_CLUSTER_OMPIC_END_PHYS   /**< PIC End              */
	#define OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS /**< Kernel Code and Data */
	#define OPTIMSOC_CLUSTER_KERNEL_END_VIRT  OPTIMSOC_CLUSTER_KERNEL_END_PHYS  /**< Kernel End           */
	#define OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  /**< Kernel Page Pool     */
	#define OPTIMSOC_CLUSTER_KPOOL_END_VIRT   OPTIMSOC_CLUSTER_KPOOL_END_PHYS   /**< Kernel Pool End      */
	#define OPTIMSOC_CLUSTER_USER_BASE_VIRT   0xa0000000                    /**< User Base            */
	#define OPTIMSOC_CLUSTER_USER_END_VIRT    0xc0000000                    /**< User End             */
	#define OPTIMSOC_CLUSTER_USTACK_BASE_VIRT 0xc0000000                    /**< User Stack Base      */
	#define OPTIMSOC_CLUSTER_USTACK_END_VIRT  0xb0000000                    /**< User Stack End       */
	/**@}*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_MEM_SIZE \
		OPTIMSOC_CLUSTER_DRAM_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KMEM_SIZE \
		(OPTIMSOC_CLUSTER_KERNEL_END_PHYS - OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KPOOL_SIZE \
		(OPTIMSOC_CLUSTER_KPOOL_END_PHYS - OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_UMEM_SIZE \
		(OPTIMSOC_CLUSTER_USER_END_PHYS - OPTIMSOC_CLUSTER_USER_BASE_PHYS)

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KSTACK_SIZE OPTIMSOC_PAGE_SIZE

	/**
	 * @brief OMPIC Registers and flags.
	 */
	/**@{*/
	#define OPTIMSOC_OMPIC_CPUBYTES	        8
	#define OPTIMSOC_OMPIC_CTRL(cpu)        (OPTIMSOC_CLUSTER_OMPIC_BASE_VIRT + (0x0 + ((cpu) * OPTIMSOC_OMPIC_CPUBYTES)))
	#define OPTIMSOC_OMPIC_STAT(cpu)        (OPTIMSOC_CLUSTER_OMPIC_BASE_VIRT + (0x4 + ((cpu) * OPTIMSOC_OMPIC_CPUBYTES)))
	#define OPTIMSOC_OMPIC_CTRL_IRQ_ACK	    (1 << 31)
	#define OPTIMSOC_OMPIC_CTRL_IRQ_GEN	    (1 << 30)
	#define OPTIMSOC_OMPIC_CTRL_DST(cpu)    (((cpu) & 0x3fff) << 16)
	#define OPTIMSOC_OMPIC_STAT_IRQ_PENDING (1 << 30)
	#define OPTIMSOC_OMPIC_DATA(x)          ((x) & 0xffff)
	#define OPTIMSOC_OMPIC_STAT_SRC(x)      (((x) >> 16) & 0x3fff)
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the Memory Interface.
	 */
	EXTERN void optimsoc_cluster_mem_setup(void);

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond optimsoc_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE  OPTIMSOC_CLUSTER_MEM_SIZE         /**< @see OPTIMSOC_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE    OPTIMSOC_CLUSTER_KMEM_SIZE        /**< @see OPTIMSOC_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE    OPTIMSOC_CLUSTER_UMEM_SIZE        /**< @see OPTIMSOC_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE  OPTIMSOC_CLUSTER_KSTACK_SIZE      /**< @see OPTIMSOC_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE   OPTIMSOC_CLUSTER_KPOOL_SIZE       /**< @see OPTIMSOC_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS   OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS /**< @see OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS   OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  /**< @see OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS   OPTIMSOC_CLUSTER_USER_BASE_PHYS   /**< @see OPTIMSOC_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT  OPTIMSOC_CLUSTER_USTACK_BASE_VIRT /**< @see OPTIMSOC_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT   OPTIMSOC_CLUSTER_USER_BASE_VIRT   /**< @see OPTIMSOC_CLUSTER_USER_BASE_VIRT   */
	#define KBASE_VIRT   OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT /**< @see OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT   OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  /**< @see OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_OPTIMSOC_CLUSTER_MEMORY_H_ */
