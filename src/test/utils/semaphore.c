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
 * Semaphore                                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the semaphore with the given value.
 *
 * @param s Semaphore to be initialized.
 * @param c Semaphore counter.
 */
PUBLIC void semaphore_init(struct semaphore *s, int c)
{
	spinlock_init(&s->lock);
	s->count = c;
}

/**
 * @brief Tries to decrement the semaphore counter:
 * if greater than zero, decrements and returns,
 * otherwise, does a busy-waiting until the counter
 * becomes greater than zero.
 *
 * @param s Semaphore to decremented.
 */
PUBLIC void semaphore_down(struct semaphore *s)
{
	while (true)
	{
		spinlock_lock(&s->lock);
			if (s->count > 0)
				break;
		spinlock_unlock(&s->lock);
	}
	s->count--;
	spinlock_unlock(&s->lock);
}

/**
 * @brief Increments the semaphore value.
 *
 * @param s Semaphore to be incremented.
 */
PUBLIC void semaphore_up(struct semaphore *s)
{
	spinlock_lock(&s->lock);
		s->count++;
	spinlock_unlock(&s->lock);
}
