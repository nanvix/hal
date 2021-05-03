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
#define __NEED_CORE_TYPES
#define __NEED_CORE_REGS

#include <arch/core/arm64/ctx.h>
#include <arch/core/arm64/excp.h>
#include <arch/core/arm64/int.h>
#include <arch/core/arm64/mcall.h>
#include <arch/core/arm64/types.h>
#include <arch/core/arm64/timer.h>
#include <arch/core/arm64/gic.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC void arm64_dump_all_csr(void)
{
	//TODO
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC NORETURN void arm64_do_mbad(const struct context *ctx)
{
	arm64_context_dump(ctx);
	arm64_dump_all_csr();

	kprintf("[arm64] bad exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC NORETURN void arm64_do_mexcp(const struct context *ctx)
{
	arm64_context_dump(ctx);
	arm64_dump_all_csr();

	kprintf("[arm64] unhandled exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC void arm64_do_mcall(struct context *ctx)
{
	UNUSED(ctx);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC void arm64_do_mint(const struct context *ctx)
{
	ctx = ctx;
	//arm64_context_dump(ctx);
	//arm64_dump_all_csr();
	irq_no a = arm64_gic_find_pending_irq();
	if (a < 0 || a > ARM64_INT_NUM) {
		kprintf("[arm64] unhandled interruption %x", a);
		UNREACHABLE();
	}
	kprintf("[arm64] interruption %d", a);
	arm64_do_interrupt(a);
	//UNREACHABLE();
}