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

/* Must come first. */
#define __NEED_HAL_CLUSTER
#define __NEED_SECTION_GUARD

#include <nanvix/hal/cluster.h>
#include <nanvix/hal/section_guard.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * @brief Table of events.
 */
PUBLIC struct events_table
{
	unsigned pending; /**< Pending Events  */
	unsigned handled; /**< Handled Events  */
} events[CORES_NUM];

/**
 * @brief Event system lock.
 */
spinlock_t event_lock;

/**
 * @brief Handler to IPI events.
 */
PRIVATE event_handler_t _event_handler = NULL;

/*============================================================================*
 * event_handler()                                                            *
 *============================================================================*/

/**
 * @brief IPI handler.
 */
PRIVATE void event_handler(int ev_src)
{
	int coreid = core_get_id();

	UNUSED(ev_src);

#if (CLUSTER_HAS_IPI)
	cluster_ipi_ack();
#endif

#if (CLUSTER_HAS_IPI)
	if (LIKELY(ev_src > 0))
		spinlock_lock(&event_lock);
#else
	KASSERT(ev_src < 0);
#endif

	/* Handle event. */
	for (int i = 0; i < CORES_NUM; i++)
	{
		if (events[coreid].pending & (1 << i))
		{
			events[coreid].pending &= ~(1 << i);
			events[coreid].handled |=  (1 << i);

			spinlock_unlock(&event_lock);
				_event_handler();
			spinlock_lock(&event_lock);

			/* We maybe change the core. */
			coreid = core_get_id();
		}
	}

#if (CLUSTER_HAS_IPI)
	if (LIKELY(ev_src > 0))
		spinlock_unlock(&event_lock);
#endif
}

/**
 * @brief IPI handler.
 */
PRIVATE void default_handler(void)
{
	/* noop. */
}

/**
 * @brief Register a event handler.
 *
 * @param handler Handler function.
 *
 * @return Zero if successfully register, non-zero otherwize.
 */
PUBLIC int event_register_handler(event_handler_t handler)
{
	if (_event_handler != default_handler || handler == NULL)
		return (-EINVAL);

	_event_handler = handler;

	dcache_invalidate();

	return (0);
}

/**
 * @brief Resets event handler.
 */
PUBLIC int event_unregister_handler(void)
{
	_event_handler = default_handler;

	dcache_invalidate();

	return (0);
}

/*============================================================================*
 * event_setup()                                                              *
 *============================================================================*/

/**
 * Initializes the table of events.
 *
 * @author Davidson Francis
 */
PUBLIC void event_setup(void)
{
	for (int i = 0; i < CORES_NUM; i++)
	{
		events[i].pending = 0;
		events[i].handled = 0;
	}

	spinlock_init(&event_lock);

	_event_handler = default_handler;

#if (CLUSTER_HAS_IPI)
	KASSERT(interrupt_register(INTERRUPT_IPI, event_handler) == 0);
#endif
}

/*============================================================================*
 * event_wait()                                                               *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int event_notify(int coreid)
{
	int mycoreid;
	struct section_guard guard;

	/* Invalid core. */
	if (UNLIKELY(!WITHIN(coreid, 0, CORES_NUM)))
		return (-EINVAL);

	mycoreid = core_get_id();

	/* Bad core. */
	if (UNLIKELY(coreid == mycoreid))
		return (-EINVAL);

	/* Prevent this call be preempted by any maskable interrupt. */
	section_guard_init(&guard, &event_lock, INTERRUPT_LEVEL_NONE);

	section_guard_entry(&guard);

		/* Set the pending event flag. */
		events[coreid].pending |= (1 << mycoreid);

#if (CLUSTER_HAS_IPI)
		cluster_ipi_send(coreid);
#elif (CLUSTER_HAS_EVENTS)
		__event_notify(coreid);
#endif

	section_guard_exit(&guard);

	return (0);
}

/*============================================================================*
 * event_wait()                                                               *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC void event_drop(void)
{
	int mycoreid = core_get_id();
	struct section_guard guard;

	/* Prevent this call be preempted by any maskable interrupt. */
	section_guard_init(&guard, &event_lock, INTERRUPT_LEVEL_NONE);

	section_guard_entry(&guard);

		if (events[mycoreid].pending)
		{
			kprintf("[hal][cluster] core %d dropping events %x",
				mycoreid,
				events[mycoreid].pending
			);
		}

		events[mycoreid].pending = 0;

	section_guard_exit(&guard);
}

/*============================================================================*
 * event_wait()                                                               *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC void event_wait(void)
{
	int mycoreid;
	struct section_guard guard;

	mycoreid = core_get_id();

	/* Prevent this call be preempted by any maskable interrupt. */
	section_guard_init(&guard, &event_lock, INTERRUPT_LEVEL_NONE);

	while (true)
	{
		section_guard_entry(&guard);

			if (events[mycoreid].pending || events[mycoreid].handled)
				break;

		section_guard_exit(&guard);

#if (CLUSTER_HAS_IPI)
		cluster_ipi_wait();
#elif (CLUSTER_HAS_EVENTS)
		__event_wait();
#endif
	}

		/* Handles pending events. */
		if (events[mycoreid].pending)
			event_handler(-1);

		/* Clear event. */
		for (int i = 0; i < CORES_NUM; i++)
		{
			if (events[mycoreid].handled & (1 << i))
			{
				events[mycoreid].handled &= ~(1 << i);
				break;
			}
		}

	section_guard_exit(&guard);
}

