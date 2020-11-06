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

#ifndef NANVIX_HAL_CLUSTER_EVENT_H_
#define NANVIX_HAL_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if (CLUSTER_HAS_EVENTS)
#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_EVENT)

	/* Functions */
	#ifndef __event_notify_fn
	#error "event_notify() not defined?"
	#endif
	#ifndef __event_wait_fn
	#error "event_wait() not defined?"
	#endif
	#ifndef __event_reset_fn
	#error "event_reset() not defined?"
	#endif

#endif
#endif

/*============================================================================*
 * Event Interface                                                            *
 *============================================================================*/

/**
 * @defgroup kernel-hal-cluster-event Event
 * @ingroup kernel-hal-cluster
 *
 * @brief Event Interface
 */
/**@{*/

	/**
	 * @brief Hardware interrupt handler.
	 */
	typedef void (*event_handler_t)(void);

	/**
	 * @brief Notifies a local core about an event.
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN int event_notify(int coreid);

	/**
	 * @brief Waits for an event.
	 */
	EXTERN void event_wait(void);

	/**
	 * @brief Drops any pending events in the local core.
	 */
	EXTERN void event_drop(void);

	/**
	 * @brief Resets the pending IPI interrupt.
	 *
	 * If the current architecture have events and have a valid
	 * interrupt number for the IPI handler, exports the function,
	 * otherwise, defines a dummy reset function.
	 */
	static inline void event_reset(void)
	{
#if (CLUSTER_HAS_EVENTS)
		__event_reset();
#endif
	}

	/**
	 * @brief Register a event handler.
	 *
	 * @param handler Handler function.
	 *
	 * @return Zero if successfully register, non-zero otherwize.
	 */
	EXTERN int event_register_handler(event_handler_t handler);

	/**
	 * @brief Resets event handler.
	 */
	EXTERN int event_unregister_handler(void);

	/**
	 * @brief Initializes the events table.
	 */
	EXTERN void event_setup(void);

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_EVENT_H_ */

