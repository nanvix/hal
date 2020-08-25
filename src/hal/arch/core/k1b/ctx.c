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

#include <arch/core/k1b/ctx.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_context_dump(const struct context *ctx)
{
	/* Dump general purpose registers. */
	kprintf("[k1b]   r0=%x    r1=%x    r2=%x    r3=%x", ctx->r0,  ctx->r1,   ctx->r2,    ctx->r3);
	kprintf("[k1b]   r4=%x    r5=%x    r6=%x    r7=%x", ctx->r4,  ctx->r5,   ctx->r6,    ctx->r7);
	kprintf("[k1b]   r8=%x    r9=%x   r10=%x   r11=%x", ctx->r8,  ctx->r9,   ctx->r10,   ctx->r11);
	kprintf("[k1b]  r12=%x   r13=%x   r14=%x   r15=%x", ctx->r12, ctx->r13,  ctx->r14,   ctx->r15);
	kprintf("[k1b]  r16=%x   r17=%x   r18=%x   r19=%x", ctx->r16, ctx->r17,  ctx->r18,   ctx->r19);
	kprintf("[k1b]  r20=%x   r21=%x   r22=%x   r23=%x", ctx->r20, ctx->r21,  ctx->r22,   ctx->r23);
	kprintf("[k1b]  r24=%x   r25=%x   r26=%x   r27=%x", ctx->r24, ctx->r25,  ctx->r26,   ctx->r27);
	kprintf("[k1b]  r28=%x   r29=%x   r30=%x   r31=%x", ctx->r28, ctx->r29,  ctx->r30,   ctx->r31);
	kprintf("[k1b]  r32=%x   r33=%x   r34=%x   r35=%x", ctx->r32, ctx->r33,  ctx->r34,   ctx->r35);
	kprintf("[k1b]  r36=%x   r37=%x   r38=%x   r39=%x", ctx->r36, ctx->r37,  ctx->r38,   ctx->r39);
	kprintf("[k1b]  r40=%x   r41=%x   r42=%x   r43=%x", ctx->r40, ctx->r41,  ctx->r42,   ctx->r43);
	kprintf("[k1b]  r44=%x   r45=%x   r46=%x   r47=%x", ctx->r44, ctx->r45,  ctx->r46,   ctx->r47);
	kprintf("[k1b]  r48=%x   r49=%x   r50=%x   r51=%x", ctx->r48, ctx->r49,  ctx->r50,   ctx->r51);
	kprintf("[k1b]  r52=%x   r53=%x   r54=%x   r55=%x", ctx->r52, ctx->r53,  ctx->r52,   ctx->r53);
	kprintf("[k1b]  r56=%x   r57=%x   r58=%x   r59=%x", ctx->r56, ctx->r57,  ctx->r58,   ctx->r59);
	kprintf("[k1b]  r60=%x   r61=%x   r62=%x   r63=%x", ctx->r60, ctx->r61,  ctx->r62,   ctx->r63);

	/* Dump special function registers. */
	kprintf("[k1b]   ps=%x   sps=%x  ssps=%x sssps=%x", ctx->ps,  ctx->sps,  ctx->ssps,  ctx->sssps);
	kprintf("[k1b]  spc=%x  sspc=%x ssspc=%x",          ctx->spc, ctx->sspc, ctx->ssspc);
	kprintf("[k1b]  ssp=%x  sssp=%x ssssp=%x   ksp=%x", ctx->ssp, ctx->sssp, ctx->ssssp, ctx->ksp);
	kprintf("[k1b]   lc=%x    ls=%x    le=%x",          ctx->lc,  ctx->ls,   ctx->le);
}

