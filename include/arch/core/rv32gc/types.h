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

/**
 * @cond rv32gc
 */

#if (!defined(__NEED_CORE_TYPES) && !defined(__NEED_MEMORY_TYPES))
	#error "do not include this file"
#endif

#ifndef _ASM_FILE_
	#include <posix/stdint.h>
#endif

/**
 * @addtogroup rv32gc-core
 */
/**@{*/

#ifdef __NEED_CORE_TYPES
#ifndef __RV32GC_CORE_CONSTANTS
#define __RV32GC_CORE_CONSTANTS

	/**
	 * @name Bit-Length of Core Types
	 */
	/**@{*/
	#define RV32GC_BYTE_BIT    8 /**< Byte        */
	#define RV32GC_HWORD_BIT  16 /**< Half Word   */
	#define RV32GC_WORD_BIT   32 /**< Word        */
	#define RV32GC_DWORD_BIT  64 /**< Double Word */
	/**@}*/

	/**
	 * @name Byte-Length of Core Types
	 */
	/**@{*/
	#define RV32GC_BYTE_SIZE   1 /**< Byte        */
	#define RV32GC_HWORD_SIZE  2 /**< Half Word   */
	#define RV32GC_WORD_SIZE   4 /**< Word        */
	#define RV32GC_DWORD_SIZE  8 /**< Double Word */
	/**@}*/

	/**
	 * @name Casters for Core Types
	 */
	/**@{*/
	#define RV32GC_BYTE(x)       ((rv32gc_byte_t)(x))    /**< To Byte                */
	#define RV32GC_HWORD(x)      ((rv32gc_hword_t)(x))   /**< To Half Word           */
	#define RV32GC_WORD(x)       ((rv32gc_word_t)(x))    /**< To Word                */
	#define RV32GC_DWORD(x)      ((rv32gc_dword_t)(x))   /**< To Double Word         */
	#define RV32GC_BYTE_PTR(x)   ((rv32gc_byte_t *)(x))  /**< To Byte Pointer        */
	#define RV32GC_HWORDP_PTR(x) ((rv32gc_hword_t *)(x)) /**< To Half Word Pointer   */
	#define RV32GC_WORD_PTR(x)   ((rv32gc_word_t *)(x))  /**< To Word Pointer        */
	#define RV32GC_DWORD_PTR(x)  ((rv32gc_dword_t *)(x)) /**< To Double Word Pointer */
	/**@}*/

#endif /* __RV32GC_CORE_CONSTANTS */
#endif

/**@}*/

/**
 * @addtogroup rv32gc-core-mmu
 */
/**@{*/

#ifdef __NEED_MEMORY_TYPES
#ifndef __RV32GC_MEMORY_CONSTANTS
#define __RV32GC_MEMORY_CONSTANTS

	/**
	 * @name Bit-Length of Memory Types
	 */
	/**@{*/
	#define RV32GC_PADDR_BIT  32 /**< Physical Address */
	#define RV32GC_VADDR_BIT  32 /**< Virtual Address  */
	/**@}*/

	/**
	 * @name Byte-Length of Memory Types
	 */
	/**@{*/
	#define RV32GC_PADDR_BYTE 4 /**< Physical Address */
	#define RV32GC_VADDR_BYTE 4 /**< Virtual Address  */
	/**@}*/

#endif /* __RV32GC_MEMORY_CONSTANTS */
#endif

/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @addtogroup rv32gc-core-mmu
	 */
	/**@{*/

	#ifdef __NEED_MEMORY_TYPES
	#ifndef __RV32GC_MEMORY_TYPES
	#define __RV32GC_MEMORY_TYPES

		/**
		 * @name Casters for Memory Types
		 */
		/**@{*/
		#define RV32GC_PADDR(x) ((rv32gc_paddr_t)(x)) /**< To Physical Address */
		#define RV32GC_VADDR(x) ((rv32gc_vaddr_t)(x)) /**< To Virtual Address  */
		#define RV32GC_FRAME(x) ((rv32gc_frame_t)(x)) /**< To Frame Number     */
		/**@}*/

		/**
		 * @name Memory Types
		 */
		/**@{*/
		typedef uint32_t rv32gc_paddr_t; /**< Physical Address */
		typedef uint32_t rv32gc_frame_t; /**< Frame Number     */
		typedef uint32_t rv32gc_vaddr_t; /**< Virtual Address  */
		/**@}*/

	#endif /* __RV32GC_MEMORY_TYPES */
	#endif

	/**@}*/

	/**
	 * @addtogroup rv32gc-core
	 */
	/**@{*/

	#ifdef __NEED_CORE_TYPES
	#ifndef __RV32GC_CORE_TYPES
	#define __RV32GC_CORE_TYPES

		/**
		 * @brief Makes a half word.
		 *
		 * @param a High byte.
		 * @param b Low byte.
		 *
		 * @param A half word.
		 */
		#define RV32GC_MAKE_HWORD(a,b)                     \
		RV32GC_HWORD(                                      \
			((RV32GC_BYTE(a) & 0xff) << RV32GC_BYTE_BIT) | \
			((RV32GC_BYTE(b) & 0xff))                      \
		)

		/**
		 * @brief Makes a word.
		 *
		 * @param a High half word.
		 * @param b Low half word.
		 *
		 * @param A word.
		 */
		#define RV32GC_MAKE_WORD(a,b)                          \
		RV32GC_WORD(                                           \
			((RV32GC_HWORD(a) & 0xffff) << RV32GC_HWORD_BIT) | \
			((RV32GC_HWORD(b) & 0xffff))                       \
		)

		/**
		 * @brief Makes a double word.
		 *
		 * @param a High word.
		 * @param b Low word.
		 *
		 * @returns A dobule word.
		 */
		#define RV32GC_MAKE_DWORD(a,b)                              \
		RV32GC_DWORD(                                               \
			((RV32GC_WORD(a) & 0xffffffffULL) << RV32GC_WORD_BIT) | \
			((RV32GC_WORD(b) & 0xffffffffULL))                      \
		)

		/**
		 * @name Core Types
		 */
		/**@{*/
		typedef uint8_t rv32gc_byte_t;   /**< Byte        */
		typedef uint16_t rv32gc_hword_t; /**< Half Word   */
		typedef uint32_t rv32gc_word_t;  /**< Word        */
		typedef uint64_t rv32gc_dword_t; /**< Double Word */
		/**@}*/

	#endif /* __RV32GC_CORE_TYPES */
	#endif

	/**@}*/

#endif

/**@endcond*/
