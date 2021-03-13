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

#ifndef ARCH_CLUSTER_ARM64_CLUSTER_TIMER_H_
#define ARCH_CLUSTER_ARM64_CLUSTER_TIMER_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/arm64-cluster/_arm64-cluster.h>

/**
 * @addtogroup arm64-cluster-timer Timer
 * @ingroup arm64-cluster
 *
 * @brief 64-bit Timer
 */
/**@{*/

	#include <posix/stdint.h>

	/**
	 * @brief Timer frequency (10 MHz)
	 */
	#define ARM64_CLUSTER_TIMEBASE 10000000

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64_cluster
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define CLUSTER_FREQ ARM64_CLUSTER_TIMEBASE /**< @see ARM64_CLUSTER_TIMEBASE */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __timer_init_fn   /**< timer_init() */
	#define __timer_reset_fn /**< timer_reset() */
	#define __clock_read_fn  /**< clock_read()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see timer_init().
	 */
	static inline void timer_init(unsigned freq)
	{
		UNUSED(freq);
	}

	/**
	 * @see arm64_timer_reset().
	 */
	static inline void timer_reset(void)
	{
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

/*============================================================================*/

/**@}*/

#endif /* ARCH_CLUSTER_ARM64_CLUSTER_TIMER */
