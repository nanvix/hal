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

#ifndef ARCH_CLUSTER_LINUX64_CLUSTER_MEMORY_H_
#define ARCH_CLUSTER_LINUX64_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/linux64-cluster/_linux64-cluster.h>

/**
 * @addtogroup linux64-cluster-mem Memory
 * @ingroup linux64-cluster
 *
 * @brief Memory System
 */
/**@{*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define LINUX64_MEM_SIZE (32*1024*1024)

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define LINUX64_KMEM_SIZE (4*1024*1024)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define LINUX64_KPOOL_SIZE (4*1024*1024)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define LINUX64_UMEM_SIZE (16*1024*1024)

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define LINUX64_KSTACK_SIZE LINUX64_PAGE_SIZE

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	EXTERN vaddr_t LINUX64_USER_BASE_VIRT;   /**< User Base             */
	EXTERN vaddr_t LINUX64_USER_END_VIRT;    /**< User End              */
	EXTERN vaddr_t LINUX64_USTACK_BASE_VIRT; /**< User Stack Base       */
	EXTERN vaddr_t LINUX64_KERNEL_BASE_VIRT; /**< Kernel Base           */
	EXTERN vaddr_t LINUX64_KPOOL_BASE_VIRT;  /**< Kernel Page Pool Base */
	/**@}*/

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define LINUX64_KERNEL_BASE_PHYS 0x00000000 /**< Kernel Base           */
	#define LINUX64_KPOOL_BASE_PHYS  0x01000000 /**< Kernel Page Pool Base */
	#define LINUX64_USER_BASE_PHYS   0x02000000 /**< User Base             */
	/**@}*/

	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define LINUX64_CLUSTER_MEM_REGIONS            2                            /**< Memory Regions number.            */
	#define LINUX64_CLUSTER_ROOT_PGTAB_NUM         LINUX64_CLUSTER_MEM_REGIONS  /**< Root page table size.             */
	#define LINUX64_CLUSTER_MREGION_PT_ALIGN_START 0                            /**< MRegion start page table aligned. */
	#define LINUX64_CLUSTER_MREGION_PT_ALIGN_END   0                            /**< MRegion end page table aligned.   */
	#define LINUX64_CLUSTER_MREGION_PG_ALIGN_START 0                            /**< MRegion start page aligned.       */
	#define LINUX64_CLUSTER_MREGION_PG_ALIGN_END   LINUX64_CLUSTER_MEM_REGIONS  /**< MRegion end page aligned.         */
	/**@}*/

	/**
	 * @brief Flushes the TLB.
	 */
	EXTERN int linux64_cluster_tlb_flush(void);

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

#ifdef __NANVIX_HAL

	/**
	 * @brief Powers of the virtual memory.
	 */
	EXTERN void linux64_cluster_memory_boot(void);

#endif /* __NANVIX_HAL */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE    LINUX64_MEM_SIZE         /**< @see LINUX64_MEM_SIZE         */
	#define KMEM_SIZE      LINUX64_KMEM_SIZE        /**< @see LINUX64_KMEM_SIZE        */
	#define UMEM_SIZE      LINUX64_UMEM_SIZE        /**< @see LINUX64_UMEM_SIZE        */
	#define KSTACK_SIZE    LINUX64_KSTACK_SIZE      /**< @see LINUX64_KSTACK_SIZE      */
	#define KPOOL_SIZE     LINUX64_KPOOL_SIZE       /**< @see LINUX64_KPOOL_SIZE       */
	#define KBASE_PHYS     LINUX64_KERNEL_BASE_PHYS /**< @see LINUX64_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS     LINUX64_KPOOL_BASE_PHYS  /**< @see LINUX64_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS     LINUX64_USER_BASE_PHYS   /**< @see LINUX64_USER_BASE_PHYS   */
	#define USTACK_VIRT    LINUX64_USTACK_BASE_VIRT /**< @see LINUX64_USTACK_BASE_VIRT */
	#define UBASE_VIRT     LINUX64_USER_BASE_VIRT   /**< @see LINUX64_USER_BASE_VIRT   */
	#define UEND_VIRT      LINUX64_USER_END_VIRT    /**< @see LINUX64_USER_END_VIRT    */
	#define KBASE_VIRT     LINUX64_KERNEL_BASE_VIRT /**< @see LINUX64_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT     LINUX64_KPOOL_BASE_VIRT  /**< @see LINUX64_KPOOL_BASE_VIRT  */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS            LINUX64_CLUSTER_MEM_REGIONS            /**< @see LINUX64_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM         LINUX64_CLUSTER_ROOT_PGTAB_NUM         /**< @see LINUX64_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START LINUX64_CLUSTER_MREGION_PT_ALIGN_START /**< @see LINUX64_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END   LINUX64_CLUSTER_MREGION_PT_ALIGN_END   /**< @see LINUX64_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START LINUX64_CLUSTER_MREGION_PG_ALIGN_START /**< @see LINUX64_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END   LINUX64_CLUSTER_MREGION_PG_ALIGN_END   /**< @see LINUX64_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

	/**
	 * @brief Provided Interface
	 */
	/**@{*/
	#define __tlb_flush_fn /**< tlb_flush() */
	/**@}*/

	/**
	 * @see linux64_cluster_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (linux64_cluster_tlb_flush());
	}

/**@endcond*/

#endif /* ARCH_CLUSTER_LINUX64_CLUSTER_MEMORY_H_ */
