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

#ifndef ARCH_CORE_ARM64_TRAP_H_
#define ARCH_CORE_ARM64_TRAP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup arm64-core-trap Trap
 * @ingroup arm64-core
 *
 * @brief Traps
 */
/**@{*/

	#include <arch/core/arm64/types.h>
	#include <nanvix/const.h>

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __kcall0_fn /**< arm64_kcall0() */
	#define __kcall1_fn /**< arm64_kcall1() */
	#define __kcall2_fn /**< arm64_kcall2() */
	#define __kcall3_fn /**< arm64_kcall3() */
	#define __kcall4_fn /**< arm64_kcall4() */
	#define __kcall5_fn /**< arm64_kcall5() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see arm64_kcall0()
	 */
	static inline arm64_word_t kcall0(arm64_word_t kcall_nr)
	{
		UNUSED(kcall_nr);
		return 0;
	}

	/**
	 * @see arm64_kcall1()
	 */
	static inline arm64_word_t kcall1(
		arm64_word_t kcall_nr,
		arm64_word_t arg0)
	{
		UNUSED(kcall_nr);
		UNUSED(arg0);
		return 0;
	}

	/**
	 * @see arm64_kcall2()
	 */
	static inline arm64_word_t kcall2(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1)
	{
		UNUSED(kcall_nr);
		UNUSED(arg0);
		UNUSED(arg1);
		return 0;
	}

	/**
	 * @see arm64_kcall3()
	 */
	static inline arm64_word_t kcall3(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2)
	{
		UNUSED(kcall_nr);
		UNUSED(arg0);
		UNUSED(arg1);
		UNUSED(arg2);
		return 0;
	}

	/**
	 * @see arm64_kcall4()
	 */
	static inline arm64_word_t kcall4(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2,
		arm64_word_t arg3)
	{
		UNUSED(kcall_nr);
		UNUSED(arg0);
		UNUSED(arg1);
		UNUSED(arg2);
		UNUSED(arg3);
		return 0;
	}

	/**
	 * @see arm64_kcall5()
	 */
	static inline arm64_word_t kcall5(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2,
		arm64_word_t arg3,
		arm64_word_t arg4)
	{
		UNUSED(kcall_nr);
		UNUSED(arg0);
		UNUSED(arg1);
		UNUSED(arg2);
		UNUSED(arg3);
		UNUSED(arg4);
		return 0;
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_ARM64_TRAP_H_ */
