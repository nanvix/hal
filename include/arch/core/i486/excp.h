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

#ifndef ARCH_CORE_I486_EXCP_H_
#define ARCH_CORE_I486_EXCP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES
	#define __NEED_CORE_CONTEXT

/**
 * @addtogroup i486-core-exception Exception
 * @ingroup i486-core
 *
 * @brief Exceptions
 */
/**@{*/

	#include <arch/core/i486/ctx.h>
	#include <arch/core/i486/types.h>
	#include <arch/core/i486/mmu.h>
	#include <nanvix/const.h>
	#include <stdint.h>

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define I486_EXCP_SIZE 16

	/**
	 * @name Offsets to the Exception Information Structure
	 *
	 * @see exception
	 */
	/**@{*/
	#define I486_EXCP_NR    0 /**< Offset to Exception Number      */
	#define I486_EXCP_ERR   4 /**< Offset to Error COde            */
	#define I486_EXCP_DATA  8 /**< Offset to Faulting Address      */
	#define I486_EXCP_CODE 12 /**< Offset to Faulting Instructtion */
	/**@}*/

	/**
	 * @brief Number of exceptions in the i486 core.
	 */
	#define I486_EXCP_NUM 21

	/**
	 * @name i486 Exception Codes
	 */
	/**@{*/
	#define I486_EXCP_DIVIDE                       0 /**@< Division-by-Zero Error      */
	#define I486_EXCP_DEBUG                        1 /**@< Debug Exception             */
	#define I486_EXCP_NMI                          2 /**@< Non-Maskable Interrupt      */
	#define I486_EXCP_BREAKPOINT                   3 /**@< Breakpoint Exception        */
	#define I486_EXCP_OVERFLOW                     4 /**@< Overflow Exception          */
	#define I486_EXCP_BOUNDS                       5 /**@< Bounds Check Exception      */
	#define I486_EXCP_INVALID_OPCODE               6 /**@< Invalid Opcode Exception    */
	#define I486_EXCP_COPROCESSOR_NOT_AVAILABLE    7 /**@< Coprocessor Not Available   */
	#define I486_EXCP_DOUBLE_FAULT                 8 /**@< Double Fault                */
	#define I486_EXCP_COPROCESSOR_SEGMENT_OVERRUN  9 /**@< Coprocessor Segment Overrun */
	#define I486_EXCP_INVALID_TSS                 10 /**@< Invalid TSS                 */
	#define I486_EXCP_SEGMENT_NOT_PRESENT         11 /**@< Segment Not Present         */
	#define I486_EXCP_STACK_SEGMENT_FAULT         12 /**@< Stack Segment Fault         */
	#define I486_EXCP_GENERAL_PROTECTION          13 /**@< General Protection Fault    */
	#define I486_EXCP_PAGE_FAULT                  14 /**@< Page Fault                  */
	#define I486_EXCP_FPU_ERROR                   16 /**@< Floating Point Exception    */
	#define I486_EXCP_ALIGNMENT_CHECK             17 /**@< Alignment Check Exception   */
	#define I486_EXCP_MACHINE_CHECK               18 /**@< Machine Check Exception     */
	#define I486_EXCP_SIMD_ERROR                  19 /**@< SMID Unit Exception         */
	#define I486_EXCP_VIRTUAL_EXCEPTION           20 /**@< Virtual Exception           */
	#define I486_EXCP_SECURITY_EXCEPTION          30 /**@< Security Exception.         */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Exception information.
	 */
	struct exception
	{
		i486_word_t num;         /**< Exception number.     */
		i486_word_t code;        /**< Error code.           */
		i486_word_t addr;        /**< Faulting address.     */
		i486_word_t instruction; /**< Faulting instruction. */
	} PACK;

	/**
	 * @brief Exception handler.
	 */
	typedef void (*i486_excp_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void i486_excp_dump(const struct exception *excp);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The i486_excp_get_num() function gets the exception number
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception number stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int i486_excp_get_num(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The i486_excp_get_addr() function gets the exception address
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception address stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline vaddr_t i486_excp_get_addr(const struct exception *excp)
	{
		return (excp->addr);
	}

	/**
	 * @brief Gets the program counter at an exception.
	 *
	 * The i486_excp_get_num() function gets the program counter
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The program counter stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline vaddr_t i486_excp_get_instr(const struct exception *excp)
	{
		return (excp->instruction);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define EXCEPTIONS_NUM                I486_EXCP_NUM                 /**< @ref I486_EXCP_NUM                */
	#define EXCEPTION_SIZE                I486_EXCP_SIZE                /**< @ref I486_EXCP_SIZE               */
	#define EXCEPTION_INVALID_OPCODE      I486_EXCP_INVALID_OPCODE      /**< @ref I486_EXCP_INVALID_OPCODE     */
	#define EXCEPTION_PAGE_FAULT          I486_EXCP_PAGE_FAULT          /**< @ref I486_EXCP_PAGE_FAULT         */
	#define EXCEPTION_PAGE_PROTECTION     I486_EXCP_PAGE_FAUULT         /**< @ref I486_EXCP_PAGE_FAULT         */
	#define EXCEPTION_GENERAL_PROTECTION  I486_EXCP_GENERAL_PROTECTION  /**< @ref I486_EXCP_GENERAL_PROTECTION */
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
	#define __exception_get_addr_fn    /**< @ref exception_get_addr()  */
	#define __exception_get_instr_fn   /**< @ref exception_get_instr() */
	#define __exception_get_num_fn     /**< @ref exception_get_num()   */
	#define __exception_dump_fn        /**< @ref exception_dump()      */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __exceptions_var /**< @ref exceptions */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see i486_excp_get_num().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (i486_excp_get_num(excp));
	}

	/**
	 * @see i486_excp_get_addr().
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (i486_excp_get_addr(excp));
	}

	/**
	 * @see i486_excp_get_code().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		return (i486_excp_get_instr(excp));
	}

	/**
	 * @see i486_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		i486_excp_dump(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_EXCP_H_ */
