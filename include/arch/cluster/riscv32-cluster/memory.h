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

#ifndef _ASM_FILE

	#include <nanvix/const.h>

#endif /* _ASM_FILE_ */

	/**
	 * @brief DRAM size (in bytes).
	 */
	#define RISCV32_CLUSTER_DRAM_SIZE (128*MB)

	/**
	 * @brief Base DRAM address.
	 */
	#define RISCV32_DRAM_BASE 0x80000000

	/**
	 * @brief End DRAM address.
	 */
	#define RISCV32_DRAM_END (RISCV32_DRAM_BASE + RISCV32_CLUSTER_DRAM_SIZE)

	/**
	 * @brief Memory size (in bytes).
	 */
	#define RISCV32_CLUSTER_MEM_SIZE (RISCV32_CLUSTER_DRAM_SIZE)

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define RISCV32_CLUSTER_KMEM_SIZE RV32I_PGTAB_SIZE

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define RISCV32_CLUSTER_KPOOL_SIZE RV32I_PGTAB_SIZE

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define RISCV32_CLUSTER_PIC_PHYS         0x02000000                                             /**< PIC                  */
	#define RISCV32_CLUSTER_UART_PHYS        0x10000000                                             /**< UART                 */
	#define RISCV32_CLUSTER_KERNEL_BASE_PHYS RISCV32_DRAM_BASE                                      /**< Kernel Code and Data */
	#define RISCV32_CLUSTER_KERNEL_END_PHYS  (RISCV32_CLUSTER_KERNEL_BASE_PHYS + RISCV32_CLUSTER_KMEM_SIZE) /**< Kernel End           */
	#define RISCV32_CLUSTER_KPOOL_BASE_PHYS  0x80400000                                             /**< Kernel Page Pool     */
	#define RISCV32_CLUSTER_KPOOL_END_PHYS   (RISCV32_CLUSTER_KPOOL_BASE_PHYS + RISCV32_CLUSTER_KPOOL_SIZE) /**< Kernel Pool End      */
	#define RISCV32_CLUSTER_USER_BASE_PHYS   0x80800000                                             /**< User Base            */
	#define RISCV32_CLUSTER_USER_END_PHYS    RISCV32_DRAM_END                                       /**< User End             */
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define RISCV32_CLUSTER_PIC_VIRT         0x02000000                                             /**< PIC                  */
	#define RISCV32_CLUSTER_UART_VIRT        0x10000000                                             /**< UART                 */
	#define RISCV32_CLUSTER_KERNEL_BASE_VIRT 0x80000000                                             /**< Kernel Code and Data */
	#define RISCV32_CLUSTER_KERNEL_END_VIRT  (RISCV32_CLUSTER_KERNEL_BASE_VIRT + RISCV32_CLUSTER_KMEM_SIZE) /**< Kernel End           */
	#define RISCV32_CLUSTER_KPOOL_BASE_VIRT  0x80400000                                             /**< Kernel Page Pool     */
	#define RISCV32_CLUSTER_KPOOL_END_VIRT   (RISCV32_CLUSTER_KPOOL_BASE_VIRT + RISCV32_CLUSTER_KPOOL_SIZE) /**< Kernel Pool End      */
	#define RISCV32_CLUSTER_USER_BASE_VIRT   0x90000000                                             /**< User Base            */
	#define RISCV32_CLUSTER_USER_END_VIRT    0xf0000000                                             /**< User End             */
	/**@}*/

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
	 * @brief Memory size (in bytes).
	 */
	#define _MEMORY_SIZE RISCV32_CLUSTER_MEM_SIZE

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define _KSTACK_SIZE RISCV32_CLUSTER_PAGE_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define _KMEM_SIZE RISCV32_CLUSTER_KMEM_SIZE

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define _KPOOL_SIZE RISCV32_CLUSTER_KPOOL_SIZE

	/**
	 * @brief User memory size (in bytes).
	 */
	#define _UMEM_SIZE (RISCV32_CLUSTER_USER_END_PHYS - RISCV32_CLUSTER_USER_BASE_PHYS)

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define _UBASE_VIRT  RISCV32_CLUSTER_UBASE_VIRT  /**< User Base        */
	#define _USTACK_ADDR RISCV32_CLUSTER_USTACK_ADDR /**< User Stack       */
	#define _KBASE_VIRT  RISCV32_CLUSTER_KBASE_VIRT  /**< Kernel Base      */
	#define _KPOOL_VIRT  RISCV32_CLUSTER_KPOOL_VIRT  /**< Kernel Page Pool */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define _KBASE_PHYS RISCV32_CLUSTER_KBASE_PHYS /**< Kernel Base      */
	#define _KPOOL_PHYS RISCV32_CLUSTER_KPOOL_PHYS /**< Kernel Page Pool */
	#define _UBASE_PHYS RISCV32_CLUSTER_UBASE_PHYS /**< User Base        */
	#define _UART_ADDR  RISCV32_CLUSTER_UART_PHYS  /**< UART Device      */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_RISCV32_CLUSTER_MEMORY_H_ */
