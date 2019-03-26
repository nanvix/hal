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

#ifndef ARCH_CORE_RV32I_CORE_H_
#define ARCH_CORE_RV32I_CORE_H_

	#define __NEED_CORE_TYPES
	#include <arch/core/rv32i/types.h>

/**
 * @addtogroup rv32i-core
 */
/**@{*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>

#endif /* _ASM_FILE_ */

#ifndef _ASM_FILE_

	/**
	 * @brief Gets the ID of the core.
	 *
	 * The rv32i_core_get_id() returns the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 *
	 * @FIXME Fix this.
	 */
	static inline int rv32i_core_get_id(void)
	{
		return (0);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   RV32I_BYTE_BIT   /**< @see BYTE_BIT   */
	#define HWORD_BIT  RV32I_HWORD_BIT  /**< @see HWORD_BIT  */
	#define WORD_BIT   RV32I_WORD_BIT   /**< @see WORD_BIT   */
	#define DWORD_BIT  RV32I_DWORD_BIT  /**< @see DWORD_BIT  */
	#define BYTE_SIZE  RV32I_SIZE_SIZE  /**< @see BYTE_SIZE  */
	#define HWORD_SIZE RV32I_HWORD_SIZE /**< @see HWORD_SIZE */
	#define WORD_SIZE  RV32I_WORD_SIZE  /**< @see WORD_SIZE  */
	#define DWORD_SIZE RV32I_DWORD_SIZE /**< @see DWORD_SIZE */
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
	#define ___core_reset_fn    /**< _core_reset()    */
	#define __core_clear_fn     /**< core_clear()     */
	#define __core_get_id_fn    /**< core_get_id()    */
	#define __core_notify_fn    /**< core_notify()    */
	#define __core_poweroff_fn  /**< core_poweroff()  */
	#define __core_setup_fn     /**< core_setup()     */
	#define __core_waitclear_fn /**< core_waitclear() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef rv32i_byte_t  byte_t;  /**< Byte        */
	typedef rv32i_hword_t hword_t; /**< Half Word   */
	typedef rv32i_word_t  word_t;  /**< Word        */
	typedef rv32i_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see rv32i_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (rv32i_core_get_id());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif	/* ARCH_CORE_RV32I_CORE_H_ */
