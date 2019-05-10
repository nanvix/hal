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
#define __NEED_CLUSTER_MEMMAP

#if (defined(__or1k_cluster__))
	#include <arch/cluster/or1k-cluster/cores.h>
	#include <arch/cluster/or1k-cluster/ompic.h>
	#include <arch/cluster/or1k-cluster/memmap.h>
	#include <arch/cluster/or1k-cluster/memory.h>
#elif (defined(__optimsoc_cluster__))
	#include <arch/cluster/optimsoc-cluster/cores.h>
	#include <arch/cluster/optimsoc-cluster/ompic.h>
	#include <arch/cluster/optimsoc-cluster/memmap.h>
	#include <arch/cluster/optimsoc-cluster/memory.h>
#endif

#include <nanvix/hal/cluster/mmio.h>
#include <nanvix/const.h>

/**
 * @cond or1k_cluster
 *
 * Table of events
 *
 * @endcond
 */
PUBLIC struct
{
	unsigned pending;
	or1k_spinlock_t lock;
} events[OR1K_CLUSTER_NUM_CORES] ALIGN(OR1K_CACHE_LINE_SIZE) = {
#if (defined(__or1k_cluster__))
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Master Core  */
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Slave Core 1 */
#elif (defined(__optimsoc_cluster__))
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Master Core  */
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Slave Core 1 */
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Slave Core 2 */
	{ 0, OR1K_SPINLOCK_UNLOCKED }, /* Slave Core 3 */
#endif
};

/*============================================================================*
 * or1k_cluster_event_reset()                                                 *
 *============================================================================*/

PUBLIC void or1k_cluster_event_reset(void)
{
	int coreid;       /* Core ID.       */
	uint32_t *ompic;  /* OMPIC address. */

	/* Get address of uart. */
	ompic = mmio_get(OR1K_CLUSTER_OMPIC_BASE_PHYS);

	/* Current core. */
	coreid = or1k_core_get_id();

	/* ACK IPI. */
	ompic[OR1K_OMPIC_CTRL(coreid) / OR1K_WORD_SIZE] = OR1K_OMPIC_CTRL_IRQ_ACK;
}

/*============================================================================*
 * or1k_cluster_event_notify()                                                *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_cluster_event_notify(int coreid)
{
	int mycoreid;

	/* Invalid core. */
	if (UNLIKELY(coreid < 0))
		return (-EINVAL);

	mycoreid = or1k_core_get_id();

	/* Bad core. */
	if (UNLIKELY(coreid == mycoreid))
		return (-EINVAL);

	spinlock_lock(&events[coreid].lock);

		/* Set the pending IPI flag. */
		events[coreid].pending |= (1 << mycoreid);

#if (defined(__or1k_cluster__))
		or1k_cluster_ompic_send_ipi(coreid, 0);
#endif

	spinlock_unlock(&events[coreid].lock);

	return (0);
}

/*============================================================================*
 * or1k_cluster_event_wait()                                                  *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_cluster_event_wait(void)
{
	int mycoreid = or1k_core_get_id();

	while (true)
	{
		or1k_spinlock_lock(&events[mycoreid].lock);

			if (events[mycoreid].pending)
				break;

		or1k_spinlock_unlock(&events[mycoreid].lock);

#if (defined(__or1k_cluster__))
		or1k_int_wait();
#endif
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

	return (0);
}
