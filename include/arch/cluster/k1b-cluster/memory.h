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

#ifndef CLUSTER_K1B_CLUSTER_MEMORY_H_
#define CLUSTER_K1B_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/k1b-cluster/_k1b-cluster.h>

/**
 * @addtogroup k1b-cluster-mem Memory
 * @ingroup k1b-cluster
 *
 * @brief Memory
 */
/**@{*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>

#endif /* _ASM_FILE_ */

	/**
	 * @brief Memory size (in bytes) for Compute Cluster.
	 */
	#define K1B_NODE_MEM_SIZE (2*MB)

	/**
	 * @brief Memory size (in bytes) for IO DDR Cluster.
	 */
	#define K1B_IODDR_MEM_SIZE (1*GB)

	/**
	 * @brief Memory size (in bytes) for IO Ethernet Cluster.
	 */
	#define K1B_IOETH_MEM_SIZE (4*MB)

	/**
	 * @brief Memory size (in bytes).
	 */
	#if defined(__node__)
		#define K1B_CLUSTER_MEM_SIZE K1B_NODE_MEM_SIZE
	#elif defined (__ioddr__)
		#define K1B_CLUSTER_MEM_SIZE K1B_IODDR_MEM_SIZE
	#elif defined (__ioeth__)
		#define K1B_CLUSTER_MEM_SIZE K1B_IOETH_MEM_SIZE
	#endif

	/**
	 * @brief Kernel stack size (in bytes).
	 */
	#if defined(__ioddr__) || defined(__ioeth__)
		#define K1B_CLUSTER_KSTACK_SIZE (0x1800)
	#elif defined(__node__)
		#define K1B_CLUSTER_KSTACK_SIZE (0x800)
	#endif

	/**
	 * @brief Hypervisor size.
	 */
	#define K1B_CLUSTER_HYPER_SIZE (K1B_CLUSTER_HYPER_LOW_BASE_PHYS - K1B_CLUSTER_HYPER_LOW_END_PHYS)

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define K1B_CLUSTER_KMEM_SIZE (K1B_CLUSTER_KERNEL_END_PHYS - K1B_CLUSTER_KERNEL_BASE_PHYS)

	/**
	 * @brief Kernel pool size (in bytes).
	 */
	#if defined(__ioddr__) || defined(__ioeth__)
		#define K1B_CLUSTER_KPOOL_SIZE (0x10000)
	#elif defined(__node__)
		#define K1B_CLUSTER_KPOOL_SIZE (0x10000)
	#endif

	/**
	 * @brief User memory size (in bytes).
	 */
	#if defined(__node__)
		#define K1B_CLUSTER_UMEM_SIZE (512*KB)
	#elif defined (__ioddr__)
		#define K1B_CLUSTER_UMEM_SIZE (512*MB)
	#elif defined (__ioeth__)
		#define K1B_CLUSTER_UMEM_SIZE (512*KB)
	#endif

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#if defined(__ioddr__) || defined(__ioeth__)
		#define K1B_CLUSTER_HYPER_LOW_BASE_PHYS  0x00000000 /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_PHYS   0x00010000 /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_PHYS 0x001f0000 /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_PHYS  0x00200000 /**< High Hypervisor End  */
	#elif defined(__node__)
		#define K1B_CLUSTER_HYPER_LOW_BASE_PHYS  0x00000000 /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_PHYS   0x00008000 /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_PHYS 0x001f8000 /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_PHYS  0x00200000 /**< High Hypervisor End  */
	#endif

	#if defined(__ioddr__)
		#define K1B_CLUSTER_DDR_BASE_PHYS 0x80000000 /**< DDR Base */
		#define K1B_CLUSTER_DDR_END_PHYS  0xc0000000 /**< DDR End */
	#endif

#ifndef _ASM_FILE_
	EXTERN const paddr_t K1B_CLUSTER_KERNEL_BASE_PHYS;      /**< Kernel Base          */
	EXTERN const paddr_t K1B_CLUSTER_KERNEL_END_PHYS;       /**< Kernel End           */
	EXTERN const paddr_t K1B_CLUSTER_KSTACK_BASE_PHYS;      /**< Kernel Stack Base    */
	EXTERN const paddr_t K1B_CLUSTER_KPOOL_BASE_PHYS;       /**< Kernel Pool Base     */
	EXTERN const paddr_t K1B_CLUSTER_KPOOL_END_PHYS;        /**< Kernel Pool End      */
	EXTERN const paddr_t K1B_CLUSTER_USER_BASE_PHYS;        /**< User Base            */
	EXTERN const paddr_t K1B_CLUSTER_USER_END_PHYS;         /**< User End             */
#endif
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#if defined(__ioddr__) || defined(__ioeth__)
		#define K1B_CLUSTER_HYPER_LOW_BASE_VIRT  0x00000000 /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_VIRT   0x00010000 /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_VIRT 0x001f0000 /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_VIRT  0x00200000 /**< High Hypervisor End  */
	#elif defined(__node__)
		#define K1B_CLUSTER_HYPER_LOW_BASE_VIRT  0x00000000 /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_VIRT   0x00008000 /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_VIRT 0x001f8000 /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_VIRT  0x00200000 /**< High Hypervisor End  */
	#endif

	#define K1B_CLUSTER_USTACK_BASE_VIRT K1B_CLUSTER_HYPER_HIGH_BASE_VIRT /**< User Stack   */

	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define K1B_CLUSTER_MEM_REGIONS            4                       /**< Memory Regions number.            */
	#define K1B_CLUSTER_ROOT_PGTAB_NUM         1                       /**< Root page table size.             */
	#define K1B_CLUSTER_MREGION_PT_ALIGN_START 0                       /**< MRegion start page table aligned. */
	#define K1B_CLUSTER_MREGION_PT_ALIGN_END   0                       /**< MRegion end page table aligned.   */
	#define K1B_CLUSTER_MREGION_PG_ALIGN_START 0                       /**< MRegion start page aligned.       */
	#define K1B_CLUSTER_MREGION_PG_ALIGN_END   K1B_CLUSTER_MEM_REGIONS /**< MRegion end page aligned.         */
	/**@}*/

#ifndef _ASM_FILE_
	EXTERN const vaddr_t K1B_CLUSTER_KERNEL_BASE_VIRT;                 /**< Kernel Base                       */
	EXTERN const vaddr_t K1B_CLUSTER_KERNEL_END_VIRT;                  /**< Kernel End                        */
	EXTERN const vaddr_t K1B_CLUSTER_KSTACK_BASE_VIRT;                 /**< Kernel Stack Base                 */
	EXTERN const vaddr_t K1B_CLUSTER_KPOOL_BASE_VIRT;                  /**< Kernel Pool Base                  */
	EXTERN const vaddr_t K1B_CLUSTER_KPOOL_END_VIRT;                   /**< Kernel Pool End                   */
	EXTERN const vaddr_t K1B_CLUSTER_USER_BASE_VIRT;                   /**< User Base                         */
	EXTERN const vaddr_t K1B_CLUSTER_USER_END_VIRT;                    /**< User End                          */
#endif

	#define K1B_CLUSTER_USER_BASE_VIRT         0x80000000              /**< User Base                         */
	#define K1B_CLUSTER_USER_END_VIRT          0xf0000000              /**< User End                          */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Binary Sections
	 */
	/**@{*/
	EXTERN unsigned char __TEXT_START; /**< Text Start */
	EXTERN unsigned char __TEXT_END;   /**< Text End   */
	EXTERN unsigned char __DATA_START; /**< Data Start */
	EXTERN unsigned char __DATA_END;   /**< Data End   */
	EXTERN unsigned char __BSS_START;  /**< BSS Start  */
	EXTERN unsigned char __BSS_END;    /**< BSS End    */
	/**@}*/

	/**
	 * @brief TLB lookup address mask.
	 */
	#define K1B_TLB_VADDR_MASK (~0)

	/**
	 * @brief Gets the underlying TLB entries.
	 *
	 * The k1b_cluster_tlb_get_utlb() function returns the architectural
	 * TLB entries.
	 *
	 * @returns Initial position of the specific underlying tlb entries.
	 */
	EXTERN struct tlbe *k1b_cluster_tlb_get_utlb();

	/**
	 * @brief Gets the configuration of a TLB Entry.
	 *
	 * @param vaddr Target virtual address.
	 *
	 * @return K1B TLB entry does not need configuration.
	 */
	static inline int k1b_cluster_tlb_get_vaddr_info(vaddr_t vaddr)
	{
		UNUSED(vaddr);

		return (0);
	}

	/**
	 * @brief Flushes the TLB.
	 */
	EXTERN int k1b_cluster_tlb_flush(void);

	/**
	 * @brief Initializes the architectural TLB.
	 */
	EXTERN void k1b_cluster_tlb_init(void);

	/**
	 * @brief Shoots down a TLB entry.
	 *
	 * @param vaddr Target virtual address.
	 *
	 * @return Zero if successful, non zero otherwise.
	 */
	EXTERN int k1b_cluster_tlb_shootdown(vaddr_t vaddr);

	/**
	 * @brief Dumps a TLB entry.
	 *
	 * @param idx Index of target entry in the TLB.
	 */
	EXTERN void k1b_cluster_tlbe_dump(int idx);

	/**
	 * @brief Warmups the MMU.
	 */
	EXTERN void k1b_cluster_mem_warmup(void);

	/**
	 * @brief Asserts memory layout.
	 */
	EXTERN void k1b_cluster_mem_check_layout(void);

	/**
	 * @brief Assert memory alignment.
	 */
	EXTERN void k1b_cluster_mem_check_alignment(void);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b_cluster_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE    K1B_CLUSTER_MEM_SIZE         /**< @see K1B_CLUSTER_MEM_SIZE          */
	#define KMEM_SIZE      K1B_CLUSTER_KMEM_SIZE        /**< @see K1B_CLUSTER_KMEM_SIZE         */
	#define UMEM_SIZE      K1B_CLUSTER_UMEM_SIZE        /**< @see K1B_CLUSTER_UMEM_SIZE         */
	#define KSTACK_SIZE    K1B_CLUSTER_KSTACK_SIZE      /**< @see K1B_CLUSTER_KSTACK_SIZE       */
	#define KPOOL_SIZE     K1B_CLUSTER_KPOOL_SIZE       /**< @see K1B_CLUSTER_KPOOL_SIZE        */
	#define KBASE_PHYS     K1B_CLUSTER_KERNEL_BASE_PHYS /**< @see K1B_CLUSTER_KERNEL_BASE_PHYS  */
	#define KPOOL_PHYS     K1B_CLUSTER_KPOOL_BASE_PHYS  /**< @see K1B_CLUSTER_KPOOL_BASE_PHYS   */
	#define UBASE_PHYS     K1B_CLUSTER_USER_BASE_PHYS   /**< @see K1B_CLUSTER_USER_BASE_PHYS    */
	#define UEND_VIRT      K1B_CLUSTER_USER_END_VIRT    /**< @see K1B_CLUSTER_USER_END_VIRT     */
	#define USTACK_VIRT    K1B_CLUSTER_USTACK_BASE_VIRT /**< @see K1B_CLUSTER_USTACK_BASE_VIRT  */
	#define UBASE_VIRT     K1B_CLUSTER_USER_BASE_VIRT   /**< @see K1B_CLUSTER_USER_BASE_VIRT    */
	#define KBASE_VIRT     K1B_CLUSTER_KERNEL_BASE_VIRT /**< @see K1B_CLUSTER_KERNEL_BASE_VIRT  */
	#define KPOOL_VIRT     K1B_CLUSTER_KPOOL_BASE_VIRT  /**< @see K1B_CLUSTER_KPOOL_BASE_VIRT   */
	#define TLB_VADDR_MASK K1B_TLB_VADDR_MASK           /**< @see K1B_TLB_VADDR_MASK            */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS            K1B_CLUSTER_MEM_REGIONS            /**< @see K1B_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM         K1B_CLUSTER_ROOT_PGTAB_NUM         /**< @see K1B_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START K1B_CLUSTER_MREGION_PT_ALIGN_START /**< @see K1B_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END   K1B_CLUSTER_MREGION_PT_ALIGN_END   /**< @see K1B_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START K1B_CLUSTER_MREGION_PG_ALIGN_START /**< @see K1B_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END   K1B_CLUSTER_MREGION_PG_ALIGN_END   /**< @see K1B_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Provided Interface
	 */
	/**@{*/
	#define __tlb_flush_fn          /**< tlb_flush()          */
	#define __tlb_init_fn           /**< tlb_init()           */
	#define __tlb_get_vaddr_info_fn /**< tlb_get_vaddr_info() */
	#define __tlb_get_utlb_fn       /**< tlb_get_utlb()       */
	#define __tlb_shootdown_fn      /**< tlb_shootdown()      */
	/**@}*/

	/**
	 * @brief Provided Memory Interface
	 */
	/**@{*/
	#define __mem_warmup_fn        /**< mem_warmup()       */
	#define __mem_check_layout_fn  /**< mem_check_layout() */
	#define __mem_check_align_fn   /**< mem_check_align()  */
	/**@}*/

	/**
	 * @see k1b_cluster_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (k1b_cluster_tlb_flush());
	}

	/**
	 * @see k1b_cluster_tlb_init().
	 */
	static inline void tlb_init(void)
	{
		k1b_cluster_tlb_init();
	}

	/**
	 * @see k1b_cluster_tlb_get_vaddr_info().
	 */
	static inline int tlb_get_vaddr_info(vaddr_t vaddr)
	{
		return (k1b_cluster_tlb_get_vaddr_info(vaddr));
	}

	/**
	 * @see k1b_cluster_tlb_lookup_paddr().
	 */
	static inline struct tlbe *tlb_get_utlb(int tlb_type)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (NULL);

		return (k1b_cluster_tlb_get_utlb());
	}

	/**
	 * @see k1b_cluster_tlb_shootdown().
	 */
	static inline int tlb_shootdown(vaddr_t vaddr)
	{
		return (k1b_cluster_tlb_shootdown(vaddr));
	}

	/**
	 * @see k1b_cluster_mem_warmup().
	 */
	static inline void mem_warmup(void)
	{
		k1b_cluster_mem_warmup();
	}

	/**
	 * @see k1b_cluster_mem_check_layout().
	 */
	static inline void mem_check_layout(void)
	{
		k1b_cluster_mem_check_layout();
	}

	/**
	 * @see k1b_cluster_mem_check_alignment().
	 */
	static inline void mem_check_align(void)
	{
		k1b_cluster_mem_check_alignment();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* CLUSTER_K1B_CLUSTER_MEMORY_H_ */
