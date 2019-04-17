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

#ifndef ARCH_CORE_RV32GC_EXCP_H_
#define ARCH_CORE_RV32GC_EXCP_H_

	/* Must come first. */
	#define __NEED_CORE_CONTEXT
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup rv32gc-core-exception Exception
 * @ingroup rv32gc-core
 *
 * @brief Exceptions
 */
/**@{*/

	#include <arch/core/rv32gc/ctx.h>
	#include <arch/core/rv32gc/types.h>
	#include <nanvix/const.h>

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define RV32GC_EXCP_SIZE 16

	/**
	 * @brief Number of exceptions.
	 */
	#define RV32GC_EXCP_NUM 16

	/**
	 * @brief Number of virtual exceptions.
	 */
	#define RV32GC_EXCP_NUM_VIRT 3

	/**
	 * @brief Number of extended exceptions.
	 */
	#define RV32GC_EXCP_NUM_EXT (RV32GC_EXCP_NUM + RV32GC_EXCP_NUM_VIRT)

	/**
	 * @name Hardware Exceptions
	 */
	/**@{*/
	#define RV32GC_EXCP_ALIGNMENT_INSTR           0 /**< Instruction Address Misaligned */
	#define RV32GC_EXCP_PAGE_PROTECTION_INSTR     1 /**< Instruction Access Fault       */
	#define RV32GC_EXCP_OPCODE                    2 /**< Illegal Instruction            */
	#define RV32GC_EXCP_BREAKPOINT                3 /**< Breakpoint                     */
	#define RV32GC_EXCP_ALIGNMENT_LOAD            4 /**< Load Address Misaligned        */
	#define RV32GC_EXCP_PAGE_PROTECTION_LOAD      5 /**< Load Access Fault              */
	#define RV32GC_EXCP_ALIGNMEN_STORE            6 /**< Store/AMO Address Misaligned   */
	#define RV32GC_EXCP_PAGE_PROTECTION_STORE     7 /**< Store/AMO Access Fault         */
	#define RV32GC_EXCP_GENERAL_PROTECTION_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32GC_EXCP_GENERAL_PROTECTION_SMODE  9 /**< Environment Call from S-mode   */
	#define RV32GC_EXCP_RESERVED2                10 /**< Reserved                       */
	#define RV32GC_EXCP_GENERAL_PROTECTION_MMODE 11 /**< Environment Call from M-mode   */
	#define RV32GC_EXCP_PAGE_FAULT_INSTR         12 /**< Instruction Page Fault         */
	#define RV32GC_EXCP_PAGE_FAULT_LOAD          13 /**< Load Page Fault                */
	#define RV32GC_EXCP_RESERVED1                14 /**< Reserved                       */
	#define RV32GC_EXCP_PAGE_FAULT_STORE         15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Virtual Exceptions
	 */
	/**@{*/
	#define RV32GC_VIRT_PAGE_FAULT         16 /**< Page Fault         */
	#define RV32GC_VIRT_PAGE_PROTECTION    17 /**< Page Protection    */
	#define RV32GC_VIRT_GENERAL_PROTECTION 18 /**< General Protection */
	/**@}*/

	/**
	 * @name Offsets to the Exception Information Structure
	 *
	 * @see exception
	 */
	/**@{*/
	#define RV32GC_EXCP_NR     0 /**< Exception Identifier  */
	#define RV32GC_EXCP_ADDR   4 /**< Exception Address     */
	#define RV32GC_EXCP_INSTR  8 /**< Saved Program Counter */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @cond rv32gc
	 */

		/**
		 * @brief Exception information.
		 */
		struct exception
		{
			rv32gc_word_t num;         /**< Cause.                */
			rv32gc_word_t addr;        /**< Faulting address.     */
			rv32gc_word_t instr;       /**< Faulting instruction. */
			rv32gc_byte_t RESERVED[4]; /**< Required padding.     */

		} PACK;

	/**@endcond*/

	/**
	 * @brief Exception handler.
	 */
	typedef void (*rv32gc_excp_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void rv32gc_excp_dump(const struct exception *excp);

	/**
	 * @brief Low-level exception dispatcher.
	 */
	EXTERN void _rv32gc_do_excp(void);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The rv32gc_excp_get_cause() function gets the exception number
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
	static inline int rv32gc_excp_get_cause(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The rv32gc_excp_get_addr() function gets the exception address
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
	static inline vaddr_t rv32gc_excp_get_addr(const struct exception *excp)
	{
		return (excp->addr);
	}

	/**
	 * @brief Gets the program counter at an exception.
	 *
	 * The rv32gc_excp_get_cause() function gets the program counter
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
	static inline vaddr_t rv32gc_excp_get_isntr(const struct exception *excp)
	{
		return (excp->instr);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */
	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define EXCEPTIONS_NUM               RV32GC_EXCP_NUM_EXT            /**< @ref RV32GC_EXCP_NUM_HW              */
	#define EXCEPTION_SIZE               RV32GC_EXCP_SIZE               /**< @ref RV32GC_EXCP_SIZE                */
	#define EXCEPTION_INVALID_OPCODE     RV32GC_EXCP_OPCODE             /**< @ref RV32GC_EXCP_ILLEGAL_INSTRUCTION */
	#define EXCEPTION_PAGE_FAULT         RV32GC_VIRT_PAGE_FAULT         /**< @ref RV32GC_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_PAGE_PROTECTION    RV32GC_VIRT_PAGE_PROTECTION    /**< @ref RV32GC_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_GENERAL_PROTECTION RV32GC_VIRT_GENERAL_PROTECTION /**< @ref RV32GC_EXCP_VIRT_RESET          */
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
	 * @see rv32gc_excp_get_cause().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (rv32gc_excp_get_cause(excp));
	}

	/**
	 * @see rv32gc_excp_get_addr().
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (rv32gc_excp_get_addr(excp));
	}

	/**
	 * @see rv32gc_excp_get_isntr().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		return (rv32gc_excp_get_isntr(excp));
	}

	/**
	 * @see rv32gc_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		rv32gc_excp_dump(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_EXCP_H_ */
