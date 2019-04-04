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

#ifndef ARCH_CORE_RV32I_TRAP_H_
#define ARCH_CORE_RV32I_TRAP_H_

/**
 * @addtogroup rv32i-core-trap Trap
 * @ingroup rv32i-core
 *
 * @brief Traps
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/rv32i/types.h>

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall0(rv32i_word_t syscall_nr)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	 * @param arg0       Argument 0.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall1(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t _arg0 asm("a1") = arg0;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall2(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t _arg0 asm("a1") = arg0;
		register word_t _arg1 asm("a2") = arg1;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall3(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t _arg0 asm("a1") = arg0;
		register word_t _arg1 asm("a2") = arg1;
		register word_t _arg2 asm("a3") = arg2;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 * @param arg3       Argument 3.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall4(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2,
		rv32i_word_t arg3)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t _arg0 asm("a1") = arg0;
		register word_t _arg1 asm("a2") = arg1;
		register word_t _arg2 asm("a3") = arg2;
		register word_t _arg3 asm("a4") = arg3;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 * @param arg3       Argument 3.
	 * @param arg4       Argument 4.
	 *
	 * @returns The system call return value.
	 */
	static inline rv32i_word_t rv32i_syscall5(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2,
		rv32i_word_t arg3,
		rv32i_word_t arg4)
	{
		register word_t _syscall_nr asm("a0") = syscall_nr;
		register word_t _arg0 asm("a1") = arg0;
		register word_t _arg1 asm("a2") = arg1;
		register word_t _arg2 asm("a3") = arg2;
		register word_t _arg3 asm("a4") = arg3;
		register word_t _arg4 asm("a5") = arg4;
		register word_t ret asm ("a0");

		asm volatile (
			"ecall"
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
	EXTERN void rv32i_syscall(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __syscall0_fn /**< rv32i_syscall0() */
	#define __syscall1_fn /**< rv32i_syscall1() */
	#define __syscall2_fn /**< rv32i_syscall2() */
	#define __syscall3_fn /**< rv32i_syscall3() */
	#define __syscall4_fn /**< rv32i_syscall4() */
	#define __syscall5_fn /**< rv32i_syscall5() */
	/**@}*/

	/**
	 * @see rv32i_syscall_0()
	 */
	static inline rv32i_word_t syscall0(rv32i_word_t syscall_nr)
	{
		return (
			rv32i_syscall0(syscall_nr)
		);
	}

	/**
	 * @see rv32i_syscall_1()
	 */
	static inline rv32i_word_t syscall1(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0)
	{
		return (
			rv32i_syscall1(
				syscall_nr,
				arg0
			)
		);
	}

	/**
	 * @see rv32i_syscall_2()
	 */
	static inline rv32i_word_t syscall2(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1)
	{
		return (
			rv32i_syscall2(
				syscall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see rv32i_syscall_3()
	 */
	static inline rv32i_word_t syscall3(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2)
	{
		return (
			rv32i_syscall3(
				syscall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see rv32i_syscall_4()
	 */
	static inline rv32i_word_t syscall4(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2,
		rv32i_word_t arg3)
	{
		return (
			rv32i_syscall4(
				syscall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see rv32i_syscall_5()
	 */
	static inline rv32i_word_t syscall5(
		rv32i_word_t syscall_nr,
		rv32i_word_t arg0,
		rv32i_word_t arg1,
		rv32i_word_t arg2,
		rv32i_word_t arg3,
		rv32i_word_t arg4)
	{
		return (
			rv32i_syscall5(
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

#endif /* ARCH_CORE_RV32I_TRAP_H_ */
