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

#ifndef ARCH_CORE_OR1K_TLB_H_
#define ARCH_CORE_OR1K_TLB_H_

	/* Must come first. */
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup or1k-core-tlb TLB
 * @ingroup or1k-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	#include <arch/core/or1k/types.h>
	#include <arch/core/or1k/mmu.h>
	#include <errno.h>

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define OR1K_TLB_INSTRUCTION 0 /**< Instruction TLB */
	#define OR1K_TLB_DATA        1 /**< Data TLB        */
	/**@}*/

	/**
	 * @brief Length of architectural TLB (number of entries).
	 */
	#define OR1K_TLB_LENGTH 64

	/**
	 * @brief TLB entry size (in bytes).
	 */
	#define OR1K_TLBE_SIZE 8

	/**
	 * @brief Valid bit
	 */
	#define OR1K_TLBE_VALID 1

	/**
	 * @name Page level
	 */
	/**@{*/
	#define OR1K_TLBE_PL1 1 /**< Page level 1 */
	#define OR1K_TLBE_PL2 0 /**< Page level 2 */
	/**@}*/

	/**
	 * @brief Cache Coherency
	 */
	#define OR1K_TLBE_CACHE_COHERENCY 1

	/**
	 * @brief Cache Inhibit (Cache Enabled)
	 */
	#define OR1K_TLBE_CACHE_INHIBIT 1

	/**
	 * @name Data/Instruction TLBE Cache Policies
	 */
	/**@{*/
	#define OR1K_TLBE_CACHE_POLICY_WRTHROUGH 0 /**< Write Through */
	#define OR1K_TLBE_CACHE_POLICY_WRBACK    1 /**< Write Back    */
	/**@}*/

	/**
	 * @name Data/Instruction TLBE Memory Model
	 */
	/**@{*/
	#define OR1K_TLBE_MEMORY_MODEL_STRONG 0 /**< Strongly ordered */
	#define OR1K_TLBE_MEMORY_MODEL_WEAK   1 /**< Weakly ordered   */
	/**@}*/

	/**
	 * @brief Accessed bit
	 */
	#define OR1K_TLBE_ACCESSED 1

	/**
	 * @brief Dirty bit
	 */
	#define OR1K_TLBE_DIRTY 1

	/**
	 * Data TLBE Protection Attributes
	 */
	/**@{*/
	#define OR1K_DTLBE_URE 1 /**< User Read Enable        */
	#define OR1K_DTLBE_UWE 2 /**< User Write Enable       */
	#define OR1K_DTLBE_SRE 4 /**< Supervisor Read Enable  */
	#define OR1K_DTLBE_SwE 8 /**< Supervisor Write Enable */
	/**@}*/

	/**
	 * Instruction TLBE Protection Attributes
	 */
	/**@{*/
	#define OR1K_ITLBE_SXE 1 /**< Supervisor Execute Enable */
	#define OR1K_ITLBE_UXE 2 /**< User Execute Enable       */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief TLB entry.
	 */
	struct tlbe
	{
		/* xTLBMR. */
		unsigned vpn      : 19; /**< Virtual Page Number (VPN)               */
		unsigned          :  5; /**< Reserved                                */
		unsigned lru      :  2; /**< LRU Queue                               */
		unsigned cid      :  4; /**< Context ID (CID)                        */
		unsigned pl       :  1; /**< Page Level                              */
		unsigned valid    :  1; /**< Valid (V)                               */
		/* xTLBTR. */
		unsigned ppn      : 19; /**< Physical Page Number (PPN)              */
		unsigned          :  3; /**< Reserved                                */
		unsigned perms    :  4; /**< Protection bits                         */
		unsigned dirty    :  1; /**< Dirty (D)                               */
		unsigned accessed :  1; /**< Accessed (A)                            */
		unsigned wom      :  1; /**< Weakly-Ordered Memory (WOM)             */
		unsigned wbc      :  1; /**< Write-Back Cache (WBC)                  */
		unsigned ci       :  1; /**< Cache Inhibit (CI)                      */
		unsigned cc       :  1; /**< Cache Coherency (CC)                    */
	} __attribute__((packed));

	/**
	 * @brief Gets the virtual address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The or1k_tlbe_vaddr() function gets the virtual address of a
	 * page encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The virtual address of a page enconded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Davidson Francis
	 */
	static inline vaddr_t or1k_tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		return (tlbe->vpn << PAGE_SHIFT);
	}

	/**
	 * @brief Gets the physical address of a page.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The or1k_tlbe_paddr() function gets the physical address of a
	 * page frame encoded in the TLB entry pointed to by @p tlbe.
	 *
	 * @return The physical address of a page encoded in the TLB entry
	 * pointed to by @p tlbe.
	 *
	 * @author Davidson Francis
	 */
	static inline paddr_t or1k_tlbe_paddr_get(const struct tlbe *tlbe)
	{
		return (tlbe->ppn << PAGE_SHIFT);
	}

	/**
	 * @brief Updates TLB entry.
	 *
	 * @param tlb_type TLB type.
	 * @param idx      TLB Index.
	 * @param tlbe     Target TLB entry.
	 *
	 * The or1k_tlbe_update() function updates the HW TLB using the
	 * given parameters.
	 *
	 * @author João Vicente Souto
	 */
	EXTERN void or1k_tlbe_update(int tlb_type, int idx, const struct tlbe *tlbe);

	/**
	 * @brief Writes a TLB entry.
	 *
	 * @param tlb_type Target TLB.
	 * @param user     User adderss flag.
	 * @param inst     Instruction flag.
	 * @param vaddr    Target virtual address.
	 * @param paddr    Target physical address.
	 * @param tlbe  The updated value of target TLB entry.
	 */
	EXTERN int or1k_tlbe_write(
		int tlb_type,
		int user,
		int inst,
		vaddr_t vaddr,
		paddr_t paddr,
		struct tlbe * tlbe
	);

	/**
	 * @brief Invalidates a TLB entry.
	 *
	 * @param tlb_type Target TLB.
	 * @param idx      Target TLB idx.
	 */
	static inline int or1k_tlbe_inval(int tlb_type, int idx)
	{
		/*
		* Invalidates the entry accordingly if
		* instruction or data.
		*/
		if (tlb_type == OR1K_TLB_INSTRUCTION)
			or1k_mtspr(OR1K_SPR_ITLBMR_BASE(0) | idx, 0);

		/* Data. */
		else
			or1k_mtspr(OR1K_SPR_DTLBMR_BASE(0) | idx, 0);

		return (0);
	}

	/**
	 * @brief Initializes the TLB.
	 */
	EXTERN void or1k_tlbe_init(
		int idx,
		unsigned dtlbtr,
		unsigned itlbtr,
		unsigned xtlbmr,
		struct tlbe *dtlb,
		struct tlbe *itlb
	);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __tlbe_st             /**< TLB Entry           */
	#define __tlbe_vaddr_get_fn   /**< tlbe_vaddr_get()    */
	#define __tlbe_paddr_get_fn   /**< tlbe_paddr_get()    */
	/**@}*/

	/**
	 * @brief Length of TLB (number of entries).
	 *
	 * Number of entries in the architectural TLB exposed by the
	 * hardware.
	 */
	#define TLB_LENGTH OR1K_TLB_LENGTH

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define TLB_INSTRUCTION OR1K_TLB_INSTRUCTION /**< Instruction TLB */
	#define TLB_DATA        OR1K_TLB_DATA        /**< Data TLB        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see or1k_tlbe_vaddr_get().
	 */
	static inline vaddr_t tlbe_vaddr_get(const struct tlbe *tlbe)
	{
		return (or1k_tlbe_vaddr_get(tlbe));
	}

	/**
	 * @see or1k_tlbe_paddr_get().
	 */
	static inline paddr_t tlbe_paddr_get(const struct tlbe *tlbe)
	{
		return (or1k_tlbe_paddr_get(tlbe));
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_TLB_H_ */
