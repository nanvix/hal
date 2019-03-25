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

#ifndef ARCH_CORE_I486_TRAP_H_
#define ARCH_CORE_I486_TRAP_H_

/**
 * @addtogroup i486-core-trap Trap
 * @ingroup i486-core
 *
 * @brief Traps
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/i486/types.h>

	/**
	 * @brief Trap gate.
	 */
	#define I486_TRAP_GATE 0x80

#ifndef _ASM_FILE_

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t i486_syscall0(word_t syscall_nr)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE)
			: "memory", "cc"
		);

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
	static inline word_t i486_syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE),
			  "b"  (arg0)
			: "memory", "cc"
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
	static inline word_t i486_syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE),
			  "b"  (arg0),
			  "c"  (arg1)
			: "memory", "cc"
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
	static inline word_t i486_syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE),
			  "b"  (arg0),
			  "c"  (arg1),
			  "d"  (arg2)
			: "memory", "cc"
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
	static inline word_t i486_syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE),
			  "b"  (arg0),
			  "c"  (arg1),
			  "d"  (arg2),
			  "S"  (arg3)
			: "memory", "cc"
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
	static inline word_t i486_syscall5(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4)
	{
		word_t ret;

		__asm__ volatile (
			"int %2"
			: "=a" (ret)
			: "a"  (syscall_nr),
			  "N"  (I486_TRAP_GATE),
			  "b"  (arg0),
			  "c"  (arg1),
			  "d"  (arg2),
			  "S"  (arg3),
			  "D"  (arg4)
			: "memory", "cc"
		);

		return (ret);
	}

	/**
	 * @brief System Call Hook
	 */
	EXTERN void i486_syscall(void);

#endif /* _ASM_FILE */

/**@*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __syscall0_fn /**< i486_syscall0() */
	#define __syscall1_fn /**< i486_syscall1() */
	#define __syscall2_fn /**< i486_syscall2() */
	#define __syscall3_fn /**< i486_syscall3() */
	#define __syscall4_fn /**< i486_syscall4() */
	#define __syscall5_fn /**< i486_syscall5() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see i486_syscall_0()
	 */
	static inline word_t syscall0(word_t syscall_nr)
	{
		return (
			i486_syscall0(syscall_nr)
		);
	}

	/**
	 * @see i486_syscall_1()
	 */
	static inline word_t syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		return (
			i486_syscall1(
				syscall_nr,
				arg0
			)
		);
	}

	/**
	 * @see i486_syscall_2()
	 */
	static inline word_t syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		return (
			i486_syscall2(
				syscall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see i486_syscall_3()
	 */
	static inline word_t syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		return (
			i486_syscall3(
				syscall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see i486_syscall_4()
	 */
	static inline word_t syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		return (
			i486_syscall4(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see i486_syscall_5()
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
			i486_syscall5(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			)
		);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_TRAP_H_ */
