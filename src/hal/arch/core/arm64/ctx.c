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
#define __NEED_CORE_REGS

#include <arch/core/arm64/ctx.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 */
PUBLIC void arm64_context_dump(const struct context *ctx)
{
	/* Dump general purpose registers. */
	kprintf("[hal]  x0=%x   x1=%x   x2=%x",       ctx->x0, ctx->x1, ctx->x2);
	kprintf("[hal]  x3=%x   x4=%x   x5=%x",       ctx->x3, ctx->x4, ctx->x5);
	kprintf("[hal]  x6=%x   x7=%x   x8=%x",       ctx->x6, ctx->x7, ctx->x8);
	kprintf("[hal]  x9=%x   x10=%x   x11=%x",       ctx->x9, ctx->x10, ctx->x11);
	kprintf("[hal]  x12=%x   x13=%x   x14=%x",       ctx->x12, ctx->x13, ctx->x14);
	kprintf("[hal]  x15=%x   x16=%x   x17=%x",       ctx->x15, ctx->x16, ctx->x17);
	kprintf("[hal]  x18=%x   x19=%x   x20=%x",       ctx->x18, ctx->x19, ctx->x20);
	kprintf("[hal]  x21=%x   x22=%x   x23=%x",       ctx->x21, ctx->x22, ctx->x23);
	kprintf("[hal]  x24=%x   x23=%x   x26=%x",       ctx->x24, ctx->x25, ctx->x26);
	kprintf("[hal]  x27=%x   x28=%x   x29=%x",       ctx->x27, ctx->x28, ctx->x29);
	kprintf("[hal]  x30=%x                  ",       ctx->x30);
	kprintf("[hal]  codeID=%x               ",       core_get_id());
}
