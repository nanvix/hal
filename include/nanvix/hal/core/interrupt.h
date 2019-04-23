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

#ifndef NANVIX_HAL_INTERRUPT_H_
#define NANVIX_HAL_INTERRUPT_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_INTERRUPT)

	/* Constants */
	#ifndef INTERRUPTS_NUM
	#error "INTERRUPTS_NUM not defined"
	#endif

	/* Structures & Types */
	#ifndef __context_struct
	#error "struct context not defined?"
	#endif

	/* Variables */
	#ifndef __interrupt_handlers_var
	#error "interrupts_handlers[] not defined?"
	#endif

	/* Functions */
	#ifndef __interrupts_disable_fn
	#error "interrupts_disable() not defined?"
	#endif
	#ifndef __interrupts_enable_fn
	#error "interrupts_enable() not defined?"
	#endif
	#ifndef __interrupt_mask_fn
	#error "interrupt_mask() not defined?"
	#endif
	#ifndef __interrupt_unmask_fn
	#error "interrupt_unmask() not defined?"
	#endif
	#ifndef __interrupt_next_fn
	#error "interrupt_next() not defined?"
	#endif
	#ifndef __interrupt_ack_fn
	#error "interrupt_ack() not defined?"
	#endif

#endif

/*============================================================================*
 * Interrupt Interface                                                        *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-interrupts Interrupt
 * @ingroup kernel-hal-core
 *
 * @brief HW/SW Interrupt HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Threshold for spurious interrupts.
	 *
	 * INTERRUPT_SPURIOUS_THRESHOLD states the number of spurious
	 * interrupts that we are willing to get, before enter in verbose
	 * mode.
	 */
	#define INTERRUPT_SPURIOUS_THRESHOLD 100

	/**
	 * @brief Hardware interrupt handler.
	 */
	typedef void (*interrupt_handler_t)(int);

	/**
	 * @brief interrupt handlers.
	 */
	EXTERN void (*interrupt_handlers[INTERRUPTS_NUM])(int);

	/**
	 * @brief Gets the next pending interrupt.
	 *
	 * @returns The number of the next pending interrupt, or zero if
	 * no interrupt is pending.
	 */
	EXTERN int interrupt_next(void);

	/**
	 * @brief High-level hardware interrupt dispatcher.
	 *
	 * @param intnum Number of the interrupt.
	 */
	EXTERN void do_interrupt(int intnum);

	/**
	 * @brief Disables all hardware interrupts.
	 */
	EXTERN void interrupts_disable(void);

	/**
	 * @brief Enables all hardware interrupts.
	 */
	EXTERN void interrupts_enable(void);

	/**
	 * @brief Registers an interrupt handler.
	 *
	 * @param num     Number of the interrupt.
	 * @param handler Interrupt handler.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int interrupt_register(int num, interrupt_handler_t handler);

	/**
	 * @brief Unregisters an interrupt handler.
	 *
	 * @param num Number of the interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int interrupt_unregister(int num);

	/**
	 * @brief Setups hardware interrupts.
	 */
	EXTERN void interrupt_setup(void);

/**@}*/

#endif /* NANVIX_HAL_INTERRUPT_H_ */
