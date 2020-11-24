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

#include <arch/core/or1k/lpic.h>
#include <nanvix/const.h>

/**
 * @brief Masks of interrupt levels.
 *
 * Lookup table for masks of interrupt levels.
 */
PRIVATE uint32_t intlvl_masks[OR1K_NUM_IRQLVL] = {
	OR1K_IRQLVL_MASK_0,
	OR1K_IRQLVL_MASK_1,
	OR1K_IRQLVL_MASK_2,
	OR1K_IRQLVL_MASK_3,
	OR1K_IRQLVL_MASK_4,
	OR1K_IRQLVL_MASK_5
};

/**
 * Current interrupt mask of the underlying or1k core.
 */
PRIVATE uint32_t currmask = OR1K_IRQLVL_MASK_5;

/**
 * Current interrupt level of the underlying or1k core.
 */
PRIVATE int currlevel = OR1K_IRQLVL_0;

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_pic_lvl_get(void)
{
	return (currlevel);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_pic_lvl_set(int newlevel)
{
	uint32_t mask;
	int oldlevel;

	mask = intlvl_masks[newlevel];
	or1k_mtspr(OR1K_SPR_PICMR, mask);

	/* Check if timer should be masked. */
	if (newlevel == OR1K_IRQLVL_0)
	{
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) & ~OR1K_SPR_SR_TEE);
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) & ~OR1K_SPR_SR_IEE);
	}
	else
	{
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) |  OR1K_SPR_SR_TEE);
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) |  OR1K_SPR_SR_IEE);
	}

	currmask = mask;
	oldlevel = currlevel;
	currlevel = newlevel;

	return (oldlevel);
}

/**
 * @brief Acknowledges an interrupt.
 *
 * @param irq Number of the target interrupt.
 */
PUBLIC void or1k_pic_ack(int irq)
{
	if (irq == OR1K_IRQ_TIMER)
		or1k_mtspr(OR1K_SPR_TTMR, or1k_mfspr(OR1K_SPR_TTMR) & ~OR1K_SPR_TTMR_IP);
	else
		or1k_mtspr(OR1K_SPR_PICSR, (1UL << irq));
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_pic_mask(int irq)
{
	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= OR1K_IRQ_NUM))
		return (-EINVAL);

	if (irq == OR1K_IRQ_TIMER)
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) & ~OR1K_SPR_SR_TEE);
	else
		or1k_mtspr(OR1K_SPR_PICMR, or1k_mfspr(OR1K_SPR_PICMR) & ~(1 << irq));

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_pic_unmask(int irq)
{
	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= OR1K_IRQ_NUM))
		return (-EINVAL);

	if (irq == OR1K_IRQ_TIMER)
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) | OR1K_SPR_SR_TEE);
	else
		or1k_mtspr(OR1K_SPR_PICMR, or1k_mfspr(OR1K_SPR_PICMR) | (1 << irq));

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_pic_next(void)
{
	unsigned picsr;
	int bit;

	bit   = 0;
	picsr = or1k_mfspr(OR1K_SPR_PICSR);

	while (!(picsr & 1) && bit < 32)
	{
		picsr >>= 1;
		bit++;
	}

	return ( (!picsr) ? 0 : bit );
}
