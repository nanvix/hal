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

#ifndef ARCH_I486_EXCEPTION_H_
#define ARCH_I486_EXCEPTION_H_

/*============================================================================*
 *                              Exception Interface                           *
 *============================================================================*/

/**
 * @addtogroup i486-core-exception Exception
 * @ingroup i486-core
 *
 * @brief Exceptions
 */
/**@{*/

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define I486_EXCEPTION_SIZE 16

	/**
	 * @name Offsets to the Exception Information Structure
	 *
	 * @see exception
	 */
	/**@{*/
	#define I486_EXCEPTION_NUM   0 /**< Offset to Exception Number      */
	#define I486_EXCEPTION_ERR   4 /**< Offset to Error COde            */
	#define I486_EXCEPTION_DATA  8 /**< Offset to Faulting Address      */
	#define I486_EXCEPTION_CODE 12 /**< Offset to Faulting Instructtion */
	/**@}*/

	/**
	 * @brief Number of exceptions in the i486 core.
	 */
	#define I486_NUM_EXCEPTIONS 21

	/**
	 * @name i486 Exception Codes
	 */
	/**@{*/
	#define I486_EXCEPTION_DIVIDE                       0 /**@< Division-by-Zero Error      */
	#define I486_EXCEPTION_DEBUG                        1 /**@< Debug Exception             */
	#define I486_EXCEPTION_NMI                          2 /**@< Non-Maskable Interrupt      */
	#define I486_EXCEPTION_BREAKPOINT                   3 /**@< Breakpoint Exception        */
	#define I486_EXCEPTION_OVERFLOW                     4 /**@< Overflow Exception          */
	#define I486_EXCEPTION_BOUNDS                       5 /**@< Bounds Check Exception      */
	#define I486_EXCEPTION_INVALID_OPCODE               6 /**@< Invalid Opcode Exception    */
	#define I486_EXCEPTION_COPROCESSOR_NOT_AVAILABLE    7 /**@< Coprocessor Not Available   */
	#define I486_EXCEPTION_DOUBLE_FAULT                 8 /**@< Double Fault                */
	#define I486_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN  9 /**@< Coprocessor Segment Overrun */
	#define I486_EXCEPTION_INVALID_TSS                 10 /**@< Invalid TSS                 */
	#define I486_EXCEPTION_SEGMENT_NOT_PRESENT         11 /**@< Segment Not Present         */
	#define I486_EXCEPTION_STACK_SEGMENT_FAULT         12 /**@< Stack Segment Fault         */
	#define I486_EXCEPTION_GENERAL_PROTECTION          13 /**@< General Protection Fault    */
	#define I486_EXCEPTION_PAGE_FAULT                  14 /**@< Page Fault                  */
	#define I486_EXCEPTION_FPU_ERROR                   16 /**@< Floating Point Exception    */
	#define I486_EXCEPTION_ALIGNMENT_CHECK             17 /**@< Alignment Check Exception   */
	#define I486_EXCEPTION_MACHINE_CHECK               18 /**@< Machine Check Exception     */
	#define I486_EXCEPTION_SIMD_ERROR                  19 /**@< SMID Unit Exception         */
	#define I486_EXCEPTION_VIRTUAL_EXCEPTION           20 /**@< Virtual Exception           */
	#define I486_EXCEPTION_SECURITY_EXCEPTION          30 /**@< Security Exception.         */
	/**@}*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <arch/core/i486/context.h>
	#include <arch/core/i486/mmu.h>
	#include <stdint.h>

	/**
	 * @brief Exception information.
	 */
	struct exception
	{
		i486_word_t num;         /**< Exception number.     */
		i486_word_t code;        /**< Error code.           */
		i486_word_t addr;        /**< Faulting address.     */
		i486_word_t instruction; /**< Faulting instruction. */
	} __attribute__((packed));

	/**
	 * @brief Exception handler.
	 */
	typedef void (*i486_exception_handler_fn)(const struct exception *, const struct context *);

	/**
	 * @name Software Interrupt Hooks
	 */
	/**@{*/
	EXTERN void _do_excp0(void);  /**< Division-by-Zero Error      */
	EXTERN void _do_excp1(void);  /**< Debug Exception             */
	EXTERN void _do_excp2(void);  /**< Non-Maskable Interrupt      */
	EXTERN void _do_excp3(void);  /**< Breakpoint Exception        */
	EXTERN void _do_excp4(void);  /**< Overflow Exception          */
	EXTERN void _do_excp5(void);  /**< Bounds Check Exception      */
	EXTERN void _do_excp6(void);  /**< Invalid Opcode Exception    */
	EXTERN void _do_excp7(void);  /**< Coprocessor Not Available   */
	EXTERN void _do_excp8(void);  /**< Double Fault                */
	EXTERN void _do_excp9(void);  /**< Coprocessor Segment Overrun */
	EXTERN void _do_excp10(void); /**< Invalid TSS                 */
	EXTERN void _do_excp11(void); /**< Segment Not Present         */
	EXTERN void _do_excp12(void); /**< Stack Segment Fault         */
	EXTERN void _do_excp13(void); /**< General Protection Fault    */
	EXTERN void _do_excp14(void); /**< Page Fault                  */
	EXTERN void _do_excp15(void); /**< Reserved                    */
	EXTERN void _do_excp16(void); /**< Floating Point Exception    */
	EXTERN void _do_excp17(void); /**< Alignment Check Exception   */
	EXTERN void _do_excp18(void); /**< Machine Check Exception     */
	EXTERN void _do_excp19(void); /**< SMID Unit Exception         */
	EXTERN void _do_excp20(void); /**< Virtual Exception           */
	EXTERN void _do_excp30(void); /**< Security Exception.         */
	/**@}*/

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

	/**
	 * @brief Sets a handler for an exception.
	 *
	 * @param num     Number of the target exception.
	 * @param handler Exception handler.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int i486_excp_set_handler(int num, i486_exception_handler_fn handler);

	/**
	 * @brief Unsets a handler for an exception.
	 *
	 * @param excpnum Number of the target exception.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int i486_excp_unset_handler(int excpnum);

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
	#define EXCEPTIONS_NUM                I486_NUM_EXCEPTIONS                /**< @ref I486_NUM_EXCEPTIONS               */
	#define EXCEPTION_SIZE                I486_EXCEPTION_SIZE                /**< @ref I486_EXCEPTION_SIZE               */
	#define EXCEPTION_INVALID_OPCODE      I486_EXCEPTION_INVALID_OPCODE      /**< @ref I486_EXCEPTION_INVALID_OPCODE     */
	#define EXCEPTION_PAGE_FAULT          I486_EXCEPTION_PAGE_FAULT          /**< @ref I486_EXCEPTION_PAGE_FAULT         */
	#define EXCEPTION_PAGE_PROTECTION     I486_EXCEPTION_PAGE_FAUULT         /**< @ref I486_EXCEPTION_PAGE_FAULT         */
	#define EXCEPTION_GENERAL_PROTECTION  I486_EXCEPTION_GENERAL_PROTECTION  /**< @ref I486_EXCEPTION_GENERAL_PROTECTION */
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
	#define __exception_get_addr      /**< @ref exception_get_addr()      */
	#define __exception_get_instr     /**< @ref exception_get_instr()     */
	#define __exception_get_num       /**< @ref exception_get_num()       */
	#define __exception_set_handler   /**< @ref exception_set_handler()   */
	#define __exception_unset_handler /**< @ref exception_unset_handler() */
	/**@}*/

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
	 * @see i486_excp_set_handler()
	 */
	static inline int exception_set_handler(int num, i486_exception_handler_fn handler)
	{
		return (i486_excp_set_handler(num, handler));
	}

	/**
	 * @see i486_excp_unset_handler()
	 */
	static inline int exception_unset_handler(int num)
	{
		return (i486_excp_unset_handler(num));
	}

/**@endcond*/

#endif /* _ASM_FILE_ */

#endif /* ARCH_I486_EXCEPTION_H_ */
