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

#ifndef NANVIX_HAL_CONTEXT_H_
#define NANVIX_HAL_CONTEXT_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_CONTEXT)

	/* Constants */
	#ifndef CONTEXT_SIZE
	#error "CONTEXT_SIZE not defined"
	#endif

	/* Structures */
	#ifndef __context_struct
	#error "struct context not defined?"
	#endif
	#if CORE_SUPPORTS_MULTITHREADING
		#ifndef __stack_struct
		#error "struct stack not defined?"
		#endif
	#endif

	/* Functions */
	#ifndef __context_get_sp_fn
	#error "context_get_sp() not defined?"
	#endif
	#ifndef __context_get_pc_fn
	#error "context_get_pc() not defined?"
	#endif
	#ifndef __context_set_sp_fn
	#error "context_set_sp() not defined?"
	#endif
	#ifndef __context_set_pc_fn
	#error "context_set_pc() not defined?"
	#endif
	#ifndef __context_dump_fn
	#error "context_dump() not defined?"
	#endif
	#if CORE_SUPPORTS_MULTITHREADING
		#ifndef __context_create_fn
		#error "context_create() not defined?"
		#endif
		#ifndef __context_switch_to_fn
		#error "context_switch_to() not defined?"
		#endif
	#endif

#endif

/*============================================================================*
 * Execution Context Interface                                                *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-context Execution Context
 * @ingroup kernel-hal-core
 *
 * @brief Execution Context HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Execution context.
	 */
	struct context;

	/**
	 * @brief Execution Stack.
	 */
	#if CORE_SUPPORTS_MULTITHREADING
		struct stack;
	#else
		struct stack {
			word_t dummy;
		};
	#endif

	/**
	 * @brief Gets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the stack pointer register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	EXTERN word_t context_get_sp(const struct context *ctx);

	/**
	 * @brief Gets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the program conter register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	EXTERN word_t context_get_pc(const struct context *ctx);

	/**
	 * @brief Sets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	EXTERN void context_set_sp(struct context *ctx, word_t val);

	/**
	 * @brief Sets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	EXTERN void context_set_pc(struct context *ctx, word_t val);

	/**
	 * @brief Dumps context information.
	 *
	 * @param ctx Saved execution context.
	 */
	EXTERN void context_dump(const struct context *ctx);

	/**
	 * @brief Create a context.
	 *
	 * @param start  Start routine.
	 * @param ustack User stack pointer.
	 * @param kstack Kernel stack pointer.
	 *
	 * @returns Valid pointer if the context struct was created
	 * into the kernel stack correctly, NULL pointer otherwise.
	 */
	EXTERN struct context * context_create(
		void (* start)(void),
		struct stack * ustack,
		struct stack * kstack
	);

	/**
	 * @brief Switch between contexts.
	 *
	 * @details The previous context will be store into kernel stack
	 * and the pointer to it will be store into @previous parameter.
	 * And the next context will be restore from the @next parameter.
	 *
	 * @param previous Pointer to store the pointer of the previous context.
	 * @param next     Pointer to get the pointer to the next context.
	 *
	 * @returns Zero if the context was switched and restored, non-zero if
	 * the arguments are invalid.
	 */
	EXTERN int context_switch_to(
		struct context ** previous,
		struct context ** next
	);

/**@}*/

#endif /* NANVIX_HAL_CONTEXT_H_ */
