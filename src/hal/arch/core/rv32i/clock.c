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
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <stdint.h>

/**
 * @brief Was the clock device initialized?
 */
PRIVATE int initialized = FALSE;

/**
 * @brief Clock delta
 */
PRIVATE uint64_t clock_delta = 0;

/**
 * @brief Clock delay.
 */
PRIVATE uint64_t clock_delay = 0;

/**
 * @brief 64-bit timer register.
 */
PUBLIC uint64_t *rv32i_mtime = NULL;

/**
 * @brief 64-bit timer compare register.
 */
PUBLIC uint64_t *rv32i_mtimecmp = NULL;

/**
 * @brief Calibrates the clock.
 *
 * @todo TODO provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE uint64_t rv32i_clock_calibrate(void)
{
	uint64_t t0, t1;

	t0 = rv32i_mtime_read();
	t1 = rv32i_mtime_read();

	return (t1 - t0);
}

/**
 * The rv32i_clock_reset() function resets the clock device in the
 * underlying rv32i core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_clock_reset(void)
{
	uint64_t ctime;

	/* Get current time. */
	ctime = rv32i_mtime_read();

	/* Set next timer interrupt to near future. */
	rv32i_mtimecmp_write(ctime + clock_delta + clock_delay);
}

/**
 * The rv32i_clock_init() function initializes the clock device in the
 * underlying rv32i core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_clock_init(
	uint64_t freq,
	uint64_t timebase,
	uint64_t *mtime,
	uint64_t *mtimecmp
)
{
	/* Nothing to do. */
	if (initialized)
		return;

	kprintf("[hal] initializing the clock device...");

	/* Setup memory mapped registers. */
	rv32i_mtime = mtime;
	rv32i_mtimecmp = mtimecmp;

	/* Initialize clock. */
	clock_delta = timebase/freq;
	clock_delay = rv32i_clock_calibrate();
	initialized = TRUE;

	/* Print some info. */
	kprintf("[hal] clock delay is %d ticks", clock_delay);
	kprintf("[hal] clock delta is %d ticks", clock_delta);

	/*
	 * Reset the clock for the first
	 * time, so that it starts working.
	 */
	rv32i_clock_reset();

}
