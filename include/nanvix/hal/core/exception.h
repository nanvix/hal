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

#ifndef NANVIX_HAL_EXCEPTION_H_
#define NANVIX_HAL_EXCEPTION_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Constants */
	#ifndef EXCEPTION_SIZE
	#error "EXCEPTION_SIZE not defined"
	#endif
	#ifndef EXCEPTIONS_NUM
	#error "EXCEPTIONS_NUM not defined"
	#endif
	#ifndef EXCEPTION_INVALID_OPCODE
	#error "EXCEPTION_INVALID_OPCODE not defined"
	#endif
	#ifndef EXCEPTION_PAGE_FAULT
	#error "EXCEPTION_PAGE_FAULT not defined"
	#endif
	#ifndef EXCEPTION_PAGE_PROTECTION
	#error "EXCEPTION_PAGE_PROTECTION not defined"
	#endif
	#if (!CORE_HAS_TLB_HW)
		#ifndef EXCEPTION_DTLB_FAULT
		#error "EXCEPTION_DTLB_FAULT not defined"
		#endif
		#ifndef EXCEPTION_ITLB_FAULT
		#error "EXCEPTION_ITLB_FAULT not defined"
		#endif
	#endif
	#ifndef EXCEPTION_GENERAL_PROTECTION
	#error "EXCEPTION_GENERAL_PROTECTION not defined"
	#endif

	/* Structures */
	#ifndef __exception_struct
	#error "struct exception not defined?"
	#endif

	/* Variables */
	#ifndef __exceptions_var
	#error "exceptions not defined?"
	#endif

	/* Functions */
	#ifndef __exception_get_addr_fn
	#error "exception_get_addr() not defined?"
	#endif
	#ifndef __exception_get_instr_fn
	#error "exception_get_instr() not defined?"
	#endif
	#ifndef __exception_get_num_fn
	#error "exception_get_num() not defined?"
	#endif
	#ifndef __exception_dump_fn
	#error "exception_dump() not defined?"
	#endif

#endif

/*============================================================================*
 * Exception Interface                                                        *
 *============================================================================*/

	#include <nanvix/hal/core/context.h>
	#include <nanvix/const.h>

/**
 * @addtogroup kernel-hal-core-exception Exception
 * @ingroup kernel-hal-core
 *
 * @brief Exceptions HAL Interface
  */
/**@{*/

	/**
	 * @brief Exception information
	 */
	struct exception;

	/**
	 * @brief Exception handler
	 */
	typedef void (*exception_handler_t)(const struct exception *, const struct context *);

	/**
	 * @brief Information about exceptions.
	 */
	EXTERN struct exception_info
	{
		exception_handler_t handler; /**< Handler */
		const char *name;            /**< Name    */
	} exceptions[EXCEPTIONS_NUM];

	/**
	 * @brief Gets the number of an exception.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception number stored in the exception
	 * information structure pointed to by @p excp.
	 */
	EXTERN int exception_get_num(const struct exception *excp);

	/**
	 * @brief Gets the address of an exception.
	 *
	 * The k1b_excp_get_num() function gets the exception address.
	 * stored in the exception information structure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception address stored in the exception
	 * information structure pointed to by @p excp.
	 */
	EXTERN vaddr_t exception_get_addr(const struct exception *excp);

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void exception_dump(const struct exception *excp);

	/**
	 * @brief Registers an exception handler.
	 *
	 * @param excpnum Number of the target exception.
	 * @param handler Exception handler.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int exception_register(int excpnum, exception_handler_t handler);

	/**
	 * @brief Unregisters an exception handler.
	 *
	 * @param excpnum Number of the target exception.
	 *
	 * @returns Upon successful completion zero is returned. Upon
	 * failure a negative error code is returned instead.
	 */
	EXTERN int exception_unregister(int excpnum);

	/**
	 * @brief Forwards an exception.
	 *
	 * @param excpnum Number of the target exception.
	 * @param excp    Exception to be forwarded.
	 * @param ctx     Context information of the forwarded exception.
	 */
	EXTERN void exception_forward(
		int excpnum,
		const struct exception *excp,
		const struct context *ctx
	);

/**@}*/

#endif /* NANVIX_EXCEPTION_H_ */
