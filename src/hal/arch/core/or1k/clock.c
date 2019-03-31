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
 * The or1k_clock_reset() function acknoledges the clock interrupt and
 * resets the clock counter.
 */
PUBLIC void or1k_clock_reset(void)
{
	/* Ack. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_DI);

	/* Reenable timer. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | OR1K_SPR_TTMR_IE |
		OR1K_CPU_FREQUENCY);

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
	unsigned rate; /* Timer rate.            */

	UNUSED(freq);

	upr = or1k_mfspr(OR1K_SPR_UPR);
	if (!(upr & OR1K_SPR_UPR_TTP))
		while (1);

	/* Clock rate. */
	rate = OR1K_CPU_FREQUENCY;

	/* Ensures that the clock is disabled. */
	or1k_mtspr(OR1K_SPR_TTMR, OR1K_SPR_TTMR_SR | OR1K_SPR_TTMR_IE | rate);
	or1k_mtspr(OR1K_SPR_TTCR, 0);
}
