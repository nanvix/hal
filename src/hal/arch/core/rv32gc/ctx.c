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
#define __NEED_CORE_CONTEXT
#define __NEED_CORE_REGS

#include <arch/core/rv32gc/ctx.h>
#include <arch/core/rv32gc/regs.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_context_dump(const struct context *ctx)
{
	/* Dump general purpose registers. */
	kprintf("[hal]  ra=%x   sp=%x   fp=%x",       ctx->ra, ctx->sp, ctx->fp);
	kprintf("[hal]  gp=%x   tp=%x",               ctx->gp, ctx->tp);
	kprintf("[hal]  a0=%x   a1=%x  s2=%x  a3=%x", ctx->a0, ctx->a1,  ctx->a2,  ctx->a3);
	kprintf("[hal]  a4=%x   a5=%x  s6=%x  a7=%x", ctx->a4, ctx->a5,  ctx->a6,  ctx->a7);
	kprintf("[hal]  t0=%x   t1=%x  t2=%x",        ctx->t0, ctx->t1,  ctx->t2);
	kprintf("[hal]  t3=%x   t4=%x  t5=%x  t6=%x", ctx->t3, ctx->t4,  ctx->t5,  ctx->t6);
	kprintf("[hal]  s1=%x   s2=%x  s3=%x  s4=%x", ctx->s1, ctx->s2,  ctx->s3,  ctx->s4);
	kprintf("[hal]  s5=%x   s6=%x  s7=%x  s8=%x", ctx->s5, ctx->s6,  ctx->s7,  ctx->s8);
	kprintf("[hal]  s9=%x  s10=%x s11=%x",        ctx->s9, ctx->s10, ctx->s11);

	kprintf("[hal]  sstatus=%x      sie=%x     sip=%x",
		rv32gc_sstatus_read(),
		rv32gc_sie_read(),
		rv32gc_sip_read()
	);
	kprintf("[hal]    stvec=%x   scause=%x   stval=%x",
		rv32gc_stvec_read(),
		rv32gc_scause_read(),
		rv32gc_stval_read()
	);
	kprintf("[hal]     sepc=%x    satp=%x",
		rv32gc_sepc_read(),
		rv32gc_satp_read()
	);
}
