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

#ifndef NANVIX_HAL_CLUSTER_MEMORY_H_
#define NANVIX_HAL_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_MEMORY)

	/* Constants */
	#ifndef UBASE_VIRT
	#error "UBASE_VIRT not defined"
	#endif
	#ifndef UEND_VIRT
	#error "UEND_VIRT not defined"
	#endif
	#ifndef USTACK_VIRT
	#error "USTACK_VIRT not defined"
	#endif
	#ifndef KBASE_VIRT
	#error "KBASE_VIRT not defined"
	#endif
	#ifndef KPOOL_VIRT
	#error "KPOOL_VIRT not defined"
	#endif
	#ifndef KBASE_PHYS
	#error "KBASE_PHYS not defined"
	#endif
	#ifndef KPOOL_PHYS
	#error "KPOOL_PHYS not defined"
	#endif
	#ifndef UBASE_PHYS
	#error "UBASE_PHYS not defined"
	#endif
	#ifndef MEMORY_SIZE
	#error "MEMORY_SIZE not defined"
	#endif
	#ifndef KMEM_SIZE
	#error "KMEM_SIZE not defined"
	#endif
	#ifndef KPOOL_SIZE
	#error "KPOOL_SIZE not defined"
	#endif
	#ifndef UMEM_SIZE
	#error "UMEM_SIZE not defined"
	#endif

	/* Memory regions constants. */
	#ifndef MEM_REGIONS
	#error "MEM_REGIONS not defined"
	#endif
	#ifndef MREGION_PG_ALIGN_START
	#error "MREGION_PG_ALIGN_START not defined"
	#endif
	#ifndef MREGION_PG_ALIGN_END
	#error "MREGION_PT_ALIGN_END not defined"
	#endif
	#ifndef MREGION_PT_ALIGN_START
	#error "MREGION_PT_ALIGN_START not defined"
	#endif
	#ifndef MREGION_PT_ALIGN_END
	#error "MREGION_PT_ALIGN_END not defined"
	#endif
	#ifndef ROOT_PGTAB_NUM
	#error "ROOT_PGTAB_NUM not defined"
	#endif

	/*
	 * Required interface for software-managed TLBs.
	 */
	#if (!CORE_HAS_TLB_HW)

		/* Constants. */
		#ifndef TLB_VADDR_MASK
			#error "TLB_VADDR_MASK not defined?"
		#endif
		#ifndef TLB_LENGTH
			#error "TLB_LENGTH not defined?"
		#endif

		/* Functions */
		#ifndef __tlb_get_utlb_fn
			#error "tlb_get_utlb() not defined?"
		#endif
		#ifndef __tlb_get_vaddr_info_fn
			#error "tlb_get_vaddr_info() not defined?"
		#endif

	#endif

	/*
	 * Required interface for software- and hardware-managed TLBs.
	 */
	#ifndef __tlb_flush_fn
		#error "tlb_flush() not defined?"
	#endif

#endif

/*============================================================================*
 * Memory Interface                                                           *
 *============================================================================*/

/**
 * @defgroup kernel-hal-cluster-mem Memory
 * @ingroup kernel-hal-cluster
 *
 * @brief Memory HAL Interface
 */
/**@{*/

	/**
	 * @brief Memory region.
	 */
	struct memory_region
	{
		paddr_t pbase;      /**< Base physical address.  */
		vaddr_t vbase;      /**< Base virtual address.   */
		paddr_t pend;       /**< End physical address.   */
		vaddr_t vend;       /**< End virtual address.    */
		size_t size;        /**< Size.                   */
		bool writable;      /**< Writable?               */
		bool executable;    /**< Executable?             */
		int root_pgtab_num; /**< Root page table number. */
		const char *desc;   /**< Description.            */
	};

	/**
	 * @brief Memory Layout
	 */
	EXTERN struct memory_region mem_layout[MEM_REGIONS];

#ifdef __NANVIX_HAL

	/**
	 * @brief Lookups a TLB entry by virtual address.
	 *
	 * @param tlb_type Target TLB (D-TLB or I-TLB).
	 * @param vaddr    Target virtual address.
	 *
	 * @returns Upon successful completion, a pointer to the TLB entry
	 * that matches the virtual address @p vaddr is returned. If no
	 * TLB entry matches @p vaddr, @p NULL is returned instead.
	 */
#if (!CORE_HAS_TLB_HW)
	EXTERN const struct tlbe *tlb_lookup_vaddr(int tlb_type, vaddr_t vaddr);
#else
	static const struct tlbe *tlb_lookup_vaddr(int tlb_type, vaddr_t vaddr)
	{
		UNUSED(tlb_type);
		UNUSED(vaddr);

		return (NULL);
	}
#endif

	/**
	 * @brief Lookups a TLB entry by physical address.
	 *
	 * @param tlb_type Target TLB (D-TLB or I-TLB).
	 * @param paddr    Target physical address.
	 *
	 * @returns Upon successful completion, a pointer to the TLB entry
	 * that matches the physical address @p paddr is returned. If no
	 * TLB entry matches @p paddr, @p NULL is returned instead.
	 */
#if (!CORE_HAS_TLB_HW)
	EXTERN const struct tlbe *tlb_lookup_paddr(int tlb_type, paddr_t paddr);
#else
	static const struct tlbe *tlb_lookup_paddr(int tlb_type, paddr_t paddr)
	{
		UNUSED(tlb_type);
		UNUSED(paddr);

		return (NULL);
	}
#endif

#endif /* __NANVIX_HAL */

	/**
	 * @brief Encodes a virtual address into the TLB.
	 *
	 * @param tlb_type Target TLB (D-TLB or I-TLB).
	 * @param vaddr    Target virtual address.
	 * @param paddr    Target physical address.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
#if (!CORE_HAS_TLB_HW)
	EXTERN int tlb_write(int tlb_type, vaddr_t vaddr, paddr_t paddr);
#else
	static inline int tlb_write(int tlb_type, vaddr_t vaddr, paddr_t paddr)
	{
		UNUSED(tlb_type);
		UNUSED(vaddr);
		UNUSED(paddr);

		return (0);
	}
#endif

	/**
	 * @brief Invalidates a virtual address in the TLB.
	 *
	 * @param tlb_type Target TLB (D-TLB or I-TLB).
	 * @param vaddr    Target virtual address.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
#if (!CORE_HAS_TLB_HW)
	EXTERN int tlb_inval(int tlb_type, vaddr_t vaddr);
#else
	static inline int tlb_inval(int tlb_type, vaddr_t vaddr)
	{
		UNUSED(tlb_type);
		UNUSED(vaddr);

		return (0);
	}
#endif

	/**
	 * @brief Initialize the architectural TLB of the underlying core.
	 */
#ifdef __tlb_init_fn
	EXTERN void tlb_init(void);
#else
	static inline void tlb_init(void)
	{
		noop();
	}
#endif

	/**
	 * @brief Loads the TLB
	 *
	 * The tlb_load() function loads the hardware TLB of the
	 * underlying core with the page directory pointed to by @p pgdir.
	 *
	 * @returns This function always returns zero.
	 */
#ifdef __tlb_load_fn
	EXTERN int tlb_load(paddr_t pgdir);
#else
	static inline int tlb_load(paddr_t pgdir)
	{
		UNUSED(pgdir);

		return (0);
	}
#endif

	/**
	 * @brief Flushes changes in the TLB.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int tlb_flush(void);

	/**
	 * @brief Does a memory warmup in the underlying cluster.
	 */
#ifdef __mem_warmup_fn
	EXTERN void mem_warmup(void);
#else
	static inline void mem_warmup(void)
	{
		noop();
	}
#endif

	/**
	 * @brief Prints information about memory layout.
	 */
	EXTERN void mem_info(void);

	/**
	 * @brief Asserts the memory alignment.
	 */
	EXTERN void mem_check_align(void);

	/**
	 * @brief Checks if the memory is as expected, i.e,
	 * if the virtual and physical memory are identity mapped.
	 */
	EXTERN void mem_check_layout(void);

	/**
	 * @brief Builds the memory map for the underlying
	 * architecture.
	 */
	EXTERN void mem_map(void);

	/**
	 * @brief Initializes the Memory Interface of the underlying
	 * cluster.
	 */
	EXTERN void mem_setup(void);

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_MEMORY_H_ */
