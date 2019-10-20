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

#define __NEED_CORE_LPIC

#include <arch/core/i486/lpic.h>
#include <arch/core/i486/pmio.h>
#include <nanvix/const.h>
#include <posix/errno.h>
#include <posix/stdint.h>

/**
 * @brief Masks of interrupt levels.
 *
 * Lookup table for masks of interrupt levels.
 */
PRIVATE uint16_t intlvl_masks[I486_NUM_IRQLVL] = {
	I486_IRQLVL_MASK_0,
	I486_IRQLVL_MASK_1,
	I486_IRQLVL_MASK_2,
	I486_IRQLVL_MASK_3,
	I486_IRQLVL_MASK_4,
	I486_IRQLVL_MASK_5,
};

/**
 * @brief Current interrupt level.
 *
 * Current interrupt level of the underlying i486 core.
 */
PRIVATE int currlevel = I486_IRQLVL_5;

/**
 * @brief Current interrupt mask.
 *
 * Current interrupt mask of the underlying i486 core.
 */
PRIVATE uint16_t currmask = I486_IRQLVL_MASK_5;

/*============================================================================*
 * i486_lpic_mask()                                                           *
 *============================================================================*/

/**
 * The i486_lpic_mask() function masks the interrupt request line in
 * which the interrupt @p irq is hooked up.
 */
PUBLIC int i486_lpic_mask(int irq)
{
	uint16_t port;
	uint8_t value;
	uint16_t newmask;

	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= I486_IRQ_NUM))
		return (-EINVAL);

	if (irq < 8)
	{
		port = I486_LPIC_DATA_MASTER;
		newmask = currmask | (1 << irq);
		value = newmask & 0xff;
	}
	else
	{
		port = I486_LPIC_DATA_SLAVE;
		newmask = currmask | (1 << irq);
		value = (newmask >> 8) & 0xff;
	}

	currmask = newmask;

	i486_output8(port, value);

	return (0);
}

/*============================================================================*
 * i486_lpic_unmask()                                                         *
 *============================================================================*/

/**
 * The i486_lpic_unmask() function unmasks the interrupt request line
 * in which the interrupt @p irq is hooked up.
 */
PUBLIC int i486_lpic_unmask(int irq)
{
	uint16_t port;
	uint8_t value;
	uint16_t newmask;

	/* Invalid interrupt number. */
	if ((irq < 0) || (irq >= I486_IRQ_NUM))
		return (-EINVAL);

	if (irq < 8)
	{
		port = I486_LPIC_DATA_MASTER;
		newmask = currmask & ~(1 << irq);
		value = newmask & 0xff;
	}
	else
	{
		port = I486_LPIC_DATA_SLAVE;
		newmask = currmask & ~(1 << irq);
		value = (newmask >> 8) & 0xff;
	}

	currmask = newmask;

	i486_output8(port, value);

	return (0);
}

/*============================================================================*
 * i486_lpic_lvl_set()                                                        *
 *============================================================================*/

/**
 * The i486_lpic_set() function sets the interrupt level of the calling
 * core to @p newlevel. The old interrupt level is returned.
 */
PUBLIC int i486_lpic_lvl_set(int newlevel)
{
	int oldlevel;
	uint16_t mask;

	mask = intlvl_masks[newlevel];

	i486_output8(I486_LPIC_DATA_MASTER, mask & 0xff);
	i486_output8(I486_LPIC_DATA_SLAVE, mask >> 8);

	currmask = mask;
	oldlevel = currlevel;
	currlevel = newlevel;

	return (oldlevel);
}

/*============================================================================*
 * i486_lpic_setup()                                                          *
 *============================================================================*/

/**
 *
 * The i486_lpic_setup() function initializes the programmble interrupt
 * controler of the i486 core. Upon completion, it drops the interrupt
 * level to the slowest ones, so that all interrupt lines are enabled.
 */
PUBLIC void i486_lpic_setup(uint8_t offset1, uint8_t offset2)
{
	/*
	 * Starts initialization sequence
	 * in cascade mode.
	 */
	i486_output8(
		I486_LPIC_CTRL_MASTER,
		I486_LPIC_ICW1_INIT | I486_LPIC_ICW1_ICW4
	);
	i486_iowait();
	i486_output8(
		I486_LPIC_CTRL_SLAVE,
		I486_LPIC_ICW1_INIT | I486_LPIC_ICW1_ICW4
	);
	i486_iowait();

	/* Send new vector offset. */
	i486_output8(I486_LPIC_DATA_MASTER, offset1);
	i486_iowait();
	i486_output8(I486_LPIC_DATA_SLAVE, offset2);
	i486_iowait();

	/*
	 * Tell the master that there is a slave
	 * LPIC hired up at IRQ line 2 and tell
	 * the slave LPIC that it is the second LPIC.
	 */
	i486_output8(I486_LPIC_DATA_MASTER, 0x04);
	i486_iowait();
	i486_output8(I486_LPIC_DATA_SLAVE, 0x02);
	i486_iowait();

	/* Set 8086 mode. */
	i486_output8(I486_LPIC_DATA_MASTER, I486_LPIC_ICW4_8086);
	i486_iowait();
	i486_output8(I486_LPIC_DATA_SLAVE, I486_LPIC_ICW4_8086);
	i486_iowait();

	/* Clears interrupt mask. */
	i486_lpic_lvl_set(I486_IRQLVL_0);
}
