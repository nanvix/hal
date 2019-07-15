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

#ifndef ARCH_CLUSTER_LINUX64_CLUSTER_EVENT_H_
#define ARCH_CLUSTER_LINUX64_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/linux64-cluster/_linux64-cluster.h>

/**
 * @addtogroup linux64-cluster-event Events
 * @ingroup linux64-cluster
 *
 * @brief Events Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Notifies a local core about an event.
	 *
	 */
	EXTERN void linux64_cluster_event_notify(int coreid);

	/**
	 * @brief Waits for an event.
	 */
	EXTERN void linux64_cluster_event_wait(void);

    /**
	 * @brief Setup the event table.
	 */
    EXTERN void linux64_cluster_event_setup(void);

    /**
	 * @brief Drop all the pending events.
	 */
    EXTERN void linux64_cluster_event_drop(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __event_notify_fn /**< event_notify() */
	#define __event_wait_fn   /**< event_wait()   */
	/**@}*/

	/**
	 * @see linux64_cluster_event_notify().
	 */
	static inline void event_notify(int coreid)
	{
		linux64_cluster_event_notify(coreid);
	}

	/**
	 * @see linux64_cluster_event_wait().
	 */
	static inline void event_wait(void)
	{
		linux64_cluster_event_wait();
	}

    /**
	 * @see linux64_cluster_event_drop().
	 */
    static inline void event_drop(void)
    {
        linux64_cluster_event_drop();
    }

    /**
	 * @see linux64_cluster_event_setup().
	 */
    static inline void event_setup(void)
    {
        linux64_cluster_event_setup();
    }

/**@endcond*/

#endif /* ARCH_CLUSTER_LINUX64_CLUSTER_EVENT_H_ */

