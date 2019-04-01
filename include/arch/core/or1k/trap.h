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

#ifndef ARCH_CORE_OR1K_TRAP_H_
#define ARCH_CORE_OR1K_TRAP_H_

/**
 * @addtogroup or1k-core-trap Trap
 * @ingroup or1k-core
 *
 * @brief Traps
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/or1k/types.h>

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline word_t or1k_syscall0(word_t syscall_nr)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr)
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
	static inline word_t or1k_syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t _arg0 __asm__("r3") = arg0;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr),
			  "r"  (_arg0)
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
	static inline word_t or1k_syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t _arg0 __asm__("r3") = arg0;
		register word_t _arg1 __asm__("r4") = arg1;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr),
			  "r"  (_arg0),
			  "r"  (_arg1)
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
	static inline word_t or1k_syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t _arg0 __asm__("r3") = arg0;
		register word_t _arg1 __asm__("r4") = arg1;
		register word_t _arg2 __asm__("r5") = arg2;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr),
			  "r"  (_arg0),
			  "r"  (_arg1),
			  "r"  (_arg2)
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
	static inline word_t or1k_syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t _arg0 __asm__("r3") = arg0;
		register word_t _arg1 __asm__("r4") = arg1;
		register word_t _arg2 __asm__("r5") = arg2;
		register word_t _arg3 __asm__("r6") = arg3;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr),
			  "r"  (_arg0),
			  "r"  (_arg1),
			  "r"  (_arg2),
			  "r"  (_arg3)
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
	static inline word_t or1k_syscall5(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4)
	{
		register word_t _syscall_nr __asm__("r11") = syscall_nr;
		register word_t _arg0 __asm__("r3") = arg0;
		register word_t _arg1 __asm__("r4") = arg1;
		register word_t _arg2 __asm__("r5") = arg2;
		register word_t _arg3 __asm__("r6") = arg3;
		register word_t _arg4 __asm__("r7") = arg4;
		register word_t ret __asm__ ("r11");

		__asm__ volatile (
			"l.sys 1"
			: "=r" (ret)
			: "r"  (_syscall_nr),
			  "r"  (_arg0),
			  "r"  (_arg1),
			  "r"  (_arg2),
			  "r"  (_arg3),
			  "r"  (_arg4)
			: "memory", "cc"
		);

		return (ret);
	}

	/**
	 * @brief System Call Hook
	 */
	EXTERN void or1k_syscall(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __syscall0_fn /**< or1k_syscall0() */
	#define __syscall1_fn /**< or1k_syscall1() */
	#define __syscall2_fn /**< or1k_syscall2() */
	#define __syscall3_fn /**< or1k_syscall3() */
	#define __syscall4_fn /**< or1k_syscall4() */
	#define __syscall5_fn /**< or1k_syscall5() */
	/**@}*/

	/**
	 * @see or1k_syscall_0()
	 */
	static inline word_t syscall0(word_t syscall_nr)
	{
		return (
			or1k_syscall0(syscall_nr)
		);
	}

	/**
	 * @see or1k_syscall_1()
	 */
	static inline word_t syscall1(
		word_t syscall_nr,
		word_t arg0)
	{
		return (
			or1k_syscall1(
				syscall_nr,
				arg0
			)
		);
	}

	/**
	 * @see or1k_syscall_2()
	 */
	static inline word_t syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1)
	{
		return (
			or1k_syscall2(
				syscall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see or1k_syscall_3()
	 */
	static inline word_t syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2)
	{
		return (
			or1k_syscall3(
				syscall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see or1k_syscall_4()
	 */
	static inline word_t syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3)
	{
		return (
			or1k_syscall4(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see or1k_syscall_5()
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
			or1k_syscall5(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3,
				arg4
			)
		);
	}

/**@endcond*/

#endif /* ARCH_CORE_OR1K_TRAP_H_ */
