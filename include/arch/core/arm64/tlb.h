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

#ifndef ARCH_CORE_ARM64_TLB_H_
#define ARCH_CORE_ARM64_TLB_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup arm64-core-tlb TLB
 * @ingroup arm64-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	#include <arch/core/arm64/types.h>
	#include <arch/core/arm64/mmu.h>
	#include <nanvix/const.h>


/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __tlb_flush_fn /**< tlb_flush() */
	#define __tlb_load_fn  /**< tlb_load()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief TLB entry.
	 */
	struct tlbe
	{
		unsigned valid		: 1; 	/**< Valid entry ?			*/
		unsigned vasign		: 1; 	/**< VA[48] sign bit		*/
		unsigned va			: 29;	/**< Virtual Adress			*/
		unsigned ns			: 1;	/**< Security state that the entry was fetched in */
		unsigned vmid		: 8;	/**< Virtual Machine Id		*/
		unsigned asid		: 16;	/**< Adress Space Id		*/
		unsigned size		: 3;	/**< Encoding for page size	*/
		unsigned ng			: 1;	/**< Global ?				*/
		unsigned ap			: 3;	/**< Acess Permissions 		*/
		unsigned hap		: 2;	/**< Hypervisor access permissions	*/
		unsigned nsd		: 1;	/**< Security state allocated to memory region */
		unsigned pa			: 28;	/**< Physical Adress		*/
		unsigned xs1nu		: 1;	/**< Non user executable permissions */
		unsigned 			: 1;	/**< Unused, Stage 2 exec persmissions */
		unsigned sh			: 8;	/**< Memory Type and shareability */
		unsigned 			: 4;	/**< Unused					*/
		unsigned s1size		: 3;	/**< Stage 1 size			*/
		unsigned 			: 3;	/**< Stage 2 level			*/
		unsigned			: 3;	/**< Parity, unused			*/
	} __attribute__((packed));


	/**
	 * @see arm64_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		dsb(ishst);						// ensure write has completed
		asm ("tlbi vmalle1" : : ); 		// invalidate all TLB entries
		dsb(ish); 						// ensure completion of TLB invalidation
		isb();							// synchronize context and ensure that no instructions are
		return 0;
	}

	/**
	 * @see arm64_tlb_load().
	 */
	static inline int tlb_load(paddr_t pgdir)
	{
		UNUSED(pgdir);
		return 0;
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_ARM64_TLB_H_ */
