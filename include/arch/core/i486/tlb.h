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

#ifndef ARCH_I486_TLB_H_
#define ARCH_I486_TLB_H_

/**
 * @addtogroup i486-core-tlb TLB
 * @ingroup i486-core
 *
 * @brief Translation Lookaside Buffer
 */
/**@{*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __tlb_flush_fn /**< tlb_flush() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Flushes changes in the TLB.
	 *
	 * The i486_tlb_flush() function flushes the changes made to the
	 * TLB of the underlying i486 core.
	 *
	 * @returns This function always returns zero.
	 *
	 * @todo We can improve this by using the invlpg instruction.
	 */
	static inline int i486_tlb_flush(void)
	{
		asm volatile (
			"movl %%cr3, %%eax;\
			movl %%eax, %%cr3;"
			:
			:
			:
		);

		return (0);
	}

	/**
	 * @see i486_tlb_flush().
	 */
	static inline int tlb_flush(void)
	{
		return (i486_tlb_flush());
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_I486_TLB_H_ */
