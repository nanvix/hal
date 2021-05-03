/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
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

#include <arch/core/arm64/perf.h>
#include <nanvix/const.h>
#include <posix/errno.h>
#include <posix/stdint.h>

/**
 * @brief Asserts a valid performance monitor.
 *
 * @param perf Monitor to assert.
 *
 * @returns 1 if the performance monitor is valid and 0
 * otherwise.
 */
static inline int arm64_perf_monitor_is_valid(int perf)
{
	return ((perf >= 0) && (perf < ARM64_PERF_MONITORS_NUM));
}

/**
 * @brief Asserts a valid performance event.
 *
 * @param event Event to assert.
 *
 * @returns Returns 1 if the performance event is valid and
 * 0 otherwise.
 */
static inline int arm64_perf_event_is_valid(int event)
{
	if (event < 0 || event >= ARM64_PERF_EVENTS_NUM)
		return (0);

	return (1);
}

/**
 * The arm64_perf_start() function starts watching for the @p event
 * using the performance monitor @p perf.
 */
PUBLIC int arm64_perf_start(int perf, int event)
{

	/* Invalid performance monitor. */
	if (UNLIKELY(!arm64_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	/* Invalid event. */
	if (UNLIKELY(!arm64_perf_event_is_valid(event)))
		return (-EINVAL);

	event &= ARM64_PMEVTYPER_EVTCOUNT_MASK;
	asm volatile("isb");
	asm volatile("msr pmevtyper0_el0, %0" : : "r" (event));

	uint32_t r = 0;
	asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
	asm volatile("msr pmcntenset_el0, %0" : : "r" (r|1));
	return (0);
}

/**
 * The arm64_perf_stop() function stops the performance monitor @p perf.
 */
PUBLIC int arm64_perf_stop(int perf)
{
	/* Invalid performance monitor. */
	if (UNLIKELY(!arm64_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	arm64_hword_t r = 0;
	asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
	asm volatile("msr pmcntenset_el0, %0" : : "r" (r&0xfffffffe));

	return (0);
}

/**
 * @brief Restarts a performance monitor.
 *
 * @param perf Target performance monitor.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int linux64_perf_restart(int perf)
{
	/* Invalid performance monitor. */
	if (UNLIKELY(!arm64_perf_monitor_is_valid(perf)))
		return (-EINVAL);

	asm volatile("msr pmcr_el0, %0" : : "r" (ARM64_PMCR_P));

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
PUBLIC uint64_t arm64_perf_read(int perf)
{
	arm64_hword_t pccr; /* Count register. */

	/* Invalid performance monitor. */
	if (UNLIKELY(!arm64_perf_monitor_is_valid(perf)))
		return ((arm64_word_t) - 1);

	asm volatile("mrs %0, pmevcntr0_el0" : "=r" (pccr));
	return ((arm64_word_t) pccr);
}

/**
 *	The arm64_perf_setup() function initializes performance monitors. It
 *	stops all counters and resets them.
 */
PUBLIC void arm64_perf_setup(void)
{
	/*
	*	Enable user-mode access to counters. 
	*	configure the cyclecounter module
	*/
	asm volatile("msr pmuserenr_el0, %0" : : "r"((arm64_word_t)ARM64_PMUSERENR_EN_EL0|ARM64_PMUSERENR_ER|ARM64_PMUSERENR_CR));

	/*  
	*	Performance Monitors Count Enable Set register bit 30:0 disable, 31 enable. 
	* 	Can also enable other event counters here. 
	*/ 
	asm volatile("msr pmcntenset_el0, %0" : : "r" (ARM64_PMCNTENSET_EL0_ENABLE));

	/* Enable counters */
	arm64_word_t val=0;
	asm volatile("mrs %0, pmcr_el0" : "=r" (val));
	asm volatile("msr pmcr_el0, %0" : : "r" (val|ARM64_PMCR_E));
}
