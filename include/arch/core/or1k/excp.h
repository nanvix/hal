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

#ifndef ARCH_CORE_OR1K_EXCP_H_
#define ARCH_CORE_OR1K_EXCP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES
	#define __NEED_OR1K_CONTEXT

/**
 * @addtogroup or1k-core-exception Exception
 * @ingroup or1k-core
 *
 * @brief Exceptions
 */
/**@{*/

	#include <arch/core/or1k/context.h>
	#include <arch/core/or1k/types.h>
	#include <nanvix/const.h>
	#include <nanvix/klib.h>

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define OR1K_EXCP_SIZE 16

	/**
	 * @name Offsets to the Exception Information structure.
	 *
	 * @see exception
	 */
	/**@{*/
	#define OR1K_EXCP_NR    0 /**< Exception Number      */
	#define OR1K_EXCP_EEAR  4 /**< Exception Address     */
	#define OR1K_EXCP_EPCR  8 /**< Saved Program Counter */
	/**@}*/

	/**
	 * @brief Number of exceptions in the or1k core.
	 */
	#define OR1K_EXCP_NUM 10

	/**
	 * @name or1k Exception Codes
	 */
	/**@{*/
	#define OR1K_EXCP_RESET                 0 /**@< Reset exception          */
	#define OR1K_EXCP_BUSERROR              1 /**@< Bus Error Exception      */
	#define OR1K_EXCP_PAGE_FAULT            2 /**@< Page fault Exception     */
	#define OR1K_EXCP_ALIGNMENT             3 /**@< Alignment Exception      */
	#define OR1K_EXCP_ILLEGAL_INSTRUCTION   4 /**@< Illegal Instruction      */
	#define OR1K_EXCP_DTLB_FAULT            5 /**@< Data TLB Fault           */
	#define OR1K_EXCP_ITLB_FAULT            6 /**@< Instruction TLB Fault    */
	#define OR1K_EXCP_RANGE                 7 /**@< Range Exception          */
	#define OR1K_EXCP_FLOAT_POINT           8 /**@< Floating Point Exception */
	#define OR1K_EXCP_TRAP                  9 /**@< Trap Exception           */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @cond or1k
	 */

		/**
		 * @brief Exception information.
		 */
		struct exception
		{
			or1k_word_t num;         /**< Exception number.      */
			or1k_word_t eear;        /**< Exception address.     */
			or1k_word_t epcr;        /**< Saved program counter. */
			or1k_byte_t RESERVED[4]; /**< Required padding.      */

		} PACK;

	/**@endcond*/

	/**
	 * @brief Exception handler.
	 */
	typedef void (*or1k_excp_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void or1k_excp_dump(const struct exception *excp);

	/**
	 * @brief Low-level exception dispatcher.
	 */
	EXTERN void _or1k_do_excp(void);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The or1k_excp_get_num() function gets the exception number
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception number stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Davidson Francis
	 */
	static inline int or1k_excp_get_num(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The or1k_excp_get_addr() function gets the exception address
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception address stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Davidson Francis
	 */
	static inline vaddr_t or1k_excp_get_addr(const struct exception *excp)
	{
		return (excp->eear);
	}

	/**
	 * @brief Gets the program counter at an exception.
	 *
	 * The or1k_excp_get_num() function gets the program counter
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The program counter stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Davidson Francis
	 */
	static inline vaddr_t or1k_excp_get_epcr(const struct exception *excp)
	{
		return (excp->epcr);
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
	 * @name Exported Constants
	 */
	/**@{*/
	#define EXCEPTIONS_NUM                OR1K_EXCP_NUM                 /**< @ref OR1K_EXCP_NUMS                */
	#define EXCEPTION_SIZE                OR1K_EXCP_SIZE                /**< @ref OR1K_EXCP_SIZE                */
	#define EXCEPTION_INVALID_OPCODE      OR1K_EXCP_ILLEGAL_INSTRUCTION /**< @ref OR1K_EXCP_ILLEGAL_INSTRUCTION */
	#define EXCEPTION_PAGE_FAULT          OR1K_EXCP_PAGE_FAULT          /**< @ref OR1K_EXCP_PAGE_FAULT          */
	#define EXCEPTION_PAGE_PROTECTION     OR1K_EXCP_PAGE_FAULT          /**< @ref OR1K_EXCP_PAGE_FAULT          */
	#define EXCEPTION_ITLB_FAULT          OR1K_EXCP_ITLB_FAULT          /**< @ref OR1K_EXCP_ITLB_FAULT          */
	#define EXCEPTION_DTLB_FAULT          OR1K_EXCP_DTLB_FAULT          /**< @ref OR1K_EXCP_DTLB_FAULT          */
	#define EXCEPTION_GENERAL_PROTECTION  OR1K_EXCP_RESET               /**< @ref OR1K_EXCP_RESET               */
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
	 * @see or1k_excp_get_num().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (or1k_excp_get_num(excp));
	}

	/**
	 * @see or1k_excp_get_addr().
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (or1k_excp_get_addr(excp));
	}

	/**
	 * @see or1k_excp_get_epcr().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		return (or1k_excp_get_epcr(excp));
	}

	/**
	 * @see or1k_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		or1k_excp_dump(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_EXCP_H_ */
