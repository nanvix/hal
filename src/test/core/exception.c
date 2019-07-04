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
#include "../test.h"

/**
 * @brief Run destructive tests?
 */
#define TEST_EXCEPTION_DESTRUCTIVE 0

#if defined(__rv32gc__)

	/**
	 * @brief Triggers an invalid instruction exception.
	 *
	 * The trigger_exception() function triggers an invalid
	 * instruction exception in the rv32gc core. It does so by writing
	 * a random value into the mhartid special function register.
	 * Hopefully, if the core is compliant to the standards this is
	 * not allowed.
	 *
	 * @author Pedro Henrique Penna
	 */
	PRIVATE inline void trigger_exception(void)
	{
		asm volatile (
			"csrw mhartid, %0;"
			:
			: "r" (0xdeadc0de)
		);
	}

#else

	/**
	 * @brief Triggers a page fault exception.
	 *
	 * The trigger_exception() function forces a page fault exception
	 * by writing some data into a random memory location. Hopefully,
	 * the target memory location is not mapped.
	 *
	 * @author Pedro Henrique Penna
	 */
	PRIVATE inline void trigger_exception(void)
	{
		unsigned char *p = (unsigned char *) 0x4badbeef; /* I like beefs. */
		*p = 0xfe;
	}

#endif

/**
 * @brief Dummy exception handler
 */
PRIVATE void dummy_handler(
	const struct exception *excp,
	const struct context *ctx
)
{
	UNUSED(excp);
	UNUSED(ctx);
}

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Set and Unset an Exception Handler                                         *
 *----------------------------------------------------------------------------*/

/**
 * API Test: Set and Unset and Exception Handler
 */
PRIVATE void test_exception_set_unset_handler(void)
{
#ifndef __unix64__
	KASSERT(exception_register(EXCEPTION_PAGE_FAULT, dummy_handler) == 0);
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == 0);
#else
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == 0);
	KASSERT(exception_register(EXCEPTION_PAGE_FAULT, dummy_handler) == 0);
#endif
}

/*----------------------------------------------------------------------------*
 * Trigger an Exception                                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Trigger an Exception
 */
PRIVATE void test_trigger_exception(void)
{
	/* Don't run destructive tests. */
	if (!TEST_EXCEPTION_DESTRUCTIVE)
		return;

	trigger_exception();
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test exception_api_tests[] = {
	{ test_exception_set_unset_handler, "set and unset exception handler" },
	{ test_trigger_exception,           "trigger exception"               },
	{ NULL,                              NULL                             },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Set a Handler for Invalid Exception                                        *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection test: Set a Handler for an Invalid Exception
 */
PRIVATE void test_exception_register_inval(void)
{
	KASSERT(exception_register(-1, dummy_handler) == -EINVAL);
	KASSERT(exception_register(EXCEPTIONS_NUM + 1, dummy_handler) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Handler for a Bad Exception                                          *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection test: Set a Handler for a Bad Exception
 */
PRIVATE void test_exception_register_bad(void)
{
#ifndef __unix64__
	KASSERT(exception_register(EXCEPTION_PAGE_FAULT, dummy_handler) == 0);
#endif
	KASSERT(exception_register(EXCEPTION_PAGE_FAULT, dummy_handler) == -EBUSY);
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == 0);
}

/*----------------------------------------------------------------------------*
 * Unset a Handler for Invalid Exception                                      *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection test: Unset a Handler for an Invalid Exception
 */
PRIVATE void test_exception_unregister_inval(void)
{
	KASSERT(exception_unregister(-1) == -EINVAL);
	KASSERT(exception_unregister(EXCEPTIONS_NUM + 1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Unset a Handler for a Bad Exception                                        *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection test: Unset a Handler for a Bad Exception
 */
PRIVATE void test_exception_unregister_bad(void)
{
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test exception_fault_tests[] = {
	{ test_exception_register_inval,   "set handler for invalid exception"   },
	{ test_exception_register_bad,     "set handler for bad exception"       },
	{ test_exception_unregister_inval, "unset handler for invalid exception" },
	{ test_exception_unregister_bad,   "unset handler for bad exception"     },
	{ NULL,                             NULL                                 },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

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
		kprintf("[test][api][exception] %s [passed]", exception_api_tests[i].name);
	}

	for (int i = 0; exception_fault_tests[i].test_fn != NULL; i++)
	{
		exception_fault_tests[i].test_fn();
		kprintf("[test][fault][exception] %s [passed]", exception_fault_tests[i].name);
	}
}