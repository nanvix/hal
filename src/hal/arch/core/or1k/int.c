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

#include <nanvix/hal/core/interrupt.h>
#include <arch/core/or1k/int.h>
#include <arch/core/or1k/core.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/**
 * @brief Interrupt handlers.
 */
PUBLIC void (*interrupt_handlers[OR1K_INT_NUM])(int) = {
	NULL, NULL, NULL, NULL, NULL
};

/**
 * @brief Receives an interrupt number, checks if belongs
 * to a timer interrupt or a external device and
 * call the generic handler with the appropriate
 * interrupt number.
 *
 * @param num Interrupt type identifier.
 */
PUBLIC void or1k_do_interrupt(int num)
{
	int interrupt; /* Interrupt to be served. */

	if (num == OR1K_INT_EXTERNAL)
		interrupt = or1k_pic_next();
	else
		interrupt = num;

	do_interrupt(interrupt);
}
