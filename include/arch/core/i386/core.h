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

#ifndef ARCH_I386_CORE_H_
#define ARCH_I386_CORE_H_

/**
 * @addtogroup i386-core-core Core
 * @ingroup i386-core
 *
 * @brief i386 Core
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/i386/types.h>

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <stdint.h>

	/**
	 * @brief Gets the ID of the core.
	 *
	 * The i386_core_get_id() returns the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 */
	static inline int i386_core_get_id(void)
	{
		return (0);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @cond i386
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   I386_BYTE_BIT   /**< @see BYTE_BIT   */
	#define HWORD_BIT  I386_HWORD_BIT  /**< @see HWORD_BIT  */
	#define WORD_BIT   I386_WORD_BIT   /**< @see WORD_BIT   */
	#define DWORD_BIT  I386_DWORD_BIT  /**< @see DWORD_BIT  */
	#define BYTE_SIZE  I386_SIZE_SIZE  /**< @see BYTE_SIZE  */
	#define HWORD_SIZE I386_HWORD_SIZE /**< @see HWORD_SIZE */
	#define WORD_SIZE  I386_WORD_SIZE  /**< @see WORD_SIZE  */
	#define DWORD_SIZE I386_DWORD_SIZE /**< @see DWORD_SIZE */
	/**@}*/

	/**
	 * @brief Exported Types
	 */
	/**@{*/
	#define __byte_t  /**< @see byte_t  */
	#define __hword_t /**< @see hword_t */
	#define __word_t  /**< @see word_t  */
	#define __dword_t /**< @see dword_t */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __core_get_id   /**< core_get_id()   */
	#define __core_shutdown /**< core_shutdown() */
	#define __core_sleep    /**< core_sleep()    */
	#define __core_wakeup   /**< core_wakeup()   */
	#define __core_start    /**< core_start()    */
	#define __core_reset    /**< core_reset()    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef i386_byte_t  byte_t;  /**< Byte        */
	typedef i386_hword_t hword_t; /**< Half Word   */
	typedef i386_word_t  word_t;  /**< Word        */
	typedef i386_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see i386_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (i386_core_get_id());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_I386_CORE_H_ */
