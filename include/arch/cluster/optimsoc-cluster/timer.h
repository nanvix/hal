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

#ifndef ARCH_CLUSTER_OPTIMSOC_CLUSTER_TIMER_H_
#define ARCH_CLUSTER_OPTIMSOC_CLUSTER_TIMER_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

/**
 * @addtogroup optimsoc-cluster-timer Timer
 * @ingroup optimsoc-cluster
 *
 * @brief Integrated Timer Device
 */
/**@{*/

	#include <nanvix/const.h>

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @optimsoc_cluster
 */

	/**
	 * @brief Estimated CPU frequency (in Hz), 50Mhz.
	 */
	#define OR1K_CPU_FREQUENCY 50000000

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __timer_init_fn  /**< timer_init()  */
	#define __timer_reset_fn /**< timer_reset() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see or1k_timer_init().
	 */
	static inline void timer_init(unsigned freq)
	{
		or1k_timer_init(freq);
	}

	/**
	 * @see or1k_timer_reset().
	 */
	static inline void timer_reset(void)
	{
		or1k_timer_reset();
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_OPTIMSOC_CLUSTER_TIMER */
