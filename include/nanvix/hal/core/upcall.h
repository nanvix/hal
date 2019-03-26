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

#ifndef NANVIX_HAL_CORE_UPCALL_H_
#define NANVIX_HAL_CORE_UPCALL_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Constants. */
	#ifndef NR_upcall_ret
	#error "NR_upcall_ret not defined!"
	#endif

	/* Functions */
	#ifndef __upcall_ret_fn
	#error "upcall_ret() not defined?"
	#endif

#endif

/*============================================================================*
 * Upcall Interface                                                           *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-upcall Upcall
 * @ingroup kernel-hal-core
 *
 * @brief Upcall HAL Interface
 */
/**@{*/

	#include <nanvix/hal/core/context.h>
	#include <nanvix/const.h>
	#include <nanvix/klib.h>

	/**
	 * @name Offsets to the Upcall Stack Frame
	 */
	/**@{*/
	#define UPCALL_STACK_FRAME_FN_OFF      (0*WORD_SIZE) /**< Target Function */
	#define UPCALL_STACK_FRAME_ARGSIZE_OFF (1*WORD_SIZE) /**< Argument Size   */
	#define UPCALL_STACK_FRAME_ARG_OFF     (2*WORD_SIZE) /**< Argument        */
	/**@}*/

	/**
	 * @brief Returns the stack frame size of an upcall.
	 *
	 * @param x Size of upcall argument.
	 */
	#define UPCALL_STACK_FRAME_SIZE(x) \
		(CONTEXT_SIZE + sizeof(word_t) + sizeof(word_t) + TRUNCATE(x, DWORD_SIZE))

	/**
	 * @brief Forges an upcall.
	 *
	 * @param ctx     Saved context.
	 * @param fn      Target function.
	 * @param arg     Argument.
	 * @param argsize Size of argument.
	 */
	EXTERN void upcall_forge(
		struct context *ctx,
		void (*fn)(void *),
		void *arg,
		word_t argsize
	);

	/**
	 * @brief Returns from an upcall.
	 */
	EXTERN void upcall_ret(void);

/**@}*/

#endif /* NANVIX_HAL_CORE_UPCALL_H_ */
