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

#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

#if (!CLUSTER_HAS_EVENTS)

/**
 * @brief Table of events.
 */
PUBLIC struct events_table
{
	unsigned pending; /**< Pending Events  */
	spinlock_t lock;  /**< Event Line Lock */
} events[CORES_NUM];

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
	int i; /* Loop index. */

	for (i = 0; i < CORES_NUM; i++)
	{
		events[i].pending = 0;
		spinlock_init(&events[i].lock);
	}
}

/*============================================================================*
 * event_wait()                                                               *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC void event_notify(int coreid)
{
	int mycoreid = core_get_id();

	spinlock_lock(&events[coreid].lock);

		events[coreid].pending |= (1 << mycoreid);

	spinlock_unlock(&events[coreid].lock);
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

	spinlock_lock(&events[mycoreid].lock);

		if (events[mycoreid].pending)
		{
			kprintf("[hal][cluster] core %d dropping events %x",
				mycoreid,
				events[mycoreid].pending
			);
		}

		events[mycoreid].pending = 0;

	spinlock_unlock(&events[mycoreid].lock);
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
	int mycoreid = core_get_id();

	while (true)
	{
		spinlock_lock(&events[mycoreid].lock);

			if (events[mycoreid].pending)
				break;

		spinlock_unlock(&events[mycoreid].lock);
	}

		/* Clear event. */
		for (int i = 0; i < CORES_NUM; i++)
		{
			if (events[mycoreid].pending & (1 << i))
			{
				events[mycoreid].pending &= ~(1 << i);
				break;
			}
		}

	spinlock_unlock(&events[mycoreid].lock);
}

#endif /* !CLUSTER_HAS_EVENTS */
