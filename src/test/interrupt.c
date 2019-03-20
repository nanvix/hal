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
 * @brief Verbose interrupt test?
 */
#define TEST_INTERRUPT_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief Dummy interrupt handler.
 */
PRIVATE void dummy_handler(int num)
{
	UNUSED(num);
}

/*----------------------------------------------------------------------------*
 * Set and Clear an Interrupt Handler                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set and Clear an Interrupt Handler
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_set_clear_handler(void)
{
	KASSERT(interrupt_set_handler(HAL_INT_CLOCK, dummy_handler) == 0);
	KASSERT(interrupt_set_handler(HAL_INT_CLOCK, NULL) == 0);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test interrupt_tests_api[] = {
	{ test_interrupt_set_clear_handler, "Set and Clear an Interrupt Handler" },
	{ NULL,                             NULL                                 },
};

/**
 * The test_interrupt() function launches testing units on the
 * Interrupt Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_interrupt(void)
{
	/* API Tests */
	for (int i = 0; interrupt_tests_api[i].test_fn != NULL; i++)
	{
		interrupt_tests_api[i].test_fn();
		kprintf("[test][api][interrupt] %s [passed]", interrupt_tests_api[i].name);
	}
}

