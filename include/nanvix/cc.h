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

#ifndef NANVIX_CC_H_
#define NANVIX_CC_H_

/**
 * @addtogroup kernel-constants-cc C Compiler Extensions
 * @ingroup kernel-consts
 */
/**@{*/

/*============================================================================*
 * Aliases                                                                    *
 *============================================================================*/

	/**
	 * @name Aliases for Standard C Extensions
	 */
	/**@{*/
	#define inline __inline__     /**< Inline Function */
	#define asm    __asm__        /**< Inline Assembly */
	#define volatile __volatile__ /**< Volatile Symbol */
	/**@}*/

/*============================================================================*
 * Function Attributes                                                        *
 *============================================================================*/

	/**
	 * @brief Casts a function with no return.
	 */
	#define NORETURN __attribute__((noreturn))

	/**
	 * @brief Places object in a binary section.
	 *
	 * @param x Target binary section.
	 */
	#define SECTION(x) __attribute__((section(x)))

	/**
	 * @brief Makes a symbol overwritable.
	 */
	#define OVERRIDE __attribute__((weak))

/*============================================================================*
 * Type Attributes                                                            *
 *============================================================================*/

	/**
	 * @brief Aligns an object at a boundary.
	 *
	 * @param x Boundary.
	 */
	#define ALIGN(x) __attribute__((aligned(x)))

	/**
	 * @brief Packs a structure
	 *
	 * @param x Boundary.
	 */
	#define PACK __attribute__((packed))

/*============================================================================*
 * Builtin Functions                                                          *
 *============================================================================*/

	/**
	 * @brief Makes code unreachable.
	 */
	#define UNREACHABLE() \
		{ while(TRUE) ; __builtin_unreachable(); }

	/**
	 * @brief Declares something to be unused.
	 *
	 * @param x Thing.
	 */
	#define UNUSED(x) ((void) (x))

	/**
	 * @brief Hints for branch condition.
	 *
	 * @param expr Target expression.
	 * @param val  Excepted logic value (one or zero).
	 */
	#define EXPECT(expr, val) __builtin_expect(expr, val)

	/**
	 * @brief No operation.
	 */
	#define noop() __asm__ __volatile__ ("":::"memory")

/**@}*/

#endif
