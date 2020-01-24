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
#define __NEED_CORE_LINUX64

#include <arch/core/linux64.h>
#include <nanvix/const.h>

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
	UNUSED(ctx);
	UNUSED(argsize);

	fn(arg);

	linux64_core_poweroff(true);
}
