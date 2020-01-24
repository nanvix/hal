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

#ifndef ARCH_CORE_OR1K_CORE_H_
#define ARCH_CORE_OR1K_CORE_H_

	/* Must Come first. */
	#define __NEED_OR1K_REGS
	#define __NEED_CORE_TYPES

/**
 * @addtogroup or1k-core
 */
/**@{*/

	#include <arch/core/or1k/regs.h>
	#include <arch/core/or1k/spinlock.h>
	#include <arch/core/or1k/types.h>
	#include <nanvix/const.h>
	#include <posix/stdint.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN NORETURN void or1k_core_poweroff(void);

	/**
	 * @brief Gets the ID of the core.
	 *
	 * The or1k_core_get_id() returns the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 */
	static inline int or1k_core_get_id(void)
	{
		return (or1k_mfspr(OR1K_SPR_COREID));
	}


#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   OR1K_BYTE_BIT   /**< @see BYTE_BIT   */
	#define HWORD_BIT  OR1K_HWORD_BIT  /**< @see HWORD_BIT  */
	#define WORD_BIT   OR1K_WORD_BIT   /**< @see WORD_BIT   */
	#define DWORD_BIT  OR1K_DWORD_BIT  /**< @see DWORD_BIT  */
	#define BYTE_SIZE  OR1K_SIZE_SIZE  /**< @see BYTE_SIZE  */
	#define HWORD_SIZE OR1K_HWORD_SIZE /**< @see HWORD_SIZE */
	#define WORD_SIZE  OR1K_WORD_SIZE  /**< @see WORD_SIZE  */
	#define DWORD_SIZE OR1K_DWORD_SIZE /**< @see DWORD_SIZE */
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
	#define ___core_reset_fn   /**< _core_reset()   */
	#define __core_get_id_fn   /**< core_get_id()   */
	#define __core_poweroff_fn /**< core_poweroff() */
	#define __core_setup_fn    /**< core_setup()    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef or1k_byte_t  byte_t;  /**< Byte        */
	typedef or1k_hword_t hword_t; /**< Half Word   */
	typedef or1k_word_t  word_t;  /**< Word        */
	typedef or1k_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see or1k_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (or1k_core_get_id());
	}

	/**
	 * @see or1k_core_poweroff().
	 */
	static inline NORETURN void core_poweroff(void)
	{
		or1k_core_poweroff();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif	/* ARCH_CORE_OR1K_CORE_H_ */
