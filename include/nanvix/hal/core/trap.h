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

#ifndef NANVIX_HAL_CORE_TRAP_H_
#define NANVIX_HAL_CORE_TRAP_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_TRAP)

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

#ifdef __unix64__
	#define TYPE dword_t
#else
	#define TYPE word_t
#endif

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param syscall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN TYPE syscall0(TYPE syscall_nr);

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN TYPE syscall1(
		TYPE syscall_nr,
		TYPE arg0);

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param syscall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN TYPE syscall2(
		TYPE syscall_nr,
		TYPE arg0,
		TYPE arg1);

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
	EXTERN TYPE syscall3(
		TYPE syscall_nr,
		TYPE arg0,
		TYPE arg1,
		TYPE arg2);

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
	EXTERN TYPE syscall4(
		TYPE syscall_nr,
		TYPE arg0,
		TYPE arg1,
		TYPE arg2,
		TYPE arg3);

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
	EXTERN TYPE syscall5(
		TYPE syscall_nr,
		TYPE arg0,
		TYPE arg1,
		TYPE arg2,
		TYPE arg3,
		TYPE arg4);

	/**
	 * @brief Handles a system call.
	 */
	EXTERN int do_syscall(
		unsigned arg0,
		unsigned arg1,
		unsigned arg2,
		unsigned arg3,
		unsigned arg4,
		unsigned syscall_nr);

/**@}*/

#endif /* NANVIX_HAL_CORE_TRAP_H_ */
