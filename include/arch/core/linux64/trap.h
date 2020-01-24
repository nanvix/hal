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

#ifndef ARCH_CORE_LINUX64_TRAP_H_
#define ARCH_CORE_LINUX64_TRAP_H_

/**
 * @addtogroup linux64-core-trap Trap
 * @ingroup linux64-core
 *
 * @brief Traps
 */
/**@{*/

	#define __NEED_CORE_TYPES

	#include <arch/core/linux64/types.h>
	#include <nanvix/const.h>

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
	EXTERN linux64_word_t linux64_core_do_kcall(
		linux64_word_t arg0,
		linux64_word_t arg1,
		linux64_word_t arg2,
		linux64_word_t arg3,
		linux64_word_t arg4,
		linux64_word_t kcall_nr);

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param kcall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline linux64_word_t linux64_kcall0(linux64_word_t kcall_nr)
	{
		return (linux64_core_do_kcall(0, 0, 0, 0, 0, kcall_nr));
	}

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline linux64_word_t linux64_kcall1(
		linux64_word_t kcall_nr,
		linux64_word_t arg0)
	{
		return (linux64_core_do_kcall(arg0, 0, 0, 0, 0, kcall_nr));
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
	static inline linux64_word_t linux64_kcall2(
		linux64_word_t kcall_nr,
		linux64_word_t arg0,
		linux64_word_t arg1)
	{
		return (linux64_core_do_kcall(arg0, arg1, 0, 0, 0, kcall_nr));
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
	static inline linux64_word_t linux64_kcall3(
		linux64_word_t kcall_nr,
		linux64_word_t arg0,
		linux64_word_t arg1,
		linux64_word_t arg2)
	{
		return (linux64_core_do_kcall(arg0, arg1, arg2, 0, 0, kcall_nr));
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
	static inline linux64_word_t linux64_kcall4(
		linux64_word_t kcall_nr,
		linux64_word_t arg0,
		linux64_word_t arg1,
		linux64_word_t arg2,
		linux64_word_t arg3)
	{
		return (linux64_core_do_kcall(arg0, arg1, arg2, arg3, 0, kcall_nr));
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
	static inline linux64_word_t linux64_kcall5(
		linux64_word_t kcall_nr,
		linux64_word_t arg0,
		linux64_word_t arg1,
		linux64_word_t arg2,
		linux64_word_t arg3,
		linux64_word_t arg4)
	{
		return (linux64_core_do_kcall(arg0, arg1, arg2, arg3, arg4, kcall_nr));
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __kcall0_fn /**< linux64_kcall0() */
	#define __kcall1_fn /**< linux64_kcall1() */
	#define __kcall2_fn /**< linux64_kcall2() */
	#define __kcall3_fn /**< linux64_kcall3() */
	#define __kcall4_fn /**< linux64_kcall4() */
	#define __kcall5_fn /**< linux64_kcall5() */
	/**@}*/

	/**
	 * @see linux64_kcall_0()
	 */
	static inline word_t kcall0(word_t kcall_nr)
	{
		return (
			linux64_kcall0(kcall_nr)
		);
	}

	/**
	 * @see linux64_kcall_1()
	 */
	static inline word_t kcall1(
		word_t kcall_nr,
		word_t arg0)
	{
		return (
			linux64_kcall1(
				kcall_nr,
				arg0
			)
		);
	}

	/**
	 * @see linux64_kcall_2()
	 */
	static inline word_t kcall2(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1)
	{
		return (
			linux64_kcall2(
				kcall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see linux64_kcall_3()
	 */
	static inline word_t kcall3(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		return (
			linux64_kcall3(
				kcall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see linux64_kcall_4()
	 */
	static inline word_t kcall4(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		return (
			linux64_kcall4(
				kcall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see linux64_kcall_5()
	 */
	static inline word_t kcall5(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4)
	{
		return (
			linux64_kcall5(
				kcall_nr,
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			)
		);
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_TRAP_H_ */
