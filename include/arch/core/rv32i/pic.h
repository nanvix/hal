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

#ifndef ARCH_CORE_RV32I_PIC_H_
#define ARCH_CORE_RV32I_PIC_H_

	#ifndef __NEED_CORE_PIC
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32i-core-pic PIC
 * @ingroup rv32i-core
 *
 * @brief Programmble Interrupt Controller
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CORE_REGS
	#define __NEED_CORE_TYPES

	#include <arch/core/rv32i/regs.h>
	#include <arch/core/rv32i/types.h>
	#include <nanvix/cc.h>
	#include <errno.h>

	/**
	 * @name PIC Memory Map
	 */
	/**@{*/
	#define PIC_BASE            0x02000000 /** Base Address                   */
	#define PIC_MSIP            0x00000000 /**< Offset for msip Register      */
	#define PIC_MTIMECMP_OFFSET 0x00004000 /**< Offset for mtimecmp Register  */
	#define PIC_MTIME_OFFSET    0x0000bff8 /**< Offset for mtime Register     */
	/**@}*/

	/**
	 * @brief Number of interrupt requests (IRQs).
	 */
	#define RV32I_IRQ_NUM 12

	/**
	 * Table of Interrupt Requests (IRQs).
	 */
	extern const int irqs[RV32I_IRQ_NUM];

	/**
	 * @brief Asserts a reserved interrupt number.
	 *
	 * @param irqnum Number of target interrupt request.
	 *
	 * @returns One if @p irqnum refers to a reserved interrupt
	 * request line, and non-zero otherwise.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int rv32i_pic_is_reserved(int irqnum)
	{
		return (
			(irqnum == RV32I_MIE_R0) ||
			(irqnum == RV32I_MIE_R1) ||
			(irqnum == RV32I_MIE_R2)
		);
	}

	/**
	 * @brief Unmasks an interrupt.
	 *
	 * @param irqnum Number of target interrupt request.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int rv32i_pic_unmask(int irqnum)
	{
		rv32i_word_t mie;

		/* Invalid interrupt number. */
		if ((irqnum < 0) || (irqnum >= RV32I_IRQ_NUM))
			return (-EINVAL);

		/* Unsupported interrupt number. */
		if (rv32i_pic_is_reserved(irqnum))
			return (-ENOTSUP);

		asm __volatile__ (
			"csrrs %0, mie, %1"
			: "=r"(mie)
			: "r"(irqs[irqnum])
		);

		return (0);
	}

	/**
	 * @brief Masks an interrupt.
	 *
	 * @param irqnum Number of the target interrupt request.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int rv32i_pic_mask(int irqnum)
	{
		rv32i_word_t mie;

		/* Invalid interrupt number. */
		if ((irqnum < 0) || (irqnum >= RV32I_IRQ_NUM))
			return (-EINVAL);

		/* Unsupported interrupt number. */
		if (rv32i_pic_is_reserved(irqnum))
			return (-ENOTSUP);

		asm __volatile__ (
			"csrrc %0, mie, %1"
			: "=r"(mie)
			: "r"(irqs[irqnum])
		);

		return (0);
	}

	/**
	 * @brief Unmasks all interrupts.
	 *
	 * the rv32i_pic_unmask_all() unmasks all interrupt request lines
	 * in the underlying rv32i core.
	 *
	 * @author Pedro Henrique Penna
	 *
	 * @todo FIXME we should do this atomically.
	 */
	static inline void rv32i_pic_unmask_all(void)
	{
		for (int i = 0; i < RV32I_IRQ_NUM; i++)
			rv32i_pic_unmask(i);
	}

	/**
	 * @brief Masks all interrupts.
	 *
	 * the rv32i_pic_mask_all() masks all interrupts request lines in
	 * the underlying rv32i core.
	 *
	 * @author Pedro Henrique Penna
	 *
	 * @todo FIXME we should do this atomically.
	 */
	static inline void rv32i_pic_mask_all(void)
	{
		for (int i = 0; i < RV32I_IRQ_NUM; i++)
			rv32i_pic_unmask(i);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __interrupt_mask   /**< interrupt_mask()   */
	#define __interrupt_unmask /**< interrupt_unmask() */
	/**@}*/

	/**
	 * @see rv32i_pic_mask().
	 */
	static inline int interrupt_mask(int irqnum)
	{
		return (rv32i_pic_mask(irqnum));
	}

	/**
	 * @see rv32i_pic_unmask().
	 */
	static inline int interrupt_unmask(int irqnum)
	{
		return (rv32i_pic_unmask(irqnum));
	}

/**@endcond*/

#endif /* ARCH_CORE_RV32I_PIC_H_  */
