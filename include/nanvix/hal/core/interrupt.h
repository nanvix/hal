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

	/* Constants */
	#ifndef _HAL_INT_NR
	#error "_HAL_INT_NR not defined"
	#endif

	/* Structures & Types */
	#ifndef __context_struct
	#error "struct context not defined?"
	#endif

	/* Functions */
	#ifndef __interrupts_disable
	#error "interrupts_disable() not defined?"
	#endif
	#ifndef __interrupts_enable
	#error "interrupts_enable() not defined?"
	#endif
	#ifndef __interrupt_mask
	#error "interrupt_mask() not defined?"
	#endif
	#ifndef __interrupt_unmask
	#error "interrupt_unmask() not defined?"
	#endif
	#ifndef __interrupt_ack
	#error "interrupt_ack() not defined?"
	#endif
	#ifndef __interrupt_set_handler
	#error "interrupt_set_handler() not defined?"
	#endif
	#ifndef __hal_intlvl_set
	#error "hal_intlvl_set() not defined?"
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
	 * @brief Number of hardware interrupts.
	 */
	#define HAL_INT_NR _HAL_INT_NR

	/**
	 * @brief Hardware interrupt handler.
	 */
	typedef void (*interrupt_handler_t)(int);

	/**
	 * @brief Disables all hardware interrupts.
	 */
	EXTERN void interrupts_disable(void);

	/**
	 * @brief Enables all hardware interrupts.
	 */
	EXTERN void interrupts_enable(void);

	/**
	 * @brief Sets a handler for an interrupt.
	 *
	 * @param num     Number of the target interrupt.
	 * @param handler Handler.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @note This function does not check if a handler is already
	 * set for the target hardware interrupt.
	 */
	EXTERN int interrupt_set_handler(int num, interrupt_handler_t handler);

	/**
	 * @brief Sets the interrupt level of the underlying core.
	 *
	 * @param newlevel New interrupt level.
	 *
	 * @returns The old interrupt level.
	 */
	EXTERN int hal_intlvl_set(int newlevel);

	/**
	 * @brief Acknowledges an interrupt.
	 *
	 * @param intnum Number of the target interrupt.
	 */
	EXTERN void interrupt_ack(int intnum);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param intnum Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int interrupt_mask(int intnum);

	/**
	 * @brief Unmasks an interrupt.
	 *
	 * @param intnum Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error number is returned instead.
	 */
	EXTERN int interrupt_unmask(int intnum);

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
