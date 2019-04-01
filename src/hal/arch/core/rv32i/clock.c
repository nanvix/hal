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

#include <arch/core/rv32i/clock.h>
#include <arch/core/rv32i/int.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @brief Was the clock device initialized?
 */
PRIVATE int initialized = FALSE;

/**
 * @brief Clock delta
 */
PRIVATE unsigned clock_delta = 0;

/**
 * @brief Clock jitter.
 */
PRIVATE unsigned clock_delay = 0;

/**
 * The rv32i_clock_reset() function resets the clock device in the
 * underlying rv32i core.
 */
PUBLIC void rv32i_clock_reset(void)
{
	rv32i_dword_t ctime;

	/* Get current time. */
	ctime = rv32i_mtimecmp_read();

	/* Set next timer interrupt to near future. */
	rv32i_mtime_write(ctime + clock_delta + clock_delay);
}

/**
 * The rv32i_clock_init() function initializes the clock device in the
 * underlying rv32i core.
 */
PUBLIC void rv32i_clock_init(unsigned freq)
{
	rv32i_dword_t t0, t1;

	kprintf("[hal] initializing the clock device...");

	/* Initialize clock. */
	clock_delta = MTIME_TIMEBASE/freq;
	t0 = rv32i_mtimecmp_read();
	t1 = rv32i_mtimecmp_read();
	clock_delay = t1 - t0;

	/* Reset the clock device. */
	rv32i_clock_reset();

	initialized = TRUE;
}
