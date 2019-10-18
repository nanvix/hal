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

#ifndef ARCH_CORE_I486_LPIC_H_
#define ARCH_CORE_I486_LPIC_H_

	#ifndef __NEED_CORE_LPIC
		#error "do not include this file"
	#endif

/**
 * @addtogroup i486-core-pic LPIC
 * @ingroup i486-core
 *
 * @brief Local Programmable Interrupt Controller (LPIC)
 */
/**@{*/

	#include <arch/core/i486/pmio.h>
	#include <nanvix/const.h>
	#include <posix/stdint.h>

	/**
	 * @name Master LPIC Registers
	 */
	/**@{*/
	#define I486_LPIC_CTRL_MASTER 0x20 /**< Control */
	#define I486_LPIC_DATA_MASTER 0x21 /**< Data    */
	/**@}*/

	/**
	 * @name Slave Slave LPIC Registers
	 */
	/**@{*/
	#define I486_LPIC_CTRL_SLAVE 0xa0 /**< Control register. */
	#define I486_LPIC_DATA_SLAVE 0xa1 /**< Data register.    */
	/**@}*/

	/**
	 * @name Commands Codes
	 */
	/**@{*/
	#define I486_LPIC_EOI             0x20 /**< End of Interrupt            */
	#define I486_LPIC_ICW1_ICW4       0x01 /**< ICW4 (not) needed           */
	#define I486_LPIC_ICW1_SINGLE     0x02 /**< Single (Cascade) mode       */
	#define I486_LPIC_ICW1_INTERVAL4  0x04 /**< Call Address Interval 4 (8) */
	#define I486_LPIC_ICW1_LEVEL      0x08 /**< Level triggered (edge) mode */
	#define I486_LPIC_ICW1_INIT       0x10 /**< Initialization Required     */
	#define I486_LPIC_ICW4_8086       0x01 /**< 8086/88 (MCS-80/85) Mode    */
	#define I486_LPIC_ICW4_AUTO       0x02 /**< Auto EOI                    */
	#define I486_LPIC_ICW4_BUF_SLAVE  0x08 /**< Buffered Mode/Slave         */
	#define I486_LPIC_ICW4_BUF_MASTER 0x0C /**< Buffered Mode/Master        */
	#define I486_LPIC_ICW4_SFNM       0x10 /**< Special Fully Nested (not)  */
	/**@}*/

	/**
	 * @brief Number of interrupt lines.
	 */
	#define I486_IRQ_NUM 16

	/**
	 * @brief Number of interrupt levels.
	 */
	#define I486_NUM_IRQLVL 6

	/**
	 * @name Interrupt Levels
	 */
	/**@{*/
	#define I486_IRQLVL_5 5 /**< Level 0: all hardware interrupts disabled. */
	#define I486_IRQLVL_4 4 /**< Level 1: timer interrupts enabled.         */
	#define I486_IRQLVL_3 3 /**< Level 2: disk interrupts enabled.          */
	#define I486_IRQLVL_2 2 /**< Level 3: network interrupts enabled        */
	#define I486_IRQLVL_1 1 /**< Level 4: terminal interrupts enabled.      */
	#define I486_IRQLVL_0 0 /**< Level 5: all hardware interrupts enabled.  */
	/**@}*/

	/**
	 * @name Interrupt Levels Masks
	 */
	/**@{*/
	#define I486_IRQLVL_MASK_5 0xfffb /**< Mask for interrupt level 5.  */
	#define I486_IRQLVL_MASK_4 0xfefa /**< Mask for interrupt level 4.  */
	#define I486_IRQLVL_MASK_3 0x3eba /**< Mask for interrupt level 3.  */
	#define I486_IRQLVL_MASK_2 0x30ba /**< Mask for interrupt level 2.  */
	#define I486_IRQLVL_MASK_1 0x2000 /**< Mask for interrupt level 1.  */
	#define I486_IRQLVL_MASK_0 0x0000 /**< Mask for interrupt level 0.  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the LPIC.
	 *
	 * @param offset1 Vector offset for master PIC.
	 * @param offset2 Vector offset for slave PIC.
	 */
	EXTERN void i486_lpic_setup(uint8_t offset1, uint8_t offset2);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int i486_lpic_mask(int irq);

	/**
	 * @brief Unmasks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int i486_lpic_unmask(int irq);

	/**
	 * @brief Sets the interrupt level of the calling core.
	 *
	 * @param newlevel New interrupt level.
	 *
	 * @returns The old interrupt level.
	 */
	EXTERN int i486_lpic_lvl_set(int newlevel);

	/**
	 * @brief Acknowledges an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 */
	static inline void i486_lpic_ack(int irq)
	{
		if (irq >= 8)
			i486_output8(I486_LPIC_CTRL_SLAVE, I486_LPIC_EOI);

		i486_output8(I486_LPIC_CTRL_MASTER,I486_LPIC_EOI);
	}

	/**
	 * @brief Gets the next pending interrupt.
	 *
	 * @returns The number of the next pending interrupt, or zero if
	 * no interrupt is pending.
	 */
	static inline int i486_lpic_next(void)
	{
		return (0);
	}


#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_I486_LPIC_H_ */

