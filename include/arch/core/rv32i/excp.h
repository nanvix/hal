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

#ifndef ARCH_CORE_RV32I_EXCP_H_
#define ARCH_CORE_RV32I_EXCP_H_

/**
 * @addtogroup rv32i-core-exception Exception
 * @ingroup rv32i-core
 *
 * @brief Exceptions
 */
/**@{*/

	/* Must come first. */
	#define __NEED_IVT
	#define __NEED_CONTEXT
	#define __NEED_MEMORY_TYPES

	#include <arch/core/rv32i/context.h>

#ifndef _ASM_FILE_

	#include <arch/core/rv32i/core.h>
	#include <arch/core/rv32i/ivt.h>
	#include <arch/core/rv32i/types.h>
	#include <nanvix/const.h>
	#include <stdint.h>

#endif /* _ASM_FILE_ */

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define RV32I_EXCP_SIZE 16

	/**
	 * @brief Number of virtual exceptions.
	 */
	#define RV32I_EXCP_NUM_VIRT 3

	/**
	 * @brief Number of extended exceptions.
	 */
	#define RV32I_EXCP_NUM_EXT (RV32I_EXCP_NUM + RV32I_EXCP_NUM_VIRT)

	/**
	 * @name Hardware Exceptions
	 */
	/**@{*/
	#define RV32I_EXCP_ALIGNMENT_INSTR           0 /**< Instruction Address Misaligned */
	#define RV32I_EXCP_PAGE_PROTECTION_INSTR     1 /**< Instruction Access Fault       */
	#define RV32I_EXCP_OPCODE                    2 /**< Illegal Instruction            */
	#define RV32I_EXCP_BREAKPOINT                3 /**< Breakpoint                     */
	#define RV32I_EXCP_ALIGNMENT_LOAD            4 /**< Load Address Misaligned        */
	#define RV32I_EXCP_PAGE_PROTECTION_LOAD      5 /**< Load Access Fault              */
	#define RV32I_EXCP_ALIGNMEN_STORE            6 /**< Store/AMO Address Misaligned   */
	#define RV32I_EXCP_PAGE_PROTECTION_STORE     7 /**< Store/AMO Access Fault         */
	#define RV32I_EXCP_GENERAL_PROTECTION_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32I_EXCP_GENERAL_PROTECTION_SMODE  9 /**< Environment Call from S-mode   */
	#define RV32I_EXCP_RESERVED2                10 /**< Reserved                       */
	#define RV32I_EXCP_GENERAL_PROTECTION_MMODE 11 /**< Environment Call from M-mode   */
	#define RV32I_EXCP_PAGE_FAULT_INSTR         12 /**< Instruction Page Fault         */
	#define RV32I_EXCP_PAGE_FAULT_LOAD          13 /**< Load Page Fault                */
	#define RV32I_EXCP_RESERVED1                14 /**< Reserved                       */
	#define RV32I_EXCP_PAGE_FAULT_STORE         15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Virtual Exceptions
	 */
	/**@{*/
	#define RV32I_VIRT_PAGE_FAULT         16 /**< Page Fault         */
	#define RV32I_VIRT_PAGE_PROTECTION    17 /**< Page Protection    */
	#define RV32I_VIRT_GENERAL_PROTECTION 18 /**< General Protection */
	/**@}*/

	/**
	 * @name Offsets to the Exception Information Structure
	 *
	 * @see exception
	 */
	/**@{*/
	#define RV32I_EXCP_ID     0 /**< Exception Identifier  */
	#define RV32I_EXCP_ADDR   4 /**< Exception Address     */
	#define RV32I_EXCP_INSTR  8 /**< Saved Program Counter */
	/**@}*/

#ifndef _ASM_FILE_

/**
 * @cond rv32i
 */

	/**
	 * @brief Exception information.
	 */
	struct exception
	{
		rv32i_word_t id;         /**< Cause.                */
		rv32i_word_t addr;        /**< Faulting address.     */
		rv32i_word_t instr;       /**< Faulting instruction. */
		rv32i_byte_t RESERVED[4]; /**< Required padding.     */

	} PACK;

/**@endcond*/

	/**
	 * @brief Dumps all GPRs.
	 *
	 * @brief ctx  Saved execution context.
	 */
	EXTERN void rv32i_dump_all_gpr(const struct context *ctx);

	/**
	 * @brief Exception handler.
	 */
	typedef void (*rv32i_exception_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The rv32i_excp_get_cause() function gets the exception number
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
	static inline int rv32i_excp_get_cause(const struct exception *excp)
	{
		return (excp->id);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The rv32i_excp_get_addr() function gets the exception address
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
	static inline vaddr_t rv32i_excp_get_addr(const struct exception *excp)
	{
		return (excp->addr);
	}

	/**
	 * @brief Gets the program counter at an exception.
	 *
	 * The rv32i_excp_get_cause() function gets the program counter
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
	static inline vaddr_t rv32i_excp_get_isntr(const struct exception *excp)
	{
		return (excp->instr);
	}

	/**
	 * @brief Sets a handler for an exception.
	 *
	 * @param num     Number of the target exception.
	 * @param handler Exception handler.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int rv32i_excp_set_handler(int num, rv32i_exception_handler_fn handler);

	/**
	 * @brief Unsets a handler for an exception.
	 *
	 * @param num Number of the target exception.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int rv32i_excp_unset_handler(int num);

	/**
	 * @brief Low-level exception dispatcher.
	 */
	EXTERN void _do_excp(void);

	/**
	 * @brief High-level exception dispatcher.
	 *
	 * @brief excp Exception information.
	 * @brief ctx  Saved execution context.
	 *
	 * @note This function is called from assembly code.
	 */
	EXTERN void do_excp(const struct exception *excp, const struct context *ctx);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */
	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define EXCEPTIONS_NUM               RV32I_EXCP_NUM_EXT            /**< @ref RV32I_EXCP_NUM_HW              */
	#define EXCEPTION_SIZE               RV32I_EXCP_SIZE               /**< @ref RV32I_EXCP_SIZE                */
	#define EXCEPTION_INVALID_OPCODE     RV32I_EXCP_OPCODE             /**< @ref RV32I_EXCP_ILLEGAL_INSTRUCTION */
	#define EXCEPTION_PAGE_FAULT         RV32I_VIRT_PAGE_FAULT         /**< @ref RV32I_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_PAGE_PROTECTION    RV32I_VIRT_PAGE_PROTECTION    /**< @ref RV32I_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_GENERAL_PROTECTION RV32I_VIRT_GENERAL_PROTECTION /**< @ref RV32I_EXCP_VIRT_RESET          */
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
	#define __exception_get_addr      /**< @ref exception_get_addr()      */
	#define __exception_get_instr     /**< @ref exception_get_instr()     */
	#define __exception_get_num       /**< @ref exception_get_num()       */
	#define __exception_set_handler   /**< @ref exception_set_handler()   */
	#define __exception_unset_handler /**< @ref exception_unset_handler() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see rv32i_excp_get_cause().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (rv32i_excp_get_cause(excp));
	}

	/**
	 * @see rv32i_excp_get_addr().
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (rv32i_excp_get_addr(excp));
	}

	/**
	 * @see rv32i_excp_get_isntr().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		return (rv32i_excp_get_isntr(excp));
	}

	/**
	 * @see rv32i_excp_set_handler().
	 */
	static inline int exception_set_handler(int num, rv32i_exception_handler_fn handler)
	{
		return (rv32i_excp_set_handler(num, handler));
	}

	/**
	 * @see rv32i_excp_unset_handler().
	 */
	static inline int exception_unset_handler(int num)
	{
		return (rv32i_excp_unset_handler(num));
	}

	/**
	 * @brief Forwards an exception.
	 *
	 * @param num  Target exception.
	 * @param excp Exception to be forwarded.
	 * @param ctx  Context information of the forwarded exception.
	 */
	EXTERN void forward_excp(int num, const struct exception *excp, const struct context *ctx);

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_RV32I_EXCP_H_ */
