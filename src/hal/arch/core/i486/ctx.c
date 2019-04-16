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

#define __NEED_CORE_CONTEXT

#include <arch/core/i486/ctx.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @uthor Pedro Henrique Penna
 */
PUBLIC void i486_ctx_dump(const struct context *ctx)
{
	kprintf("[i486] eax=%x ebx=%x ecx=%x edx=%x", ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);
	kprintf("[i486] esi=%x edi=%x ebp=%x esp=%x", ctx->esi, ctx->edi, ctx->ebp, ctx->useresp);
	kprintf("[i486]  cs=%x  ds=%x  ss=%x", 0xff & ctx->cs, 0xff & ctx->ds, 0xff & ctx->ss);
	kprintf("[i486]  es=%x  fs=%x  gs=%x", 0xff & ctx->es, 0xff & ctx->fs, 0xff & ctx->gs);
	kprintf("[i486] eip=%x eflags=%x", ctx->eip, ctx->eflags);
}

