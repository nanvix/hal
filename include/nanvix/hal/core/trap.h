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

#ifndef NANVIX_HAL_CORE_TRAP_H_
#define NANVIX_HAL_CORE_TRAP_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_TRAP)

	/* Functions. */
	#ifndef __kcall0_fn
	#error "kcall0() not defined?"
	#endif
	#ifndef __kcall1_fn
	#error "kcall1() not defined?"
	#endif
	#ifndef __kcall2_fn
	#error "kcall2() not defined?"
	#endif
	#ifndef __kcall3_fn
	#error "kcall3() not defined?"
	#endif
	#ifndef __kcall4_fn
	#error "kcall4() not defined?"
	#endif
	#ifndef __kcall5_fn
	#error "kcall5() not defined?"
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

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param kcall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN word_t kcall0(word_t kcall_nr);

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN word_t kcall1(
		word_t kcall_nr,
		word_t arg0);

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0 System call number.
	 * @param arg1 System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN word_t kcall2(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1);

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
	EXTERN word_t kcall3(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2);

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
	EXTERN word_t kcall4(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3);

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
	EXTERN word_t kcall5(
		word_t kcall_nr,
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4);

	/**
	 * @brief Handles a system call.
	 */
	EXTERN int do_kcall(
		word_t arg0,
		word_t arg1,
		word_t arg2,
		word_t arg3,
		word_t arg4,
		word_t kcall_nr);

/**@}*/

#endif /* NANVIX_HAL_CORE_TRAP_H_ */
