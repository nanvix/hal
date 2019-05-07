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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @brief Was the timer device initialized?
 */
PRIVATE bool initialized = false;

/**
 * @brief Timer delta
 */
PRIVATE uint32_t timer_delta = 0;

/**
 * @brief Timer delay.
 */
PRIVATE uint32_t timer_delay = 0;

/**
 * @brief Calibrates the timer.
 *
 * Since the timer frequency is sensible to the proper instruction
 * execution, the or1k_timer_calibrate gets the minimal overhead
 * between two reads, in order to adjust the timer value to a more
 * stable value.
 *
 *
 * @author Davidson Francis
 */
PRIVATE uint32_t or1k_timer_calibrate(void)
{
	uint32_t t0, t1;

	t0 = or1k_mfspr(OR1K_SPR_TTCR);
	t1 = or1k_mfspr(OR1K_SPR_TTCR);

	return (t1 - t0);
}

/**
 * The or1k_timer_reset() function acknoledges the timer interrupt and
 * resets the timer counter.
 */
PUBLIC void or1k_timer_reset(void)
{
	/* Ack. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_DI);

	/* Reenable timer. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | OR1K_SPR_TTMR_IE |
		(timer_delta + timer_delay));

	/* Reset counter. */
	or1k_mtspr(OR1K_SPR_TTCR, 0);
}

/**
 * The or1k_timer_init() function initializes the timer driver in the
 * or1k architecture. The frequency of the device is set to @p freq
 * Hz.
 */
PUBLIC void or1k_timer_init(unsigned freq)
{
	unsigned upr;  /* Unit Present Register. */

	/* Nothing to do. */
	if (initialized)
		return;

	/* Checks if Timer available. */
	upr = or1k_mfspr(OR1K_SPR_UPR);
	if (!(upr & OR1K_SPR_UPR_TTP))
		while (1);

	/* Timer rate. */
	timer_delta = (OR1K_CLUSTER_FREQUENCY/freq);

	/*
	 * Timer calibrate.
	 *
	 * Since the timer is disabled by default, its necessary to
	 * temporarily enable the timer (with interrupts disabled)
	 * to get the minimal timer variation between two consecutive
	 * reads.
	 */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | timer_delta);
	or1k_mtspr(OR1K_SPR_TTCR, 0);
	timer_delay = or1k_timer_calibrate();
	initialized = true;

	/* Print some info. */
	kprintf("[hal] timer delay is %d ticks", timer_delay);
	kprintf("[hal] timer delta is %d ticks", timer_delta);

	/*
	 * Reset the timer for the first
	 * time, so that it starts working.
	 */
	or1k_timer_reset();
}
