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

#include <nanvix/const.h>

/**
 * @brief Flag that indicates if the device was initialized.
 */
PRIVATE bool systrace_initialized = false;

/**
 * Writes one character into systrace.
 * @param c Character to be written.
 */
PRIVATE inline void systrace_putchar(char c)
{
	asm volatile
	(
		"l.addi r3, %0, 0"
		:
		: "r" (c) : "r3"
	);
	asm volatile ("l.nop 4");
}

/**
 * Writes into the systrace.
 * @param buf Data to be written.
 * @param n Data size.
 */
PUBLIC void systrace_write(const char *buf, size_t n)
{
	/**
	 * It's important to only try to write if the device
	 * was already initialized.
	 */
	if (!systrace_initialized)
		return;

	for (size_t i = 0; i < n; i++)
		systrace_putchar(buf[i]);
}

/**
 * Initializes the systrace device.
 */
PUBLIC void systrace_init(void)
{
	/* Device initialized. */
	systrace_initialized = true;
}
