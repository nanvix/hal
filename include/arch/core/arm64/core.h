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

#ifndef ARCH_CORE_ARM64_CORE_H_
#define ARCH_CORE_ARM64_CORE_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_CORE_REGS

/**
 * @addtogroup arm64-core
 */
/**@{*/

	#include <arch/core/arm64/types.h>
	#include <arch/core/arm64/mcall.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN NORETURN void arm64_core_poweroff(void);

	/**
	 * @see Gets the ID of the underlying core.
	 */
	static inline int arm64_core_get_id(void)
	{
		return (arm64_mcall_cpu_id());
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   ARM64_BYTE_BIT   /**< @see BYTE_BIT   */
	#define HWORD_BIT  ARM64_HWORD_BIT  /**< @see HWORD_BIT  */
	#define WORD_BIT   ARM64_WORD_BIT   /**< @see WORD_BIT   */
	#define DWORD_BIT  ARM64_DWORD_BIT  /**< @see DWORD_BIT  */
	#define BYTE_SIZE  ARM64_SIZE_SIZE  /**< @see BYTE_SIZE  */
	#define HWORD_SIZE ARM64_HWORD_SIZE /**< @see HWORD_SIZE */
	#define WORD_SIZE  ARM64_WORD_SIZE  /**< @see WORD_SIZE  */
	#define DWORD_SIZE ARM64_DWORD_SIZE /**< @see DWORD_SIZE */
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
	#define __core_get_id_fn    /**< core_get_id()    */
	#define __core_poweroff_fn  /**< core_poweroff()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef arm64_byte_t  byte_t;  /**< Byte        */
	typedef arm64_hword_t hword_t; /**< Half Word   */
	typedef arm64_word_t  word_t;  /**< Word        */
	typedef arm64_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see arm64_get_core_id
	 */
	static inline int core_get_id(void)
	{
		return (arm64_core_get_id());
	}

	/**
	 * @see arm64_core_poweroff().
	 */
	static inline NORETURN void core_poweroff(void)
	{
		arm64_core_poweroff();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_CORE_H_ */