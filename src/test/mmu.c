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
#include <errno.h>
#include "test.h"

/**
 * @brief Run destructive tests?
 */
#define TEST_MMU_DESTRUCTIVE 0

/**
 * @brief Launch verbose tests?
 */
#define TEST_MMU_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test mmu_api_tests[] = {
	{ NULL, NULL },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test mmu_fault_tests[] = {
	{ NULL, NULL },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_mmu() function launches testing units on the
 * MMU Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_mmu(void)
{
	for (int i = 0; mmu_api_tests[i].test_fn != NULL; i++)
	{
		mmu_api_tests[i].test_fn();
		kprintf("[test][api][mmu] %s [passed]", mmu_api_tests[i].name);
	}

	for (int i = 0; mmu_fault_tests[i].test_fn != NULL; i++)
	{
		mmu_fault_tests[i].test_fn();
		kprintf("[test][fault][mmu] %s [passed]", mmu_fault_tests[i].name);
	}
}

