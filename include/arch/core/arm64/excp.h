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
	#define ARM64_EXCP_NUM 61

	/**
	 * @brief Number of virtual exceptions.
	 */
	#define ARM64_EXCP_NUM_VIRT 3

	/**
	 * @brief Number of extended exceptions.
	 */
	#define ARM64_EXCP_NUM_EXT (ARM64_EXCP_NUM + ARM64_EXCP_NUM_VIRT)

	/**
	 * @brief Shift to get the exception cause.
	 */
	#define ARM64_EXCP_CAUSE_SHIFT (26)

	/**
	 * @name Hardware Exceptions
	 */
	/**@{*/
	#define ARM64_EXCP_UNKNOWN_REASON                           0 /**<  */
	#define ARM64_EXCP_TRAPPED_WFI           			        1 /**<  */
	#define ARM64_EXCP_TRAPPED_LDC_STC_ACESS           			6 /**<  */
	#define ARM64_EXCP_SVE_SIMD_FP           					7 /**<  */
	#define ARM64_EXCP_BRANCH_TARGET           					13 /**<  */
	#define ARM64_EXCP_ILLEGAL_EXECUTION                		14 /**<  */
	#define ARM64_EXCP_SVC_AARCH64           					21 /**<  */
	#define ARM64_EXCP_HVC_AARCH64           					22 /**<  */
	#define ARM64_EXCP_SMC_AARCH64           					23 /**<  */
	#define ARM64_EXCP_TRAPPED_MSR_MRS_SYSTEM_AARCH64           24 /**<  */
	#define ARM64_EXCP_ACESS_SVE           						25 /**<  */
	#define ARM64_EXCP_ABORT_FROM_LOWER_EXCP_LEVEL  			32 /**<  */
	#define ARM64_EXCP_ABORT_WITHOUT_CHANGE_EXCP_LEVEL  		33 /**<  */
	#define ARM64_EXCP_PC_ALIGNMENT_FAULT  						34 /**<  */
	#define ARM64_EXCP_DATA_ABORT_FROM_LOWER_EXCP_LEVEL  		36 /**<  */
	#define ARM64_EXCP_DATA_ABORT_WITHOUT_CHANGE_EXCP_LEVEL  	37 /**<  */
	#define ARM64_EXCP_SP_ALIGNMENT_FAULT  						38 /**<  */
	#define ARM64_EXCP_TRAPPED_FP_AARCH64  						44 /**<  */
	#define ARM64_EXCP_SError  									47 /**<  */
	#define ARM64_EXCP_BREAKPOINT_FROM_LOWER_EXCP_LEVEL  		48 /**<  */
	#define ARM64_EXCP_BREAKPOINT_WITHOUT_CHANGE_EXCP_LEVEL  	49 /**<  */
	#define ARM64_EXCP_STEP_FROM_LOWER_EXCP_LEVEL  				50 /**<  */
	#define ARM64_EXCP_STEP_WITHOUT_CHANGE_EXCP_LEVEL  			51 /**<  */
	#define ARM64_EXCP_WATCHPOINT_FROM_LOWER_EXCP_LEVEL  		52 /**<  */
	#define ARM64_EXCP_WATCHPOINT_WITHOUT_CHANGE_EXCP_LEVEL  	53 /**<  */
	#define ARM64_EXCP_BRK_AARCH64  							60 /**<  */
	/**@}*/

	/**
	 * @name Virtual Exceptions
	 */
	/**@{*/
	#define ARM64_VIRT_PAGE_FAULT         61 /**< Page Fault         */
	#define ARM64_VIRT_PAGE_PROTECTION    62 /**< Page Protection    */
	#define ARM64_VIRT_GENERAL_PROTECTION 63 /**< General Protection */
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
			arm64_word_t code;        /**< Code Error            */
			arm64_word_t addr;        /**< Faulting address.     */
		} PACK;

	/**
	 * @brief Dumps information about an exception.
	 */
	EXTERN void arm64_excp_dump(const struct exception *excp);

	/**
	 * @brief Handle and map the hardware excp to a valid number.
	 */
	EXTERN void arm64_handle_exception(const struct context *ctx, arm64_word_t excp_cause, arm64_word_t fault_addr);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The arm64_excp_get_cause() function gets the exception number
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 */
	static inline int arm64_excp_get_cause(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The arm64_excp_get_addr() function gets the exception address
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 */
	static inline arm64_vaddr_t arm64_excp_get_addr(const struct exception *excp)
	{
		return (excp->addr);
	}

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
	#define EXCEPTIONS_NUM               ARM64_EXCP_NUM_EXT            /**< @ref ARM64_EXCP_NUM_HW              */
	#define EXCEPTION_SIZE               ARM64_EXCP_SIZE           /**< @ref ARM64_EXCP_SIZE                */
	#define EXCEPTION_INVALID_OPCODE     ARM64_EXCP_ILLEGAL_EXECUTION             /**< @ref ARM64_EXCP_ILLEGAL_INSTRUCTION */
	#define EXCEPTION_PAGE_FAULT         ARM64_VIRT_PAGE_FAULT         /**< @ref ARM64_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_PAGE_PROTECTION    ARM64_VIRT_PAGE_PROTECTION    /**< @ref ARM64_EXCP_VIRT_PAGE_FAULT     */
	#define EXCEPTION_GENERAL_PROTECTION ARM64_VIRT_GENERAL_PROTECTION /**< @ref ARM64_EXCP_VIRT_RESET          */
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
		return (arm64_excp_get_cause(excp));
	}

	/**
	 * @see arm64_excp_get_addr().
	 */
	static inline arm64_vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (arm64_excp_get_addr(excp));
	}

	/**
	 * @see arm64_excp_get_isntr().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		UNUSED(excp);
		return (0);
	}

	/**
	 * @see arm64_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		arm64_excp_dump(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_EXCP_H_ */
