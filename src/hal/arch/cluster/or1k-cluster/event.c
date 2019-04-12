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

/* Must come first. */
#define __NEED_HAL_CLUSTER

#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @brief Table of events.
 */
PRIVATE struct
{
	unsigned pending;     /**< Pending Events  */
	or1k_spinlock_t lock; /**< Event Line Lock */
} events[OR1K_CLUSTER_NUM_CORES] ALIGN(OR1K_CACHE_LINE_SIZE) = {
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Master Core  */
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Slave Core 1 */
};

/*============================================================================*
 * or1k_cluster_event_wait()                                                  *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC inline void or1k_cluster_event_notify(int coreid)
{
	int mycoreid = or1k_core_get_id();

	or1k_spinlock_lock(&events[coreid].lock);

		events[coreid].pending |= (1 << mycoreid);

	or1k_spinlock_unlock(&events[coreid].lock);
}

/*============================================================================*
 * or1k_cluster_event_wait()                                                  *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC void or1k_cluster_event_drop(void)
{
	int mycoreid = or1k_core_get_id();

	or1k_spinlock_lock(&events[mycoreid].lock);

		events[mycoreid].pending = 0;

	or1k_spinlock_unlock(&events[mycoreid].lock);
}

/*============================================================================*
 * or1k_cluster_event_wait()                                                  *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC void or1k_cluster_event_wait(void)
{
	int mycoreid = or1k_core_get_id();

	while (TRUE)
	{
		or1k_spinlock_lock(&events[mycoreid].lock);

			if (events[mycoreid].pending)
				break;

		or1k_spinlock_unlock(&events[mycoreid].lock);
	}

		/* Clear event. */
		for (int i = 0; i < OR1K_CLUSTER_NUM_CORES; i++)
		{
			if (events[mycoreid].lock & (1 << i))
			{
				events[mycoreid].pending &= ~(1 << i);
				break;
			}
		}

	or1k_spinlock_unlock(&events[mycoreid].lock);
}
