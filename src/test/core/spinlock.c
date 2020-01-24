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

/**
 * @brief Launch verbose tests?
 */
#define TEST_SPINLOCK_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Lock and Unlock a Spinlock                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Lock and Unlock a Spinlock
 */
PRIVATE void test_api_spinlock_lock_unlock(void)
{
	spinlock_t lock = SPINLOCK_UNLOCKED;

    spinlock_init(&lock);

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] locking...");
#endif
    spinlock_lock(&lock);


#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] unlocking...");
#endif
    spinlock_unlock(&lock);
}

/*----------------------------------------------------------------------------*
 * Spinlock Trylock                                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Try Lock
 */
PRIVATE void test_api_spinlock_trylock(void)
{
	spinlock_t lock = SPINLOCK_UNLOCKED;

    spinlock_init(&lock);

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] trying to lock...");
#endif
    KASSERT(spinlock_trylock(&lock) != 0);

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] trying to lock again...");
#endif
    KASSERT(spinlock_trylock(&lock) == 0);

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] unlocking...");
#endif
    spinlock_unlock(&lock);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test test_api_spinlock[] = {
	{ test_api_spinlock_lock_unlock, "lock/unlock " },
	{ test_api_spinlock_trylock,     "trylock     " },
	{ NULL,                           NULL          },
};

/**
 * The test_spinlock() function launches testing units on the spinlock
 * interface of the HAL.
 *
 * @author Daniel Coscia
 */
PUBLIC void test_spinlock(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; test_api_spinlock[i].test_fn != NULL; i++)
	{
		test_api_spinlock[i].test_fn();
		kprintf("[test][api][spinlock] %s [passed]", test_api_spinlock[i].name);
	}
}
