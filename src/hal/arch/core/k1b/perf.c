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

/* Must come first. */
#define __NEED_CORE_TYPES

#include <arch/core/k1b/types.h>
#include <arch/core/k1b/perf.h>
#include <nanvix/const.h>
#include <errno.h>
#include <stdint.h>

/**
 * @brief Asserts a valid performance monitor.
 *
 * @param perf Monitor to assert.
 *
 * @returns True if the performance monitor is valid and false
 * otherwise.
 */
static inline bool k1b_perf_monitor_is_valid(int perf)
{
	return ((perf >= 0) && (perf < K1B_PERF_MONITORS_NUM));
}

/**
 * @brief Asserts a valid performance event.
 *
 * @param event Event to assert.
 *
 * @returns True if the eventormance event is valid and false
 * otherwise.
 */
static inline bool k1b_perf_event_is_valid(int event)
{
	return ((event >= 0) && (event < K1B_PERF_EVENTS_NUM));
}

/**
 * @brief Writes to PMC register.
 *
 * @param pmc Value for PMC register.
 */
static inline void k1b_pmc_write(k1b_word_t pmc)
{
	asm volatile ("set $pmc, %0;;" :: "r" (pmc));
}

/**
 * @brief Reads the PMC register.
 *
 * @returns The value of the PMC register
 */
static inline k1b_word_t k1b_pmc_read(void)
{
	k1b_word_t pmc;

	asm volatile ("get %0, $pmc;;" : "=r" (pmc));

	return (pmc);
}

/**
 * The k1b_perf_start() function starts watching for the @p event
 * using the performance monitor @p perf.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_perf_start(int perf, int event)
{
	k1b_word_t pmc;
	uint32_t pmX, pmY;

	/* Invalid performance monitor. */
	if (UNLIKELY(!k1b_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Invalid event. */
	if (UNLIKELY(!k1b_perf_event_is_valid(event)))
		return (-EINVAL);

	/* Chain mode. */
	pmY = (pmX = perf << 1) + 1;

	/* Reset performance monitor. */
	pmc = k1b_pmc_read();
	pmc &= ~(K1B_PERF_MASK(pmX) | K1B_PERF_MASK(pmY));
	pmc |= K1B_PERF_RESET(pmX) | K1B_PERF_RESET(pmY);
	k1b_pmc_write(pmc);

	/* Start performance monitor. */
	pmc &= ~(K1B_PERF_MASK(pmX) | K1B_PERF_MASK(pmY));
	pmc |= K1B_PERF_START(pmX, event) | K1B_PERF_START(pmY, event);
	k1b_pmc_write(pmc);

	return (0);
}

/**
 * The k1b_perf_stop() function stops the performance monitor @p perf.
 */
PUBLIC int k1b_perf_stop(int perf)
{
	k1b_word_t pmc;
	uint32_t pmX, pmY;

	/* Invalid performance monitor. */
	if (UNLIKELY(!k1b_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Chain mode. */
	pmY = (pmX = perf << 1) + 1;

	/* Stop performance monitor. */
	pmc = k1b_pmc_read();
	pmc &= ~(K1B_PERF_MASK(pmX) | K1B_PERF_MASK(pmY));
	pmc |= K1B_PERF_STOP(pmX) | K1B_PERF_STOP(pmY);
	k1b_pmc_write(pmc);

	return (0);
}

/**
 * The k1b_perf_setup() function initializes performance monitors. It
 * stops al monitors and setup them to be disabled in idle, privileged
 * and debug modes.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_perf_setup(void)
{
	k1b_word_t pmc;

	/* Stop all monitors. */
	pmc = K1B_PERF_STOP(0) | K1B_PERF_STOP(1) |
		K1B_PERF_STOP(2) | K1B_PERF_STOP(3);

	/*
	 * - Disengage Monitors Timer
	 * - Stop Monitors in Privileged Mode
	 * - Stop Monitors in Debug
	 * - Enable PM0 and PM1 Chaining
	 * - Enable PM2 and PM3 Chaining
	 */
	pmc |= K1B_PERF_SMD | K1B_PERF_SMP | K1B_PERF_DMC |
		  K1B_PERF_CHAIN_0_1 | K1B_PERF_CHAIN_2_3;

	/* Write configuration. */
	k1b_pmc_write(pmc);

	/* Reset performance monitors. */
	pmc = k1b_pmc_read();
	pmc &= ~(K1B_PERF_MASK(0) | K1B_PERF_MASK(1));
	pmc &= ~(K1B_PERF_MASK(2) | K1B_PERF_MASK(2));
	pmc |= K1B_PERF_RESET(0) | K1B_PERF_RESET(1);
	pmc |= K1B_PERF_RESET(2) | K1B_PERF_RESET(3);
	k1b_pmc_write(pmc);
}
