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
#define __NEED_CORE_LPIC

#include <arch/core/k1b/lpic.h>
#include <arch/core/k1b/mOS.h>
#include <nanvix/const.h>
#include <errno.h>
#include <stdint.h>

/**
 * @brief Current interrupt mask.
 */
PRIVATE uint32_t currmask = K1B_IRQLVL_MASK_5;

/**
 * @brief Current interrupt level.
 */
PRIVATE int currlevel = K1B_IRQLVL_0;

/**
 * @brief Lookup table for masks of interrupt levels.
 */
PRIVATE uint32_t intlvl_masks[K1B_NUM_IRQLVL] = {
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
 * The k1b_pic_mask() function masks the interrupt line in which
 * the interrupt @p irq is hooked up in the underlying k1b
 * core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_pic_mask(int irq)
{
	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= K1B_IRQ_NUM))
		return (-EINVAL);

	mOS_it_disable_num(irq);

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
	if ((irq < 0) || (irq >= K1B_IRQ_NUM))
		return (-EINVAL);

	mOS_it_enable_num(irq);

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
