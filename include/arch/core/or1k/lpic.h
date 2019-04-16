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

#ifndef ARCH_CORE_OR1K_LPIC_H_
#define ARCH_CORE_OR1K_LPIC_H_

	#ifndef __NEED_CORE_LPIC
		#error "do not include this file"
	#endif

	/* Must come first. */
	#define __NEED_OR1K_REGS

/**
 * @addtogroup or1k-core-pic LPIC Chip
 * @ingroup or1k-core
 *
 * @brief Programmable Interrupt Controller
 */
/**@{*/

	#include <arch/core/or1k/regs.h>
	#include <nanvix/const.h>
	#include <errno.h>
	#include <stdint.h>

	/**
	 * @brief Number of Interrupt Request (IRQ) lines.
	 */
	#define OR1K_IRQ_NUM 3

	/**
	 * @name Interrupt Request (IRQ) lines.
	 */
	/**@{*/
	#define OR1K_IRQ_CLOCK   0 /*< Tmer.               */
	#define OR1K_IRQ_OMPIC   1 /*< OMLPIC.             */
	#define OR1K_IRQ_COM1    2 /*< COM1.               */
	#define OR1K_IRQ_EXT   256 /*< External interrupt. */
	/**@}*/

	/**
	 * @brief Number of interrupt levels.
	 */
	#define OR1K_NUM_IRQLVL 6

	/**
	 * @name Interrupt Levels
	 */
	/**@{*/
	#define OR1K_IRQLVL_5 5 /**< Level 5: all hardware interrupts enabled.         */
	#define OR1K_IRQLVL_4 4 /**< Level 4: all hardware interrupts enabled.         */
	#define OR1K_IRQLVL_3 3 /**< Level 3: clock, ompic, serial interrupts enabled. */
	#define OR1K_IRQLVL_2 2 /**< Level 2: clock, ompic interrupts enabled.         */
	#define OR1K_IRQLVL_1 1 /**< Level 1: clock interrupts enabled.                */
	#define OR1K_IRQLVL_0 0 /**< Level 0: all hardware interrupts disabled.        */
	/**@}*/

	/**
	 * @name Interrupt Levels Masks
	 */
	/**@{*/
	#define OR1K_IRQLVL_MASK_5 0x0006 /**< Mask for interrupt level 5.  */
	#define OR1K_IRQLVL_MASK_4 0x0006 /**< Mask for interrupt level 4.  */
	#define OR1K_IRQLVL_MASK_3 0x0006 /**< Mask for interrupt level 3.  */
	#define OR1K_IRQLVL_MASK_2 0x0002 /**< Mask for interrupt level 2.  */
	#define OR1K_IRQLVL_MASK_1 0x0001 /**< Mask for interrupt level 1.  */
	#define OR1K_IRQLVL_MASK_0 0x0000 /**< Mask for interrupt level 0.  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the LPIC.
	 */
	EXTERN void or1k_pic_setup(void);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int or1k_pic_mask(int irq);

	/**
	 * @brief Unmasks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int or1k_pic_unmask(int irq);

	/**
	 * @brief Acknowledges an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 */
	EXTERN void or1k_pic_ack(int irq);

	/**
	 * @brief Gets the next pending interrupt.
	 *
	 * @returns The number of the next pending interrupt, or zero if
	 * no interrupt is pending.
	 */
	EXTERN int or1k_pic_next(void);

	/**
	 * @brief Sets the interrupt level of the calling core.
	 *
	 * @param newlevel New interrupt level.
	 *
	 * @returns The old interrupt level.
	 */
	EXTERN int or1k_pic_lvl_set(int newlevel);

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_OR1K_LPIC_H_ */
