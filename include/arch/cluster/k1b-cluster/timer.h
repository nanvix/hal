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

#ifndef ARCH_CLUSTER_K1B_CLUSTER_TIMER_H_
#define ARCH_CLUSTER_K1B_CLUSTER_TIMER_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/k1b-cluster/_k1b-cluster.h>

/**
 * @addtogroup k1b-cluster-timer Timer
 * @ingroup k1b-cluster
 *
 * @brief Programmable Timer Interface
 */
/**@{*/

	#include <posix/stdint.h>

	/**
	 * @brief Cluster frequency.
	 */
	#define K1B_CLUSTER_FREQ 400000000

#ifndef _ASM_FILE_

	/**
	 * @brief Reads the timestamp counter from the DSU.
	 *
	 * @returns The timestamp counter from the DSU.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline uint64_t k1b_cluster_clock_read(void)
	{
		return (__k1_read_dsu_timestamp());
	}

#endif /* !_ASM_FILE_ */

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b_cluster
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define CLUSTER_FREQ K1B_CLUSTER_FREQ /**< @see K1B_CLUSTER_FREQ */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __timer_init_fn  /**< timer_init()  */
	#define __timer_reset_fn /**< timer_reset() */
	#define __clock_read_fn  /**< clock_read()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_timer_init().
	 */
	#define __timer_init(freq) \
		k1b_timer_init(freq)

	/**
	 * Dummy function
	 */
	static inline void timer_reset(void)
	{
		/* noop */
	}

	/**
	 * @see k1b_cluster_clock_read().
	 */
	#define clock_read(void) k1b_cluster_clock_read(void)

#endif /* !_ASM_FILE_ */

/**@endcond*/

/*============================================================================*/

/**@}*/

#endif /* ARCH_CLUSTER_K1B_CLUSTER_TIMER */
