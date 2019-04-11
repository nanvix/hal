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

#ifndef ARCH_CORE_RV32I_INT_H_
#define ARCH_CORE_RV32I_INT_H_

	/* Must come first. */
	#define __NEED_CORE_LPIC
	#define __NEED_CORE_REGS
	#define __NEED_CORE_TYPES
	#define __NEED_CORE_CONTEXT

/**
 * @addtogroup rv32i-core-int Interrupts
 * @ingroup rv32i-core
 *
 * @brief Interrupt Interface
 */
/**@{*/

	#include <arch/core/rv32i/context.h>
	#include <arch/core/rv32i/lpic.h>
	#include <arch/core/rv32i/mcall.h>
	#include <arch/core/rv32i/regs.h>
	#include <arch/core/rv32i/types.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define RV32I_INT_NUM RV32I_IRQ_NUM

	/**
	 * @name Interrupts
	 */
	/**@{*/
	#define RV32I_INT_SWINT_USER      RV32I_IRQ_UEVENT  /**< User Software Interrupt       */
	#define RV32I_INT_SWINT_KERNEL    RV32I_IRQ_SEVENT  /**< Supervisor Software Interrupt */
	#define RV32I_INT_SWINT_MACHINE   RV32I_IRQ_HEVENT  /**< Machine Software Interrupt    */
	#define RV32I_INT_TIMER_USER      RV32I_IRQ_UTIMER  /**< User Timer Interrupt          */
	#define RV32I_INT_TIMER_KERNEL    RV32I_IRQ_STIMER  /**< Supervisor Timer Interrupt    */
	#define RV32I_INT_TIMER_MACHINE   RV32I_IRQ_HTIMER  /**< Machine Timer Interrupt       */
	#define RV32I_INT_EXTERN_USER     RV32I_IRQ_UEXTERN /**< User External Interrupt       */
	#define RV32I_INT_EXTERN_KERNEL   RV32I_IRQ_SEXTERN /**< Supervisor External Interrupt */
	#define RV32I_INT_EXTERN_MACHINE  RV32I_IRQ_HEXTERN /**< Machine External Interrupt    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Enables interrupts.
	 *
	 * The rv32i_int_enable() enables all interrupts in the underlying
	 * rv32i core.
	 */
	static inline void rv32i_int_enable(void)
	{
		rv32i_word_t sstatus;

		asm volatile (
			"csrrs %0, sstatus, %1"
				: "=r"(sstatus)
				: "r" (RV32I_SSTATUS_SIE)
		);

		rv32i_mcall_mint_enable();
	}

	/**
	 * @brief Disables interrupts.
	 *
	 * The rv32i_int_disable() disables all interrupts in the rv32i
	 * underlying core.
	 */
	static inline void rv32i_int_disable(void)
	{
		rv32i_word_t sstatus;

		rv32i_mcall_mint_disable();

		asm volatile (
			"csrrc %0, sstatus, %1"
				: "=r"(sstatus)
				: "r"
				(RV32I_SSTATUS_SIE)
		);
	}

	/**
	 * @brief Waits for an interrupt.
	 */
	static inline void rv32i_int_wait(void)
	{
		asm volatile ("wfi");
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM  RV32I_INT_NUM          /**< @ref RV32I_INT_NUM          */
	#define INTERRUPT_CLOCK RV32I_INT_TIMER_KERNEL /**< @ref RV32I_INT_TIMER_KERNEL */
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
	 * @see rv32i_int_enable().
	 */
	static inline void interrupts_enable(void)
	{
		rv32i_int_enable();
	}

	/**
	 * @see rv32i_int_disable().
	 */
	static inline void interrupts_disable(void)
	{
		rv32i_int_disable();
	}

	/**
	 * @see rv32i_int_mask().
	 */
	static inline int interrupt_mask(int irq)
	{
		return (rv32i_pic_mask(irq));
	}

	/**
	 * @see rv32i_int_unmask().
	 */
	static inline int interrupt_unmask(int irq)
	{
		return (rv32i_pic_unmask(irq));
	}

	/**
	 * @see rv32i_int_ack().
	 */
	static inline void interrupt_ack(int irq)
	{
		rv32i_pic_ack(irq);
	}

	/**
	 * @see rv32i_int_next().
	 */
	static inline int interrupt_next(void)
	{
		return (0);
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32I_INT_H_ */
