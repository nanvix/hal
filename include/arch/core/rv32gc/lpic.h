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

#ifndef ARCH_CORE_RV32GC_LPIC_H_
#define ARCH_CORE_RV32GC_LPIC_H_

	#ifndef __NEED_CORE_LPIC
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32gc-core-pic LPIC Chip
 * @ingroup rv32gc-core
 *
 * @brief Programmable Interrupt Controller
 */
/**@{*/

	#include <nanvix/const.h>
	#include <posix/errno.h>
	#include <posix/stdint.h>

	/**
	 * @brief Number of Interrupt Request (IRQ) lines.
	 */
	#define RV32GC_IRQ_NUM 12

	/**
	 * @name Interrupt Request (IRQ) lines.
	 */
	/**@{*/
	#define RV32GC_IRQ_UEVENT   0 /**< User Event          */
	#define RV32GC_IRQ_SEVENT   1 /**< Supervisor Event    */
	#define RV32GC_IRQ_HEVENT   3 /**< Hypervisor Event    */
	#define RV32GC_IRQ_UTIMER   4 /**< User Timer          */
	#define RV32GC_IRQ_STIMER   5 /**< Supervisor Timer    */
	#define RV32GC_IRQ_HTIMER   7 /**< Hypervisor Timer    */
	#define RV32GC_IRQ_UEXTERN  8 /**< User External       */
	#define RV32GC_IRQ_SEXTERN  9 /**< Supervisor External */
	#define RV32GC_IRQ_HEXTERN 11 /**< Hypervisor External */
	#define RV32GC_IRQ_IPI    256 /**< Dummy IPI irq.      */
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
	EXTERN int rv32gc_pic_mask(int irq);

	/**
	 * @brief Unmasks an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_pic_unmask(int irq);

	/**
	 * @brief Acknowledges an interrupt.
	 *
	 * @param irq Number of the target interrupt.
	 */
	EXTERN int rv32gc_pic_ack(int irq);

	/**
	 * @brief Initializes the LPIC.
	 */
	static inline void rv32gc_pic_setup(void)
	{
		/* noop */
	}

	/**
	 * @brief Gets the next pending interrupt.
	 *
	 * @returns The number of the next pending interrupt, or zero if
	 * no interrupt is pending.
	 */
	static inline int rv32gc_pic_next(void)
	{
		return (0);
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32GC_LPIC_H_ */
