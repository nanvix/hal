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

#ifndef ARCH_CORE_K1B_EXCP_H_
#define ARCH_CORE_K1B_EXCP_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES
	#define __NEED_CORE_CONTEXT

/**
 * @addtogroup k1b-core-exception Exception
 * @ingroup k1b-core
 *
 * @brief Exception Interface
 */
/**@{*/

	#include <arch/core/k1b/ctx.h>
	#include <arch/core/k1b/types.h>
	#include <nanvix/const.h>
	#include <nanvix/hlib.h>

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define K1B_EXCP_SIZE 16

	/**
	 * @name Offsets to the Exception Information structure.
	 */
	/**@{*/
	#define K1B_EXCP_NR  0 /**< Exception Number      */
	#define K1B_EXCP_EA  4 /**< Exception Address     */
	#define K1B_EXCP_SPC 8 /**< Saved Program Counter */
	/**@}*/

	/**
	 * @brief Number of exceptions.
	 *
	 * Number of exceptions natively supported by the hardware.
	 */
	#define K1B_EXCP_NUM 16

	/**
	 * @brief Number of virtual exceptions.
	 *
	 * Number of exceptions virtualized in software.
	 */
	#define K1B_EXCP_NUM_VIRT 1

	/**
	 * @brief Number of extended exceptions.
	 */
	#define K1B_EXCP_NUM_EXT (K1B_EXCP_NUM + K1B_EXCP_NUM_VIRT)

	/**
	 * @name Hardware Exceptions
	 */
	/**@{*/
	#define K1B_EXCP_RESET            0 /**< Reset Exception                               */
	#define K1B_EXCP_OPCODE           1 /**< Bad Instruction Bundle                        */
	#define K1B_EXCP_PROTECTION       2 /**< Protection Fault                              */
	#define K1B_EXCP_ALIGNMENT        3 /**< Alignment Check                               */
	#define K1B_EXCP_RANGE_CODE       4 /**< Instruction Out of Range                      */
	#define K1B_EXCP_RANGE_DATA       5 /**< Data Out of Range                             */
	#define K1B_EXCP_DOUBLE_ECC_CODE  6 /**< Double ECC Fault on Out of Range Instruction  */
	#define K1B_EXCP_DOUBLE_ECC_DATA  7 /**< Double ECC Fault on Out of Range Data         */
	#define K1B_EXCP_PARITY_CODE      8 /**< Parity Error on Out of Range Instruction      */
	#define K1B_EXCP_PARITY_DATA      9 /**< Parity Error on Out of Range Data             */
	#define K1B_EXCP_SINGLE_ECC_CODE 10 /**< Single ECC Fault on Out of Range Instruction  */
	#define K1B_EXCP_SINGLE_ECC_DATA 11 /**< Single ECC Fault on Out of Range Data         */
	#define K1B_EXCP_TLB_FAULT       12 /**< TLB Fault                                     */
	#define K1B_EXCP_PAGE_PROTECTION 13 /**< Page Protection                               */
	#define K1B_EXCP_WRITE_CLEAN     14 /**< Write to Clean Exception                      */
	#define K1B_EXCP_ATOMIC_CLEAN    15 /**< Atomic to Clean Exception                     */
	/**@}*/

	/**
	 * @name Virtual Exceptions
	 */
	/**@{*/
	#define K1B_EXCP_VIRT_PAGE_FAULT 16 /**< Page Fault (Virtual Exception) */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @cond k1b
	 */

		/**
		 * Exception information.
		 */
		struct exception
		{
			k1b_word_t num;         /**< Exception number.      */
			k1b_word_t ea;          /**< Exception address.     */
			k1b_word_t spc;         /**< Saved program counter. */
			k1b_byte_t RESERVED[4]; /**< Required padding.      */
		} PACK;

	/**@endcond*/

	/**
	 * @brief Exception handler.
	 */
	typedef void (*k1b_excp_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void k1b_excp_dump(const struct exception *excp);

	/**
	 * @brief Low-level exception dispatcher.
	 */
	EXTERN void _k1b_do_excp(void);

	/**
	 * @brief High-level exception dispatcher.
	 *
	 * @brief excp Exception information.
	 * @brief ctx  Saved execution context.
	 *
	 * @note This function is called from assembly code.
	 */
	EXTERN void k1b_do_excp(const struct exception *excp, const struct context *ctx);

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The k1b_excp_get_num() function gets the exception number
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
	static inline int k1b_excp_get_num(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The k1b_excp_get_addr() function gets the exception address
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
	static inline vaddr_t k1b_excp_get_addr(const struct exception *excp)
	{
		return (excp->ea);
	}

	/**
	 * @brief Gets the program counter at an exception.
	 *
	 * The k1b_excp_get_num() function gets the program counter
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
	static inline vaddr_t k1b_excp_get_spc(const struct exception *excp)
	{
		return (excp->spc);
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
	#define EXCEPTIONS_NUM                K1B_EXCP_NUM_EXT          /**< @ref K1B_EXCP_NUM             */
	#define EXCEPTION_SIZE                K1B_EXCP_SIZE             /**< @ref K1B_EXCP_SIZE            */
	#define EXCEPTION_INVALID_OPCODE      K1B_EXCP_OPCODE           /**< @ref K1B_EXCP_OPCODE          */
	#define EXCEPTION_PAGE_FAULT          K1B_EXCP_VIRT_PAGE_FAULT  /**< @ref K1B_EXCP_VIRT_PAGE_FAULT */
	#define EXCEPTION_PAGE_PROTECTION     K1B_EXCP_PAGE_PROTECTION  /**< @ref K1B_EXCP_PAGE_PROTECTION */
	#define EXCEPTION_ITLB_FAULT          K1B_EXCP_TLB_FAULT        /**< @ref K1B_EXCP_TLB_FAULT       */
	#define EXCEPTION_DTLB_FAULT          K1B_EXCP_TLB_FAULT        /**< @ref K1B_EXCP_TLB_FAULT       */
	#define EXCEPTION_GENERAL_PROTECTION  K1B_EXCP_PROTECTION       /**< @ref K1B_EXCP_PROTECTION      */
	/**@}*/

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __exception_struct      /**< @ref exception */
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
	 * @see k1b_excp_get_num().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (k1b_excp_get_num(excp));
	}

	/**
	 * @see k1b_excp_get_addr().
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		return (k1b_excp_get_addr(excp));
	}

	/**
	 * @see k1b_excp_get_spc().
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		return (k1b_excp_get_spc(excp));
	}

	/**
	 * @see k1b_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		k1b_excp_dump(excp);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_EXCP_H_ */
