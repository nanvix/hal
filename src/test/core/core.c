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
#include <nanvix/hlib.h>
#include "../test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_CORE_VERBOSE 0

/**
 * @brief Run destructive tests?
 */
#define TEST_CORE_DESTRUCTIVE 0


/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Get Core ID                                                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Query Core ID
 */
PRIVATE void test_core_get_id(void)
{
	int coreid;

	coreid = core_get_id();

#if (TEST_CORE_VERBOSE)
	kprintf("[test][core][core][api] core %d running", coreid);
#endif

	KASSERT(coreid == COREID_MASTER);
}


/*----------------------------------------------------------------------------*
 * Core Poweroff                                                              *
 *----------------------------------------------------------------------------*/


/**
 * @brief API Test: Poweroff the core
 */
PRIVATE void test_core_poweroff(void)
{
	/* Don't run destructive tests. */
	if (!TEST_CORE_DESTRUCTIVE)
		return;

	core_poweroff();
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief API Tests.
 */
PRIVATE struct test core_tests_api[] = {
	{ test_core_get_id,   "get core id   " },
	{ test_core_poweroff, "power off core" },
	{ NULL,                NULL            },
};

/**
 * The test_core() function launches testing units on the core
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_core(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; core_tests_api[i].test_fn != NULL; i++)
	{
		core_tests_api[i].test_fn();
		kprintf("[test][core][core][api] %s [passed]", core_tests_api[i].name);
	}
}
