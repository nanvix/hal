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

#ifndef ARCH_CORE_ARM64_EXCP_H_
#define ARCH_CORE_ARM64_EXCP_H_

	/* Must come first. */
	#define __NEED_CORE_CONTEXT
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup arm64-core-exception Exception
 * @ingroup arm64-core
 *
 * @brief Exceptions
 */
/**@{*/

	#include <arch/core/arm64/core.h>
	#include <arch/core/arm64/types.h>
	#include <arch/core/arm64/ctx.h>
	#include <nanvix/const.h>

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define ARM64_EXCP_SIZE 24

	/**
	 * @brief Number of exceptions.
	 */
	#define ARM64_EXCP_NUM 4

	/**
	 * @name Hardware Exceptions
	 */
	/**@{*/
	#define ARM64_EXCP_TESTE           0 /**<  */
	/**@}*/


#ifndef _ASM_FILE_

	/**
	 * @cond arm64
	 */

		/**
		 * @brief Exception information.
		 */
		struct exception
		{
			arm64_word_t num;         /**< Cause.                */
			arm64_word_t addr;        /**< Faulting address.     */
			arm64_word_t instr;       /**< Faulting instruction. */
		} PACK ALIGN(ARM64_DWORD_SIZE);
#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */
	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define EXCEPTIONS_NUM               ARM64_EXCP_NUM            /**< @ref ARM64_EXCP_NUM_HW              */
	#define EXCEPTION_SIZE               ARM64_EXCP_SIZE           /**< @ref ARM64_EXCP_SIZE                */
	#define EXCEPTION_INVALID_OPCODE     0             /**< @ref ARM64_EXCP_ILLEGAL_INSTRUCTION */
	#define EXCEPTION_PAGE_FAULT         1         /**< @ref ARM64_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_PAGE_PROTECTION    2    /**< @ref ARM64_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_GENERAL_PROTECTION 3 /**< @ref ARM64_EXCP_VIRT_RESET          */
	/**@}*/

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __exception_struct /**< @ref exception */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __exception_get_addr_fn  /**< @ref exception_get_addr()  */
	#define __exception_get_instr_fn /**< @ref exception_get_instr() */
	#define __exception_get_num_fn   /**< @ref exception_get_num()   */
	#define __exception_dump_fn      /**< @ref exception_dump()      */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __exceptions_var /**< @ref exceptions */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see arm64_excp_get_cause().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		UNUSED(excp);
		return 0;
	}

	/**
	 * @see arm64_excp_get_addr().
	 */
	static inline arm64_vaddr_t exception_get_addr(const struct exception *excp)
	{
		UNUSED(excp);
		return 0;
	}

	/**
	 * @see arm64_excp_get_isntr().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		UNUSED(excp);
		return 0;
	}

	/**
	 * @see arm64_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		UNUSED(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_EXCP_H_ */
