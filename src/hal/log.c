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

#include <nanvix/hal/target/stdout.h>
#include <nanvix/hal/log.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <sys/types.h>
#include <stdint.h>

/**
 * @brief HAL log.
 */
PRIVATE struct
{
	size_t head;           /**< First element in the buffer.   */
	size_t tail;            /**< Next free slot in the buffer. */
	char buf[HAL_LOG_SIZE]; /**< Ring buffer.                  */
} logdev = { 0, 0, {0, }};

/*============================================================================*
 * hal_log_flush()                                                            *
 *============================================================================*/

/**
 * @brief Flushes the HAL log buffer.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void hal_log_flush(void)
{
	ssize_t i;              /* Number of bytes to flush. */
	char buf[KBUFFER_SIZE]; /* Temporary buffer.         */

	/* No data, so nothing to do. */
	if (logdev.head == logdev.tail)
		return;

	/* Copy data from ring buffer. */
	for (i = 0; i < KBUFFER_SIZE; /* noop */ )
	{
		buf[i] = logdev.buf[logdev.head];

		i++;
		logdev.head = (logdev.head + 1)%HAL_LOG_SIZE;
		dcache_invalidate();

		if (logdev.head == logdev.tail)
			break;
	}

	stdout_write(buf, i);
}

/*============================================================================*
 * hal_log_write()                                                            *
 *============================================================================*/

/**
 * The hal_log_write() function writes @p n bytes of the buffer
 * pointed to by @p buf in the HAL log and in the standard output
 * device of the underlying target.
 *
 * @author Pedro Henrique Penna
 *
 * @todo Make this thread safe.
 */
PUBLIC void hal_log_write(const char *buf, size_t n)
{
	/* Avoid race conditions. */
	if (core_get_id() == COREID_MASTER)
	{
		/* Copy data to ring buffer. */
		for (size_t i = 0; i < n; i++)
		{
			logdev.buf[logdev.tail] = buf[i];
			logdev.tail = (logdev.tail + 1)%HAL_LOG_SIZE;
			dcache_invalidate();
		}
	}

	stdout_write(buf, n);
}

/*============================================================================*
 * hal_log_setup()                                                            *
 *============================================================================*/

/**
 * The hal_log_setup() initializes the standard output device of the
 * underlying target and flushes onto it the current context stored in
 * the HAL log
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void hal_log_setup(void)
{
	stdout_init();
}
