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
 * @brief Run destructive tests?
 */
#define TEST_EXCEPTION_DESTRUCTIVE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Trigger Exception
 */
PRIVATE void test_trigger_exception(void)
{
	char *p = (char *) 0x4badbeef; /* I like beefs. */

	/* Don't run destructive tests. */
	if (!TEST_EXCEPTION_DESTRUCTIVE)
		return;

	*p = 1;
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test exception_api_tests[] = {
	{ test_trigger_exception, "Trigger Exception" },
	{ NULL,                   NULL                },
};

/**
 * The test_exception() function launches testing units on the
 * Exception Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_exception(void)
{
	for (int i = 0; exception_api_tests[i].test_fn != NULL; i++)
	{
		exception_api_tests[i].test_fn();
		kprintf("[test][stress][clock] %s [passed]", exception_api_tests[i].name);
	}
}

