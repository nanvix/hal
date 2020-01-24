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

#ifndef ARCH_CORE_RV32GC_TRAP_H_
#define ARCH_CORE_RV32GC_TRAP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup rv32gc-core-trap Trap
 * @ingroup rv32gc-core
 *
 * @brief Traps
 */
/**@{*/

	#include <arch/core/rv32gc/types.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Issues a system call with no arguments.
	 *
	 * @param kcall_nr System call number.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall0(rv32gc_word_t kcall_nr);

	/**
	 * @brief Issues a system call with one argument.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0       Argument 0.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall1(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0);

	/**
	 * @brief Issues a system call with two arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall2(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1);

	/**
	 * @brief Issues a system call with three arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall3(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2);

	/**
	 * @brief Issues a system call with four arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 * @param arg3       Argument 3.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall4(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2,
		rv32gc_word_t arg3);

	/**
	 * @brief Issues a system call with five arguments.
	 *
	 * @param kcall_nr System call number.
	 * @param arg0       Argument 0.
	 * @param arg1       Argument 1.
	 * @param arg2       Argument 2.
	 * @param arg3       Argument 3.
	 * @param arg4       Argument 4.
	 *
	 * @returns The system call return value.
	 */
	EXTERN rv32gc_word_t rv32gc_kcall5(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2,
		rv32gc_word_t arg3,
		rv32gc_word_t arg4);

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __kcall0_fn /**< rv32gc_kcall0() */
	#define __kcall1_fn /**< rv32gc_kcall1() */
	#define __kcall2_fn /**< rv32gc_kcall2() */
	#define __kcall3_fn /**< rv32gc_kcall3() */
	#define __kcall4_fn /**< rv32gc_kcall4() */
	#define __kcall5_fn /**< rv32gc_kcall5() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see rv32gc_kcall0()
	 */
	static inline rv32gc_word_t kcall0(rv32gc_word_t kcall_nr)
	{
		return (
			rv32gc_kcall0(kcall_nr)
		);
	}

	/**
	 * @see rv32gc_kcall1()
	 */
	static inline rv32gc_word_t kcall1(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0)
	{
		return (
			rv32gc_kcall1(
				kcall_nr,
				arg0
			)
		);
	}

	/**
	 * @see rv32gc_kcall2()
	 */
	static inline rv32gc_word_t kcall2(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1)
	{
		return (
			rv32gc_kcall2(
				kcall_nr,
				arg0,
				arg1
			)
		);
	}

	/**
	 * @see rv32gc_kcall3()
	 */
	static inline rv32gc_word_t kcall3(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2)
	{
		return (
			rv32gc_kcall3(
				kcall_nr,
				arg0,
				arg1,
				arg2
			)
		);
	}

	/**
	 * @see rv32gc_kcall4()
	 */
	static inline rv32gc_word_t kcall4(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2,
		rv32gc_word_t arg3)
	{
		return (
			rv32gc_kcall4(
				kcall_nr,
				arg0,
				arg1,
				arg2,
				arg3
			)
		);
	}

	/**
	 * @see rv32gc_kcall5()
	 */
	static inline rv32gc_word_t kcall5(
		rv32gc_word_t kcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1,
		rv32gc_word_t arg2,
		rv32gc_word_t arg3,
		rv32gc_word_t arg4)
	{
		return (
			rv32gc_kcall5(
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

#endif /* ARCH_CORE_RV32GC_TRAP_H_ */
