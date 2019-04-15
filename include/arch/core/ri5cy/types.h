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

#if (!defined(__NEED_CORE_TYPES) && !defined(__NEED_MEMORY_TYPES))
	#error "ri5cy core types not required"
#endif

#include <arch/core/rv32gc/types.h>

/**
 * @addtogroup ri5cy-core-types Core Types
 * @ingroup ri5cy-core
 *
 * @brief RI5CY Core Types
 */
/**@{*/

#ifdef __NEED_CORE_TYPES
#ifndef __RI5CY_CORE_CONSTANTS
#define __RI5CY_CORE_CONSTANTS

	/**
	 * @name Bit-Length of Core Types
	 */
	/**@{*/
	#define RI5CY_BYTE_BIT   RV32GC_BYTE_BIT  /**< Byte        */
	#define RI5CY_HWORD_BIT  RV32GC_HWORD_BIT /**< Half Word   */
	#define RI5CY_WORD_BIT   RV32GC_WORD_BIT  /**< Word        */
	#define RI5CY_DWORD_BIT  RV32GC_DWORD_BIT /**< Double Word */
	/**@}*/

	/**
	 * @name Byte-Length of Core Types
	 */
	/**@{*/
	#define RI5CY_BYTE_SIZE   RV32GC_BYTE_SIZE  /**< Byte        */
	#define RI5CY_HWORD_SIZE  RV32GC_HWORD_SIZE /**< Half Word   */
	#define RI5CY_WORD_SIZE   RV32GC_WORD_SIZE  /**< Word        */
	#define RI5CY_DWORD_SIZE  RV32GC_DWORD_SIZE /**< Double Word */
	/**@}*/

	/**
	 * @name Casters for Core Types
	 */
	/**@{*/
	#define RI5CY_BYTE(x)       ((ri5cy_byte_t)(x))    /**< To Byte                */
	#define RI5CY_HWORD(x)      ((ri5cy_hword_t)(x))   /**< To Half Word           */
	#define RI5CY_WORD(x)       ((ri5cy_word_t)(x))    /**< To Word                */
	#define RI5CY_DWORD(x)      ((ri5cy_dword_t)(x))   /**< To Double Word         */
	#define RI5CY_BYTE_PTR(x)   ((ri5cy_byte_t *)(x))  /**< To Byte Pointer        */
	#define RI5CY_HWORDP_PTR(x) ((ri5cy_hword_t *)(x)) /**< To Half Word Pointer   */
	#define RI5CY_WORD_PTR(x)   ((ri5cy_word_t *)(x))  /**< To Word Pointer        */
	#define RI5CY_DWORD_PTR(x)  ((ri5cy_dword_t *)(x)) /**< To Double Word Pointer */
	/**@}*/

#endif /* __RI5CY_CORE_CONSTANTS */
#endif

/**@}*/

/**
 * @addtogroup ri5cy-core-mmu
 */
/**@{*/

#ifdef __NEED_MEMORY_TYPES
#ifndef __RI5CY_MEMORY_CONSTANTS
#define __RI5CY_MEMORY_CONSTANTS

	/**
	 * @name Bit-Length of Memory Types
	 */
	/**@{*/
	#define RI5CY_PADDR_BIT RV32GC_PADDR_BIT /**< Physical Address */
	/**@}*/

	/**
	 * @name Byte-Length of Memory Types
	 */
	/**@{*/
	#define RI5CY_PADDR_BYTE RV32GC_PADDR_BYTE /**< Physical Address */
	/**@}*/

#endif /* __RI5CY_MEMORY_CONSTANTS */
#endif

/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @addtogroup ri5cy-core-mmu
	 */
	/**@{*/

	#ifdef __NEED_MEMORY_TYPES
	#ifndef __RI5CY_MEMORY_TYPES
	#define __RI5CY_MEMORY_TYPES

		/**
		 * @name Casters for Memory Types
		 */
		/**@{*/
		#define RI5CY_PADDR(x) ((ri5cy_paddr_t)(x)) /**< To Physical Address */
		#define RI5CY_VADDR(x) RI5CY_PADDR(x)       /**< To Virtual Address  */
		#define RI5CY_FRAME(x) RI5CY_PADDR(x)       /**< To Frame Number     */
		/**@}*/

		/**
		 * @name Memory Types
		 */
		/**@{*/
		typedef rv32gc_paddr_t ri5cy_paddr_t; /**< Physical Address */
		typedef ri5cy_paddr_t ri5cy_frame_t;  /**< Frame Number     */
		typedef ri5cy_paddr_t ri5cy_vaddr_t;  /**< Virtual Address  */
		/**@}*/

	#endif /* __RI5CY_MEMORY_TYPES */
	#endif

	/**@}*/

	/**
	 * @addtogroup ri5cy-core
	 */
	/**@{*/

	#ifdef __NEED_CORE_TYPES
	#ifndef __RI5CY_CORE_TYPES
	#define __RI5CY_CORE_TYPES

		/**
		 * @brief Makes a half word.
		 *
		 * @param a High byte.
		 * @param b Low byte.
		 *
		 * @param A half word.
		 */
		#define RI5CY_MAKE_HWORD(a,b) RV32GC_MAKE_HWORD(a,b)

		/**
		 * @brief Makes a word.
		 *
		 * @param a High half word.
		 * @param b Low half word.
		 *
		 * @param A word.
		 */
		#define RI5CY_MAKE_WORD(a,b) RV32GC_MAKE_WORD(a, b)

		/**
		 * @brief Makes a double word.
		 *
		 * @param a High word.
		 * @param b Low word.
		 *
		 * @returns A dobule word.
		 */
		#define RI5CY_MAKE_DWORD(a,b) RV32GC_MAKE_DWORD(a, b)

		/**
		 * @name Core Types
		 */
		/**@{*/
		typedef rv32gc_byte_t  ri5cy_byte_t;  /**< Byte        */
		typedef rv32gc_hword_t ri5cy_hword_t; /**< Half Word   */
		typedef rv32gc_word_t  ri5cy_word_t;  /**< Word        */
		typedef rv32gc_dword_t ri5cy_dword_t; /**< Double Word */
		/**@}*/

	#endif /* __RI5CY_CORE_TYPES */
	#endif

	/**@}*/

#endif

/**@endcond*/
