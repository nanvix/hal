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
#define __NEED_HAL_CLUSTER

#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <posix/errno.h>

/**
 * @brief Timer handler.
 */
PRIVATE interrupt_handler_t timer_handler = NULL;

/**
 * @brief Event handler.
 */
PRIVATE interrupt_handler_t event_handler = NULL;

/**
 * @brief Number of spurious interrupts.
 */
PRIVATE unsigned spurious = 0;

/**
 * @brief Timer value for archs that lacks RTC.
 */
PUBLIC uint64_t timer_value = 0;

/**
 * @brief Default hardware interrupt handler.
 *
 * @param num Number of triggered interrupt.
 */
PRIVATE void default_handler(int num)
{
	UNUSED(num);

	/* Too many spurious interrupts. */
	if (++spurious >= INTERRUPT_SPURIOUS_THRESHOLD)
		kprintf("[hal] spurious interrupt %d", num);

	noop();
}

/**
 * @brief Wrapper for timer interrupt.
 */
PRIVATE void do_timer(int num)
{
	/* Increment timer value. */
	timer_value++;

	/* Forward timer interrupt handling. */
	if (timer_handler != NULL)
		timer_handler(num);

	timer_reset();
}

/**
 * @brief Wrapper for event IPI.
 */
PRIVATE void do_event(int num)
{
	/* Forward event interrupt handling. */
	if (event_handler != default_handler)
		event_handler(num);

	event_reset();
}

/**
 * The do_interrupt() function dispatches a hardware interrupt request
 * that was triggered to a previously-registered handler. If no
 * function was previously registered to handle the triggered hardware
 * interrupt request, this function returns immediately.
 *
 * @note This function is called from assembly code.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void do_interrupt(int intnum)
{
	interrupt_ack(intnum);

	/* Nothing to do. */
	if (interrupt_handlers[intnum] == NULL)
		return;

	interrupt_handlers[intnum](intnum);

	/*
	 * Lets also check for external interrupt, if
	 * there's any pending, handle.
	 */
	while ((intnum = interrupt_next()) != 0)
	{
		/* ack. */
		interrupt_ack(intnum);

		/* Nothing to do. */
		if (interrupt_handlers[intnum] == NULL)
			return;

		interrupt_handlers[intnum](intnum);
	}
}

/**
 * The interrupt_register() function registers @p handler as the
 * handler function for the interrupt whose number is @p num. If a
 * handler function was previously registered with this number, the
 * interrupt_register() function fails.
 */
PUBLIC int interrupt_register(int num, interrupt_handler_t handler)
{
	/* Invalid interrupt number. */
	if ((num < 0) || (num >= INTERRUPTS_NUM))
		return (-EINVAL);

	if (num != INTERRUPT_TIMER && num != INTERRUPT_IPI)
	{
		/* Handler function already registered. */
		if (interrupt_handlers[num] != default_handler)
			return (-EBUSY);

		interrupt_handlers[num] = handler;
	}
	else
	{
		if (num == INTERRUPT_TIMER)
		{
			/* Handler function already registered. */
			if (timer_handler != default_handler)
				return (-EBUSY);

			timer_handler = handler;
		}
		else
		{
			/* Handler function already registered. */
			if (event_handler != default_handler)
				return (-EBUSY);

			event_handler = handler;
		}
	}

	dcache_invalidate();
	interrupt_unmask(num);

	kprintf("[hal] interrupt handler registered for irq %d", num);

	return (0);
}

/**
 * The interrupt_unregister() function unregisters a handler function
 * for the interrupt whose number is @p num. If no handler function
 * was previously registered with this number, the
 * interrupt_unregister() function fails.
 */
PUBLIC int interrupt_unregister(int num)
{
	/* Invalid interrupt number. */
	if ((num < 0) || (num >= INTERRUPTS_NUM))
		return (-EINVAL);

	if (num != INTERRUPT_TIMER && num != INTERRUPT_IPI)
	{
		/* No handler function is registered. */
		if (interrupt_handlers[num] == default_handler)
			return (-EINVAL);

		interrupt_handlers[num] = default_handler;
	}
	else
	{
		if (num == INTERRUPT_TIMER)
		{
			/* No handler function is registered. */
			if (timer_handler == default_handler)
				return (-EINVAL);

			timer_handler = default_handler;
		}
		else
		{
			/* No handler function is registered. */
			if (event_handler == default_handler)
				return (-EINVAL);

			event_handler = default_handler;
		}
	}

		dcache_invalidate();
	interrupt_mask(num);

	kprintf("[hal] interrupt handler unregistered for irq %d", num);

	return (0);
}

/**
 * Initializes hardware interrupts by registering a default handler to
 * all available interrupts.
 */
PUBLIC void interrupt_setup(void)
{
	kputs("[hal] initializing interrupts...\n");

	for (int i = 0; i < INTERRUPTS_NUM; i++)
	{
		interrupt_handler_t handler;

		if (i == INTERRUPT_TIMER)
			handler = do_timer;
		else if (i == INTERRUPT_IPI)
			handler = do_event;
		else
			handler = default_handler;

		interrupt_handlers[i] = handler;
	}

	timer_handler = default_handler;

#if (CLUSTER_HAS_EVENTS)
	event_handler = default_handler;
#endif

	dcache_invalidate();
}

