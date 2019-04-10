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

#include <arch/core/or1k/context.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void or1k_context_dump(const struct context *ctx)
{
	/* Dump general purpose registers. */
	kprintf("[or1k]  r0=%x  r1=%x  r2=%x  r3=%x", ctx->r0,  ctx->r1,  ctx->r2,  ctx->r3);
	kprintf("[or1k]  r4=%x  r5=%x  r6=%x  r7=%x", ctx->r4,  ctx->r5,  ctx->r6,  ctx->r7);
	kprintf("[or1k]  r8=%x  r9=%x r10=%x r11=%x", ctx->r8,  ctx->r9,  ctx->r10, ctx->r11);
	kprintf("[or1k] r12=%x r13=%x r14=%x r15=%x", ctx->r12, ctx->r13, ctx->r14, ctx->r15);
	kprintf("[or1k] r16=%x r17=%x r18=%x r19=%x", ctx->r16, ctx->r17, ctx->r18, ctx->r19);
	kprintf("[or1k] r20=%x r21=%x r22=%x r23=%x", ctx->r20, ctx->r21, ctx->r22, ctx->r23);
	kprintf("[or1k] r24=%x r25=%x r26=%x r27=%x", ctx->r24, ctx->r25, ctx->r26, ctx->r27);
	kprintf("[or1k] r28=%x r29=%x r30=%x r31=%x", ctx->r28, ctx->r29, ctx->r30, ctx->r31);

	/* Dump special function registers. */
	kprintf("[or1k] epcr=%x  eear=%x  esr=%x", ctx->epcr, ctx->eear, ctx->esr);
}
