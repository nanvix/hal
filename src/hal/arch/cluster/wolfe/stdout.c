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

#define __NEED_WOLFE_STDOUT

#include <arch/cluster/wolfe/stdout.h>
#include <nanvix/const.h>
#include <sys/types.h>

/**
 * @brief Stdout device.
 */
PRIVATE struct
{
	volatile char *mem; /**< Base Memory Address */
	bool initialized;   /**< Device Initialized? */
} stdout = {
	NULL,
	FALSE,
};

/**
 * The wolfe_stdout_write() function writes @p n bytes from the buffer
 * pointed to by @p buf onto the standard output device.
 *
 * @bug FIXME this function is not thread safe.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void wolfe_stdout_write(const char *buf, size_t n)
{
	/* Device not initialized. */
	if (!stdout.initialized)
		return;

	for (size_t i = 0; i < n; i++)
		stdout.mem[(0x00 << 3) + (0x1f << 7)] = buf[i];
}

/**
 * The wolfe_stdout_init() function initializes the standard output
 * device, which is mapped at physical memory address reffered by @p
 * base.
 *
 * @note This function is not thread safe.
 */
PUBLIC void wolfe_stdout_init(paddr_t base)
{
	/* Nothing to be done. */
	if (stdout.initialized)
		return;

	/* Initialize the device. */
	stdout.mem = (volatile char*)(base + WOLFE_STDOUT_PUTC_OFF);
	stdout.initialized = TRUE;
}

