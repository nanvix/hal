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

#include <arch/core/rv32i/int.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <nanvix/hal/core/clock.h>
#include <errno.h>

/**
 * @brief Interrupt handlers.
 */
PRIVATE void (*handlers[RV32I_INT_NUM])(int) = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
};

/**
 * The do_int() function dispatches an interrupt request to a
 * previously-registered handler. If no function was previously
 * registered to handle the triggered hardware interrupt request, this
 * function returns immediately.
 *
 * @note This function is called from assembly code.
 */
PUBLIC void rv32i_do_int(int num, const struct context *ctx)
{
	void (*handler)(int);

	UNUSED(ctx);

	/* Unknown interrupt. */
	if (num >= RV32I_INT_NUM)
	{
		kprintf("[hal] unknown interrupt source %d", num);
		return;
	}

	/* Nothing to do. */
	if ((handler = handlers[num]) == NULL)
		return;

	handler(num);
}

/**
 * The rv32i_int_handler_set() function sets the function pointed to
 * by @p handler as the handler for the hardware interrupt whose
 * number is @p num.
 */
PUBLIC int rv32i_int_handler_set(int num, void (*handler)(int))
{
	/* Invalid interrupt number. */
	if ((num < 0) || (num >= RV32I_INT_NUM))
		return (-EINVAL);

	handlers[num] = handler;

	return (0);
}
