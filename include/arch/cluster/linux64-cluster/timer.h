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

#ifndef ARCH_CLUSTER_LINUX64_CLUSTER_TIMER_H_
#define ARCH_CLUSTER_LINUX64_CLUSTER_TIMER_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/linux64-cluster/_linux64-cluster.h>

/**
 * @addtogroup linux64-cluster-timer Timer
 * @ingroup linux64-cluster
 *
 * @brief Programmable Timer Interface
 */
/**@{*/

	#include <posix/stdint.h>

	#define INTERRUPT_TIMER 0

	/**
	 * @brief Cluster frequency.
	 *
	 * @note This is CLOCKS_PER_SEC but hard coded.
	 * @todo TODO: remove hard coded value.
	 */
	#define LINUX64_CLUSTER_CLUSTER_FREQ 1000000

#ifndef _ASM_FILE_

	/**
	 * @brief Reads the timestamp counter from the linux system.
	 *
	 * @returns The timestamp counter from the linux system.
	 *
	 * @author Daniel Coscia
	 */
	EXTERN uint64_t linux64_cluster_clock_read(void);


	/**
	 * @brief Initialize the timer at the current clock()
	 *
	 * @author Daniel Coscia
	 */
	EXTERN void linux64_timer_init(void);

#endif /* !_ASM_FILE_ */

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_cluster
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define CLUSTER_FREQ LINUX64_CLUSTER_CLUSTER_FREQ /**< @see LINUX64_CLUSTER_CLUSTER_FREQ */
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
	 * @see linux64_timer_init().
	 */
	static inline void __timer_init(unsigned freq)
	{
		UNUSED(freq);
		linux64_timer_init();
	}

	/**
	 * @brief Dummy function.
	 */
	static inline void timer_reset(void)
	{
		/* noop */
	}

	/**
	 * @see linux64_cluster_clock_read().
	 */
	static inline uint64_t clock_read(void)
	{
 		return (linux64_cluster_clock_read());
 	}

#endif /* !_ASM_FILE_ */

/**@endcond*/

/*============================================================================*/

/**@}*/

#endif /* ARCH_CLUSTER_LINUX64_CLUSTER_TIMER */
