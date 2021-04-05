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

#ifndef _ASM_FILE_
/**
	 * @brief Low-level kernel call dispatcher.
	 *
	 * @param arg0     First kernel call argument.
	 * @param arg1     Second kernel call argument.
	 * @param arg2     Third kernel call argument.
	 * @param arg3     Fourth kernel call argument.
	 * @param arg4     Fifth kernel call argument.
	 * @param kcall_nr Kernel call number.
	 */
	EXTERN arm64_word_t arm64_core_do_kcall(
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2,
		arm64_word_t arg3,
		arm64_word_t arg4,
		arm64_word_t kcall_nr);

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param kcall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall0(arm64_word_t kcall_nr)
	{
		return (arm64_core_do_kcall(0, 0, 0, 0, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall1(
		arm64_word_t kcall_nr,
		arm64_word_t arg0)
	{
		return (arm64_core_do_kcall(arg0, 0, 0, 0, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall2(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1)
	{
		return (arm64_core_do_kcall(arg0, arg1, 0, 0, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with three arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall3(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2)
	{
		return (arm64_core_do_kcall(arg0, arg1, arg2, 0, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with four arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 * @param arg3 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall4(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2,
		arm64_word_t arg3)
	{
		return (arm64_core_do_kcall(arg0, arg1, arg2, arg3, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with five arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 * @param arg3 System call number.
	 * @param arg4 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline arm64_word_t arm64_kcall5(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1,
		arm64_word_t arg2,
		arm64_word_t arg3,
		arm64_word_t arg4)
	{
		return (arm64_core_do_kcall(arg0, arg1, arg2, arg3, arg4, kcall_nr));
	}

#endif

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
		return (
			arm64_kcall0(kcall_nr)
		);
	}

	/**
	 * @see arm64_kcall1()
	 */
	static inline arm64_word_t kcall1(
		arm64_word_t kcall_nr,
		arm64_word_t arg0)
	{
		return (
			arm64_kcall1(
				kcall_nr,
				arg0
			)
		);
	}

	/**
	 * @see arm64_kcall2()
	 */
	static inline arm64_word_t kcall2(
		arm64_word_t kcall_nr,
		arm64_word_t arg0,
		arm64_word_t arg1)
	{
		return (
			arm64_kcall2(
				kcall_nr,
				arg0,
				arg1
			)
		);
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
		return (
			arm64_kcall3(
				kcall_nr,
				arg0,
				arg1,
				arg2
			)
		);
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
		return (
			arm64_kcall4(
				kcall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
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
		return (
			arm64_kcall5(
				kcall_nr,
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			)
		);
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_ARM64_TRAP_H_ */
