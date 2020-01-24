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

#ifndef ARCH_CORE_MOR1KX_PERF_H_
#define ARCH_CORE_MOR1KX_PERF_H_

/**
 * @addtogroup mor1kx-core-perf Perf Monitoring
 * @ingroup mor1kx-core
 *
 * @brief Performance Monitoring Interface
 */
/**@{*/

	/* Need mo1kx. */
	#define __NEED_CORE_MOR1KX

	#include <arch/core/mor1kx.h>
	#include <nanvix/const.h>
	#include <posix/errno.h>
	#include <posix/stdint.h>

	/**
	 * @brief Number of performance events.
	 */
	#define MOR1KX_PERF_EVENTS_NUM 7

	/**
	 * @brief Number of performance monitors.
	 */
	#define MOR1KX_PERF_MONITORS_NUM 2

	/**
	 * @brief Event Offset.
	 */
	#define MOR1KX_PERF_EVENT_OFFSET 4

	/**
	 * @name Performance Events
	 *
	 * @note We have tweaked these for Bluedragon cluster.
	 */
	/**@{*/
	#define MOR1KX_PERF_LSU_HITS      0 /**< LSU Hits             */
	#define MOR1KX_PERF_BRANCH_STALLS 1 /**< Branch Stalls        */
	#define MOR1KX_PERF_ICACHE_HITS   2 /**< I-Cache Hits         */
	#define MOR1KX_PERF_REG_STALLS    3 /**< Register File Stalls */
	#define MOR1KX_PERF_ICACHE_MISSES 4 /**< I-Cache Misses       */
	#define MOR1KX_PERF_IFETCH_STALLS 5 /**< I-Fetch Stalls       */
	#define MOR1KX_PERF_LSU_STALLS    6 /**< LSU Stalls           */
	/**@}*/

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes performance monitors.
	 */
	EXTERN void mor1kx_perf_setup(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Starts a performance monitor.
	 *
	 * @param perf  Target performance monitor.
	 * @param event Target event to watch.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int mor1kx_perf_start(int perf, int event);

	/**
	 * @brief Stops a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int mor1kx_perf_stop(int perf);

	/**
	 * @brief Restarts a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int mor1kx_perf_restart(int perf);

	/**
	 * @brief Reads a PM register.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, the value of the target
	 * performance monitor. Upon failure, -1 converted to uint64_t is
	 * returned instead.
	 */
	EXTERN uint64_t mor1kx_perf_read(int perf);

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond mor1kx
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define PERF_MONITORS_NUM  MOR1KX_PERF_MONITORS_NUM  /**< @ref MOR1KX_PERF_MONITORS_NUM  */
	#define PERF_EVENTS_NUM    MOR1KX_PERF_EVENTS_NUM    /**< @ref MOR1KX_PERF_EVENTS_NUM    */
	#define PERF_CYCLES        -1                        /**< Not present.                   */
	#define PERF_ICACHE_MISSES MOR1KX_PERF_ICACHE_HITS   /**< @ref MOR1KX_PERF_ICACHE_MISSES */
	#define PERF_DCACHE_MISSES -1                        /**< Not present                    */
	#define PERF_BRANCH_STALLS MOR1KX_PERF_BRANCH_STALLS /**< @ref MOR1KX_PERF_BRANCH_STALLS */
	#define PERF_ICACHE_STALLS MOR1KX_PERF_IFETCH_STALLS /**< @ref MOR1KX_PERF_IFETCH_STALLS */
	#define PERF_DCACHE_STALLS MOR1KX_PERF_LSU_STALLS    /**< @ref MOR1KX PERF_LSU_STALLS    */
	#define PERF_REG_STALLS    MOR1KX_PERF_REG_STALLS    /**< @ref MOR1KX_PERF_REG_STALLS    */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __perf_setup_fn   /**< perf_setup()   */
	#define __perf_start_fn   /**< perf_start()   */
	#define __perf_stop_fn    /**< perf_stop()    */
	#define __perf_restart_fn /**< perf_restart() */
	#define __perf_read_fn    /**< perf_read()    */
	/**@}*/

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @see mor1kx_perf_setup().
	 */
	static inline void perf_setup(void)
	{
		mor1kx_perf_setup();
	}

#endif

	/**
	 * @see mor1kx_perf_start().
 	 */
	static inline int perf_start(int perf, int event)
	{
		return (mor1kx_perf_start(perf, event));
	}

	/**
	 * @see mor1kx_perf_stop().
	 */
	static inline int perf_stop(int perf)
	{
		return (mor1kx_perf_stop(perf));
	}

	/**
	 * @see mor1kx_perf_restart().
	 */
	static inline int perf_restart(int perf)
	{
		return (mor1kx_perf_restart(perf));
	}

	/**
	 * @see mor1kx_perf_read().
	 */
	static inline uint64_t perf_read(int perf)
	{
		return (mor1kx_perf_read(perf));
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_MOR1KX_PERF_H_ */
