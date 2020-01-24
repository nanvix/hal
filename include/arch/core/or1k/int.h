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

#ifndef ARCH_CORE_OR1K_INT_H_
#define ARCH_CORE_OR1K_INT_H_

	/* Must come first. */
	#define __NEED_CORE_LPIC

/**
 * @addtogroup or1k-core-int Hardware Interrupts
 * @ingroup or1k-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <arch/core/or1k/core.h>
	#include <arch/core/or1k/lpic.h>
	#include <nanvix/const.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define OR1K_INT_NUM OR1K_IRQ_NUM

	/**
	 * @name Hardware Interrupts
	 */
	/**@{*/
	#define OR1K_INT_TIMER    OR1K_IRQ_TIMER /*< Tmer               */
	#define OR1K_INT_OMPIC    OR1K_IRQ_OMPIC /*< OMPIC              */
	#define OR1K_INT_COM1     OR1K_IRQ_COM1  /*< COM1               */
	#define OR1K_INT_ETHOC    OR1K_IRQ_ETHOC /*< Ethernet           */
	#define OR1K_INT_EXTERNAL OR1K_IRQ_EXT   /*< External Interrupt */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief System Call Hook
	 */
	EXTERN void kcall(void);

	/**
	 * @name Hardware Interrupt Hooks
	 */
	/**@{*/
	EXTERN void hwint0(void);
	EXTERN void hwint1(void);
	EXTERN void hwint2(void);
	/**@}*/

	/**
	 * @brief Calls the HAL do_interrupt function
	 * with the appropriate values.
	 */
	EXTERN void or1k_do_interrupt(int num);

	/**
	 * @brief Enables hardware interrupts.
	 *
	 * The or1k_int_enable() function enables all hardware interrupts
	 * in the underlying or1k core.
	 */
	static inline void or1k_int_enable(void)
	{
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) | OR1K_SPR_SR_IEE
			| OR1K_SPR_SR_TEE);
	}

	/**
	 * @brief Disables hardware interrupts.
	 *
	 * The or1k_int_disable() function disables all hardware
	 * interrupts in the underlying or1k core.
	 */
	static inline void or1k_int_disable(void)
	{
		 or1k_mtspr(
			OR1K_SPR_SR,
			or1k_mfspr(OR1K_SPR_SR) & ~(OR1K_SPR_SR_IEE | OR1K_SPR_SR_TEE)
		);
	}

	/**
	 * @brief Waits for an interrupt.
	 */
	static inline void or1k_int_wait(void)
	{
		if (or1k_mfspr(OR1K_SPR_UPR) & OR1K_SPR_UPR_PMP)
			or1k_mtspr(OR1K_SPR_PMR, OR1K_SPR_PMR_DME);
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM  OR1K_INT_NUM   /**< @ref OR1K_INT_NUM   */
	#define INTERRUPT_TIMER OR1K_INT_TIMER /**< @ref OR1K_INT_TIMER */
	#define INTERRUPT_IPI   OR1K_INT_OMPIC /**< @ref OR1K_INT_TIMER */
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
	 * @see or1k_int_enable().
	 */
	static inline void interrupts_enable(void)
	{
		or1k_int_enable();
	}

	/**
	 * @see or1k_int_disable().
	 */
	static inline void interrupts_disable(void)
	{
		or1k_int_disable();
	}

	/**
	 * @see or1k_pic_mask()
	 */
	static inline int interrupt_mask(int intnum)
	{
		return (or1k_pic_mask(intnum));
	}

	/**
	 * @see or1k_pic_unmask()
	 */
	static inline int interrupt_unmask(int intnum)
	{
		return (or1k_pic_unmask(intnum));
	}

	/**
	 * @see or1k_pic_ack()
	 */
	static inline void interrupt_ack(int intnum)
	{
		or1k_pic_ack(intnum);
	}

	/**
	 * @see or1k_pic_next().
	 */
	static inline int interrupt_next(void)
	{
		return (or1k_pic_next());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_INT_H_ */
