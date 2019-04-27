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
	#define K1B_IODDR_MEM_SIZE (4*MB)

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
	#if defined(__ioddr__) || defined(__ioeth__)
		#define K1B_CLUSTER_UMEM_SIZE (512*KB)
	#elif defined(__node__)
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
		#define K1B_CLUSTER_HYPER_LOW_BASE_VIRT  0x00000000       /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_VIRT   0x00010000       /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_VIRT 0x001f0000       /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_VIRT  0x00200000       /**< High Hypervisor End  */
	#elif defined(__node__)
		#define K1B_CLUSTER_HYPER_LOW_BASE_VIRT  0x00000000       /**< Low Hypervisor Base  */
		#define K1B_CLUSTER_HYPER_LOW_END_VIRT   0x00008000       /**< Low Hypervisor End   */
		#define K1B_CLUSTER_HYPER_HIGH_BASE_VIRT 0x001f8000       /**< High Hypervisor Base */
		#define K1B_CLUSTER_HYPER_HIGH_END_VIRT  0x00200000       /**< High Hypervisor End  */
	#endif
	#define K1B_CLUSTER_USTACK_BASE_VIRT K1B_CLUSTER_HYPER_HIGH_BASE_VIRT /**< User Stack */
#ifndef _ASM_FILE_
	EXTERN const vaddr_t K1B_CLUSTER_KERNEL_BASE_VIRT;            /**< Kernel Base          */
	EXTERN const vaddr_t K1B_CLUSTER_KERNEL_END_VIRT;             /**< Kernel End           */
	EXTERN const vaddr_t K1B_CLUSTER_KSTACK_BASE_VIRT;            /**< Kernel Stack Base    */
	EXTERN const vaddr_t K1B_CLUSTER_KPOOL_BASE_VIRT;             /**< Kernel Pool Base     */
	EXTERN const vaddr_t K1B_CLUSTER_KPOOL_END_VIRT;              /**< Kernel Pool End      */
	EXTERN const vaddr_t K1B_CLUSTER_USER_BASE_VIRT;              /**< User Base            */
	EXTERN const vaddr_t K1B_CLUSTER_USER_END_VIRT;               /**< User End             */
#endif
	#define K1B_CLUSTER_USER_BASE_VIRT 0x80000000                 /**< User Base            */
	#define K1B_CLUSTER_USER_END_VIRT  0xf0000000                 /**< User End             */
	/**@}*/

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the Memory Interface.
	 */
	EXTERN void k1b_cluster_mem_setup(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Writes a TLB entry.
	 *
	 * @param vaddr      Target virtual address.
	 * @param paddr      Target physical address.
	 * @param shift      Page shift.
	 * @param way        Target set-associative way.
	 * @param protection Protection attributes.
	 */
	EXTERN int k1b_tlb_write(
			vaddr_t vaddr,
			paddr_t paddr,
			unsigned shift,
			unsigned way,
			unsigned protection
	);

	/**
	 * @brief Invalidates a TLB entry.
	 *
	 * @param vaddr Target virtual address.
	 * @param shift Page shift.
	 * @param way   Target set-associative way.
	 */
	EXTERN int k1b_tlb_inval(vaddr_t vaddr, unsigned shift, unsigned way);

	/**
	 * @brief Lookups a TLB entry by virtual address.
	 *
	 * @param vaddr Target virtual address.
	 *
	 * @returns Upon successful completion, a pointer to the TLB entry
	 * that matches the virtual address @p vaddr is returned. If no
	 * entry that meets this criteria is found, @p NULL is returned.
	 */
	EXTERN const struct tlbe *k1b_tlb_lookup_vaddr(vaddr_t vaddr);

	/**
	 * @brief Lookups a TLB entry by physical address.
	 *
	 * @param paddr Target physical address.
	 *
	 * @returns Upon successful completion, a pointer to the TLB entry
	 * that matches the physical address @p paddr is returned. If no
	 * entry that meets this criteria is found, @p NULL is returned.
	 */
	EXTERN const struct tlbe *k1b_tlb_lookup_paddr(paddr_t paddr);

	/**
	 * @brief Flushes the TLB.
	 */
	EXTERN int k1b_tlb_flush(void);

	/**
	 * @brief Dumps a TLB entry.
	 *
	 * @param idx Index of target entry in the TLB.
	 */
	EXTERN void k1b_tlbe_dump(int idx);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE  K1B_CLUSTER_MEM_SIZE         /**< @see K1B_CLUSTER_MEM_SIZE          */
	#define KMEM_SIZE    K1B_CLUSTER_KMEM_SIZE        /**< @see K1B_CLUSTER_KMEM_SIZE         */
	#define UMEM_SIZE    K1B_CLUSTER_UMEM_SIZE        /**< @see K1B_CLUSTER_UMEM_SIZE         */
	#define KSTACK_SIZE  K1B_CLUSTER_KSTACK_SIZE      /**< @see K1B_CLUSTER_KSTACK_SIZE       */
	#define KPOOL_SIZE   K1B_CLUSTER_KPOOL_SIZE       /**< @see K1B_CLUSTER_KPOOL_SIZE        */
	#define KBASE_PHYS   K1B_CLUSTER_KERNEL_BASE_PHYS /**< @see K1B_CLUSTER_KERNEL_BASE_PHYS  */
	#define KPOOL_PHYS   K1B_CLUSTER_KPOOL_BASE_PHYS  /**< @see K1B_CLUSTER_KPOOL_BASE_PHYS   */
	#define UBASE_PHYS   K1B_CLUSTER_USER_BASE_PHYS   /**< @see K1B_CLUSTER_USER_BASE_PHYS    */
	#define USTACK_VIRT  K1B_CLUSTER_USTACK_BASE_VIRT /**< @see K1B_CLUSTER_USTACK_BASE_VIRT  */
	#define UBASE_VIRT   K1B_CLUSTER_USER_BASE_VIRT   /**< @see K1B_CLUSTER_USER_BASE_VIRT    */
	#define KBASE_VIRT   K1B_CLUSTER_KERNEL_BASE_VIRT /**< @see K1B_CLUSTER_KERNEL_BASE_VIRT  */
	#define KPOOL_VIRT   K1B_CLUSTER_KPOOL_BASE_VIRT  /**< @see K1B_CLUSTER_KPOOL_BASE_VIRT   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Provided Interface
	 */
	/**@{*/
	#define __tlb_lookup_vaddr_fn /**< tlbe_lookup_vaddr() */
	#define __tlb_lookup_paddr_fn /**< tlbe_lookup()       */
	#define __tlb_write_fn        /**< tlb_write()         */
	#define __tlb_inval_fn        /**< tlb_inval()         */
	#define __tlb_flush_fn        /**< tlb_flush()         */
	/**@}*/

	/**
	 * @see k1b_tlb_lookup_vaddr().
	 */
	static inline const struct tlbe *tlb_lookup_vaddr(int tlb_type, vaddr_t vaddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (NULL);

		return (k1b_tlb_lookup_vaddr(vaddr));
	}

	/**
	 * @see k1b_tlb_lookup_paddr().
	 */
	static inline const struct tlbe *tlb_lookup_paddr(int tlb_type, paddr_t paddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (NULL);

		return (k1b_tlb_lookup_paddr(paddr));
	}

	/**
	 * @see k1b_tlb_write()
	 */
	static inline int tlb_write(int tlb_type, vaddr_t vaddr, paddr_t paddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (-EINVAL);

		return (k1b_tlb_write(vaddr, paddr, 12, 0, K1B_TLBE_PROT_RW));
	}

	/**
	 * @see k1b_tlb_inval()
	 */
	static inline int tlb_inval(int tlb_type, vaddr_t vaddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (-EINVAL);

		return (k1b_tlb_inval(vaddr, 12, 0));
	}

	/**
	 * @see k1b_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (k1b_tlb_flush());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* CLUSTER_K1B_CLUSTER_MEMORY_H_ */
