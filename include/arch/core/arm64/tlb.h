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
	/**@}*/

#ifndef _ASM_FILE_


	/**
	 * @see arm64_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		asm volatile(
			"tlbi alle1"
			:
			:
		);
		return arm64_invalidate_tlb();
	}



#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_ARM64_TLB_H_ */
