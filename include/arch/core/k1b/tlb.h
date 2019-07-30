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

#ifndef ARCH_CORE_K1B_TLB_H_
#define ARCH_CORE_K1B_TLB_H_

/**
 * @addtogroup k1b-core-tlb TLB
 * @ingroup k1b-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	#include <arch/core/k1b/mmu.h>
	#include <arch/core/k1b/mOS.h>
	#include <nanvix/klib.h>
	#include <errno.h>

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define K1B_TLB_INSTRUCTION 0 /**< Instruction TLB */
	#define K1B_TLB_DATA        1 /**< Data TLB        */
	/**@}*/

	/**
	 * @brief Length of Locked TLB (number of entries).
	 */
	#ifdef __k1io__
		#define K1B_LTLB_LENGTH 16
	#else
		#define K1B_LTLB_LENGTH 8
	#endif

	/**
	 * @brief Length of Join TLB (number of entries).
	 */
	#define K1B_JTLB_LENGTH 128

	/**
	 * @brief Offset of JTLB in the TLB (number of entries).
	 */
	#define K1B_JTLB_OFFSET 0

	/**
	 * @brief Offset of LTLB in the TLB (number of entries).
	 */
	#define K1B_LTLB_OFFSET 256

	/**
	 * @brief Length of architectural TLB (number of entries).
	 */
	#define K1B_TLB_LENGTH (K1B_JTLB_LENGTH + K1B_LTLB_LENGTH)

	/**
	 * @brief TLB entry size (in bytes).
	 */
	#define K1B_TLBE_SIZE 8

	/**
	 * @name Data TLBE Cache Policies
	 */
	/**@{*/
	#define K1B_DTLBE_CACHE_POLICY_DEVICE    0 /**< Device Access */
	#define K1B_DTLBE_CACHE_POLICY_BYPASS    1 /**< Cache Bypass  */
	#define K1B_DTLBE_CACHE_POLICY_WRTHROUGH 2 /**< Write Through */
	/**@*/

	/**
	 * @name Instruction TLBE Cache Policies
	 */
	/**@{*/
	#define K1B_ITLBE_CACHE_POLICY_BYPASS 0 /**< Cache Bypass  */
	#define K1B_ITLBE_CACHE_POLICY_ENABLE 2 /**< Cache Enable  */
	/**@}*/

	/**
	 * @brief TLB Entry Protection Attributes
	 *
	 * @todo When running in bare-metal (without Hypervisor) we should
	 * revise this.
	 */
	/**@{*/
	#define K1B_TLBE_PROT_NONE  0 /**< None                    */
	#define K1B_TLBE_PROT_R     5 /**< Read-Only               */
	#define K1B_TLBE_PROT_RW    9 /**< Read and Write          */
	#define K1B_TLBE_PROT_RX   11 /**< Read and Execute        */
	#define K1B_TLBE_PROT_RWX  13 /**< Read, Write and Execute */
	/**@}*/

	/**
	 * @name TLB Entry Status
	 */
	/**@{*/
	#define K1B_TLBE_STATUS_INVALID   0 /**< Invalid         */
	#define K1B_TLBE_STATUS_PRESENT   1 /**< Present         */
	#define K1B_TLBE_STATUS_MODIFIED  2 /**< Modified        */
	#define K1B_TLBE_STATUS_AMODIFIED 3 /**< Atomic Modified */
	/**@}*/

	/**
	 * @brief Static initializer for a TLB entry.
	 *
	 */
	#define K1B_TLBE_INITIALIZER(pn, s, g, ans, fn, ae, pa, cp, es) \
	(                                  \
		(((es)  & 0x3ULL)     <<  0) | \
		(((cp)  & 0x3ULL)     <<  2) | \
		(((pa)  & 0xfULL)     <<  4) | \
		(((ae)  & 0xfULL)     <<  8) | \
		(((fn)  & 0xfffffULL) << 12) | \
		(((ans) & 0x1ffULL)   << 32) | \
		(((g)   & 0x1ULL)     << 42) | \
		(((s)   & 0x1ULL)     << 43) | \
		(((pn)  & 0xfffffULL) << 44)   \
	)

#ifndef _ASM_FILE_

	/**
	 * @cond k1b
	 */

		/**
		 * @brief TLB entry.
		 */
		struct tlbe
		{
			unsigned status       :  2; /**< Entry Status (ES)          */
			unsigned cache_policy :  2; /**< Cache Policy (CP)          */
			unsigned protection   :  4; /**< Protection Attributes (PA) */
			unsigned addr_ext     :  4; /**< Address  Extension (AE)    */
			unsigned frame        : 20; /**< Frame Number (FN)          */
			unsigned addrspace    :  9; /**< Address Space Number (ANS) */
			unsigned              :  1; /**< Reserved                   */
			unsigned global       :  1; /**< Global Page Indicator (G)  */
			unsigned size         :  1; /**< Page Size (S)              */
			unsigned page         : 20; /**< Page Number (PN)           */
		} PACK;

	/**@endcond*/

	/**
	 * @brief Gets the virtual address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The k1b_tlbe_vaddr() function gets the virtual address of a
	 * page encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The virtual address of a page enconded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline vaddr_t k1b_tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		unsigned pns = ((tlbe->page << 1) | tlbe->size);
		unsigned pn = pns >> (__builtin_k1_ctz(pns) + 1);
		return (pn << (__builtin_k1_ctz(pns) + 12));
	}

	/**
	 * @brief Gets the physical address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The k1b_tlbe_paddr() function gets the physical address of a
	 * page frame encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The physical address of a page encoded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline paddr_t k1b_tlbe_paddr_get(const struct tlbe *tlbe)
	{
		return (tlbe->frame << 12);
	}

	/**
	 * @brief Gets the tlbe entry index on TLB structure.
	 * 
	 * @param vaddr Target virtual address.
	 * @param shift Page shift.
	 * @param way   Target set-associative way.
	 *
	 * @return Index of target entry in the TLB.
	 */
	static inline unsigned k1b_tlbe_get_index(vaddr_t vaddr, unsigned shift, unsigned way)
	{
		return (2*((vaddr >> shift) & 0x3f) + way);
	}

	/**
	 * @brief Assesses if a TLB entry is valid
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The k1b_tlbe_is_valid() function assess if a TLB entry
	 * has the status bit valid.
	 *
	 * @return Non zero if is the TLB entry is valid, zero otherwise.
	 *
	 * @author João Vicente Souto
	 */
	static inline int k1b_tlbe_is_valid(const struct tlbe *tlbe)
	{
		return (tlbe->status != K1B_TLBE_STATUS_INVALID);
	}

	/**
	 * @brief Gets the size of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The k1b_tlbe_pgsize() function gets the size (in bytes) of a
	 * page enconded in the TLB entry pointed to by @p tlbe.
	 *
	 * @returns The size (in bytes) of the page encoded in the TLB
	 * entry pointed to by @p tlbe.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline size_t k1b_tlbe_pgsize_get(const struct tlbe *tlbe)
	{
		unsigned pns = ((tlbe->page << 1) | tlbe->size);
		return (1 << (__builtin_k1_ctz(pns) + 12));
	}

	/**
	 * @brief Reads a TLB entry.
	 *
	 * The k1b_tlbe_read() function reads the TLB entry @p idx and
	 * saves it in the location pointed to by @p tlbe.
	 *
	 * @param tlbe Target store location.
	 * @param idx  Index of target TLB entry.
	 *
	 * @returns Upon successful completion, non zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int k1b_tlbe_read(struct tlbe *tlbe, int idx)
	{
		__k1_tlb_entry_t _tlbe;
		_tlbe = mOS_readtlb(idx);
		kmemcpy(tlbe, &_tlbe, K1B_TLBE_SIZE);
		return (0);
	}

	/**
	 * @brief Writes a TLB entry.
	 *
	 * @param tlbe       The updated value of target TLB entry.
	 * @param vaddr      Target virtual address.
	 * @param paddr      Target physical address.
	 * @param shift      Page shift.
	 * @param way        Target set-associative way.
	 * @param protection Protection attributes.
	 * 
	 * @return Zero if successfully writes a TLB entry,
	 * non zero otherwise.
	 */
	EXTERN int k1b_tlbe_write(
		struct tlbe *tlbe,
		vaddr_t vaddr,
		paddr_t paddr,
		unsigned shift,
		unsigned way,
		unsigned protection
	);

	/**
	 * @brief Invalidates a TLB entry.
	 *
	 * @param tlbe  The updated value of target TLB entry.
	 * @param vaddr Target virtual address.
	 * @param shift Page shift.
	 * @param way   Target set-associative way.
	 * 
	 * @return Zero if successfully writes a TLB entry,
	 * non zero otherwise.
	 */
	EXTERN int k1b_tlbe_inval(
		struct tlbe *tlbe,
		vaddr_t vaddr,
		unsigned shift,
		unsigned way
	);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @brief Provided Interface
	 */
	/**@{*/
	#define __tlbe_st             /**< TLB Entry        */
	#define __tlbe_vaddr_get_fn   /**< tlbe_vaddr_get() */
	#define __tlbe_paddr_get_fn   /**< tlbe_paddr_get() */
	#define __tlbe_get_index_fn   /**< tlbe_get_index() */
	#define __tlbe_is_valid_fn    /**< tlbe_is_valid()  */
	#define __tlbe_write_fn       /**< tlbe_write()     */
	#define __tlbe_inval_fn       /**< tlbe_inval()     */
	/**@}*/

	/**
	 * @name Length of TLB (number of entries).
	 *
	 * Number of entries in the architectural TLB exposed by the
	 * hardware.
	 *
	 * @note The Hypervisor only exposes an interface for playing with
	 * the JTLB, therefore this should not be @p K1B_TLB_SIZE.
	 */
	#define TLB_LENGTH K1B_JTLB_LENGTH

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define TLB_INSTRUCTION K1B_TLB_INSTRUCTION /**< Instruction TLB */
	#define TLB_DATA        K1B_TLB_DATA        /**< Data TLB        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_tlbe_vaddr_get().
	 */
	static inline vaddr_t tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		return (k1b_tlbe_vaddr_get(tlbe));
	}

	/**
	 * @see k1b_tlbe_paddr_get().
	 */
	static inline paddr_t tlbe_paddr_get(const struct tlbe *tlbe)
	{
		return (k1b_tlbe_paddr_get(tlbe));
	}

	/**
	 * @see k1b_tlbe_get_index().
	 */
	static inline unsigned tlbe_get_index(vaddr_t vaddr)
	{
		return (k1b_tlbe_get_index(vaddr, 12, 0));
	}

	/**
	 * @see k1b_tlbe_is_valid().
	 */
	static inline int tlbe_is_valid(const struct tlbe *tlbe)
	{
		return (k1b_tlbe_is_valid(tlbe));
	}

	/**
	 * @see k1b_tlbe_inval().
	 */
	static inline int tlbe_write(
		struct tlbe *tlbe,
		int tlb_type,
		vaddr_t vaddr,
		paddr_t paddr,
		int vadd_info
	)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (-EINVAL);
		
		UNUSED(vadd_info);

		return (k1b_tlbe_write(tlbe, vaddr, paddr, 12, 0, K1B_TLBE_PROT_RW));
	}

	/**
	 * @see k1b_tlbe_inval().
	 */
	static inline int tlbe_inval(struct tlbe *tlbe, int tlb_type, vaddr_t vaddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != K1B_TLB_INSTRUCTION) && (tlb_type != K1B_TLB_DATA))
			return (-EINVAL);

		return (k1b_tlbe_inval(tlbe, vaddr, 12, 0));
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_TLB_H_ */
