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

#include <arch/core/k1b/cache.h>
#include <arch/core/k1b/context.h>
#include <arch/core/k1b/int.h>
#include <arch/core/k1b/ivt.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * Lookup table for interrupt request lines of hardware interrupts.
 */
PRIVATE k1b_irq_t k1b_irqs[K1B_NUM_INT] = {
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
 * @brief Interrupt handlers.
 */
PRIVATE void (*k1b_handlers[K1B_NUM_INT])(int) = {
	NULL, NULL, NULL,
	NULL, NULL, NULL,
	NULL, NULL, NULL,
#ifdef __k1io__
	NULL, NULL, NULL
#endif
};

/**
 * The k1b_do_hwint() function dispatches a hardware interrupt that
 * was triggered to a specific hardware interrupt handler. The ID of
 * hardware interrupt is used to index the table of hardware interrupt
 * handlers, and the context and the interrupted context pointed to by
 * ctx is currently unsed.
 */
PUBLIC void k1b_do_hwint(k1b_hwint_id_t hwintid, struct context *ctx)
{
	int num = 0;

	UNUSED(ctx);

	/* Get interrupt number. */
	for (int i = 0; i < K1B_NUM_INT; i++)
	{
		if (hwints[i] == hwintid)
		{
			num = i;
			goto found;
		}
	}

	return;

found:

	k1b_handlers[num](num);
}

/**
 * The k1b_hwint_handler_set() function sets the function pointed to
 * by @p handler as the handler for the hardware interrupt whose
 * number is @p num.
 */
PUBLIC int k1b_hwint_handler_set(int num, void (*handler)(int))
{
	/* Invalid interrupt number. */
	if ((num < 0) || (num >= K1B_NUM_INT))
		return (-EINVAL);

	k1b_handlers[num] = handler;
	k1b_dcache_inval();

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_int_mask(int intnum)
{
	/* Invalid interrupt number. */
	if ((intnum < 0) || (intnum >= K1B_NUM_INT))
		return (-EINVAL);

	k1b_pic_mask(k1b_irqs[intnum]);

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_int_unmask(int intnum)
{
	/* Invalid interrupt number. */
	if ((intnum < 0) || (intnum >= K1B_NUM_INT))
		return (-EINVAL);

	k1b_pic_unmask(k1b_irqs[intnum]);

	return (0);
}
