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

/**
 * @cond rv32i
 */

#if (!defined(__NEED_CORE_TYPES) && !defined(__NEED_MEMORY_TYPES))
	#error "do not include this file"
#endif

#ifndef _ASM_FILE_
	#include <stdint.h>
#endif

/**
 * @addtogroup rv32i-core
 */
/**@{*/

#ifdef __NEED_CORE_TYPES
#ifndef __CORE_CONSTANTS
#define __CORE_CONSTANTS

	/**
	 * @name Bit-Length of Core Types
	 */
	/**@{*/
	#define RV32I_BYTE_BIT    8 /**< Byte        */
	#define RV32I_HWORD_BIT  16 /**< Half Word   */
	#define RV32I_WORD_BIT   32 /**< Word        */
	#define RV32I_DWORD_BIT  64 /**< Double Word */
	/**@}*/

	/**
	 * @name Byte-Length of Core Types
	 */
	/**@{*/
	#define RV32I_BYTE_SIZE   1 /**< Byte        */
	#define RV32I_HWORD_SIZE  2 /**< Half Word   */
	#define RV32I_WORD_SIZE   4 /**< Word        */
	#define RV32I_DWORD_SIZE  8 /**< Double Word */
	/**@}*/

	/**
	 * @name Casters for Core Types
	 */
	/**@{*/
	#define RV32I_BYTE(x)       ((rv32i_byte_t)(x))    /**< To Byte                */
	#define RV32I_HWORD(x)      ((rv32i_hword_t)(x))   /**< To Half Word           */
	#define RV32I_WORD(x)       ((rv32i_word_t)(x))    /**< To Word                */
	#define RV32I_DWORD(x)      ((rv32i_dword_t)(x))   /**< To Double Word         */
	#define RV32I_BYTE_PTR(x)   ((rv32i_byte_t *)(x))  /**< To Byte Pointer        */
	#define RV32I_HWORDP_PTR(x) ((rv32i_hword_t *)(x)) /**< To Half Word Pointer   */
	#define RV32I_WORD_PTR(x)   ((rv32i_word_t *)(x))  /**< To Word Pointer        */
	#define RV32I_DWORD_PTR(x)  ((rv32i_dword_t *)(x)) /**< To Double Word Pointer */
	/**@}*/

#endif
#endif

/**@}*/

/**
 * @addtogroup rv32i-core-mmu
 */
/**@{*/

#ifdef __NEED_MEMORY_TYPES
#ifndef __MEMORY_CONSTANTS
#define __MEMORY_CONSTANTS

	/**
	 * @name Bit-Length of Memory Types
	 */
	/**@{*/
	#define RV32I_PADDR_BIT  32 /**< Physical Address */
	#define RV32I_VADDR_BIT  32 /**< Virtual Address  */
	/**@}*/

	/**
	 * @name Byte-Length of Memory Types
	 */
	/**@{*/
	#define RV32I_PADDR_BYTE 4 /**< Physical Address */
	#define RV32I_VADDR_BYTE 4 /**< Virtual Address  */
	/**@}*/

#endif
#endif

/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @addtogroup rv32i-core-mmu
	 */
	/**@{*/

	#ifdef __NEED_MEMORY_TYPES
	#ifndef __MEMORY_TYPES
	#define __MEMORY_TYPES

		/**
		 * @name Casters for Memory Types
		 */
		/**@{*/
		#define RV32I_PADDR(x) ((paddr_t)(x)) /**< To Physical Address */
		#define RV32I_VADDR(x) ((vaddr_t)(x)) /**< To Virtual Address  */
		#define RV32I_FRAME(x) ((frame_t)(x)) /**< To Frame Number     */
		/**@}*/

		/**
		 * @brief Virtual address.
		 */
		typedef uint32_t vaddr_t;

		/**
		 * @brief Physical address.
		 */
		typedef uint32_t paddr_t;

	#endif
	#endif

	/**@}*/

	/**
	 * @addtogroup rv32i-core
	 */
	/**@{*/

	#ifdef __NEED_CORE_TYPES
	#ifndef __CORE_TYPES
	#define __CORE_TYPES

		/**
		 * @brief Makes a double word.
		 *
		 * @param a High word.
		 * @param b Low word.
		 *
		 * @returns A dobule word.
		 */
		#define RV32I_MAKE_DWORD(a,b)                             \
		RV32I_DWORD(                                              \
			((RV32I_WORD(a) & 0xffffffffULL) << RV32I_WORD_BIT) | \
			((RV32I_WORD(b) & 0xffffffffULL))                     \
		)

		/**
		 * @brief Makes a word.
		 *
		 * @param a High half word.
		 * @param b Low half word.
		 *
		 * @param A word.
		 */
		#define RV32I_MAKE_WORD(a,b)                         \
		RV32I_WORD(                                          \
			((RV32I_HWORD(a) & 0xffff) << RV32I_HWORD_BIT) | \
			((RV32I_HWORD(b) & 0xffff))                      \
		)

		/**
		 * @brief Makes a half word.
		 *
		 * @param a High byte.
		 * @param b Low byte.
		 *
		 * @param A half word.
		 */
		#define RV32I_MAKE_HWORD(a,b)                    \
		RV32I_HWORD(                                     \
			((RV32I_BYTE(a) & 0xff) << RV32I_BYTE_BIT) | \
			((RV32I_BYTE(b) & 0xff))                     \
		)

		/**
		 * @name Core Types
		 */
		/**@{*/
		typedef uint8_t rv32i_byte_t;   /**< Byte        */
		typedef uint16_t rv32i_hword_t; /**< Half Word   */
		typedef uint32_t rv32i_word_t;  /**< Word        */
		typedef uint64_t rv32i_dword_t; /**< Double Word */
		/**@}*/

	#endif
	#endif

	/**@}*/

#endif

/**@endcond*/
