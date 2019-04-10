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
#include <nanvix/klib.h>
#include <nanvix/hal/core/clock.h>
#include <arch/core/or1k/core.h>

/**
 * @brief Was the clock device initialized?
 */
PRIVATE int initialized = FALSE;

/**
 * @brief Clock delta
 */
PRIVATE uint32_t clock_delta = 0;

/**
 * @brief Clock delay.
 */
PRIVATE uint32_t clock_delay = 0;

/**
 * @brief Calibrates the clock.
 *
 * Since the timer frequency is sensible to the proper instruction
 * execution, the or1k_clock_calibrate gets the minimal overhead
 * between two reads, in order to adjust the timer value to a more
 * stable value.
 *
 *
 * @author Davidson Francis
 */
PRIVATE uint32_t or1k_clock_calibrate(void)
{
	uint32_t t0, t1;

	t0 = or1k_mfspr(OR1K_SPR_TTCR);
	t1 = or1k_mfspr(OR1K_SPR_TTCR);

	return (t1 - t0);
}

/**
 * The or1k_clock_reset() function acknoledges the clock interrupt and
 * resets the clock counter.
 */
PUBLIC void or1k_clock_reset(void)
{
	/* Ack. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_DI);

	/* Reenable timer. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | OR1K_SPR_TTMR_IE |
		(clock_delta + clock_delay));

	/* Reset counter. */
	or1k_mtspr(OR1K_SPR_TTCR, 0);
}

/**
 * The or1k_clock_init() function initializes the clock driver in the
 * or1k architecture. The frequency of the device is set to @p freq
 * Hz.
 */
PUBLIC void or1k_clock_init(unsigned freq)
{
	unsigned upr;  /* Unit Present Register. */

	UNUSED(freq);
	
	/* Nothing to do. */
	if (initialized)
		return;

	/* Checks if Timer available. */
	upr = or1k_mfspr(OR1K_SPR_UPR);
	if (!(upr & OR1K_SPR_UPR_TTP))
		while (1);

	/* Clock rate. */
	clock_delta = OR1K_CPU_FREQUENCY;
	
	/* 
	 * Clock calibrate.
	 *
	 * Since the timer is disabled by default, its necessary to
	 * temporarily enable the timer (with interrupts disabled)
	 * to get the minimal timer variation between two consecutive
	 * reads.
	 */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | clock_delta);
	or1k_mtspr(OR1K_SPR_TTCR, 0);	
	clock_delay = or1k_clock_calibrate();
	initialized = TRUE;
	
	/* Print some info. */
	kprintf("[hal] clock delay is %d ticks", clock_delay);
	kprintf("[hal] clock delta is %d ticks", clock_delta);

	/*
	 * Reset the clock for the first
	 * time, so that it starts working.
	 */
	or1k_clock_reset();
}
