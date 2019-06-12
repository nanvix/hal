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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include "../test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_SPINLOCK_VERBOSE 0

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_spinlock() function launches testing units on the spinlock
 * interface of the HAL.
 *
 * @author Daniel Coscia
 */
PUBLIC void test_spinlock(void)
{
	spinlock_t lock = SPINLOCK_UNLOCKED;
    int ret = 0;

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] init testing");
#endif
    spinlock_init(&lock);
    kprintf("[test][core][spinlock][api] init [passed]");

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] lock testing");
#endif
    spinlock_lock(&lock);
    kprintf("[test][core][spinlock][api] lock [passed]");

    ret = spinlock_trylock(&lock);
#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] trylock testing (locked) | ret = %d", ret);
#endif
    KASSERT(ret != 0);
    kprintf("[test][core][spinlock][api] trylock (locked) [passed]");

#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] unlock testing");
#endif
    spinlock_unlock(&lock);
    kprintf("[test][core][spinlock][api] unlock [passed]");

    ret = spinlock_trylock(&lock);
#if (TEST_SPINLOCK_VERBOSE)
	kprintf("[test][core][spinlock][api] trylock testing (unlocked) | ret = %d", ret);
#endif
    KASSERT(ret == 0);
    kprintf("[test][core][spinlock][api] trylock (unlocked) [passed]");

}
