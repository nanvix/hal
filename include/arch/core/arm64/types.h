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
 * @cond arm64
 */

#if (!defined(__NEED_CORE_TYPES))
	#error "do not include this file"
#endif

#ifndef _ASM_FILE_
	#include <posix/stdint.h>
#endif

/**
 * @addtogroup arm64-core
 */
/**@{*/

#ifdef __NEED_CORE_TYPES
#ifndef __ARM64_CORE_CONSTANTS
#define __ARM64_CORE_CONSTANTS

	/**
	 * @name Bit-Length of Core Types
	 */
	/**@{*/
	#define ARM64_BYTE_BIT    8 /**< Byte        */
	#define ARM64_HWORD_BIT  32 /**< Half Word   */
	#define ARM64_WORD_BIT   64 /**< Word        */
	#define ARM64_DWORD_BIT  64 /**< Double Word */
	/**@}*/

	/**
	 * @name Byte-Length of Core Types
	 */
	/**@{*/
	#define ARM64_BYTE_SIZE   1 /**< Byte        */
	#define ARM64_HWORD_SIZE  4 /**< Half Word   */
	#define ARM64_WORD_SIZE   8 /**< Word        */
	#define ARM64_DWORD_SIZE  8 /**< Double Word */
	/**@}*/

	/**
	 * @name Casters for Core Types
	 */
	/**@{*/
	#define ARM64_BYTE(x)       ((arm64_byte_t)(x))    /**< To Byte                */
	#define ARM64_HWORD(x)      ((arm64_hword_t)(x))   /**< To Half Word           */
	#define ARM64_WORD(x)       ((arm64_word_t)(x))    /**< To Word                */
	#define ARM64_DWORD(x)      ((arm64_dword_t)(x))   /**< To Double Word         */
	#define ARM64_BYTE_PTR(x)   ((arm64_byte_t *)(x))  /**< To Byte Pointer        */
	#define ARM64_HWORDP_PTR(x) ((arm64_hword_t *)(x)) /**< To Half Word Pointer   */
	#define ARM64_WORD_PTR(x)   ((arm64_word_t *)(x))  /**< To Word Pointer        */
	#define ARM64_DWORD_PTR(x)  ((arm64_dword_t *)(x)) /**< To Double Word Pointer */
	/**@}*/

#endif /* __ARM64_CORE_CONSTANTS */
#endif

/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @addtogroup arm64-core
	 */
	/**@{*/

	#ifdef __NEED_MEMORY_TYPES
	#ifndef __ARM64_MEMORY_TYPES
	#define __ARM64_MEMORY_TYPES

		/**
		 * @name Casters for Memory Types
		 */
		/**@{*/
		#define ARM64_PADDR(x) ((arm64_paddr_t)(x)) /**< To Physical Address */
		#define ARM64_VADDR(x) ((arm64_vaddr_t)(x)) /**< To Virtual Address  */
		#define ARM64_FRAME(x) ((arm64_frame_t)(x)) /**< To Frame Number     */
		/**@}*/

		/**
		 * @name Memory Types
		 */
		/**@{*/
		typedef uint64_t arm64_paddr_t; /**< Physical Address */
		typedef uint64_t arm64_frame_t; /**< Frame Number     */
		typedef uint64_t arm64_vaddr_t; /**< Virtual Address  */
		/**@}*/

	#endif /* __ARM64_MEMORY_TYPES */
	#endif

	#ifdef __NEED_CORE_TYPES
	#ifndef __ARM64_CORE_TYPES
	#define __ARM64_CORE_TYPES

		/**
		 * @name Core Types
		 */
		/**@{*/
		typedef uint8_t arm64_byte_t;   /**< Byte        */
		typedef uint32_t arm64_hword_t; /**< Half Word   */
		typedef uint64_t arm64_word_t;  /**< Word        */
		typedef uint64_t arm64_dword_t; /**< Double Word */
		/**@}*/

	#endif /* __ARM64_CORE_TYPES */
	#endif

	/**@}*/

#endif

/**@endcond*/
