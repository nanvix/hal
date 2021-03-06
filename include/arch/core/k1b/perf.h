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

#ifndef ARCH_CORE_K1B_PERF_H_
#define ARCH_CORE_K1B_PERF_H_

/**
 * @addtogroup k1b-core-perf Perf Monitoring
 * @ingroup k1b-core
 *
 * @brief Performance Monitoring Interface
 */
/**@{*/

	#include <arch/core/k1b/mOS.h>
	#include <nanvix/const.h>
	#include <posix/errno.h>
	#include <posix/stdint.h>

	/**
	 * @brief Number of performance events.
	 */
	#define K1B_PERF_EVENTS_NUM 14

	/**
	 * @brief Number of performance monitors.
	 */
	#define K1B_PERF_MONITORS_NUM 2

	/**
	 * @name Performance Monitors 
	 */
	/**@{*/
	#define K1B_PERF_PM_0_1 0 /**< Performance Monitor 0 + 1 */
	#define K1B_PERF_PM_2_3 1 /**< Performance Monitor 2 + 3 */
	/**@}*/

	/**
	 * @name Performance Events
	 */
	/**@{*/
	#define K1B_PERF_CYCLES         _K1_CYCLE_COUNT        /**< Timer Cycles                    */
	#define K1B_PERF_ICACHE_HITS    _K1_IHITS              /**< Instruction Cache Hits          */
	#define K1B_PERF_ICACHE_MISSES  _K1_IMISS              /**< Instruction Cache Misses        */
	#define K1B_PERF_ICACHE_STALLS  _K1_IMISS_STALLS       /**< Instruction Cache Misses Stalls */
	#define K1B_PERF_DCACHE_HITS    _K1_DHITS              /**< Data Cache Hits                 */
	#define K1B_PERF_DCACHE_MISSES  _K1_DMISS              /**< Data Cache Misses               */
	#define K1B_PERF_DCACHE_STALLS  _K1_DMISS_STALLS       /**< Data Cache Misses Stalls        */
	#define K1B_PERF_BUNDLES        _K1_EXEC_BUNDLES       /**< Bundles Executed                */
	#define K1B_PERF_BRANCH_TAKEN   _K1_BR_TAKEN           /**< Branches Taken                  */
	#define K1B_PERF_BRANCH_STALLS  _K1_BR_TAKEN_STALLS    /**< Branches Stalled                */
	#define K1B_PERF_REG_STALLS     _K1_RAW_STALLS         /**< Register Dependence Stalls      */
	#define K1B_PERF_ITLB_STALLS    _K1_ITLB_STALLS        /**< Instruction TLB Stalls          */
	#define K1B_PERF_DTLB_STALLS    _K1_DTLB_STALLS        /**< Data TLB Stalls                 */
	#define K1B_PERF_STREAM_STALLS  _K1_STREAM_LOAD_STALLS /**< Stream Buffer Stalls            */
	/**@}*/

#ifdef __NANVIX_HAL

	/**
	 * @name Commands
	 */
	/**@{*/
	#define K1B_PERF_START(r,x) ((x) << ((r) << 2)) /**< Stop Counter                */
	#define K1B_PERF_STOP(r)    (0xe << ((r) << 2)) /**< Stop Counter                */
	#define K1B_PERF_RESET(r)   (0xf << ((r) << 2)) /**< Reset Counter to Zero       */
	#define K1B_PERF_MASK(r)    (0xf << ((r) << 2)) /**< Mask for Counter            */
	#define K1B_PERF_CHAIN_0_1  (1 << 16)           /**< Enable PM0 and PM1 Chaining */
	#define K1B_PERF_CHAIN_2_3  (1 << 17)           /**< Enable PM2 and PM3 Chaining */
	#define K1B_PERF_DMC        (1 << 18)           /**< Disengage Monitors Timer    */
	#define K1B_PERF_SMP        (1 << 19)           /**< Stop Monitors in Privileged */
	#define K1B_PERF_SMD        (1 << 20)           /**< Stop Monitors in Debug      */
	/**@}*/

#endif /* __NANVIX_HAL */

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes performance monitors.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN void k1b_perf_setup(void);

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
	EXTERN int k1b_perf_start(int perf, int event);

	/**
	 * @brief Stops a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_perf_stop(int perf);

	/**
	 * @brief Restarts a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int k1b_perf_restart(int perf);

	/**
	 * @brief Reads a PM register.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, the value of the target
	 * performance monitor. Upon failure, -1 converted to uint64_t is
	 * returned instead.
	 */
	static inline uint64_t k1b_pm_read(int perf)
	{
		uint32_t hi;
		uint32_t lo;

		switch (perf)
		{
			case K1B_PERF_PM_0_1:
				asm volatile ("get %0, $pm0;;": "=r" (lo));
				asm volatile ("get %0, $pm1;;": "=r" (hi));
				break;

			case K1B_PERF_PM_2_3:
				asm volatile ("get %0, $pm2;;": "=r" (lo));
				asm volatile ("get %0, $pm3;;": "=r" (hi));
				break;

			default:
				return ((uint64_t) - 1);
				break;
		}

		return ((((uint64_t) hi) << 32ull) | (lo));
	}

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define PERF_MONITORS_NUM  K1B_PERF_MONITORS_NUM   /**< @ref K1B_PERF_MONITORS_NUM  */
	#define PERF_EVENTS_NUM    K1B_PERF_EVENTS_NUM     /**< @ref K1B_PERF_EVENTS_NUM    */
	#define PERF_CYCLES        K1B_PERF_CYCLES         /**< @ref K1B_PERF_CYCLES        */
	#define PERF_ICACHE_HITS   K1B_PERF_ICACHE_HITS    /**< @ref K1B_PERF_ICACHE_HITS   */
	#define PERF_ICACHE_MISSES K1B_PERF_ICACHE_MISSES  /**< @ref K1B_PERF_ICACHE_MISSES */
	#define PERF_ICACHE_STALLS K1B_PERF_ICACHE_STALLS  /**< @ref K1B_PERF_ICACHE_STALLS */
	#define PERF_DCACHE_HITS   K1B_PERF_DCACHE_HITS    /**< @ref K1B_PERF_DCACHE_HITS   */
	#define PERF_DCACHE_MISSES K1B_PERF_DCACHE_MISSES  /**< @ref K1B_PERF_DCACE_MISSES  */
	#define PERF_DCACHE_STALLS K1B_PERF_DCACHE_STALLS  /**< @ref K1B_PERF_DCACHE_STALLS */
	#define PERF_BUNDLES       K1B_PERF_BUNDLES        /**< @ref K1B_PERF_BUNDLES       */
	#define PERF_BRANCH_TAKEN  K1B_PERF_BRANCH_TAKEN   /**< @ref K1B_PERF_BRANCH_TAKEN  */
	#define PERF_BRANCH_STALLS K1B_PERF_BRANCH_STALLS  /**< @ref K1B_PERF_BRANCH_STALLS */
	#define PERF_REG_STALLS    K1B_PERF_REG_STALLS     /**< @ref K1B_PERF_REG_STALLS    */
	#define PERF_ITLB_STALLS   K1B_PERF_ITLB_STALLS    /**< @ref K1B_PERF_ITLB_STALLS   */
	#define PERF_DTLB_STALLS   K1B_PERF_DTLB_STALLS    /**< @ref K1B_PERF_DTLB_STALLS   */
	#define PERF_STREAM_STALLS K1B_PERF_STREAM_STALLS  /**< @ref K1B_PERF_STREAM_STALLS */
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
	 * @see k1b_perf_setup().
	 */
	static inline void perf_setup(void)
	{
		k1b_perf_setup();
	}

#endif

	/**
	 * @see k1b_perf_start().
 	 */
	static inline int perf_start(int perf, int event)
	{
		return (k1b_perf_start(perf, event));
	}

	/**
	 * @see k1b_perf_stop().
	 */
	static inline int perf_stop(int perf)
	{
		return (k1b_perf_stop(perf));
	}

	/**
	 * @see k1b_perf_restart().
	 */
	static inline int perf_restart(int perf)
	{
		return (k1b_perf_restart(perf));
	}

	/**
	 * @see k1b_perf_read().
	 */
	static inline uint64_t perf_read(int perf)
	{
		return (k1b_pm_read(perf));
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_PERF_H_ */
