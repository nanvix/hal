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

#ifndef ARCH_CORE_K1B_CORE_H_
#define ARCH_CORE_K1B_CORE_H_

/**
 * @addtogroup k1b-core
 */
/**@{*/

	/* Must comme first. */
	#define __NEED_CORE_TYPES

	#include <arch/core/k1b/types.h>
	#include <arch/core/k1b/mOS.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the underlying core.
	 *
	 * @param stack Stack for interrupts, exceptions and traps.
	 */
	EXTERN void k1b_core_setup(void *stack);

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN NORETURN void k1b_core_poweroff(void);

	/**
	 * @brief Gets the ID of the core.
	 *
	 * The k1b_core_get_id() returns the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 */
	static inline int k1b_core_get_id(void)
	{
		return (__k1_get_cpu_id());
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   K1B_BYTE_BIT    /**< @ref BYTE_BIT   */
	#define HWORD_BIT  K1B_HWORD_BIT   /**< @ref HWORD_BIT  */
	#define WORD_BIT   K1B_WORD_BIT    /**< @ref WORD_BIT   */
	#define DWORD_BIT  K1B_DWORD_BIT   /**< @ref DWORD_BIT  */
	#define BYTE_SIZE  K1B_SIZE_SIZE   /**< @ref BYTE_SIZE  */
	#define HWORD_SIZE K1B_HWORD_SIZE  /**< @ref HWORD_SIZE */
	#define WORD_SIZE  K1B_WORD_SIZE   /**< @ref WORD_SIZE  */
	#define DWORD_SIZE K1B_DWORD_SIZE  /**< @ref DWORD_SIZE */
	/**@}*/

	/**
	 * @name Exported Types
	 */
	/**@{*/
	#define __byte_t  /**< @ref byte_t  */
	#define __hword_t /**< @ref hword_t */
	#define __word_t  /**< @ref word_t  */
	#define __dword_t /**< @ref dword_t */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __core_get_id_fn    /**< core_get_id()    */
	#define __core_poweroff_fn  /**< core_poweroff()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef k1b_byte_t  byte_t;  /**< Byte        */
	typedef k1b_hword_t hword_t; /**< Half Word   */
	typedef k1b_word_t  word_t;  /**< Word        */
	typedef k1b_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see k1b_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (k1b_core_get_id());
	}

	/**
	 * @see k1b_core_poweroff().
	 */
	static inline NORETURN void core_poweroff(void)
	{
		k1b_core_poweroff();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_CORE_H_ */
