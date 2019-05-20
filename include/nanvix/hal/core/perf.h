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

#ifndef NANVIX_HAL_CORE_PERF_H_
#define NANVIX_HAL_CORE_PERF_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/


#if (CORE_HAS_PERF)
#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_PERF)

	/* Variables. */
	/* Constants */
	#ifndef PERF_EVENTS_NUM
	#error "PERF_EVENTS_NUM not defined"
	#endif
	#if (!(PERF_EVENTS_NUM >= 1))
	#error "no performance events?"
	#endif
	#ifndef PERF_MONITORS_NUM
	#error "PERF_MONITORS_NUM not defined"
	#endif
	#if (!(PERF_MONITORS_NUM >= 1))
	#error "no performance monitors?"
	#endif

	/* Functions */
	#ifndef __perf_setup_fn
	#error "perf_setup() not defined?"
	#endif
	#ifndef __perf_start_fn
	#error "perf_start() not defined?"
	#endif
	#ifndef __perf_stop_fn
	#error "perf_stop() not defined?"
	#endif
	#ifndef __perf_restart_fn
	#error "perf_restart() not defined?"
	#endif
	#ifndef __perf_read_fn
	#error "perf_read() not defined?"
	#endif

#endif
#endif

/*============================================================================*
 * Performance Monitoring Interface                                           *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-perf Perf Monitoring
 * @ingroup kernel-hal-core
 *
 * @brief Performance Monitoring Interface
 */
/**@{*/

	#include <nanvix/klib.h>
	#include <stdint.h>

	/**
	 * @brief Total number of known events.
	 */
	#define PERF_EVENTS_MAX 14

	/**
	 * @name Dummy Performance Events
	 */
	/**@{*/
	#ifndef PERF_MONITORS_NUM
	#define PERF_MONITORS_NUM  -1
	#endif
	#ifndef PERF_EVENTS_NUM
	#define PERF_EVENTS_NUM    -1
	#endif
	#ifndef PERF_CYCLES
	#define PERF_CYCLES        -1
	#endif
	#ifndef PERF_ICACHE_HITS
	#define PERF_ICACHE_HITS   -1
	#endif
	#ifndef PERF_ICACHE_MISSES
	#define PERF_ICACHE_MISSES -1
	#endif
	#ifndef PERF_ICACHE_STALLS
	#define PERF_ICACHE_STALLS -1
	#endif
	#ifndef PERF_DCACHE_HITS
	#define PERF_DCACHE_HITS   -1
	#endif
	#ifndef PERF_DCACHE_MISSES
	#define PERF_DCACHE_MISSES -1
	#endif
	#ifndef PERF_DCACHE_STALLS
	#define PERF_DCACHE_STALLS -1
	#endif
	#ifndef PERF_BUNDLES
	#define PERF_BUNDLES       -1
	#endif
	#ifndef PERF_BRANCH_TAKEN
	#define PERF_BRANCH_TAKEN  -1
	#endif
	#ifndef PERF_BRANCH_STALLS
	#define PERF_BRANCH_STALLS -1
	#endif
	#ifndef PERF_REG_STALLS
	#define PERF_REG_STALLS    -1
	#endif
	#ifndef PERF_ITLB_STALLS
	#define PERF_ITLB_STALLS   -1
	#endif
	#ifndef PERF_DTLB_STALLS
	#define PERF_DTLB_STALLS   -1
	#endif
	#ifndef PERF_STREAM_STALLS
	#define PERF_STREAM_STALLS -1
	#endif
	/**@}*/

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes performance monitors.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN void perf_setup(void);
	#else
	static inline void perf_setup(void)
	{
		/* noop. */
	}
	#endif

#endif

	/**
	 * @brief Queries a performance event.
	 *
	 * @param event Target event.
	 *
	 * @returns Upon successful completion, either zero or non zero is
	 * returned, indicating whether or not the queried performance event
	 * may be monitored in the underlying core. Upon failure, a negative
	 * error code is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN int perf_query(int event);
	#else
	static inline int perf_query(int event)
	{
		UNUSED(event);

		return (-ENOTSUP);
	}
	#endif

	/**
	 * @brief Starts a performance monitor.
	 *
	 * @param perf  Target performance monitor.
	 * @param event Target event to watch.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN int perf_start(int perf, int event);
	#else
	static inline int perf_start(int perf, int event)
	{
		UNUSED(perf);
		UNUSED(event);

		return (-ENOTSUP);
	}
	#endif

	/**
	 * @brief Stops a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN int perf_stop(int perf);
	#else
	static inline int perf_stop(int perf)
	{
		UNUSED(perf);

		return (-ENOTSUP);
	}
	#endif

	/**
	 * @brief Restarts a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN int perf_restart(int perf);
	#else
	static inline int perf_restart(int perf)
	{
		UNUSED(perf);

		return (-ENOTSUP);
	}
	#endif

	/**
	 * @brief Reads a PM register.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, the value of the target
	 * performance monitor. Upon failure, a negative error code,
	 * converted to uint64_t is returned instead.
	 */
	#if (CORE_HAS_PERF)
	EXTERN uint64_t perf_read(int perf);
	#else
	static inline uint64_t perf_read(int perf)
	{
		UNUSED(perf);

		return ((uint64_t) -ENOTSUP);
	}
	#endif

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* NANVIX_HAL_CORE_PERF_H_ */

