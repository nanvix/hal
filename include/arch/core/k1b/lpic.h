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

#ifndef ARCH_CORE_K1B_LPIC_H_
#define ARCH_CORE_K1B_LPIC_H_

	#ifndef __NEED_CORE_LPIC
		#error "do not include this file"
	#endif

/**
 * @addtogroup k1b-core-pic LPIC
 * @ingroup k1b-core
 *
 * @brief Local Programmble Interrupt Controller (LPIC)
 */
/**@{*/

	#include <arch/core/k1b/mOS.h>
	#include <nanvix/const.h>

	/**
	 * @brief Number of interrupt request lines.
	 */
	#ifdef __k1io__
		#define K1B_IRQ_NUM 13
	#else
		#define K1B_IRQ_NUM 10
	#endif

	/**
	 * @name Interrupt Request Lines
	 */
	/**@{*/
		#define K1B_IRQ_0 MOS_VC_IT_TIMER_0           /**< Timer 0           */
		#define K1B_IRQ_1 MOS_VC_IT_TIMER_1           /**< Timer 1           */
		#define K1B_IRQ_2 MOS_VC_IT_WATCHDOG          /**< Watchdog Timer    */
		#define K1B_IRQ_3 MOS_VC_IT_MAILBOX           /**< C-NoC Mailbox     */
		#define K1B_IRQ_4 MOS_VC_IT_DNOC_RX           /**< D-NoC RX          */
		#define K1B_IRQ_5 MOS_VC_IT_UCORE             /**< U-Core            */
		#define K1B_IRQ_6 MOS_VC_IT_NOCERR            /**< NoC Error         */
		#define K1B_IRQ_7 MOS_VC_IT_USER_0            /**< Virtual Line      */
		#define K1B_IRQ_8 MOS_VC_IT_WATCHDOG_OVERFLOW /**< Watchdog Overflow */
		#define K1B_IRQ_9 MOS_VC_IT_DEBUG             /**< Debug             */
	#ifdef __k1io__
		#define K1B_IRQ_10 MOS_VC_IT_GIC_1            /**< GIC 1             */
		#define K1B_IRQ_11 MOS_VC_IT_GIC_2            /**< GIC 2             */
		#define K1B_IRQ_12 MOS_VC_IT_GIC_3            /**< GIC 3             */
	#endif
	/**@}*/

	/**
	 * @brief Number of interrupt levels.
	 */
	#define K1B_NUM_IRQLVL 16

	/**
	 * @name Interrupt Levels
	 */
	/**@{*/
	#define K1B_IRQLVL_0   0 /**< Interrupt level 0.  */
	#define K1B_IRQLVL_1   1 /**< Interrupt level 1.  */
	#define K1B_IRQLVL_2   2 /**< Interrupt level 2.  */
	#define K1B_IRQLVL_3   3 /**< Interrupt level 3.  */
	#define K1B_IRQLVL_4   4 /**< Interrupt level 4.  */
	#define K1B_IRQLVL_5   5 /**< Interrupt level 5.  */
	#define K1B_IRQLVL_6   6 /**< Interrupt level 6.  */
	#define K1B_IRQLVL_7   7 /**< Interrupt level 7.  */
	#define K1B_IRQLVL_8   8 /**< Interrupt level 8.  */
	#define K1B_IRQLVL_9   9 /**< Interrupt level 9.  */
	#define K1B_IRQLVL_10 10 /**< Interrupt level 10. */
	#define K1B_IRQLVL_11 11 /**< Interrupt level 11. */
	#define K1B_IRQLVL_12 12 /**< Interrupt level 12. */
	#define K1B_IRQLVL_13 13 /**< Interrupt level 13. */
	#define K1B_IRQLVL_14 14 /**< Interrupt level 14. */
	#define K1B_IRQLVL_15 15 /**< Interrupt level 15. */
	/**@}*/

	/**
	 * @name Interrupt Levels Masks
	 */
	/**@{*/
	#define K1B_IRQLVL_MASK_0  0x0 /**< Mask for interrupt level 0.  */
	#define K1B_IRQLVL_MASK_1  0x1 /**< Mask for interrupt level 1.  */
	#define K1B_IRQLVL_MASK_2  0x2 /**< Mask for interrupt level 2.  */
	#define K1B_IRQLVL_MASK_3  0x3 /**< Mask for interrupt level 3.  */
	#define K1B_IRQLVL_MASK_4  0x4 /**< Mask for interrupt level 4.  */
	#define K1B_IRQLVL_MASK_5  0x5 /**< Mask for interrupt level 5.  */
	#define K1B_IRQLVL_MASK_6  0x6 /**< Mask for interrupt level 6.  */
	#define K1B_IRQLVL_MASK_7  0x7 /**< Mask for interrupt level 7.  */
	#define K1B_IRQLVL_MASK_8  0x8 /**< Mask for interrupt level 8.  */
	#define K1B_IRQLVL_MASK_9  0x9 /**< Mask for interrupt level 9.  */
	#define K1B_IRQLVL_MASK_10 0xa /**< Mask for interrupt level 10. */
	#define K1B_IRQLVL_MASK_11 0xb /**< Mask for interrupt level 11. */
	#define K1B_IRQLVL_MASK_12 0xc /**< Mask for interrupt level 12. */
	#define K1B_IRQLVL_MASK_13 0xd /**< Mask for interrupt level 13. */
	#define K1B_IRQLVL_MASK_14 0xe /**< Mask for interrupt level 14. */
	#define K1B_IRQLVL_MASK_15 0xf /**< Mask for interrupt level 15. */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_pic_mask(int irq);

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_pic_unmask(int irq);

	/**
	 * @brief Initializes the PIC.
	 */
	EXTERN void k1b_pic_setup(void);

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_K1B_LPIC_H_ */
