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
	#define K1B_INT_TIMER0     0 /**< Timer 0              */
	#define K1B_INT_TIMER1     1 /**< Timer 1              */
	#define K1B_INT_TIMER      2 /**< Watchdog Timer       */
	#define K1B_INT_CNOC       3 /**< Control NoC          */
	#define K1B_INT_DNOC       4 /**< Data NoC             */
	#define K1B_INT_DMA        5 /**< DMA                  */
	#define K1B_INT_NOC_ERR    6 /**< NoC Error            */
	#define K1B_INT_IPI        7 /**< IPI Line             */
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
	 * @brief Convert a valid interrupt number to the corresponding IRQ number.
	 *
	 * @returns Upon successful completion, interrupt number is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @author João Vicente Souto
	 */
	EXTERN int k1b_int_to_irq(int intnum);

	/**
	 * @brief Convert a valid IRQ number to the corresponding interrupt number.
	 *
	 * @returns Upon successful completion, interrupt number is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @author João Vicente Souto
	 */
	EXTERN int k1b_irq_to_int(int irqnum);

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
	#define INTERRUPTS_NUM         K1B_INT_NUM    /**< @ref K1b_INT_NUM    */
	#define INTERRUPT_TIMER        K1B_INT_TIMER0 /**< @ref K1B_INT_TIMER0 */
	#define INTERRUPT_IPI          K1B_INT_IPI    /**< @ref K1B_INT_IPI    */
	#define HAL_INT_CNOC           K1B_INT_CNOC   /**< @ref K1B_INT_CNOC   */
	#define INTERRUPT_LEVEL_LOW    K1B_IRQLVL_0   /**< @ref K1B_IRQLVL_0   */
	#define INTERRUPT_LEVEL_MEDIUM K1B_IRQLVL_6   /**< @ref K1B_IRQLVL_6   */
	#define INTERRUPT_LEVEL_HIGH   K1B_IRQLVL_10  /**< @ref K1B_IRQLVL_10  */
	#define INTERRUPT_LEVEL_NONE   K1B_IRQLVL_13  /**< @ref K1B_IRQLVL_13  */
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
	#define __interrupts_level_fn   /**< @ref interrupts_level()   */
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
	 * @see k1b_pic_lvl_set().
	 */
	static inline int interrupts_level(int newlevel)
	{
		return (k1b_pic_lvl_set(newlevel));
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
