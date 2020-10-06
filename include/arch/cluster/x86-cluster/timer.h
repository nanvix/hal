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

#ifndef ARCH_CLUSTER_X86_CLUSTER_TIMER_H_
#define ARCH_CLUSTER_X86_CLUSTER_TIMER_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/x86-cluster/_x86-cluster.h>

/**
 * @addtogroup x86-cluster-timer Timer
 * @ingroup x86-cluster
 *
 * @brief Integrated Timer Device
 */
/**@{*/

	#include <nanvix/const.h>
	#include <posix/stdint.h>

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @x86_cluster
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define CLUSTER_FREQ PIT_FREQUENCY /**< @see PIT_FREQUENCY */
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
	 * @see i486_timer_init().
	 */
	#define __timer_init(freq) \
		i486_timer_init(freq)

	/**
	 * @see i486_timer_reset().
	 */
	static inline void timer_reset(void)
	{
	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

/*============================================================================*/

/**@}*/

#endif /* ARCH_CLUSTER_X86_CLUSTER_TIMER */
