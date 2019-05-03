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

	/*
	 * Required interface for software-managed TLBs.
	 */
	#if (!CORE_HAS_TLB_HW)

		/* Constants. */
		#ifndef TLB_VADDR_MASK
			#error "TLB_VADDR_MASK not defined?"
		#endif
		#ifndef LOOKUP_TLB_LENGTH
			#error "LOOKUP_TLB_LENGTH not defined?"
		#endif

		/*
		* Required interface for software- and hardware-managed TLBs.
		*/
		#ifndef __tlb_flush_fn
			#error "tlb_flush() not defined?"
		#endif

		/* Functions */
		#ifndef __tlb_get_utlb_fn
			#error "tlb_get_utlb() not defined?"
		#endif
		#ifndef __tlb_get_vaddr_info_fn
			#error "tlb_get_vaddr_info() not defined?"
		#endif

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

#ifndef __NANVIX_HAL

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
	 * @brief Flushes changes in the TLB.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int tlb_flush(void);

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_MEMORY_H_ */
