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

#include <nanvix/const.h>
#include <arch/core/i486/8259.h>
#include <arch/core/i486/pmio.h>
#include <errno.h>
#include <stdint.h>

/**
 * @brief Masks of interrupt levels.
 *
 * Lookup table for masks of interrupt levels.
 */
PRIVATE uint16_t intlvl_masks[I486_NUM_INTLVL] = {
	I486_INTLVL_MASK_0,
	I486_INTLVL_MASK_1,
	I486_INTLVL_MASK_2,
	I486_INTLVL_MASK_3,
	I486_INTLVL_MASK_4,
	I486_INTLVL_MASK_5,
};

/**
 * @brief Current interrupt level.
 *
 * Current interrupt level of the underlying i486 core.
 */
PRIVATE int currlevel = I486_INTLVL_5;

/**
 * @brief Current interrupt mask.
 *
 * Current interrupt mask of the underlying i486 core.
 */
PRIVATE uint16_t currmask = I486_INTLVL_MASK_5;

/*============================================================================*
 * i486_pic_mask()                                                            *
 *============================================================================*/

/**
 * The i486_pic_mask() function masks the interrupt request line in
 * which the interrupt @p intnum is hooked up.
 */
PUBLIC int i486_pic_mask(int intnum)
{
	uint16_t port;
	uint8_t value;
	uint16_t newmask;

	/* Invalid interrupt number. */
	if ((intnum < 0) || (intnum >= I486_NUM_HWINT))
		return (-EINVAL);

	if (intnum < 8)
	{
		port = PIC_DATA_MASTER;
		newmask = currmask | (1 << intnum);
		value = newmask & 0xff;
	}
	else
	{
		port = PIC_DATA_SLAVE;
		newmask = currmask | (1 << intnum);
		value = (newmask >> 8) & 0xff;
	}

	currmask = newmask;

	i486_output8(port, value);

	return (0);
}

/*============================================================================*
 * i486_pic_unmask()                                                          *
 *============================================================================*/

/**
 * The i486_pic_unmask() function unmasks the interrupt request line
 * in which the interrupt @p intnum is hooked up.
 */
PUBLIC int i486_pic_unmask(int intnum)
{
	uint16_t port;
	uint8_t value;
	uint16_t newmask;

	/* Invalid interrupt number. */
	if ((intnum < 0) || (intnum >= I486_NUM_HWINT))
		return (-EINVAL);

	if (intnum < 8)
	{
		port = PIC_DATA_MASTER;
		newmask = currmask & ~(1 << intnum);
		value = newmask & 0xff;
	}
	else
	{
		port = PIC_DATA_SLAVE;
		newmask = currmask & ~(1 << intnum);
		value = (newmask >> 8) & 0xff;
	}

	currmask = newmask;

	i486_output8(port, value);

	return (0);
}

/*============================================================================*
 * i486_pic_lvl_set()                                                         *
 *============================================================================*/

/**
 * The i486_pic_set() function sets the interrupt level of the calling
 * core to @p newlevel. The old interrupt level is returned.
 */
PUBLIC int i486_pic_lvl_set(int newlevel)
{
	int oldlevel;
	uint16_t mask;

	mask = intlvl_masks[newlevel];

	i486_output8(PIC_DATA_MASTER, mask & 0xff);
	i486_output8(PIC_DATA_SLAVE, mask >> 8);

	currmask = mask;
	oldlevel = currlevel;
	currlevel = newlevel;

	return (oldlevel);
}

/*============================================================================*
 * i486_pic_setup()                                                           *
 *============================================================================*/

/**
 *
 * The i486_pic_setup() function initializes the programmble interrupt
 * controler of the i486 core. Upon completion, it drops the interrupt
 * level to the slowest ones, so that all interrupt lines are enabled.
 */
PUBLIC void i486_pic_setup(uint8_t offset1, uint8_t offset2)
{
	/*
	 * Starts initialization sequence
	 * in cascade mode.
	 */
	i486_output8(PIC_CTRL_MASTER, 0x11);
	i486_iowait();
	i486_output8(PIC_CTRL_SLAVE, 0x11);
	i486_iowait();

	/* Send new vector offset. */
	i486_output8(PIC_DATA_MASTER, offset1);
	i486_iowait();
	i486_output8(PIC_DATA_SLAVE, offset2);
	i486_iowait();

	/*
	 * Tell the master that there is a slave
	 * PIC hired up at IRQ line 2 and tell
	 * the slave PIC that it is the second PIC.
	 */
	i486_output8(PIC_DATA_MASTER, 0x04);
	i486_iowait();
	i486_output8(PIC_DATA_SLAVE, 0x02);
	i486_iowait();

	/* Set 8086 mode. */
	i486_output8(PIC_DATA_MASTER, 0x01);
	i486_iowait();
	i486_output8(PIC_DATA_SLAVE, 0x01);
	i486_iowait();

	/* Clears interrupt mask. */
	i486_pic_lvl_set(I486_INTLVL_0);
}
