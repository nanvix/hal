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
#include "test.h"

/**
 * @brief Clock frequency.
 */
#define CLOCK_FREQ 30

/**
 * @brief Number of clock interrupts.
 */
PRIVATE unsigned ticks = 0;

/**
 * @brief Handles a clock interrupt.
 *
 * @param num Number of interrupt.
 */
PRIVATE void do_clock(int num)
{
	KASSERT(num == HAL_INT_CLOCK);

	ticks++;
	dcache_invalidate();
}

/*============================================================================*
 * Stress Tests                                                               *
 *============================================================================*/

/**
 * @brief Stress Test: Handle Clock Interrupts
 *
 */
PRIVATE void test_do_clock(void)
{
	const unsigned nticks = 10;

	hal_clock_init(CLOCK_FREQ);

	KASSERT(interrupt_register(HAL_INT_CLOCK, do_clock) == 0);

	interrupts_enable();
	interrupt_unmask(HAL_INT_CLOCK);

		/* Wait for enough clock interrupts. */
		while (ticks < nticks)
			noop();

	interrupts_disable();
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test clock_stress_tests[] = {
	{ test_do_clock, "Handle Clock Interrupts" },
	{ NULL,          NULL                      },
};

/**
 * The test_clock() function launches testing units on the clock
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_clock(void)
{
	for (int i = 0; clock_stress_tests[i].test_fn != NULL; i++)
	{
		clock_stress_tests[i].test_fn();
		kprintf("[test][stress][clock] %s [passed]", clock_stress_tests[i].name);
	}
}
