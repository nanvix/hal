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

#ifndef CLUSTER_ARM64_CLUSTER_MEMORY_H_
#define CLUSTER_ARM64_CLUSTER_MEMORY_H_
#define __NEED_CLUSTER_MEMMAP

	/* Cluster Interface Implementation */
	#include <arch/cluster/arm64-cluster/_arm64-cluster.h>
	#include <arch/cluster/arm64-cluster/memmap.h>

/**
 * @addtogroup arm64_cluster-cluster-mem Memory
 * @ingroup arm64_cluster-cluster
 *
 * @brief Memory System
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define ARM64_CLUSTER_KERNEL_BASE_PHYS		ARM64_CLUSTER_KERNEL_BASE_VIRT
	#define ARM64_CLUSTER_KERNEL_END_PHYS 		ARM64_CLUSTER_KERNEL_END_VIRT
	#define ARM64_CLUSTER_USER_BASE_PHYS		ARM64_CLUSTER_USER_BASE_VIRT
	#define ARM64_CLUSTER_USER_END_PHYS			ARM64_CLUSTER_USER_END_VIRT
	#define ARM64_CLUSTER_MODULE_START_PHYS		ARM64_CLUSTER_MODULE_START_VIRT
	#define ARM64_CLUSTER_MODULE_END_PHYS		ARM64_CLUSTER_MODULE_END_VIRT
	#define ARM64_CLUSTER_MEM_SIZE 				ARM64_CLUSTER_DRAM_SIZE
	#define ARM64_CLUSTER_KPOOL_BASE_PHYS		0
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	//#define ARM64_CLUSTER_KERNEL_BASE_VIRT 		(ARM64_CLUSTER_KERNEL_END_VIRT << (ARM64_VADDR_BIT - 1))
	#define ARM64_CLUSTER_KERNEL_BASE_VIRT 		(KERNEL_VA_START)
	#define ARM64_CLUSTER_KERNEL_END_VIRT		(ARM64_CLUSTER_KERNEL_BASE_VIRT + 4 * ARM64_PAGE_SIZE)
	//#define ARM64_CLUSTER_KERNEL_END_VIRT		(0xffffffffffe00000)
	#define ARM64_CLUSTER_USER_BASE_VIRT		(0x0000000000000000)
	#define ARM64_CLUSTER_USER_END_VIRT			(ARM64_CLUSTER_USER_BASE_VIRT + ARM64_PGTAB_SIZE)
	#define ARM64_CLUSTER_MODULE_END_VIRT		ARM64_CLUSTER_KERNEL_BASE_VIRT
	#define ARM64_CLUSTER_MODULE_START_VIRT		(ARM64_CLUSTER_MODULE_END_VIRT - 0x04000000)

	#define ARM64_CLUSTER_KPOOL_BASE_VIRT		0
	#define ARM64_CLUSTER_USTACK_BASE_VIRT		0
	/**@}*/


	#define ARM64_CLUSTER_KMEM_SIZE \
		ARM64_CLUSTER_KERNEL_END_VIRT - ARM64_CLUSTER_KERNEL_BASE_VIRT


	#define ARM64_CLUSTER_UMEM_SIZE \
		ARM64_CLUSTER_USER_END_VIRT - ARM64_CLUSTER_USER_BASE_VIRT

	#define ARM64_CLUSTER_MODULE_SIZE \
		ARM64_CLUSTER_MODULE_END_VIRT - ARM64_CLUSTER_MODULE_START_VIRT

	#define ARM64_CLUSTER_KSTACK_SIZE 0
	#define ARM64_CLUSTER_KPOOL_SIZE 0


	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define ARM64_CLUSTER_MEM_REGIONS            2                            /**< Memory Regions number.            */
	#define ARM64_CLUSTER_ROOT_PGTAB_NUM         ARM64_CLUSTER_MEM_REGIONS  /**< Root page table size.             */
	#define ARM64_CLUSTER_MREGION_PT_ALIGN_START 0                            /**< MRegion start page table aligned. */
	#define ARM64_CLUSTER_MREGION_PT_ALIGN_END   0                            /**< MRegion end page table aligned.   */
	#define ARM64_CLUSTER_MREGION_PG_ALIGN_START 0                            /**< MRegion start page aligned.       */
	#define ARM64_CLUSTER_MREGION_PG_ALIGN_END   ARM64_CLUSTER_MEM_REGIONS  /**< MRegion end page aligned.         */
	#define TASK_SIZE							(1 << ARM64_VADDR_BIT)
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
 * @cond arm64_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE ARM64_CLUSTER_MEM_SIZE         	/**< @see ARM64_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE   ARM64_CLUSTER_KMEM_SIZE        	/**< @see ARM64_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE   ARM64_CLUSTER_UMEM_SIZE        	/**< @see ARM64_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE ARM64_CLUSTER_KSTACK_SIZE      	/**< @see ARM64_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE  ARM64_CLUSTER_KPOOL_SIZE       	/**< @see ARM64_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS  ARM64_CLUSTER_KERNEL_BASE_PHYS 	/**< @see ARM64_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS  ARM64_CLUSTER_KPOOL_BASE_PHYS  	/**< @see ARM64_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS  ARM64_CLUSTER_USER_BASE_PHYS   	/**< @see ARM64_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT ARM64_CLUSTER_USTACK_BASE_VIRT 	/**< @see ARM64_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT  ARM64_CLUSTER_USER_BASE_VIRT   	/**< @see ARM64_CLUSTER_USER_BASE_VIRT   */
	#define UEND_VIRT   ARM64_CLUSTER_USER_END_VIRT    	/**< @see ARM64_CLUSTER_USER_END_VIRT    */
	#define KBASE_VIRT  ARM64_CLUSTER_KERNEL_BASE_VIRT 	/**< @see ARM64_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT  ARM64_CLUSTER_KPOOL_BASE_VIRT  	/**< @see ARM64_CLUSTER_KPOOL_BASE_VIRT  */
	#define _UART_ADDR  UART_PA_BASE        			/**< @see UART Device                      */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS            ARM64_CLUSTER_MEM_REGIONS            /**< @see ARM64_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM         ARM64_CLUSTER_ROOT_PGTAB_NUM         /**< @see ARM64_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START ARM64_CLUSTER_MREGION_PT_ALIGN_START /**< @see ARM64_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END   ARM64_CLUSTER_MREGION_PT_ALIGN_END   /**< @see ARM64_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START ARM64_CLUSTER_MREGION_PG_ALIGN_START /**< @see ARM64_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END   ARM64_CLUSTER_MREGION_PG_ALIGN_END   /**< @see ARM64_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_ARM64_CLUSTER_MEMORY_H_ */
