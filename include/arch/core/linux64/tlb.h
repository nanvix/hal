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

#ifndef ARCH_CORE_LINUX64_TLB_H_
#define ARCH_CORE_LINUX64_TLB_H_

/**
 * @addtogroup linux64-core-tlb TLB
 * @ingroup linux64-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	#include <arch/core/linux64/mmu.h>
	#include <posix/errno.h>

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define LINUX64_TLB_INSTRUCTION 0 /**< Instruction TLB */
	#define LINUX64_TLB_DATA        1 /**< Data TLB        */
	/**@}*/

	#define LINUX64_TLB_SHIFT 8							/**< Shift of the TLB size */
	#define LINUX64_TLB_LENGTH (1 << LINUX64_TLB_SHIFT)	/**< Size of the TLB       */

	/**
	 * @brief TLB entry size (in bytes).
	 */
	#define LINUX64_TLBE_SIZE 16

	/**
	 * @name Data TLBE Cache Policies
	 */
	/**@{*/
	#define LINUX64_DTLBE_CACHE_POLICY_DEVICE    0 /**< Device Access */
	#define LINUX64_DTLBE_CACHE_POLICY_BYPASS    1 /**< Cache Bypass  */
	#define LINUX64_DTLBE_CACHE_POLICY_WRTHROUGH 2 /**< Write Through */
	/**@*/

	/**
	 * @name Instruction TLBE Cache Policies
	 */
	/**@{*/
	#define LINUX64_ITLBE_CACHE_POLICY_BYPASS 0 /**< Cache Bypass  */
	#define LINUX64_ITLBE_CACHE_POLICY_ENABLE 2 /**< Cache Enable  */
	/**@}*/

	/**
	 * @brief TLB Entry Protection Attributes
	 *
	 * @todo When running in bare-metal (without Hypervisor) we should
	 * revise this.
	 */
	/**@{*/
	#define LINUX64_TLBE_PROT_NONE  0 /**< None                    */
	#define LINUX64_TLBE_PROT_R     5 /**< Read-Only               */
	#define LINUX64_TLBE_PROT_RW    9 /**< Read and Write          */
	#define LINUX64_TLBE_PROT_RX   11 /**< Read and Execute        */
	#define LINUX64_TLBE_PROT_RWX  13 /**< Read, Write and Execute */
	/**@}*/

	/**
	 * @name TLB Entry Status
	 */
	/**@{*/
	#define LINUX64_TLBE_STATUS_INVALID   0 /**< Invalid         */
	#define LINUX64_TLBE_STATUS_PRESENT   1 /**< Present         */
	#define LINUX64_TLBE_STATUS_MODIFIED  2 /**< Modified        */
	#define LINUX64_TLBE_STATUS_AMODIFIED 3 /**< Atomic Modified */
	/**@}*/

	/**
	 * @cond linux64
	 */

		/**
		 * @brief TLB entry.
		 */
		struct tlbe
		{
			unsigned status       :  2; /**< Entry Status (ES)          */
			unsigned cache_policy : 22; /**< Cache Policy (CP)          */
			unsigned protection   :  4; /**< Protection Attributes (PA) */
			unsigned addr_ext     :  4; /**< Address Extension (AE)     */
			uint64_t frame        : 42; /**< Frame Number (FN)          */
			unsigned addrspace    :  9; /**< Address Space Number (ANS) */
			unsigned              :  1; /**< Reserved                   */
			unsigned global       :  1; /**< Global Page Indicator (G)  */
			unsigned size         :  1; /**< Page Size (S)              */
			uint64_t page         : 42; /**< Page Number (PN)           */
		} PACK;

	/**@endcond*/

	/**
	 * @brief Gets the virtual address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The linux64_tlbe_vaddr() function gets the virtual address of a
	 * page encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The virtual address of a page enconded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline vaddr_t linux64_tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		vaddr_t page = tlbe->page;
		return (page << LINUX64_PAGE_SHIFT);
	}

	/**
	 * @brief Gets the physical address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The linux64_tlbe_paddr() function gets the physical address of a
	 * page frame encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The physical address of a page encoded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline paddr_t linux64_tlbe_paddr_get(const struct tlbe *tlbe)
	{
		paddr_t frame = tlbe->frame;
		return (frame << LINUX64_PAGE_SHIFT);
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
	static inline unsigned linux64_tlbe_get_index(vaddr_t vaddr)
	{
		return (vaddr % LINUX64_TLB_LENGTH);
	}

	/**
	 * @brief Assesses if a TLB entry is valid
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The linux64_tlbe_is_valid() function assess if a TLB entry
	 * has the status bit valid.
	 *
	 * @return Non zero if is the TLB entry is valid, zero otherwise.
	 *
	 * @author João Vicente Souto
	 */
	static inline int linux64_tlbe_is_valid(const struct tlbe *tlbe)
	{
		return (tlbe->status != LINUX64_TLBE_STATUS_INVALID);
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
	EXTERN int linux64_tlbe_write(
		struct tlbe *tlbe,
		vaddr_t vaddr,
		paddr_t paddr,
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
	EXTERN int linux64_tlbe_inval(
		struct tlbe *tlbe,
		vaddr_t vaddr
	);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
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
	 */
	#define TLB_LENGTH LINUX64_TLB_LENGTH

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define TLB_INSTRUCTION LINUX64_TLB_INSTRUCTION /**< Instruction TLB */
	#define TLB_DATA        LINUX64_TLB_DATA        /**< Data TLB        */
	/**@}*/

	/**
	 * @see linux64_tlbe_vaddr_get().
	 */
	static inline vaddr_t tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		return (linux64_tlbe_vaddr_get(tlbe));
	}

	/**
	 * @see linux64_tlbe_paddr_get().
	 */
	static inline paddr_t tlbe_paddr_get(const struct tlbe *tlbe)
	{
		return (linux64_tlbe_paddr_get(tlbe));
	}

	/**
	 * @see linux64_tlbe_get_index().
	 */
	static inline unsigned tlbe_get_index(vaddr_t vaddr)
	{
		return (linux64_tlbe_get_index(vaddr));
	}

	/**
	 * @see linux64_tlbe_is_valid().
	 */
	static inline int tlbe_is_valid(const struct tlbe *tlbe)
	{
		return (linux64_tlbe_is_valid(tlbe));
	}

	/**
	 * @see linux64_tlbe_inval().
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
		if ((tlb_type != LINUX64_TLB_INSTRUCTION) && (tlb_type != LINUX64_TLB_DATA))
			return (-EINVAL);

		UNUSED(vadd_info);

		return (linux64_tlbe_write(tlbe, vaddr, paddr, LINUX64_TLBE_PROT_RW));
	}

	/**
	 * @see linux64_tlbe_inval().
	 */
	static inline int tlbe_inval(struct tlbe *tlbe, int tlb_type, vaddr_t vaddr)
	{
		/* Invalid TLB type. */
		if ((tlb_type != LINUX64_TLB_INSTRUCTION) && (tlb_type != LINUX64_TLB_DATA))
			return (-EINVAL);

		return (linux64_tlbe_inval(tlbe, vaddr));
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_TLB_H_ */
