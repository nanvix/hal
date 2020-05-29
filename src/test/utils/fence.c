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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include "../test.h"

/*----------------------------------------------------------------------------*
 * Fence                                                                      *
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes a fence.
 *
 * @param b      Target fence.
 * @param ncores Number of cores in the fence.
 */
PUBLIC void fence_init(struct fence *b, int ncores)
{
	b->nreached = 0;
	b->ncores = ncores;
	spinlock_init(&b->lock);
	dcache_invalidate();
}

/**
 * @brief Waits in a fence.
 */
PUBLIC void fence_wait(struct fence *b)
{
	again:

		spinlock_lock(&b->lock);

			if (b->nreached < b->ncores)
			{
				spinlock_unlock(&b->lock);
				goto again;
			}

		spinlock_unlock(&b->lock);
}

/**
 * @brief Joins a fence.
 *
 * @param b Target fence.
 */
PUBLIC void fence_join(struct fence *b)
{
	spinlock_lock(&b->lock);

		b->nreached++;

	spinlock_unlock(&b->lock);
}
