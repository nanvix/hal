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

#ifndef ARCH_CORE_ARM64_MCALL_H_
#define ARCH_CORE_ARM64_MCALL_H_

/**
 * @addtogroup arm64-core-mcall Machine Call
 * @ingroup arm64-core
 *
 * @brief Machine Call Interface
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CORE_TYPES

	#include <arch/core/arm64/types.h>

	/**
	 * @name Machine Calls
	 */
	/**@{*/
	#define ARM64_MCALL_CSR_READ       1 /**< arm64_mcall_csr_read()    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Issues a machine call with one argument.
	 *
	 * @param arg0     Argument 0.
	 *
	 * @returns The machine call return value.
	 */
	static inline arm64_word_t arm64_mcall_cpu_id(void)
	{
		
		register arm64_word_t ret __asm__ ("x0");

		__asm__ __volatile__ (
			"mrs x0, mpidr_el1\n;"
			"and x0, x0, #3\n;"
		);
		return (ret);
	}

	/**
	 * @brief Issues a machine call with one argument.
	 *
	 * @param arg0     Argument 0.
	 *
	 * @returns The machine call return value.
	 */
	static inline arm64_word_t arm64_mcall_exception_level(void)
	{
		register arm64_word_t ret __asm__ ("x0");

		__asm__ __volatile__ (
			"mrs x0, CurrentEL\n;"
			"and x0, x0, #12\n;"
		);
		return (ret);
	}


#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_ARM64_MCALL_H_  */
