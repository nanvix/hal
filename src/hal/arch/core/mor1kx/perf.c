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

#include <arch/core/mor1kx/perf.h>
#include <nanvix/const.h>
#include <errno.h>
#include <stdint.h>

/**
 * @brief Asserts a valid performance monitor.
 *
 * @param perf Monitor to assert.
 *
 * @returns 1 if the performance monitor is valid and 0
 * otherwise.
 */
static inline int mor1kx_perf_monitor_is_valid(int perf)
{
	return ((perf >= 0) && (perf < MOR1KX_PERF_MONITORS_NUM));
}

/**
 * @brief Asserts a valid performance event.
 *
 * @param event Event to assert.
 *
 * @returns Returns 1 if the performance event is valid and
 * 0 otherwise.
 */
static inline int mor1kx_perf_event_is_valid(int event)
{
	if (event < 0 || event >= MOR1KX_PERF_EVENTS_NUM)
		return (0);

	return (1);
}

/**
 * The mor1kx_perf_start() function starts watching for the @p event
 * using the performance monitor @p perf.
 *
 * @author Davidson Francis
 */
PUBLIC int mor1kx_perf_start(int perf, int event)
{
	or1k_word_t pcmr; /* Mode register. */

	/* Invalid performance monitor. */
	if (UNLIKELY(!mor1kx_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Invalid event. */
	if (UNLIKELY(!mor1kx_perf_event_is_valid(event)))
		return (-EINVAL);

	/* Invalid counter. */
	if (UNLIKELY(!(or1k_mfspr(OR1K_SPR_PCMR(perf)) & OR1K_SPR_PCMR_CP)))
		return (-EINVAL);

	/* Reset counter and starts. */
	pcmr = OR1K_SPR_PCMR_CP | OR1K_SPR_PCMR_CISM | OR1K_SPR_PCMR_CIUM
		| (1 << (event + MOR1KX_PERF_EVENT_OFFSET));

	or1k_mtspr(OR1K_SPR_PCCR(perf), 0);
	or1k_mtspr(OR1K_SPR_PCMR(perf), pcmr);

	return (0);
}

/**
 * The mor1kx_perf_stop() function stops the performance monitor @p perf.
 */
PUBLIC int mor1kx_perf_stop(int perf)
{
	/* Invalid performance monitor. */
	if (UNLIKELY(!mor1kx_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Invalid counter. */
	if (UNLIKELY(!(or1k_mfspr(OR1K_SPR_PCMR(perf)) & OR1K_SPR_PCMR_CP)))
		return (-EINVAL);

	or1k_mtspr(OR1K_SPR_PCMR(perf), OR1K_SPR_PCMR_CP);

	return (0);
}

/**
 * The mor1kx_perf_restart() function restarts the performance monitor @p perf.
 */
PUBLIC int mor1kx_perf_restart(int perf)
{
	or1k_word_t pcmr; /* Mode register.  */

	/* Invalid performance monitor. */
	if (UNLIKELY(!mor1kx_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Stop counter. */
	pcmr = or1k_mfspr(OR1K_SPR_PCMR(perf));
	or1k_mtspr(OR1K_SPR_PCMR(perf), 0);

	/* Resets counter. */
	or1k_mtspr(OR1K_SPR_PCCR(perf), 0);
	
	/* Start counter again. */
	or1k_mtspr(OR1K_SPR_PCMR(perf), pcmr);

	return (0);
}

/**
 * @brief Reads a PM register.
 *
 * @param perf Target performance monitor.
 *
 * @returns Upon successful completion, the value of the target
 * performance monitor. Upon failure, -1 converted to uint64_t is
 * returned instead.
 */
PUBLIC uint64_t mor1kx_perf_read(int perf)
{
	uint32_t pccr; /* Count register. */

	/* Invalid performance monitor. */
	if (UNLIKELY(!mor1kx_perf_monitor_is_valid(perf)))
		return ((uint64_t) - 1);

	/* Invalid counter. */
	if (UNLIKELY(!(or1k_mfspr(OR1K_SPR_PCMR(perf)) & OR1K_SPR_PCMR_CP)))
		return ((uint64_t) - 1);

	pccr = or1k_mfspr(OR1K_SPR_PCCR(perf));
	return ((uint64_t) pccr);
}

/**
 * The mor1kx_perf_setup() function initializes performance monitors. It
 * stops all counters and resets them.
 *
 * @author Davidson Francis
 */
PUBLIC void mor1kx_perf_setup(void)
{
	or1k_word_t pcmr; /* Mode register. */
	int i;            /* Loop index.    */

	/* If performance counters available. */
	if (or1k_mfspr(OR1K_SPR_UPR) & OR1K_SPR_UPR_PCUP)
	{
		/*
		 * For each counter, resets and disables all
		 * counters availables.
		 */
		for (i = 0; i < MOR1KX_PERF_MONITORS_NUM; i++)
		{
			pcmr = or1k_mfspr(OR1K_SPR_PCMR(i));
			if (pcmr & OR1K_SPR_PCMR_CP)
			{
				or1k_mtspr(OR1K_SPR_PCCR(i), 0);
				or1k_mtspr(OR1K_SPR_PCMR(i), pcmr & OR1K_SPR_PCMR_CP);
			}
		}
	}
}
