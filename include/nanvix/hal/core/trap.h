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

#ifndef HAL_CORE_TRAP_H_
#define HAL_CORE_TRAP_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

	/* Functions. */
	#ifndef __syscall0_fn
	#error "syscall0() not defined?"
	#endif
	#ifndef __syscall1_fn
	#error "syscall1() not defined?"
	#endif
	#ifndef __syscall2_fn
	#error "syscall2() not defined?"
	#endif
	#ifndef __syscall3_fn
	#error "syscall3() not defined?"
	#endif
	#ifndef __syscall4_fn
	#error "syscall4() not defined?"
	#endif
	#ifndef __syscall5_fn
	#error "syscall5() not defined?"
	#endif

/*============================================================================*
 * Trap Interface                                                             *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-trap Trap
 * @ingroup kernel-hal-core
 *
 * @brief Traps
 */
/**@{*/

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN inline word_t syscall0(word_t syscall_nr);

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN inline word_t syscall1(
		word_t syscall_nr,
		word_t arg0);

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN inline word_t syscall2(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1);

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
	EXTERN inline word_t syscall3(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2);

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
	EXTERN inline word_t syscall4(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3);

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
	EXTERN inline word_t syscall5(
		word_t syscall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4);

	/**
	 * @brief Handles a system call.
	 */
	EXTERN int do_syscall(
		int arg0,
		int arg1,
		int arg2,
		int arg3,
		int arg4,
		int arg5,
		int syscall_nr);

/**@*/

#endif /* HAL_CORE_TRAP_H_ */
