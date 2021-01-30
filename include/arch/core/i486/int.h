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

#ifndef ARCH_CORE_I486_INT_H_
#define ARCH_CORE_I486_INT_H_

	/* Must come first. */
	#define __NEED_CORE_LPIC

/**
 * @addtogroup i486-core-int Hardware Interrupts
 * @ingroup i486-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <arch/core/i486/lpic.h>
	#include <nanvix/const.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define I486_INT_NUM I486_IRQ_NUM

	/**
	 * @name Hardware Interrupts for the IBM PC Target
	 */
	/**@{*/
	#define I486_INT_TIMER    0 /*< Programmable interrupt timer.              */
	#define I486_INT_KEYBOARD 1 /*< Keyboard.                                  */
	#define I486_INT_COM2     3 /*< COM2.                                      */
	#define I486_INT_COM1     4 /*< COM1.                                      */
	#define I486_INT_LPT2     5 /*< LPT2.                                      */
	#define I486_INT_FLOPPY   6 /*< Floppy disk.                               */
	#define I486_INT_LPT1     7 /*< LPT1.                                      */
	#define I486_INT_CMOS     8 /*< CMOS real-time timer.                      */
	#define I486_INT_SCSI1    9 /*< Free for peripherals (legacy SCSI or NIC). */
	#define I486_INT_SCSI2   10 /*< Free for peripherals (legacy SCSI or NIC). */
	#define I486_INT_SCSI3   11 /*< Free for peripherals (legacy SCSI or NIC). */
	#define I486_INT_MOUSE   12 /*< PS2 mouse.                                 */
	#define I486_INT_COPROC  13 /*< FPU, coprocessor or inter-processor.       */
	#define I486_INT_ATA1    14 /*< Primary ATA hard disk.                     */
	#define I486_INT_ATA2    15 /*< Secondary ATA hard disk.                   */
	#define I486_INT_IPI    256 /*< Dummy IPI interrupt.                       */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Hardware Interrupt Hooks
	 */
	/**@{*/
	EXTERN void _do_hwint0(void);
	EXTERN void _do_hwint1(void);
	EXTERN void _do_hwint2(void);
	EXTERN void _do_hwint3(void);
	EXTERN void _do_hwint4(void);
	EXTERN void _do_hwint5(void);
	EXTERN void _do_hwint6(void);
	EXTERN void _do_hwint7(void);
	EXTERN void _do_hwint8(void);
	EXTERN void _do_hwint9(void);
	EXTERN void _do_hwint10(void);
	EXTERN void _do_hwint11(void);
	EXTERN void _do_hwint12(void);
	EXTERN void _do_hwint13(void);
	EXTERN void _do_hwint14(void);
	EXTERN void _do_hwint15(void);
	/**@}*/

	/**
	 * @brief Enables hardware interrupts.
	 *
	 * The i486_hwint_enable() function enables all hardware interrupts in the
	 * underlying i486 core.
	 */
	static inline void i486_int_enable(void)
	{
		 asm("sti");
	}

	/**
	 * @brief Disables hardware interrupts.
	 *
	 * The i486_hwint_disable() function disables all hardware interrupts in the
	 * underlying i486 core.
	 */
	static inline void i486_int_disable(void)
	{
		 asm("cli");
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM         I486_INT_NUM   /**< @ref I486_INT_NUM   */
	#define INTERRUPT_TIMER        I486_INT_TIMER /**< @ref I486_INT_TIMER */
	#define INTERRUPT_IPI          I486_INT_IPI   /**< @ref I486_INT_IPI   */
	#define INTERRUPT_LEVEL_LOW    I486_IRQLVL_0  /**< @ref I486_IRQLVL_0  */
	#define INTERRUPT_LEVEL_MEDIUM I486_IRQLVL_2  /**< @ref I486_IRQLVL_2  */
	#define INTERRUPT_LEVEL_HIGH   I486_IRQLVL_4  /**< @ref I486_IRQLVL_4  */
	#define INTERRUPT_LEVEL_NONE   I486_IRQLVL_5  /**< @ref I486_IRQLVL_5  */
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
	#define __interrupts_disable_fn   /**< @ref interrupts_disable()   */
	#define __interrupts_enable_fn    /**< @ref interrupts_enable()    */
	#define __interrupts_get_level_fn /**< @ref interrupts_get_level() */
	#define __interrupts_set_level_fn /**< @ref interrupts_set_level() */
	#define __interrupt_next_fn       /**< @ref interrupt_next()       */
	#define __interrupt_mask_fn       /**< @ref interrupt_mask()       */
	#define __interrupt_unmask_fn     /**< @ref interrupt_unmask()     */
	#define __interrupt_ack_fn        /**< @ref interrupt_ack()        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see i486_int_enable()
	 */
	static inline void interrupts_enable(void)
	{
		i486_int_enable();
	}

	/**
	 * @see i486_int_disable()
	 */
	static inline void interrupts_disable(void)
	{
		i486_int_disable();
	}

	/**
	 * @see i486_lpic_lvl_get().
	 */
	static inline int interrupts_get_level(void)
	{
		return (i486_lpic_lvl_get());
	}

	/**
	 * @see i486_lpic_lvl_set().
	 */
	static inline int interrupts_set_level(int newlevel)
	{
		return (i486_lpic_lvl_set(newlevel));
	}

	/**
	 * @see i486_lpic_mask().
	 */
	static inline int interrupt_mask(int intnum)
	{
		return (i486_lpic_mask(intnum));
	}

	/**
	 * @see i486_lpic_unmask().
	 */
	static inline int interrupt_unmask(int intnum)
	{
		return (i486_lpic_unmask(intnum));
	}

	/**
	 * @see i486_lpic_ack().
	 */
	static inline void interrupt_ack(int intnum)
	{
		i486_lpic_ack(intnum);
	}

	/**
	 * @see i486_lpic_next().
	 */
	static inline int interrupt_next(void)
	{
		return (i486_lpic_next());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_INT_H_ */
