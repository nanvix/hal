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

#include <arch/core/k1b/lpic.h>
#include <nanvix/const.h>
#include <mOS_vcore_u.h>
#include <errno.h>
#include <stdint.h>

/**
 * Current interrupt mask of the underlying k1b core.
 */
PUBLIC uint32_t currmask = K1B_IRQLVL_MASK_5;

/**
 * Current interrupt level of the underlying k1b core.
 */
PUBLIC int currlevel = K1B_IRQLVL_0;

/**
 * Lookup table for masks of interrupt levels.
 */
PUBLIC uint32_t intlvl_masks[K1B_NUM_IRQLVL] = {
	K1B_IRQLVL_MASK_0,
	K1B_IRQLVL_MASK_1,
	K1B_IRQLVL_MASK_2,
	K1B_IRQLVL_MASK_3,
	K1B_IRQLVL_MASK_4,
	K1B_IRQLVL_MASK_5,
	K1B_IRQLVL_MASK_6,
	K1B_IRQLVL_MASK_7,
	K1B_IRQLVL_MASK_8,
	K1B_IRQLVL_MASK_9,
	K1B_IRQLVL_MASK_10,
	K1B_IRQLVL_MASK_11,
	K1B_IRQLVL_MASK_12,
	K1B_IRQLVL_MASK_13,
	K1B_IRQLVL_MASK_14,
	K1B_IRQLVL_MASK_15
};

/**
 * Lookup table for interrupt request lines of hardware interrupts.
 */
PUBLIC k1b_irq_t k1b_irqs[K1B_NUM_IRQ] = {
	K1B_IRQ_0,
	K1B_IRQ_1,
	K1B_IRQ_2,
	K1B_IRQ_3,
	K1B_IRQ_4,
	K1B_IRQ_5,
	K1B_IRQ_6,
	K1B_IRQ_7,
	K1B_IRQ_8,
	K1B_IRQ_9,
#ifdef __k1io__
	K1B_IRQ_10,
	K1B_IRQ_11,
	K1B_IRQ_12
#endif
};

/**
 * @brief Sets the interrupt level of the underlying core.
 *
 * @param newlevel New interrupt level.
 *
 * @returns The old interrupt level.
 */
PUBLIC int k1b_pic_lvl_set(int newlevel)
{
	uint32_t mask;
	int oldlevel;

	mOS_set_it_level(mask = intlvl_masks[newlevel]);

	currmask = mask;
	oldlevel = currlevel;
	currlevel = newlevel;

	return (oldlevel);
}

/**
 *
 * The k1b_pic_mask() function masks the interrupt line in which
 * the interrupt @p irq is hooked up in the underlying k1b
 * core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_pic_mask(int irq)
{
	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= K1B_NUM_HWINT))
		return (-EINVAL);

	mOS_it_disable_num(k1b_irqs[irq]);

	return (0);
}

/**
 * The k1b_pic_unmask() function unmasks the interrupt line in which
 * the interrupt @p irq is hooked up in the underlying k1b
 * core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_pic_unmask(int irq)
{
	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= K1B_NUM_HWINT))
		return (-EINVAL);

	mOS_it_enable_num(k1b_irqs[irq]);

	return (0);
}

/**
 * The k1b_pic_setup() function initializes the programmble
 * interrupt controler of the k1b core. Upon completion, it drops
 * the interrupt level to the slowest ones, so that all interrupt
 * lines are enabled.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_pic_setup(void)
{
	k1b_pic_lvl_set(K1B_IRQLVL_0);
}