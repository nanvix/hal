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

#ifndef ARCH_CORE_RV32GC_PERF_H_
#define ARCH_CORE_RV32GC_PERF_H_

/**
 * @addtogroup RV32GC-core-perf Perf Monitoring
 * @ingroup RV32GC-core
 *
 * @brief Performance Monitoring Interface
 */
/**@{*/

	#include <nanvix/const.h>
	#include <linux/perf_event.h>
	#include <posix/stdint.h>

	/**
	 * @brief Number of performance events.
	 */
	#define RV32GC_PERF_EVENTS_NUM 14

	/**
	 * @brief Number of performance monitors.
	 */
	#define RV32GC_PERF_MONITORS_NUM 1

	/**
	 * @name Performance Events
	 */
	/**@{*/
	#define RV32GC_PERF_CYCLES         PERF_COUNT_HW_CPU_CYCLES                      /**< Timer Cycles                    */
	#define RV32GC_PERF_ICACHE_HITS    (PERF_COUNT_HW_CACHE_L1I) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Instruction Cache Hits          */
	#define RV32GC_PERF_ICACHE_MISSES  (PERF_COUNT_HW_CACHE_L1I) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16) /**< Instruction Cache Misses        */
	#define RV32GC_PERF_ICACHE_STALLS  (PERF_COUNT_HW_CACHE_L1I) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Instruction Cache Misses Stalls */
	#define RV32GC_PERF_DCACHE_HITS    (PERF_COUNT_HW_CACHE_L1D) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Data Cache Hits                 */
	#define RV32GC_PERF_DCACHE_MISSES  (PERF_COUNT_HW_CACHE_L1D) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16) /**< Data Cache Misses               */
	#define RV32GC_PERF_DCACHE_STALLS  (PERF_COUNT_HW_CACHE_L1D) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Data Cache Misses Stalls        */
	#define RV32GC_PERF_BUNDLES        0                                             /**< Bundles Executed                */
	#define RV32GC_PERF_BRANCH_TAKEN   PERF_COUNT_HW_BRANCH_INSTRUCTIONS             /**< Branches Taken                  */
	#define RV32GC_PERF_BRANCH_STALLS  PERF_COUNT_HW_BRANCH_MISSES                   /**< Branches Stalled                */
	#define RV32GC_PERF_REG_STALLS     PERF_SAMPLE_REGS_INTR                         /**< Register Dependence Stalls      */
	#define RV32GC_PERF_ITLB_STALLS    (PERF_COUNT_HW_CACHE_ITLB) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Instruction TLB Stalls          */
	#define RV32GC_PERF_DTLB_STALLS    (PERF_COUNT_HW_CACHE_DTLB) |\
											(PERF_COUNT_HW_CACHE_OP_READ << 8) |\
											(PERF_COUNT_HW_CACHE_RESULT_MISS << 16)   /**< Data TLB Stalls                 */
	#define RV32GC_PERF_STREAM_STALLS  PERF_SAMPLE_REGS_INTR                         /**< Stream Buffer Stalls            */
	#define RV32GC_PERF_NUM_INSTRUCTIONS PERF_COUNT_HW_INSTRUCTIONS                 /**< Hardware Instructions            */
	/**@}*/

	#define RV32GC_PERF_ARG1  0 /**< Default arg1 */
	#define RV32GC_PERF_ARG2 -1 /**< Default arg2 */
	#define RV32GC_PERF_ARG3 -1 /**< Default arg3 */
	#define RV32GC_PERF_ARG4  0 /**< Default arg4 */

	/**
	 * @Brief Array of File Descriptor containing the values of the counters
	 */
	EXTERN int rv32gc_perf_monitors[RV32GC_PERF_MONITORS_NUM];

	/**
	 * @Brief Check if the perf and event values are valide
	 */
	EXTERN int perf_isvalid(int perf);

	EXTERN int event_isvalid(int event);

	/**
	 * @brief Initializes performance monitors.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN void rv32gc_perf_setup(void);

	/**
	 * @brief Starts a performance monitor.
	 *
	 * @param perf  Target performance monitor.
	 * @param event Target event to watch.
	 *
	 * @returns Upon successful completion, zero is returned.
	 * Upon failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_perf_start(int perf, int event);

	/**
	 * @brief Stops a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_perf_stop(int perf);

	/**
	 * @brief Restarts a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_perf_restart(int perf);

	/**
	 * @brief Reads a PM register.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, the value of the target
	 * performance monitor.
	 */
	EXTERN uint64_t rv32gc_perf_read(int perf);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond RV32GC
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define PERF_MONITORS_NUM  RV32GC_PERF_MONITORS_NUM        /**< @ref RV32GC_PERF_MONITORS_NUM     */
	#define PERF_EVENTS_NUM    RV32GC_PERF_EVENTS_NUM          /**< @ref RV32GC_PERF_EVENTS_NUM       */
	#define PERF_CYCLES        RV32GC_PERF_CYCLES              /**< @ref RV32GC_PERF_CYCLES           */
	#define PERF_ICACHE_HITS   RV32GC_PERF_ICACHE_HITS         /**< @ref RV32GC_PERF_ICACHE_HITS      */
	#define PERF_ICACHE_MISSES RV32GC_PERF_ICACHE_MISSES       /**< @ref RV32GC_PERF_ICACHE_MISSES    */
	#define PERF_ICACHE_STALLS RV32GC_PERF_ICACHE_STALLS       /**< @ref RV32GC_PERF_ICACHE_STALLS    */
	#define PERF_DCACHE_HITS   RV32GC_PERF_DCACHE_HITS         /**< @ref RV32GC_PERF_DCACHE_HITS      */
	#define PERF_DCACHE_MISSES RV32GC_PERF_DCACHE_MISSES       /**< @ref RV32GC_PERF_DCACE_MISSES     */
	#define PERF_DCACHE_STALLS RV32GC_PERF_DCACHE_STALLS       /**< @ref RV32GC_PERF_DCACHE_STALLS    */
	#define PERF_BUNDLES       RV32GC_PERF_BUNDLES             /**< @ref RV32GC_PERF_BUNDLES          */
	#define PERF_BRANCH_TAKEN  RV32GC_PERF_BRANCH_TAKEN        /**< @ref RV32GC_PERF_BRANCH_TAKEN     */
	#define PERF_BRANCH_STALLS RV32GC_PERF_BRANCH_STALLS       /**< @ref RV32GC_PERF_BRANCH_STALLS    */
	#define PERF_REG_STALLS    RV32GC_PERF_REG_STALLS          /**< @ref RV32GC_PERF_REG_STALLS       */
	#define PERF_ITLB_STALLS   RV32GC_PERF_ITLB_STALLS         /**< @ref RV32GC_PERF_ITLB_STALLS      */
	#define PERF_DTLB_STALLS   RV32GC_PERF_DTLB_STALLS         /**< @ref RV32GC_PERF_DTLB_STALLS      */
	#define PERF_STREAM_STALLS RV32GC_PERF_STREAM_STALLS       /**< @ref RV32GC_PERF_STREAM_STALLS    */
	#define PERF_NUM_INSTRUCTIONS RV32GC_PERF_NUM_INSTRUCTIONS /**< @ref RV32GC_PERF_NUM_INSTRUCTIONS */

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

	/**
	 * @see rv32gc_perf_setup().
	 */
	static inline void perf_setup(void)
	{
		rv32gc_perf_setup();
	}

	/**
	 * @see rv32gc_perf_start().
 	 */
	static inline int perf_start(int perf, int event)
	{
		return (rv32gc_perf_start(perf, event));
	}

	/**
	 * @see rv32gc_perf_stop().
	 */
	static inline int perf_stop(int perf)
	{
		return (rv32gc_perf_stop(perf));
	}

	/**
	 * @see rv32gc_perf_restart().
	 */
	static inline int perf_restart(int perf)
	{
		return (rv32gc_perf_restart(perf));
	}

	/**
	 * @see rv32gc_perf_read().
	 */
	static inline uint64_t perf_read(int perf)
	{
		return (rv32gc_perf_read(perf));
	}

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_PERF_H_ */
