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

/**
 * @addtogroup rv32i-core-int Interrupts
 * @ingroup rv32i-core
 *
 * @brief Interrupt Interface
 */
/**@{*/

	/* Must come first. */
	#define __NEED_IVT
	#define __NEED_CONTEXT
	#define __NEED_CORE_PIC
	#define __NEED_CORE_REGS
	#define __NEED_CORE_TYPES

	#include <arch/core/rv32i/context.h>
	#include <arch/core/rv32i/ivt.h>
	#include <arch/core/rv32i/mcall.h>
	#include <arch/core/rv32i/pic.h>
	#include <arch/core/rv32i/regs.h>
	#include <arch/core/rv32i/types.h>

	/**
	 * @name Interrupts
	 */
	/**@{*/
	#define RV32I_INT_SWINT_USER       0 /**< User Software Interrupt       */
	#define RV32I_INT_SWINT_KERNEL     1 /**< Supervisor Software Interrupt */
	#define RV32I_INT_SWINT_RESERVED   2 /**< Reserved                      */
	#define RV32I_INT_SWINT_MACHINE    3 /**< Machine Software Interrupt    */
	#define RV32I_INT_TIMER_USER       4 /**< User Timer Interrupt          */
	#define RV32I_INT_TIMER_KERNEL     5 /**< Supervisor Timer Interrupt    */
	#define RV32I_INT_TIMER_RESERVED   6 /**< Reserved                      */
	#define RV32I_INT_TIMER_MACHINE    7 /**< Machine Timer Interrupt       */
	#define RV32I_INT_EXTERN_USER      8 /**< User External Interrupt       */
	#define RV32I_INT_EXTERN_KERNEL    9 /**< Supervisor External Interrupt */
	#define RV32I_INT_EXTERN_RESERVED 10 /**< Reserved                      */
	#define RV32I_INT_EXTERN_MACHINE  11 /**< Machine External Interrupt    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Sets a handler for a interrupt.
	 *
	 * @param num     Number of the target interrupt.
	 * @param handler Hardware interrupt handler.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	extern int rv32i_int_handler_set(int num, void (*handler)(int));

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
	 * Disables all interrupts in the underlying core.
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
	#define _INTERRUPTS_NUM RV32I_INT_NUM          /**< @ref RV32I_INT_NUM          */
	#define INTERRUPT_CLOCK RV32I_INT_TIMER_KERNEL /**< @ref RV32I_INT_TIMER_KERNEL */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __interrupts_disable    /**< interrupts_disable()    */
	#define __interrupts_enable     /**< interrupts_enable()     */
	#define __interrupt_set_handler /**< interrupt_set_handler() */
	#define __interrupt_mask        /**< interrupt_mask()        */
	#define __interrupt_unmask      /**< interrupt_unmask()      */
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
	 * @see rv32i_int_handler_set().
	 */
	static inline int interrupt_set_handler(int num, void (*handler)(int))
	{
		return (rv32i_int_handler_set(num, handler));
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32I_INT_H_ */
