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


/*============================================================================*
 * Startup Fence                                                              *
 *============================================================================*/

/**
 * @brief Startup fence.
 */
PRIVATE struct
{
	bool master_alive;
	spinlock_t lock;
} fence = { false , SPINLOCK_UNLOCKED};

/**
 * @brief Releases the startup fence.
 */
PUBLIC void cluster_fence_release(void)
{
	spinlock_lock(&fence.lock);
		fence.master_alive = true;
	spinlock_unlock(&fence.lock);
}

/**
 * @brief Waits on the startup fence.
 */
PUBLIC void cluster_fence_wait(void)
{
	while (true)
	{
		spinlock_lock(&fence.lock);

			/* Fence is released. */
			if (fence.master_alive)
			{
				spinlock_unlock(&fence.lock);
				break;
			}

			noop();
		spinlock_unlock(&fence.lock);
	}
}