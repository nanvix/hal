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

#ifndef ARCH_CORE_K1B_INT_H_
#define ARCH_CORE_K1B_INT_H_

/**
 * @addtogroup k1b-core-int Interrupts
 * @ingroup k1b-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <arch/core/k1b/lpic.h>
	#include <arch/core/k1b/context.h>
	#include <arch/core/k1b/mOS.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define K1B_NUM_INT K1B_IRQ_NUM

	/**
	 * @name Hardware Interrupts for Kalray MPPA-256 Target
	 */
	/**@{*/
	#define K1B_INT_CLOCK0     0 /**< Clock 0              */
	#define K1B_INT_CLOCK1     1 /**< Clock 1              */
	#define K1B_INT_TIMER      2 /**< Watchdog Timer       */
	#define K1B_INT_CNOC       3 /**< Control NoC          */
	#define K1B_INT_DNOC       4 /**< Data NoC             */
	#define K1B_INT_DMA        5 /**< DMA                  */
	#define K1B_INT_NOC_ERR    6 /**< NoC Error            */
	#define K1B_INT_VIRT       7 /**< Virtual Line         */
	#define K1B_INT_TIMER_ERR  8 /**< Watchdog Timer Error */
	#define K1B_INT_DEBUG      9 /**< Debug                */
	#ifdef __k1io__
	#define K1B_INT_GIC1      10 /**< GIC 1                */
	#define K1B_INT_GIC2      11 /**< GIC 2                */
	#define K1B_INT_GIC3      12 /**< GIC2                 */
	#endif
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Hardware interrupt dispatcher.
	 *
	 * @param intnum Number of the interrupt that was triggered.
	 */
	EXTERN void k1b_do_int(int intnum);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param intnum Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_int_mask(int intnum);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param intnum Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_int_unmask(int intnum);

	/**
	 * @brief Sets a handler for a interrupt.
	 *
	 * @param intnum  Number of the target interrupt.
	 * @param handler Interrupt handler.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_int_handler_set(int intnum, void (*handler)(int));

	/**
	 * @brief Enables interrupts.
	 *
	 * Enables all interrupts in the underlying core.
	 */
	static inline void k1b_int_enable(void)
	{
		mOS_it_enable();
	}

	/**
	 * @brief Disables interrupts.
	 *
	 * Disables all interrupts in the underlying core.
	 */
	static inline void k1b_int_disable(void)
	{
		mOS_it_disable();
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
	 * @name Exported Functions
	 */
	/**@{*/
	#define __interrupts_disable
	#define __interrupts_enable
	#define __interrupt_set_handler
	#define __interrupt_mask
	#define __interrupt_unmask
	/**@}*/

	/**
	 * @brief Number of hardware interrupts in the Kalray MPPA-256 target.
	 */
	#define _INTERRUPTS_NUM K1B_NUM_INT

	/**
	 * @name Hardware Interrupts
	 */
	/**@{*/
	#define INTERRUPT_CLOCK K1B_INT_CLOCK0 /*< Programmable interrupt timer. */
	#define HAL_INT_CNOC    K1B_INT_CNOC   /*< Control NoC interrupt.        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_hwint_enable().
	 */
	static inline void interrupts_enable(void)
	{
		k1b_int_enable();
	}

	/**
	 * @see k1b_hwint_disable().
	 */
	static inline void interrupts_disable(void)
	{
		k1b_int_disable();
	}

	/**
	 * @see k1b_int_unmask().
	 */
	static inline int interrupt_unmask(int intnum)
	{
		return (k1b_int_unmask(intnum));
	}

	/**
	 * @see k1b_int_mask().
	 */
	static inline int interrupt_mask(int intnum)
	{
		return (k1b_int_mask(intnum));
	}

	/**
	 * @see k1b_int_ack()
	 */
	static inline void interrupt_ack(int intnum)
	{
		((void) intnum);
	}

	/**
	 * @see k1b_hwint_handler_set().
	 */
	static inline int interrupt_set_handler(int intnum, void (*handler)(int))
	{
		return (k1b_int_handler_set(intnum, handler));
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_INT_H_ */
