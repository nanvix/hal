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

#ifndef ARCH_CORE_K1B_TRAP_H_
#define ARCH_CORE_K1B_TRAP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup k1b-core-trap Trap
 * @ingroup k1b-core
 *
 * @brief Traps
 */
/**@{*/

	#include <arch/core/k1b/types.h>
	#include <arch/core/k1b/mOS.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Low-level trap dispatcher.
	 */
	EXTERN void _k1b_do_syscall(void);

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall0(word_t syscall_nr)
	{
		word_t ret;

		ret = __k1_club_syscall0(syscall_nr);

		return (ret);
	}

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		word_t ret;

		ret = __k1_club_syscall1(
			syscall_nr,
			arg0
		);

		return (ret);
	}

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		word_t ret;

		ret = __k1_club_syscall2(
			syscall_nr,
			arg0,
			arg1
		);

		return (ret);
	}

	/**
	 * @brief Issues a system call with three arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		word_t ret;

		ret = __k1_club_syscall3(
			syscall_nr,
			arg0,
			arg1,
			arg2
		);

		return (ret);
	}

	/**
	 * @brief Issues a system call with four arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 * @param arg3 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		word_t ret;

		ret = __k1_club_syscall4(
			syscall_nr,
			arg0,
			arg1,
			arg2,
			arg3
		);

		return (ret);
	}

	/**
	 * @brief Issues a system call with five arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 * @param arg2 System call number.
	 * @param arg3 System call number.
	 * @param arg4 System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t k1b_syscall5(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4)
	{
		word_t ret;

		ret = __k1_club_syscall5(
			syscall_nr,
			arg0,
			arg1,
			arg2,
			arg3,
			arg4
		);

		return (ret);
	}

	/**
	 * @brief System Call Hook
	 */
	EXTERN void k1b_syscall(void);

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __syscall0_fn /**< k1b_syscall0() */
	#define __syscall1_fn /**< k1b_syscall1() */
	#define __syscall2_fn /**< k1b_syscall2() */
	#define __syscall3_fn /**< k1b_syscall3() */
	#define __syscall4_fn /**< k1b_syscall4() */
	#define __syscall5_fn /**< k1b_syscall5() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_syscall_0()
	 */
	static inline word_t syscall0(word_t syscall_nr)
	{
		return (
			k1b_syscall0(syscall_nr)
		);
	}

	/**
	 * @see k1b_syscall_1()
	 */
	static inline word_t syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		return (
			k1b_syscall1(
				syscall_nr,
				arg0
			)
		);
	}

	/**
	 * @see k1b_syscall_2()
	 */
	static inline word_t syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		return (
			k1b_syscall2(
				syscall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see k1b_syscall_3()
	 */
	static inline word_t syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		return (
			k1b_syscall3(
				syscall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see k1b_syscall_4()
	 */
	static inline word_t syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		return (
			k1b_syscall4(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see k1b_syscall_5()
	 */
	static inline word_t syscall5(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4)
	{
		return (
			k1b_syscall5(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			)
		);
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_TRAP_H_ */
