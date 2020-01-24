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
	 * @name Physical Memory Layout Aliases
	 */
	/**@{*/
	#define OR1K_CLUSTER_KERNEL_BASE_PHYS OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS /**< Kernel Code and Data */
	#define OR1K_CLUSTER_KERNEL_END_PHYS  OPTIMSOC_CLUSTER_KERNEL_END_PHYS  /**< Kernel End           */
	#define OR1K_CLUSTER_KPOOL_BASE_PHYS  OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  /**< Kernel Page Pool     */
	#define OR1K_CLUSTER_KPOOL_END_PHYS   OPTIMSOC_CLUSTER_KPOOL_END_PHYS   /**< Kernel Pool End      */
	#define OR1K_CLUSTER_USER_BASE_PHYS   OPTIMSOC_CLUSTER_USER_BASE_PHYS   /**< User Base            */
	#define OR1K_CLUSTER_USER_END_PHYS    OPTIMSOC_CLUSTER_USER_END_PHYS    /**< User End             */
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
	#define OPTIMSOC_CLUSTER_USER_BASE_VIRT   0xa0000000                        /**< User Base            */
	#define OPTIMSOC_CLUSTER_USER_END_VIRT    0xc0000000                        /**< User End             */
	#define OPTIMSOC_CLUSTER_USTACK_BASE_VIRT 0xc0000000                        /**< User Stack Base      */
	#define OPTIMSOC_CLUSTER_USTACK_END_VIRT  0xb0000000                        /**< User Stack End       */
	/**@}*/

	/**
	 * @name Virtual Memory Layout Aliases
	 */
	/**@{*/
	#define OR1K_CLUSTER_OMPIC_BASE_VIRT  OPTIMSOC_CLUSTER_OMPIC_BASE_VIRT  /**< PIC Base             */
	#define OR1K_CLUSTER_OMPIC_END_VIRT   OPTIMSOC_CLUSTER_OMPIC_END_VIRT   /**< PIC End              */
	#define OR1K_CLUSTER_KERNEL_BASE_VIRT OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT /**< Kernel Code and Data */
	#define OR1K_CLUSTER_KERNEL_END_VIRT  OPTIMSOC_CLUSTER_KERNEL_END_VIRT  /**< Kernel End           */
	#define OR1K_CLUSTER_KPOOL_BASE_VIRT  OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  /**< Kernel Page Pool     */
	#define OR1K_CLUSTER_KPOOL_END_VIRT   OPTIMSOC_CLUSTER_KPOOL_END_VIRT   /**< Kernel Pool End      */
	#define OR1K_CLUSTER_USER_BASE_VIRT   OPTIMSOC_CLUSTER_USER_BASE_VIRT   /**< User Base            */
	#define OR1K_CLUSTER_USER_END_VIRT    OPTIMSOC_CLUSTER_USER_END_VIRT    /**< User End             */
	#define OR1K_CLUSTER_USTACK_BASE_VIRT OPTIMSOC_CLUSTER_USTACK_BASE_VIRT /**< User Stack Base      */
	#define OR1K_CLUSTER_USTACK_END_VIRT  OPTIMSOC_CLUSTER_USTACK_END_VIRT  /**< User Stack End       */
	/**@}*/

	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define OPTIMSOC_CLUSTER_MEM_REGIONS            3                            /**< Memory Regions number.            */
	#define OPTIMSOC_CLUSTER_ROOT_PGTAB_NUM         OPTIMSOC_CLUSTER_MEM_REGIONS /**< Root page table size.             */
	#define OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_START 0                            /**< MRegion start page table aligned. */
	#define OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_END   2                            /**< MRegion end page table aligned.   */
	#define OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_START 2                            /**< MRegion start page aligned.       */
	#define OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_END   OPTIMSOC_CLUSTER_MEM_REGIONS /**< MRegion end page aligned.         */
	/**@}*/

	/**
	 * @name Memory Regions Constants Aliases
	 */
	/**@{*/
	#define OR1K_CLUSTER_MEM_REGIONS            OPTIMSOC_CLUSTER_MEM_REGIONS            /**< Memory Regions number.            */
	#define OR1K_CLUSTER_ROOT_PGTAB_NUM         OPTIMSOC_CLUSTER_MEM_REGIONS            /**< Root page table size.             */
	#define OR1K_CLUSTER_MREGION_PT_ALIGN_START OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_START /**< MRegion start page table aligned. */
	#define OR1K_CLUSTER_MREGION_PT_ALIGN_END   OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_END   /**< MRegion end page table aligned.   */
	#define OR1K_CLUSTER_MREGION_PG_ALIGN_START OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_START /**< MRegion start page aligned.       */
	#define OR1K_CLUSTER_MREGION_PG_ALIGN_END   OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_END   /**< MRegion end page aligned.         */

	/**
	 * @brief Memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_MEM_SIZE \
		OPTIMSOC_CLUSTER_DRAM_SIZE

	#define OR1K_CLUSTER_MEM_SIZE \
		OR1K_CLUSTER_DRAM_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KMEM_SIZE \
		(OPTIMSOC_CLUSTER_KERNEL_END_PHYS - OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS)

	#define OR1K_CLUSTER_KMEM_SIZE OPTIMSOC_CLUSTER_KMEM_SIZE

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KPOOL_SIZE \
		(OPTIMSOC_CLUSTER_KPOOL_END_PHYS - OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS)

	#define OR1K_CLUSTER_KPOOL_SIZE OPTIMSOC_CLUSTER_KPOOL_SIZE

	/**
	 * @brief User memory size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_UMEM_SIZE \
		(OPTIMSOC_CLUSTER_USER_END_PHYS - OPTIMSOC_CLUSTER_USER_BASE_PHYS)

	#define OR1K_CLUSTER_UMEM_SIZE OPTIMSOC_CLUSTER_UMEM_SIZE

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#define OPTIMSOC_CLUSTER_KSTACK_SIZE OPTIMSOC_PAGE_SIZE
	#define OR1K_CLUSTER_KSTACK_SIZE OPTIMSOC_CLUSTER_KSTACK_SIZE

	/**
	 * @brief OMPIC Registers and flags.
	 */
	/**@{*/
	#define OPTIMSOC_OMPIC_CPUBYTES	        8
	#define OPTIMSOC_OMPIC_CTRL(cpu)        ((0x0 + ((cpu) * OPTIMSOC_OMPIC_CPUBYTES)))
	#define OPTIMSOC_OMPIC_STAT(cpu)        ((0x4 + ((cpu) * OPTIMSOC_OMPIC_CPUBYTES)))
	#define OPTIMSOC_OMPIC_CTRL_IRQ_ACK	    (1 << 31)
	#define OPTIMSOC_OMPIC_CTRL_IRQ_GEN	    (1 << 30)
	#define OPTIMSOC_OMPIC_CTRL_DST(cpu)    (((cpu) & 0x3fff) << 16)
	#define OPTIMSOC_OMPIC_STAT_IRQ_PENDING (1 << 30)
	#define OPTIMSOC_OMPIC_DATA(x)          ((x) & 0xffff)
	#define OPTIMSOC_OMPIC_STAT_SRC(x)      (((x) >> 16) & 0x3fff)
	/**@}*/

	/**
	 * @brief OMPIC Registers and flags Aliases.
	 */
	/**@{*/
	#define OR1K_OMPIC_CPUBYTES	        8
	#define OR1K_OMPIC_CTRL(cpu)        ((0x0 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_STAT(cpu)        ((0x4 + ((cpu) * OR1K_OMPIC_CPUBYTES)))
	#define OR1K_OMPIC_CTRL_IRQ_ACK	    (1 << 31)
	#define OR1K_OMPIC_CTRL_IRQ_GEN	    (1 << 30)
	#define OR1K_OMPIC_CTRL_DST(cpu)    (((cpu) & 0x3fff) << 16)
	#define OR1K_OMPIC_STAT_IRQ_PENDING (1 << 30)
	#define OR1K_OMPIC_DATA(x)          ((x) & 0xffff)
	#define OR1K_OMPIC_STAT_SRC(x)      (((x) >> 16) & 0x3fff)
	/**@}*/

#ifndef _ASM_FILE_

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

	/**
	 * @brief TLB lookup address mask.
	 */
	#define OPTIMSOC_TLB_VADDR_MASK PAGE_MASK
	#define OR1K_TLB_VADDR_MASK     PAGE_MASK

	/**
	 * @see or1k_cluster_tlb_get_utlb
	 */
	EXTERN struct tlbe *or1k_cluster_tlb_get_utlb(int tlb_type);

	/**
	 * @see or1k_cluster_tlb_get_vaddr_info
	 */
	EXTERN int or1k_cluster_tlb_get_vaddr_info(vaddr_t vaddr);

	/**
	 * @see or1k_cluster_tlb_flush
	 */
	EXTERN int or1k_cluster_tlb_flush(void);

	/**
	 * @see or1k_cluster_tlb_init
	 */
	EXTERN void or1k_cluster_tlb_init(void);

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
	#define MEMORY_SIZE    OPTIMSOC_CLUSTER_MEM_SIZE         /**< @see OPTIMSOC_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE      OPTIMSOC_CLUSTER_KMEM_SIZE        /**< @see OPTIMSOC_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE      OPTIMSOC_CLUSTER_UMEM_SIZE        /**< @see OPTIMSOC_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE    OPTIMSOC_CLUSTER_KSTACK_SIZE      /**< @see OPTIMSOC_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE     OPTIMSOC_CLUSTER_KPOOL_SIZE       /**< @see OPTIMSOC_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS     OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS /**< @see OPTIMSOC_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS     OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  /**< @see OPTIMSOC_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS     OPTIMSOC_CLUSTER_USER_BASE_PHYS   /**< @see OPTIMSOC_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT    OPTIMSOC_CLUSTER_USTACK_BASE_VIRT /**< @see OPTIMSOC_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT     OPTIMSOC_CLUSTER_USER_BASE_VIRT   /**< @see OPTIMSOC_CLUSTER_USER_BASE_VIRT   */
	#define UEND_VIRT      OPTIMSOC_CLUSTER_USER_END_VIRT    /**< @see OPTIMSOC_CLUSTER_USER_END_VIRT    */
	#define KBASE_VIRT     OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT /**< @see OPTIMSOC_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT     OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  /**< @see OPTIMSOC_CLUSTER_KPOOL_BASE_VIRT  */
	#define TLB_VADDR_MASK OPTIMSOC_TLB_VADDR_MASK           /**< @see OPTIMSOC_TLB_VADDR_MASK           */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS            OPTIMSOC_CLUSTER_MEM_REGIONS            /**< @see OPTIMSOC_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM         OPTIMSOC_CLUSTER_ROOT_PGTAB_NUM         /**< @see OPTIMSOC_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_START /**< @see OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END   OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_END   /**< @see OPTIMSOC_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_START /**< @see OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END   OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_END   /**< @see OPTIMSOC_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __tlb_flush_fn          /**< tlb_flush()          */
	#define __tlb_init_fn           /**< tlb_init()           */
	#define __tlb_get_vaddr_info_fn /**< tlb_get_vaddr_info() */
	#define __tlb_get_utlb_fn       /**< tlb_get_utlb()       */
	/**@}*/

/**
 * @name TLB Aliases
 */
/**@{*/

	/**
	 * @see or1k_cluster_tlb_get_utlb
	 */
	static inline struct tlbe *optimsoc_cluster_tlb_get_utlb(int tlb_type)
	{
		return (or1k_cluster_tlb_get_utlb(tlb_type));
	}

	/**
	 * @see or1k_cluster_tlb_get_vaddr_info
	 */
	static inline int optimsoc_cluster_tlb_get_vaddr_info(vaddr_t vaddr)
	{
		return (or1k_cluster_tlb_get_vaddr_info(vaddr));
	}

	/**
	 * @see or1k_cluster_tlb_flush
	 */
	static inline int optimsoc_cluster_tlb_flush(void)
	{
		return (or1k_cluster_tlb_flush());
	}

	/**
	 * @see or1k_cluster_tlb_init
	 */
	static inline void optimsoc_cluster_tlb_init(void)
	{
		or1k_cluster_tlb_init();
	}

/**@}*/

	/**
	 * @see or1k_cluster_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (optimsoc_cluster_tlb_flush());
	}

	/**
	 * @see or1k_cluster_tlb_get_vaddr_info().
	 */
	static inline int tlb_get_vaddr_info(vaddr_t vaddr)
	{
		return (optimsoc_cluster_tlb_get_vaddr_info(vaddr));
	}

	/**
	 * @see or1k_cluster_tlb_get_utlb().
	 */
	static inline struct tlbe *tlb_get_utlb(int tlb_type)
	{
		/* Invalid TLB type. */
		if ((tlb_type != OR1K_TLB_INSTRUCTION) && (tlb_type != OR1K_TLB_DATA))
			return (NULL);

		return (optimsoc_cluster_tlb_get_utlb(tlb_type));
	}

	/**
	 * @see or1k_cluster_tlb_init().
	 */
	static inline void tlb_init(void)
	{
		optimsoc_cluster_tlb_init();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* CLUSTER_OPTIMSOC_CLUSTER_MEMORY_H_ */
