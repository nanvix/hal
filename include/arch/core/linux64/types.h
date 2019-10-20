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
 * @cond linux64
 */

#if (!defined(__NEED_CORE_TYPES) && !defined(__NEED_MEMORY_TYPES))
	#error "do not include this file"
#endif

	#include <posix/stdint.h>

/**
 * @addtogroup linux64-core
 */
/**@{*/

#ifdef __NEED_CORE_TYPES
#ifndef __CORE_CONSTANTS
#define __CORE_CONSTANTS

	/**
	 * @name Bit-Length of Core Types
	 */
	/**@{*/
	#define LINUX64_BYTE_BIT    8 /**< Byte        */
	#define LINUX64_HWORD_BIT  16 /**< Half Word   */
	#define LINUX64_WORD_BIT   32 /**< Word        */
	#define LINUX64_DWORD_BIT  64 /**< Double Word */
	/**@}*/

	/**
	 * @name Byte-Length of Core Types
	 */
	/**@{*/
	#define LINUX64_BYTE_SIZE   1 /**< Byte        */
	#define LINUX64_HWORD_SIZE  2 /**< Half Word   */
	#define LINUX64_WORD_SIZE   4 /**< Word        */
	#define LINUX64_DWORD_SIZE  8 /**< Double Word */
	/**@}*/

#endif
#endif

/**@}*/

/**
 * @addtogroup linux64-core-mmu
 */
/**@{*/

#ifdef __NEED_MEMORY_TYPES
#ifndef __MEMORY_CONSTANTS
#define __MEMORY_CONSTANTS

	/**
	 * @name Bit-Length of Memory Types
	 */
	/**@{*/
	#define LINUX64_PADDR_BIT  48 /**< Physical Address */
	#define LINUX64_VADDR_BIT  48 /**< Virtual Address  */
	/**@}*/

	/**
	 * @name Byte-Length of Memory Types
	 */
	/**@{*/
	#define LINUX64_PADDR_BYTE 6 /**< Physical Address */
	#define LINUX64_VADDR_BYTE 6 /**< Virtual Address  */
	/**@}*/

#endif
#endif

/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @addtogroup linux64-core-mmu
	 */
	/**@{*/

	#ifdef __NEED_MEMORY_TYPES
	#ifndef __MEMORY_TYPES
	#define __MEMORY_TYPES

		/**
		 * @name Casters for Memory Types
		 */
		/**@{*/
		#define LINUX64_PADDR(x) ((paddr_t)(x)) /**< To Physical Address */
		#define LINUX64_VADDR(x) ((vaddr_t)(x)) /**< To Virtual Address  */
		#define LINUX64_FRAME(x) ((frame_t)(x)) /**< To Frame Number     */
		/**@}*/

		/**
		 * @brief Virtual address.
		 */
		typedef uint64_t vaddr_t;

		/**
		 * @brief Physical address.
		 */
		typedef uint64_t paddr_t;

		/**
		 * @brief Page Frame number.
		 */
		typedef uint64_t frame_t;

	#endif
	#endif

	/**@}*/

	/**
	 * @addtogroup linux64-core
	 */
	/**@{*/

	#ifdef __NEED_CORE_TYPES
	#ifndef __CORE_TYPES
	#define __CORE_TYPES

		/**
		 * @name Core Types
		 */
		/**@{*/
		typedef uint8_t linux64_byte_t;   /**< Byte        */
		typedef uint16_t linux64_hword_t; /**< Half Word   */
		typedef uint64_t linux64_word_t;  /**< Word        */
		typedef uint64_t linux64_dword_t; /**< Double Word */
		/**@}*/

	#endif
	#endif

	/**@}*/

#endif

/**@endcond*/
