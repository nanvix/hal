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

#ifndef ARCH_CORE_ARM64_PERF_H_
#define ARCH_CORE_ARM64_PERF_H_

/**
 * @addtogroup arm64-core-perf Perf Monitoring
 * @ingroup arm64-core
 *
 * @brief Performance Monitoring Interface
 */
/**@{*/

	/* Need arm64. */
	#define __NEED_CORE_ARM64

	#include <arch/core/arm64.h>
	#include <nanvix/const.h>
	#include <posix/errno.h>
	#include <posix/stdint.h>

	/**
	 * @brief Number of performance events.
	 */
	#define ARM64_PERF_EVENTS_NUM 9

	/**
	 * @brief Number of performance monitors.
	 */
	#define ARM64_PERF_MONITORS_NUM 1

	/**
	 * @name Performance Events
	 *
	 * @note We have tweaked these for Bluedragon cluster.
	 * Ref: see https://blog.printk.io/2015/10/using-the-perf-utility-on-arm/ and
	 */
	/**@{*/
	#define ARM64_PERF_ICACHE_REFILL		0x00 /**< Instruction cache misses                                    */
	#define ARM64_PERF_ITLB_STALLS        	0x01 /**< Instruction TLB Stalls                                      */
	#define ARM64_PERF_DCACHE_REFILL		0x02 /**< Data cache misses                                           */
	#define ARM64_PERF_DCACHE_HITS	 		0x03 /**< Data cache access                                           */
	#define ARM64_PERF_DTLB_STALL			0x04 /**< Data TLB Stalls                                             */
	#define ARM64_PERF_BR_MIS_PRED 			0x05 /**< Mispredicted or not predicted branch speculatively executed */
	#define ARM64_PERF_CPU_CYCLES			0x11 /**< Counts processor clock cycles                               */
	#define ARM64_PERF_BR_PRED				0x07 /**< Predictable branch speculatively executed                   */
	#define ARM64_PERF_ICACHE_HITS			0x08 /**< Instruction cache access                                    */
	/**@}*/

	/**
	 * @name Performance Events configuration
	 */
	/**@{*/
	#define ARM64_PMEVTYPER_EVTCOUNT_MASK 	0x3ff
	#define ARM64_PMCR_E            		(1 << 0)  /**< Enable all counters       */
	#define ARM64_PMCR_P            		(1 << 1)  /**< Reset all counters        */
	#define ARM64_PMCR_C            		(1 << 2)  /**< Cycle counter reset       */
	#define ARM64_PMUSERENR_EN_EL0  		(1 << 0)  /**< EL0 access enable         */
	#define ARM64_PMUSERENR_CR      		(1 << 2)  /**< Cycle counter read enable */
	#define ARM64_PMUSERENR_ER      		(1 << 3)  /**< Event counter read enable */
	#define ARM64_PMCNTENSET_EL0_ENABLE 	(1 << 31) /**< Enable Perf count reg     */
	/**@}*/

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes performance monitors.
	 */
	EXTERN void arm64_perf_setup(void);

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
	EXTERN int arm64_perf_start(int perf, int event);

	/**
	 * @brief Stops a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int arm64_perf_stop(int perf);

	/**
	 * @brief Restarts a performance monitor.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int arm64_perf_restart(int perf);

	/**
	 * @brief Reads a PM register.
	 *
	 * @param perf Target performance monitor.
	 *
	 * @returns Upon successful completion, the value of the target
	 * performance monitor. Upon failure, -1 converted to uint64_t is
	 * returned instead.
	 */
	EXTERN uint64_t arm64_perf_read(int perf);

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define PERF_MONITORS_NUM  	ARM64_PERF_MONITORS_NUM  	/**< @ref ARM64_PERF_MONITORS_NUM  */
	#define PERF_EVENTS_NUM    	ARM64_PERF_EVENTS_NUM    	/**< @ref ARM64_PERF_EVENTS_NUM    */
	#define PERF_CYCLES        	ARM64_PERF_CPU_CYCLES    	/**< @ref ARM64_PERF_CPU_CYCLES    */
	#define PERF_ICACHE_MISSES 	ARM64_PERF_ICACHE_REFILL   	/**< @ref ARM64_PERF_ICACHE_REFILL */
	#define PERF_DCACHE_MISSES 	ARM64_PERF_DCACHE_REFILL   	/**< @ref ARM64_PERF_DCACHE_REFILL */
	#define PERF_ICACHE_HITS 	ARM64_PERF_ICACHE_HITS
	#define PERF_DCACHE_HITS 	ARM64_PERF_DCACHE_HITS
	#define PERF_BRANCH_STALLS 	ARM64_PERF_BR_MIS_PRED 		/**< @ref ARM64_PERF_BR_MIS_PRED */
	#define PERF_ICACHE_STALLS 	ARM64_PERF_ITLB_STALLS 		/**< @ref ARM64_PERF_ITLB_STALLS */
	#define PERF_DCACHE_STALLS 	ARM64_PERF_DTLB_STALL    	/**< @ref ARM64_PERF_DTLB_STALL  */
	#define PERF_REG_STALLS    	-1    						/**< Not Implemented             */
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
	 * @see arm64_perf_setup().
	 */
	static inline void perf_setup(void)
	{
		arm64_perf_setup();
	}

#endif

	/**
	 * @see arm64_perf_start().
 	 */
	static inline int perf_start(int perf, int event)
	{
		return (arm64_perf_start(perf, event));
	}

	/**
	 * @see arm64_perf_stop().
	 */
	static inline int perf_stop(int perf)
	{
		return (arm64_perf_stop(perf));
	}

	/**
	 * @see arm64_perf_restart().
	 */
	static inline int perf_restart(int perf)
	{
		return (arm64_perf_restart(perf));
	}

	/**
	 * @see arm64_perf_read().
	 */
	static inline uint64_t perf_read(int perf)
	{
		return (arm64_perf_read(perf));
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_PERF_H_ */
