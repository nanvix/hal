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

#ifndef ARCH_CORE_OR1K_INT_H_
#define ARCH_CORE_OR1K_INT_H_

	/* Must come first. */
	#define __NEED_OR1K_PIC

/**
 * @addtogroup or1k-core-int Hardware Interrupts
 * @ingroup or1k-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <arch/core/or1k/core.h>
	#include <arch/core/or1k/pic.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief System Call Hook
	 */
	EXTERN void syscall(void);

	/**
	 * @name Hardware Interrupt Hooks
	 */
	/**@{*/
	EXTERN void hwint0(void);
	EXTERN void hwint1(void);
	EXTERN void hwint2(void);
	/**@}*/

	/**
	 * @brief Gets the next pending interrupt.
	 *
	 * @returns The number of the next pending interrupt, or zero if
	 * no interrupt is pending.
	 */
	EXTERN int or1k_int_next(void);

	/**
	 * @brief Enables hardware interrupts.
	 *
	 * The or1k_sti() function enables all hardware interrupts in the
	 * underlying or1k core.
	 */
	static inline void or1k_int_enable(void)
	{
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) | OR1K_SPR_SR_IEE
			| OR1K_SPR_SR_TEE);
	}

	/**
	 * @brief Disables hardware interrupts.
	 *
	 * The or1k_cli() function disables all hardware interrupts in the
	 * underlying or1k core.
	 */
	static inline void or1k_int_disable(void)
	{
		 or1k_mtspr(
			OR1K_SPR_SR,
			or1k_mfspr(OR1K_SPR_SR) & ~(OR1K_SPR_SR_IEE | OR1K_SPR_SR_TEE)
		);
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
	#define INTERRUPT_CLOCK OR1K_INT_CLOCK /**< @ref OR1K_INT_CLOCK */
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
	 * @see or1k_int_next().
	 */
	static inline int interrupt_next(void)
	{
		return (or1k_int_next());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_INT_H_ */
