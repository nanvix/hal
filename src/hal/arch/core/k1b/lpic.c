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

/* Must come first. */
#define __NEED_CORE_LPIC

#include <arch/core/k1b/core.h>
#include <arch/core/k1b/lpic.h>
#include <arch/core/k1b/mOS.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>
#include <posix/stdint.h>

/**
 * @brief Gets the interrupt level of the underlying core.
 *
 * @returns The curent interrupt level.
 */
PUBLIC int k1b_pic_lvl_get(void)
{
	return (_scoreboard_start.SCB_VCORE.PER_CPU[core_get_id()].SFR_PS.il);
}

/**
 * @brief Sets the interrupt level of the underlying core.
 *
 * @param newlevel New interrupt level.
 *
 * @returns The old interrupt level.
 */
PUBLIC int k1b_pic_lvl_set(int newlevel)
{
	int oldlevel;

	/* Invalid interrupt level.  */
	if (UNLIKELY(newlevel < 0 || newlevel >= K1B_NUM_IRQLVL))
		return (-EINVAL);

	/* Gets old interrupt level. */
	oldlevel = k1b_pic_lvl_get();

	/* Sets new interrupt level. */
	mOS_set_it_level(newlevel);

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
