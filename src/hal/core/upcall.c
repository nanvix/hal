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

#include <nanvix/hal/core/context.h>
#include <nanvix/hal/core/upcall.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

#ifndef __upcall_forge_fn

/**
 * The upcall_forge() creates a fake stack frame that enables an
 * upcall. It pushes the saved program counter, followed by the
 * argument pointed to by @p arg and its size, referred by @p argsize.
 * In the end, the saved execution context is updated accordingly so
 * that, once the calling thread leaves the kernel, the target
 * function pointed to by @p fn is called.
 *
 * @note It is up to the caller to check if enough memory is available
 * in the user stack to perform the upcall.
 */
PUBLIC void upcall_forge(
	struct context *ctx,
	void (*fn)(void *),
	void *arg,
	word_t argsize
)
{
	word_t sp;          /* Stack pointer    */
	byte_t padding = 0; /* Argument padding.*/

	/* We must ensure this. */
	KASSERT_SIZE(sizeof(void *), sizeof(word_t));
	KASSERT(ALIGNED(UPCALL_STACK_FRAME_SIZE(argsize), DWORD_SIZE));

	/* Compute argument padding. */
	padding = DWORD_SIZE - (argsize % DWORD_SIZE);

	sp = context_get_sp(ctx);

	/* Push saved context. */
	sp -= sizeof(struct context);
	kmemcpy((void *)sp, ctx, sizeof(struct context));

	/* Align to double word boundary. */
	if (padding > 0)
	{
		sp -= padding;
		kmemset((void *)sp, 0, padding);
	}

	/* Push arguments. */
	sp -= argsize;
	kmemcpy((void *)sp, arg, argsize);

	/* Push arguments size with padding. */
	argsize += padding;
	sp -= sizeof(word_t);
	kmemcpy((void *)sp, &argsize, sizeof(word_t));

	/* Push target function. */
	sp -= sizeof(word_t);
	kmemcpy((void *)sp, &fn, sizeof(word_t));

	/* Tweak saved context. */
	context_set_pc(ctx, (word_t) upcall_ret);
	context_set_sp(ctx, sp);
}

#endif /* !__upcall_forge_fn */
