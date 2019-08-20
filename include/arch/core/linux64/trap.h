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
	#include <unistd.h>
	#include <sys/syscall.h>

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param kcall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline linux64_dword_t linux64_kcall0(linux64_dword_t kcall_nr)
	{
		return (syscall(kcall_nr));
	}

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline linux64_dword_t linux64_kcall1(
		linux64_dword_t kcall_nr,
		linux64_dword_t arg0)
	{
		return (syscall(kcall_nr, arg0));
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
	static inline linux64_dword_t linux64_kcall2(
		linux64_dword_t kcall_nr,
		linux64_dword_t arg0,
		linux64_dword_t arg1)
	{
		return (syscall(kcall_nr, arg0, arg1));
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
	static inline linux64_dword_t linux64_kcall3(
		linux64_dword_t kcall_nr,
		linux64_dword_t arg0,
		linux64_dword_t arg1,
		linux64_dword_t arg2)
	{
		return (syscall(kcall_nr, arg0, arg1, arg2));
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
	static inline linux64_dword_t linux64_kcall4(
		linux64_dword_t kcall_nr,
		linux64_dword_t arg0,
		linux64_dword_t arg1,
		linux64_dword_t arg2,
		linux64_dword_t arg3)
	{
		return (syscall(kcall_nr, arg0, arg1, arg2, arg3));
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
	static inline linux64_dword_t linux64_kcall5(
		linux64_dword_t kcall_nr,
		linux64_dword_t arg0,
		linux64_dword_t arg1,
		linux64_dword_t arg2,
		linux64_dword_t arg3,
		linux64_dword_t arg4)
	{
		return (syscall(kcall_nr, arg0, arg1, arg2, arg3, arg4));
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
	static inline dword_t kcall0(dword_t kcall_nr)
	{
		return (
			linux64_kcall0(kcall_nr)
		);
	}

	/**
	 * @see linux64_kcall_1()
	 */
	static inline dword_t kcall1(
		dword_t kcall_nr,
		dword_t arg0)
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
	static inline dword_t kcall2(
		dword_t kcall_nr,
		dword_t arg0,
		dword_t arg1)
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
	static inline dword_t kcall3(
		dword_t kcall_nr,
		dword_t arg0,
		dword_t arg1,
		dword_t arg2)
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
	static inline dword_t kcall4(
		dword_t kcall_nr,
		dword_t arg0,
		dword_t arg1,
		dword_t arg2,
		dword_t arg3)		
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
	static inline dword_t kcall5(
		dword_t kcall_nr,
		dword_t arg0,
		dword_t arg1,
		dword_t arg2,
		dword_t arg3,
		dword_t arg4)
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
