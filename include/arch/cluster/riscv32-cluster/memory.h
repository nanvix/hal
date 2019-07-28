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

#ifndef CLUSTER_RISCV32_CLUSTER_MEMORY_H_
#define CLUSTER_RISCV32_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/riscv32-cluster/_riscv32-cluster.h>

/**
 * @addtogroup riscv_cluster-cluster-mem Memory
 * @ingroup riscv_cluster-cluster
 *
 * @brief Memory System
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CLUSTER_MEMMAP

	#include <nanvix/const.h>
	#include <arch/cluster/riscv32-cluster/memmap.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define RISCV32_CLUSTER_KERNEL_BASE_PHYS RISCV32_CLUSTER_DRAM_BASE_PHYS                        /**< Kernel Code and Data */
	#define RISCV32_CLUSTER_KERNEL_END_PHYS  (RISCV32_CLUSTER_KERNEL_BASE_PHYS + RV32GC_PGTAB_SIZE) /**< Kernel End           */
	#define RISCV32_CLUSTER_KPOOL_BASE_PHYS  (RISCV32_CLUSTER_KERNEL_END_PHYS + RV32GC_PGTAB_SIZE)  /**< Kernel Page Pool     */
	#define RISCV32_CLUSTER_KPOOL_END_PHYS   (RISCV32_CLUSTER_KPOOL_BASE_PHYS + RV32GC_PGTAB_SIZE)  /**< Kernel Pool End      */
	#define RISCV32_CLUSTER_USER_BASE_PHYS   RISCV32_CLUSTER_KPOOL_END_PHYS                        /**< User Base            */
	#define RISCV32_CLUSTER_USER_END_PHYS    RISCV32_CLUSTER_DRAM_END_PHYS                         /**< User End             */
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define RISCV32_CLUSTER_PIC_BASE_VIRT    RISCV32_CLUSTER_PIC_BASE_PHYS    /**< PIC Base             */
	#define RISCV32_CLUSTER_PIC_END_VIRT     RISCV32_CLUSTER_PIC_END_PHYS     /**< PIC End              */
	#define RISCV32_CLUSTER_UART_BASE_VIRT   RISCV32_CLUSTER_UART_BASE_PHYS   /**< UART Base            */
	#define RISCV32_CLUSTER_UART_END_VIRT    RISCV32_CLUSTER_UART_END_PHYS    /**< UART End             */
	#define RISCV32_CLUSTER_KERNEL_BASE_VIRT RISCV32_CLUSTER_KERNEL_BASE_PHYS /**< Kernel Code and Data */
	#define RISCV32_CLUSTER_KERNEL_END_VIRT  RISCV32_CLUSTER_KERNEL_END_PHYS  /**< Kernel End           */
	#define RISCV32_CLUSTER_KPOOL_BASE_VIRT  RISCV32_CLUSTER_KPOOL_BASE_PHYS  /**< Kernel Page Pool     */
	#define RISCV32_CLUSTER_KPOOL_END_VIRT   RISCV32_CLUSTER_KPOOL_END_PHYS   /**< Kernel Pool End      */
	#define RISCV32_CLUSTER_USER_BASE_VIRT   0x90000000                       /**< User Base            */
	#define RISCV32_CLUSTER_USER_END_VIRT    0xf0000000                       /**< User End             */
	/**@}*/

	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define RISCV32_CLUSTER_MEM_REGIONS 4                                       /**< Memory Regions number.            */
	#define RISCV32_CLUSTER_ROOT_PGTAB_NUM         RISCV32_CLUSTER_MEM_REGIONS  /**< Root page table size.             */
	#define RISCV32_CLUSTER_MREGION_PT_ALIGN_START 0                            /**< MRegion start page table aligned. */
	#define RISCV32_CLUSTER_MREGION_PT_ALIGN_END   2                            /**< MRegion end page table aligned.   */
	#define RISCV32_CLUSTER_MREGION_PG_ALIGN_START 2                            /**< MRegion start page aligned.       */
	#define RISCV32_CLUSTER_MREGION_PG_ALIGN_END   RISCV32_CLUSTER_MEM_REGIONS  /**< MRegion end page aligned.         */
	/**@}*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define RISCV32_CLUSTER_MEM_SIZE \
		RISCV32_CLUSTER_DRAM_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define RISCV32_CLUSTER_KMEM_SIZE \
		(RISCV32_CLUSTER_KERNEL_END_PHYS - RISCV32_CLUSTER_KERNEL_BASE_PHYS)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define RISCV32_CLUSTER_KPOOL_SIZE \
		(RISCV32_CLUSTER_KPOOL_END_PHYS - RISCV32_CLUSTER_KPOOL_BASE_PHYS)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define RISCV32_CLUSTER_UMEM_SIZE \
		(RISCV32_CLUSTER_USER_END_PHYS - RISCV32_CLUSTER_USER_BASE_PHYS)

#ifndef _ASM_FILE

	/**
	 * @brief Binary Sections
	 */
	/**@{*/
	EXTERN unsigned char __BOOTSTRAP_START; /**< Bootstrap Start */
	EXTERN unsigned char __BOOTSTRAP_END;   /**< Bootstrap End   */
	EXTERN unsigned char __TEXT_START;      /**< Text Start      */
	EXTERN unsigned char __TEXT_END;        /**< Text End        */
	EXTERN unsigned char __DATA_START;      /**< Data Start      */
	EXTERN unsigned char __DATA_END;        /**< Data End        */
	EXTERN unsigned char __BSS_START;       /**< BSS Start       */
	EXTERN unsigned char __BSS_END;         /**< BSS End         */
	/**@}*/

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond riscv_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE  RISCV32_CLUSTER_MEM_SIZE          /**< @see RISCV32_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE    RISCV32_CLUSTER_KMEM_SIZE         /**< @see RISCV32_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE    RISCV32_CLUSTER_UMEM_SIZE         /**< @see RISCV32_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE  RISCV32_CLUSTER_KSTACK_SIZE       /**< @see RISCV32_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE   RISCV32_CLUSTER_KPOOL_SIZE        /**< @see RISCV32_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS   RISCV32_CLUSTER_KERNEL_BASE_PHYS  /**< @see RISCV32_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS   RISCV32_CLUSTER_KPOOL_BASE_PHYS   /**< @see RISCV32_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS   RISCV32_CLUSTER_USER_BASE_PHYS    /**< @see RISCV32_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT  RISCV32_CLUSTER_USTACK_BASE_VIRT  /**< @see RISCV32_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT   RISCV32_CLUSTER_USER_BASE_VIRT    /**< @see RISCV32_CLUSTER_USER_BASE_VIRT   */
	#define UEND_VIRT    RISCV32_CLUSTER_USER_END_VIRT     /**< @see RISCV32_CLUSTER_USER_END_VIRT    */
	#define KBASE_VIRT   RISCV32_CLUSTER_KERNEL_BASE_VIRT  /**< @see RISCV32_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT   RISCV32_CLUSTER_KPOOL_BASE_VIRT   /**< @see RISCV32_CLUSTER_KPOOL_BASE_VIRT  */
	#define _UART_ADDR  RISCV32_CLUSTER_UART_PHYS          /**< @see UART Device                      */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS             RISCV32_CLUSTER_MEM_REGIONS            /**< @see RISCV32_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM          RISCV32_CLUSTER_ROOT_PGTAB_NUM         /**< @see RISCV32_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START  RISCV32_CLUSTER_MREGION_PT_ALIGN_START /**< @see RISCV32_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END    RISCV32_CLUSTER_MREGION_PT_ALIGN_END   /**< @see RISCV32_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START  RISCV32_CLUSTER_MREGION_PG_ALIGN_START /**< @see RISCV32_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END    RISCV32_CLUSTER_MREGION_PG_ALIGN_END   /**< @see RISCV32_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_RISCV32_CLUSTER_MEMORY_H_ */
