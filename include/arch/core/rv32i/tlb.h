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

#ifndef ARCH_CORE_RV32I_TLB_H_
#define ARCH_CORE_RV32I_TLB_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup rv32i-core-tlb TLB
 * @ingroup rv32i-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	#include <arch/core/rv32i/types.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Flushes changes in the TLB.
	 *
	 * The rv32i_tlb_flush() function flushes the changes made to the
	 * TLB of the underlying rv32i core.
	 *
	 * @returns This function always returns zero.
	 *
	 * @todo XXX We can supply address ranges to a higher performance.
	 */
	static inline int rv32i_tlb_flush(void)
	{
		asm volatile ("sfence.vma");

		return (0);
	}

	/**
	 * @brief Loads the TLB
	 *
	 * The rv32i_tlb_flush() function loads the hardware TLB of the
	 * underlying core with the page directory pointed to by @p pgdir.
	 *
	 * @returns This function always returns zero.
	 */
	static inline int rv32i_tlb_load(paddr_t pgdir)
	{
		rv32i_word_t satp;

		satp = RV32_SATP_MODE_SV32 | (pgdir >> RV32I_PAGE_SHIFT);

		rv32i_satp_write(satp);

		return (0);
	}

#endif /* _ASM_FILE_ */

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __tlb_flush_fn /**< tlb_flush() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see rv32i_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (rv32i_tlb_flush());
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32I_TLB_H_ */
