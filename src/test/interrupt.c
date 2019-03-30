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
 * @brief Verbose interrupt test?
 */
#define TEST_INTERRUPT_VERBOSE 0

/**
 * @brief Counter of handler calls.
 */
PRIVATE int ncalls = 0;

/**
 * @brief Dummy interrupt handler.
 */
PRIVATE void dummy_handler(int num)
{
	UNUSED(num);

	ncalls++;
	dcache_invalidate();
}

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Register and Unregister an Interrupt Handler                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Register and Unregister an Interrupt Handler
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_register_unregister(void)
{
	KASSERT(interrupt_register(INTERRUPT_CLOCK, dummy_handler) == 0);
	KASSERT(interrupt_unregister(INTERRUPT_CLOCK) == 0);
}

/*----------------------------------------------------------------------------*
 * Enable and Disable Interrupts                                              *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Enable and Disable Interrupts
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_enable_disable(void)
{
	const int ntrials = 1000000;

	ncalls = 0;
	dcache_invalidate();

	KASSERT(interrupt_register(INTERRUPT_CLOCK, dummy_handler) == 0);

	interrupts_enable();

		do
			dcache_invalidate();
		while (ncalls > 0);

	interrupts_disable();

	KASSERT(interrupt_unregister(INTERRUPT_CLOCK) == 0);

	/*
	 * Ensure that the handler is not
	 * longer called.
	 */
	ncalls = 0;
	dcache_invalidate();
	for (int i = 0; i < ntrials; i++)
	{
		noop();
		KASSERT(ncalls == 0);
	}
}

/*----------------------------------------------------------------------------*
 * Mask and Unmask an Interrupt                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Mask and Unmask an Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_mask_unmask(void)
{
	const int ntrials = 1000000;

	ncalls = 0;
	dcache_invalidate();

	KASSERT(interrupt_register(INTERRUPT_CLOCK, dummy_handler) == 0);

	interrupt_unmask(INTERRUPT_CLOCK);

		do
			dcache_invalidate();
		while (ncalls > 0);

	interrupt_mask(INTERRUPT_CLOCK);

	KASSERT(interrupt_unregister(INTERRUPT_CLOCK) == 0);

	/*
	 * Ensure that the handler is not
	 * longer called.
	 */
	ncalls = 0;
	dcache_invalidate();
	for (int i = 0; i < ntrials; i++)
	{
		noop();
		KASSERT(ncalls == 0);
	}
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test interrupt_tests_api[] = {
	{ test_interrupt_register_unregister, "Register and Unregister a Handler" },
	{ test_interrupt_enable_disable,      "Enable and Disable Interrupts"     },
	{ test_interrupt_mask_unmask,         "Mask and Unmask an Interrupt"      },
	{ NULL,                                NULL                               },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Register a Handler for an Invalid Interrupt                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Register a Handler for an Invalid Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_register_handler_inval(void)
{
	KASSERT(interrupt_register(-1, dummy_handler) == -EINVAL);
	KASSERT(interrupt_register(INTERRUPTS_NUM + 1, dummy_handler) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Unregister a Handler for an Invalid Interrupt                              *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Unregister a Handler for an Invalid Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_unregister_handler_inval(void)
{
	KASSERT(interrupt_unregister(-1) == -EINVAL);
	KASSERT(interrupt_unregister(INTERRUPTS_NUM + 1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Register a Handler for an Bad Interrupt                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Register a Handler for an Bad Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_register_handler_bad(void)
{
	KASSERT(interrupt_register(INTERRUPT_CLOCK, dummy_handler) == 0);
	KASSERT(interrupt_register(INTERRUPT_CLOCK, dummy_handler) == -EBUSY);
	KASSERT(interrupt_unregister(INTERRUPT_CLOCK) == 0);
}

/*----------------------------------------------------------------------------*
 * Unregister a Handler for an Bad Interrupt                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Unregister a Handler for an Bad Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_unregister_handler_bad(void)
{
	KASSERT(interrupt_unregister(INTERRUPT_CLOCK) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Mask an Invalid Interrupt                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Mask an Invalid Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_mask_handler_inval(void)
{
	KASSERT(interrupt_mask(-1) == -EINVAL);
	KASSERT(interrupt_mask(INTERRUPTS_NUM + 1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Unmak an Invalid Interrupt                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Unmak an Invalid Interrupt
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void test_interrupt_unmask_handler_inval(void)
{
	KASSERT(interrupt_unmask(-1) == -EINVAL);
	KASSERT(interrupt_unmask(INTERRUPTS_NUM + 1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test interrupt_tests_fault_injection[] = {
	{ test_interrupt_register_handler_inval,   "Register Handler for Invalid Interrupt"   },
	{ test_interrupt_unregister_handler_inval, "Unregister Handler for Invalid Interrupt" },
	{ test_interrupt_register_handler_bad,     "Register Handler for Bad Interrupt"       },
	{ test_interrupt_unregister_handler_bad,   "Unregister Handler for Bad Interrupt"     },
	{ test_interrupt_mask_handler_inval,       "Mask Invalid Interrupt"                   },
	{ test_interrupt_unmask_handler_inval,     "Unmask Invalid Interrupt"                 },
	{ NULL,                                    NULL                                       },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

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

	/* Fault Tests */
	for (int i = 0; interrupt_tests_fault_injection[i].test_fn != NULL; i++)
	{
		interrupt_tests_fault_injection[i].test_fn();
		kprintf("[test][fault][interrupt] %s [passed]", interrupt_tests_fault_injection[i].name);
	}
}

