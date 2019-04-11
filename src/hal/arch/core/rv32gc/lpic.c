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

/* Must come first. */
#define __NEED_CORE_REGS
#define __NEED_CORE_LPIC

#include <arch/core/rv32gc/lpic.h>
#include <arch/core/rv32gc/regs.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * @brief Table of Interrupt Requests (IRQs).
 */
PRIVATE const int irqs[RV32GC_IRQ_NUM] = {
	RV32GC_SIE_USIE, /* User Software Interrupt Enable       */
	RV32GC_SIE_SSIE, /* Supervisor Interrupt Enable          */
	0,
	0,
	RV32GC_SIE_UTIE, /* User Timer Interrupt Enable          */
	RV32GC_SIE_STIE, /* Supervisor Timer Interrupt Enable    */
	0,
	0,
	RV32GC_SIE_UEUE, /* User External Interrupt Enable       */
	RV32GC_SIE_SEIE, /* Supervisor External Interrupt Enable */
	0,
	0
};

/**
 * @todo TODO we should provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32gc_pic_mask(int irq)
{
	rv32gc_word_t sie;

	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= RV32GC_IRQ_NUM))
		return (-EINVAL);

	asm volatile (
		"csrrc %0, sie, %1"
		: "=r"(sie)
		: "r"(irqs[irq])
	);

	return (0);
}

/**
 * @todo TODO we should provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32gc_pic_unmask(int irq)
{
	rv32gc_word_t sie;

	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= RV32GC_IRQ_NUM))
		return (-EINVAL);

	asm volatile (
		"csrrs %0, sie, %1"
		: "=r"(sie)
		: "r"(irqs[irq])
	);

	return (0);
}

/**
 * @todo TODO we should provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32gc_pic_ack(int irq)
{
	rv32gc_word_t sie;

	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= RV32GC_IRQ_NUM))
		return (-EINVAL);

	asm volatile (
		"csrrc %0, sip, %1"
		: "=r"(sie)
		: "r"(irqs[irq])
	);

	return (0);
}

/**
 * @todo TODO we should provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 *
 * @todo FIXME we should do this atomically.
 */
PUBLIC void rv32gc_pic_unmask_all(void)
{
	for (int i = 0; i < RV32GC_IRQ_NUM; i++)
		rv32gc_pic_unmask(i);
}

/**
 * @todo TODO we should provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 *
 * @todo FIXME we should do this atomically.
 */
PUBLIC void rv32gc_pic_mask_all(void)
{
	for (int i = 0; i < RV32GC_IRQ_NUM; i++)
		rv32gc_pic_unmask(i);
}
