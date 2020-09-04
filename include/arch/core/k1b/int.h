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

#ifndef ARCH_CORE_K1B_INT_H_
#define ARCH_CORE_K1B_INT_H_

	/* Must come first. */
	#define __NEED_CORE_LPIC
	#define __NEED_CORE_CONTEXT

/**
 * @addtogroup k1b-core-int Interrupts
 * @ingroup k1b-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <arch/core/k1b/lpic.h>
	#include <arch/core/k1b/ctx.h>
	#include <arch/core/k1b/mOS.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define K1B_INT_NUM K1B_IRQ_NUM

	/**
	 * @name Hardware Interrupts for Kalray MPPA-256 Target
	 */
	/**@{*/
	#define K1B_INT_TIMER0     K1B_IRQ_0 /**< Timer 0              */
	#define K1B_INT_TIMER1     K1B_IRQ_1 /**< Timer 1              */
	#define K1B_INT_TIMER      K1B_IRQ_2 /**< Watchdog Timer       */
	#define K1B_INT_CNOC       K1B_IRQ_3 /**< Control NoC          */
	#define K1B_INT_DNOC       K1B_IRQ_4 /**< Data NoC             */
	#define K1B_INT_DMA        K1B_IRQ_5 /**< DMA                  */
	#define K1B_INT_NOC_ERR    K1B_IRQ_6 /**< NoC Error            */
	#define K1B_INT_VIRT       K1B_IRQ_7 /**< Virtual Line         */
	#define K1B_INT_TIMER_ERR  K1B_IRQ_8 /**< Watchdog Timer Error */
	#define K1B_INT_DEBUG      K1B_IRQ_9 /**< Debug                */
	#ifdef __k1io__
	#define K1B_INT_GIC1      K1B_IRQ_10 /**< GIC 1                */
	#define K1B_INT_GIC2      K1B_IRQ_11 /**< GIC 2                */
	#define K1B_INT_GIC3      K1B_IRQ_12 /**< GIC2                 */
	#endif
	#define K1B_INT_IPI              256 /**< Dummy IPI interrupt  */
	/**@}*/

#ifndef _ASM_FILE_

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
	 * @brief Low-level interrupt dispatcher.
	 */
	EXTERN void _k1b_do_int(void);

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
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM  K1B_INT_NUM    /**< @ref K1b_INT_NUM    */
	#define INTERRUPT_TIMER K1B_INT_TIMER0 /**< @ref K1B_INT_TIMER0 */
	#define INTERRUPT_IPI   K1B_INT_IPI    /**< @ref K1B_INT_IPI    */
	#define HAL_INT_CNOC    K1B_INT_CNOC   /**< @ref K1B_INT_CNOC   */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __interrupt_handlers_var /**< @ref interrupt_handlers */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __interrupts_disable_fn /**< @ref interrupts_disable() */
	#define __interrupts_enable_fn  /**< @ref interrupts_enable()  */
	#define __interrupt_next_fn     /**< @ref interrupt_next()     */
	#define __interrupt_mask_fn     /**< @ref interrupt_mask()     */
	#define __interrupt_unmask_fn   /**< @ref interrupt_unmask()   */
	#define __interrupt_ack_fn      /**< @ref interrupt_ack()      */
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
	 * @see k1b_int_next()
	 */
	static inline int interrupt_next(void)
	{
		return (0);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_INT_H_ */
