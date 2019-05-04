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

#ifndef ARCH_CLUSTER_OPTIMSOC_CLUSTER_EVENT_H_
#define ARCH_CLUSTER_OPTIMSOC_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

/**
 * @addtogroup optimsoc-cluster-event Events
 * @ingroup optimsoc-cluster
 *
 * @brief Events Interface
 */
/**@{*/

	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Notifies a local core about an event.
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN int or1k_cluster_event_notify(int coreid);

	/**
	 * @brief Waits for an event.
	 */
	EXTERN int or1k_cluster_event_wait(void);

	/**
	 * @brief Waits for an event.
	 */
	EXTERN void or1k_cluster_event_reset(void);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond optimsoc_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __event_notify_fn /**< event_notify() */
	#define __event_wait_fn   /**< event_wait()   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see or1k_cluster_event_notify()
	 */
	static inline void optimsoc_cluster_event_notify(int coreid)
	{
		or1k_cluster_event_notify(coreid);
	}

	/**
	 * @see or1k_cluster_event_wait().
	 */
	static inline void optimsoc_cluster_event_wait(void)
	{
		or1k_cluster_event_wait();
	}

	/**
	 * @see or1k_cluster_event_reset().
	 */
	static inline void optimsoc_cluster_event_reset(void)
	{
		or1k_cluster_event_reset();
	}

	/**
	 * @see optimsoc_cluster_event_notify()
	 */
	static inline void event_notify(int coreid)
	{
		optimsoc_cluster_event_notify(coreid);
	}

	/**
	 * @see optimsoc_cluster_event_wait().
	 */
	static inline void event_wait(void)
	{
		optimsoc_cluster_event_wait();
	}

	/**
	 * @see optimsoc_cluster_event_reset().
	 */
	static inline void event_reset(void)
	{
		optimsoc_cluster_event_reset();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_OPTIMSOC_EVENT_H_ */
