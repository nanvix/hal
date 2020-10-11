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

#ifndef ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_
#define ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/k1b-cluster/_k1b-cluster.h>

/**
 * @addtogroup k1b-cluster-event Events
 * @ingroup k1b-cluster
 *
 * @brief Events Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Event line used for events.
	 */
	#define K1B_EVENT_LINE 0

#ifndef _ASM_FILE_

	/**
	 * @brief Notifies a local core about an event.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline void k1b_cluster_event_notify(int coreid)
	{
		mOS_pe_notify(
			1 << coreid,    /* Target cores.                            */
			K1B_EVENT_LINE, /* Event line.                              */
			1,              /* Notify an event? (I/O clusters only)     */
			0               /* Notify an interrupt? (I/O clusters only) */
		);
	}

	/**
	 * @brief Waits for an event.
	 */
	static inline void k1b_cluster_event_wait(void)
	{
		mOS_pe_event_waitclear(K1B_EVENT_LINE);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __event_notify_fn /**< event_notify() */
	#define __event_wait_fn   /**< event_wait()   */
	#define __event_reset_fn  /**< event_reset()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_cluster_event_notify().
	 */
	static inline void __event_notify(int coreid)
	{
		k1b_cluster_event_notify(coreid);
	}

	/**
	 * @see k1b_cluster_event_wait().
	 */
	static inline void __event_wait(void)
	{
		k1b_cluster_event_wait();
	}

	/**
	 * @brief Dummy function.
	 */
	static inline void __event_reset(void)
	{
		/* noop. */
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_ */

