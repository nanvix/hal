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
#include <stdio.h>
#include <string.h>

/**
 * @brief Size of internal buffer for virtual tty device.
 */
#define TTY_VIRT_BUFSIZE 255

/**
 * @brief Flag that indicates if the device was initialized.
 */
PRIVATE bool tty_virt_initialized = false;

/**
 * Writes into the virtual tty device.
 *
 * @param buf Data to be written.
 * @param n Data size.
 */
PUBLIC void tty_virt_write(const char *buf, size_t n)
{
	char _buf[TTY_VIRT_BUFSIZE + 1];

	/**
	 * It's important to only try to write if the device
	 * was already initialized.
	 */
	if (!tty_virt_initialized)
		return;

	while (n > 0)
	{
		size_t i;

		i = (n >= TTY_VIRT_BUFSIZE) ? TTY_VIRT_BUFSIZE : n;

		_buf[i] = '\0';
		memcpy(_buf, buf, i);

		fprintf(stderr, "%s", _buf);
		n -= i;
	}
}

/**
 * Initializes the virtual tty device.
 */
PUBLIC void tty_virt_init(void)
{
	/* Device initialized. */
	tty_virt_initialized = true;
}

