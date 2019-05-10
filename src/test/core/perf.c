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
#define TEST_PERF_VERBOSE 1

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

#if (CORE_HAS_PERF)

/*----------------------------------------------------------------------------*
 * Query Performance Monitor Capability                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Query Performance Monitor Capability
 */
PRIVATE void test_perf_api_query(void)
{
	/*
	 * At least one performance event
	 * should be supported.
	 */
	for (int i = 0; i < PERF_EVENTS_MAX; i++)
	{
		if (!perf_query(i))
			return;
	}

	KASSERT(0);
}

/*----------------------------------------------------------------------------*
 * Read a Performance Monitor                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Read a Performance Monitor
 */
PRIVATE void test_perf_api_read(void)
{
	for (int i = 0; i < PERF_MONITORS_NUM; i++)
		KASSERT(perf_read(i) == 0);
}

/*----------------------------------------------------------------------------*
 * Start and Stop a Performance Monitor                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Start and Stop a Performance Monitor
 */
PRIVATE void test_perf_api_start_stop(void)
{
	int event = -1;

	/*
	 * Get the first performance
	 * event that is supported.
	 */
	for (int i = 0; i < PERF_EVENTS_MAX; i++)
	{
		if (!perf_query(i))
		{
			event = i;
			break;
		}
	}

	KASSERT(perf_start(0, event) == 0);
	KASSERT(perf_stop(event) == 0);
}

/*============================================================================*
 * Fault Injection                                                            *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Invalid Query Performance Monitor Capability                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Query Performance Monitor Capability
 */
PRIVATE void test_perf_fault_query(void)
{
	KASSERT(perf_query(-1) == -ENOTSUP);
	KASSERT(perf_query(PERF_EVENTS_NUM) == -ENOTSUP);
}

/*----------------------------------------------------------------------------*
 * Invalid Read a Performance Monitor                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Read a Performance Monitor
 */
PRIVATE void test_perf_fault_read(void)
{
	KASSERT(perf_read(-1) == (uint64_t) -1);
	KASSERT(perf_read(PERF_MONITORS_NUM) == (uint64_t) -1);
}

/*----------------------------------------------------------------------------*
 * Invalid Start Performance Monitor                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Start Performance Monitor
 */
PRIVATE void test_perf_fault_start(void)
{
	int event = -1;

	/*
	 * Get the first performance
	 * event that is supported.
	 */
	for (int i = 0; i < PERF_EVENTS_MAX; i++)
	{
		if (!perf_query(i))
		{
			event = i;
			break;
		}
	}

	KASSERT(perf_start(-1, event) == -EINVAL);
	KASSERT(perf_start(PERF_MONITORS_NUM, event) == -EINVAL);
	KASSERT(perf_start(0, -1) == -EINVAL);
	KASSERT(perf_start(0, PERF_EVENTS_NUM) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Invalid Stop Performance Monitor                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Stop Performance Monitor
 */
PRIVATE void test_perf_fault_stop(void)
{
	KASSERT(perf_stop(-1) == -EINVAL);
	KASSERT(perf_stop(PERF_MONITORS_NUM) == -EINVAL);
}

#endif

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief API Tests.
 */
PRIVATE struct test perf_tests_api[] = {
#if (CORE_HAS_PERF)
	{ test_perf_api_query,      "query performance monitor capabilities " },
	{ test_perf_api_read,       "read performance monitor               " },
	{ test_perf_api_start_stop, "start and stop performance monitor     " },
#endif
	{ NULL,                      NULL                                    },
};

/**
 * @brief Fault Injection Tests.
 */
PRIVATE struct test perf_tests_fault[] = {
#if (CORE_HAS_PERF)
	{ test_perf_fault_query, "invalid query performance monitor capabilities " },
	{ test_perf_fault_read,  "invalid read performance monitor               " },
	{ test_perf_fault_start, "invalid start performance monitor              " },
	{ test_perf_fault_stop,  "invalid stop performance monitor               " },
#endif
	{ NULL,                      NULL                                    },
};

/**
 * The test_perf() function launches testing units on the Performance
 * Monitoring Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_perf(void)
{
	/* API Tests */
	kprintf("--------------------------------------------------------------------------------");
	for (int i = 0; perf_tests_api[i].test_fn != NULL; i++)
	{
		perf_tests_api[i].test_fn();
		kprintf("[test][core][perf][api] %s [passed]", perf_tests_api[i].name);
	}

	/* Fault Injection Tests */
	kprintf("--------------------------------------------------------------------------------");
	for (int i = 0; perf_tests_fault[i].test_fn != NULL; i++)
	{
		perf_tests_fault[i].test_fn();
		kprintf("[test][core][perf][fault] %s [passed]", perf_tests_fault[i].name);
	}
}
