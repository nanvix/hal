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
#define __NEED_CORE_CONTEXT

#include <nanvix/hal/core/interrupt.h>
#include <arch/core/k1b/ctx.h>
#include <arch/core/k1b/cache.h>
#include <arch/core/k1b/int.h>
#include <arch/core/k1b/ivt.h>
#include <nanvix/const.h>
#include <mOS_vcore_u.h>
#include <vbsp.h>

/**
 * @brief High-level interrupt dispatcher.
 */
PRIVATE k1b_int_handler_fn _do_interrupt = NULL;

/**
 * @brief Low-level interrupt dispatcher.
 *
 * @param irqnum IRQ number.
 * @param ctx   Saved interrupted context.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void __k1b_do_int(int irqnum, struct context * ctx)
{
	int intnum;

	UNUSED(ctx);

	KASSERT((intnum = k1b_irq_to_int(irqnum)) >= 0);

	if (LIKELY(_do_interrupt != NULL))
		_do_interrupt(intnum);
}

/**
 * @brief Initializes the interrupt vector table.
 *
 * @param hwint_handler Default hardware interrupt handler.
 * @param swint_handler Default software interrupt handler.
 * @param excp_handler  Default exception handler.
 * @param stack         Stack for interrupts, exceptions and traps.
 *
 * The k1b_ivt_setup() function initializes the interrupt vector table
 * in the k1b architecture. It traverses all entries of this table and
 * properly registers @p do_hwintm @p do_swint and do_excp as default
 * handlers for hardware interrupts, software interrupts and
 * exceptions, respectively.
 */
PRIVATE void k1b_ivt_setup(
	k1b_int_handler_fn hwint_handler,
	k1b_swint_handler_fn swint_handler,
	void (*excp_handler)(void),
	void (*it_handler)(void),
	void *stack
)
{
	kprintf("[hal] exception stack at %x", stack);

	/* Register dispatchers. */
	_do_interrupt = hwint_handler;
	mOS_register_it_handler(it_handler);

	mOS_register_scall_handler(swint_handler);
	mOS_register_trap_handler(excp_handler);
	k1b_dcache_inval();

	/* Setup shadow stack for exceptions. */
	mOS_register_stack_handler(stack);
	mOS_trap_enable_shadow_stack();

	k1b_pic_setup();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void ivt_setup(void * stack)
{
	k1b_ivt_setup(
		do_interrupt,
		_k1b_do_kcall,
		_k1b_do_excp,
		_k1b_do_int,
		stack
	);
}

