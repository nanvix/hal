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

#include <arch/core/rv32gc/clock.h>
#include <arch/core/rv32gc/mcall.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <stdint.h>

/**
 * @brief Was the clock device initialized?
 */
PRIVATE bool initialized = false;

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
PRIVATE uint64_t *rv32gc_mtime = NULL;

/**
 * @brief 64-bit timer compare register.
 */
PRIVATE uint64_t *rv32gc_mtimecmp = NULL;

/**
 * @brief Reads the mtime register.
 *
 * @returns The value of the mtime register.
 */
PRIVATE uint64_t rv32gc_mtime_read(void)
{
	uint32_t lo;
	uint32_t hi;

	hi = *((uint32_t *)(rv32gc_mtime) + 1);
	lo = *((uint32_t *)(rv32gc_mtime));

	return (((hi & 0xffffffffull) << 32) | (lo & 0xffffffffull));
}

/**
 * @brief Reads the mtimecmp register.
 *
 * @returns The value of the mtimecmp register.
 */
PRIVATE uint64_t rv32gc_mtimecmp_read(void)
{
	uint32_t lo;
	uint32_t hi;

	hi = *((uint32_t *)(rv32gc_mtimecmp) + 1);
	lo = *((uint32_t *)(rv32gc_mtimecmp));

	return (((hi & 0xffffffffull) << 32) | (lo & 0xffffffffull));
}

/**
 * @brief Writes to the mtimecmp register.
 *
 * @param time Value to write.
 */
PRIVATE void rv32gc_mtimecmp_write(uint64_t time)
{
	*((uint32_t *)(rv32gc_mtimecmp) + 1) = 0xffffffff;
	*((uint32_t *)(rv32gc_mtimecmp)) = (uint32_t)(time & 0xffffffff);
	*((uint32_t *)(rv32gc_mtimecmp) + 1) =
		(uint32_t)((time & 0xffffffff00000000ull) >> 32);

	rv32gc_mcall_timer_ack();
}

/**
 * @brief Calibrates the clock.
 *
 * @todo TODO provide a long description for this function.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE uint64_t rv32gc_clock_calibrate(void)
{
	uint64_t t0, t1;

	t0 = rv32gc_mtime_read();
	t1 = rv32gc_mtime_read();

	return (t1 - t0);
}

/**
 * The rv32gc_clock_reset() function resets the clock device in the
 * underlying rv32gc core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_clock_reset(void)
{
	uint64_t ctime;

	/* Get current time. */
	ctime = rv32gc_mtime_read();

	/* Set next timer interrupt to near future. */
	rv32gc_mtimecmp_write(ctime + clock_delta + clock_delay);
}

/**
 * The rv32gc_clock_init() function initializes the clock device in the
 * underlying rv32gc core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_clock_init(
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
	rv32gc_mtime = mtime;
	rv32gc_mtimecmp = mtimecmp;

	/* Initialize clock. */
	clock_delta = timebase/freq;
	clock_delay = rv32gc_clock_calibrate();
	initialized = true;

	/* Print some info. */
	kprintf("[hal] clock delay is %d ticks", clock_delay);
	kprintf("[hal] clock delta is %d ticks", clock_delta);

	/*
	 * Reset the clock for the first
	 * time, so that it starts working.
	 */
	rv32gc_clock_reset();

}
