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

#ifndef ARCH_CORE_RV32GC_UPCALL_H_
#define ARCH_CORE_RV32GC_UPCALL_H_

/**
 * @addtogroup rv32gc-core-upcall Upcall
 * @ingroup rv32gc-core
 *
 * @brief Upcall Interface
 */
/**@{*/

	/**
	 * @brief System call number for upcall_ret().
	 */
	#define RV32GC_NR_upcall_ret 0

#ifndef _ASM_FILE_

	/**
	 * @brief Returns from an upcall.
	 */
	extern void rv32gc_upcall_ret(void);

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define NR_upcall_ret RV32GC_NR_upcall_ret /**< @see NR_upcall_ret */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __upcall_ret_fn /**< upcall_ret() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Alias for rv32gc_upcall_ret()
	 *
	 * @see rv32gc_upcall_ret().
	 *
	 * @note This should be an alias, otherwise the compiler may mess
	 * up with the stack that we have crafted carefully.
	 */
	#define upcall_ret rv32gc_upcall_ret

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_UPCALL_H_ */

