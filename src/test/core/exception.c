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
#include <posix/errno.h>
#include "../test.h"

#if defined(__unix64__)

	/**
	 * @brief Non-destrutive test not supported on Unix64
	 */
	#define TEST_EXCEPTION_DESTRUCTIVE 0

	/**
	 * @brief Number of exception triggered on unit test.
	 */
	#define TEST_TRIGGERED_EXCEPTION_NUM EXCEPTION_INVALID_OPCODE

	/**
	 * @brief Invalid opcode exception handler
	 */
	PRIVATE void triggered_exception_handler(
		const struct exception *excp,
		const struct context *ctx
	)
	{
		UNUSED(excp);
		UNUSED(ctx);

		kpanic("[test][core][exception] This test is destructive only.");
	}

	/**
	 * @brief Triggers an invalid instruction exception.
	 */
	PRIVATE inline void trigger_exception(void)
	{
		raise(TEST_TRIGGERED_EXCEPTION_NUM);
	}

#elif defined(__rv32gc__) || defined(__or1k__) || defined(__arm64__)

	/**
	 * @brief Non-destrutive test not supported on RiscV 32
	 */
	#define TEST_EXCEPTION_DESTRUCTIVE 0

	/**
	 * @brief Number of exception triggered on unit test.
	 */
	#define TEST_TRIGGERED_EXCEPTION_NUM EXCEPTION_INVALID_OPCODE

	/**
	 * @brief Skips invalid instruction.
	 */
	PRIVATE void triggered_exception_handler(
		const struct exception *excp,
		const struct context *ctx
	)
	{
		word_t next_pc;
		struct context * _ctx;

		KASSERT(excp->num == TEST_TRIGGERED_EXCEPTION_NUM);

		_ctx = (struct context *) ctx;

		next_pc = context_get_pc(_ctx) + WORD_SIZE;

		context_set_pc(_ctx, next_pc);
	}

	/**
	 * @brief Triggers an invalid instruction exception.
	 *
	 */
	PRIVATE inline void trigger_exception(void)
	{
#if defined(__rv32gc__)

		/**
		 * The trigger_exception() function triggers an invalid
		 * instruction exception in the rv32gc core. It does so by writing
		 * a random value into the mhartid special function register.
		 * Hopefully, if the core is compliant to the standards this is
		 * not allowed.
		 *
		 * @author Pedro Henrique Penna
		 */
		asm volatile (
			"csrw mhartid, %0;"
			:
			: "r" (0xdeadc0de)
		);

#elif defined(__or1k__)

		/**
		 * The trigger_exception() function triggers an invalid
		 * instruction exception in the or1k core. It call a custom
		 * instruction that is not implemented. Hopefully, this 
		 * instruction is not really implemented.
		 *
		 * @author João Vicente Souto 
		 */
		asm volatile (
			"l.cust4;"
		);

#else
		kpanic("Missing arch!");
#endif
	}

#else /* Others archs. */

	/**
	 * @brief Number of exception triggered on unit test.
	 */
	#ifdef EXCEPTION_DTLB_FAULT
		#define TEST_TRIGGERED_EXCEPTION_NUM EXCEPTION_DTLB_FAULT
	#else
		#define TEST_TRIGGERED_EXCEPTION_NUM EXCEPTION_PAGE_FAULT
	#endif

	/**
	 * @brief Bad address to trigger a DTLB fault execption.
	 */
	#define BAD_ADDRESS 0x4badbeef /* I like beefs. */

	/**
	 * @brief Global variable used by DTLB fault exepction handler.
	 */
	PRIVATE unsigned char exception_variable;

	/**
	 * @brief DTLB fault exception handler
	 */
	PRIVATE void triggered_exception_handler(
		const struct exception *excp,
		const struct context *ctx
	)
	{
		word_t * reg;
		word_t * end;

		KASSERT(excp->num == TEST_TRIGGERED_EXCEPTION_NUM);

		reg = (word_t *) (ctx);
		end = (word_t *) (ctx + 1);

		do
		{
			if (*reg == BAD_ADDRESS)
			{
				*reg = (word_t) &exception_variable;
				break;
			}
		} while ((++reg) != end);

		if (reg == end)
			kpanic("[test][core][exception] No register contains the invalid address.");
	}

	/**
	 * @brief Triggers a DTLB fault exception.
	 *
	 * The trigger_exception() function forces a DTLB fault exception
	 * by writing some data into a random memory location. Hopefully,
	 * the target memory location is not mapped.
	 *
	 * @author Pedro Henrique Penna
	 */
	PRIVATE inline void trigger_exception(void)
	{
		unsigned char *p = (unsigned char *) BAD_ADDRESS;
		exception_variable = 0;

		/* Triggers the DTLB fault. */
		*p = 0xfe;

		/* Correct handled. */
		KASSERT(exception_variable == 0xfe);
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
	KASSERT(exception_register(EXCEPTION_PAGE_FAULT, dummy_handler) == 0);
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == 0);
}

/*----------------------------------------------------------------------------*
 * Trigger an Exception                                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Trigger an Exception
 */
PRIVATE void test_trigger_exception(void)
{
#if defined(__rv32gc__) || defined(__unix64__) || (__arm64__)
	/* Don't run destructive tests. */
	if (!TEST_EXCEPTION_DESTRUCTIVE)
		return;
#endif

	KASSERT(exception_register(TEST_TRIGGERED_EXCEPTION_NUM, triggered_exception_handler) == 0);

	trigger_exception();

	KASSERT(exception_unregister(TEST_TRIGGERED_EXCEPTION_NUM) == 0);
}

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Set a Handler for Invalid Exception                                        *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection Test: Set a Handler for an Invalid Exception
 */
PRIVATE void test_exception_register_inval(void)
{
	KASSERT(exception_register(-1, dummy_handler) == -EINVAL);
	KASSERT(exception_register(EXCEPTIONS_NUM + 1, dummy_handler) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Unset a Handler for Invalid Exception                                      *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection Test: Unset a Handler for an Invalid Exception
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
 * Fault Injection Test: Unset a Handler for a Bad Exception
 */
PRIVATE void test_exception_unregister_bad(void)
{
	KASSERT(exception_unregister(EXCEPTION_PAGE_FAULT) == -EINVAL);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test exception_api_tests[] = {
	{ test_exception_set_unset_handler, "set and unset exception handler" },
	{ test_trigger_exception,           "trigger exception              " },
	{ NULL,                              NULL                             },
};

/**
 * @brief Unit tests.
 */
PRIVATE struct test exception_fault_tests[] = {
	{ test_exception_register_inval,   "set handler for invalid exception  " },
	{ test_exception_unregister_inval, "unset handler for invalid exception" },
	{ test_exception_unregister_bad,   "unset handler for bad exception    " },
	{ NULL,                             NULL                                 },
};

/**
 * The test_exception() function launches testing units on the
 * Exception Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_exception(void)
{
	CLUSTER_KPRINTF(HLINE);
	for (int i = 0; exception_api_tests[i].test_fn != NULL; i++)
	{
		exception_api_tests[i].test_fn();
		CLUSTER_KPRINTF("[test][api][exception] %s [passed]", exception_api_tests[i].name);
	}

	CLUSTER_KPRINTF(HLINE);
	for (int i = 0; exception_fault_tests[i].test_fn != NULL; i++)
	{
		exception_fault_tests[i].test_fn();
		CLUSTER_KPRINTF("[test][fault][exception] %s [passed]", exception_fault_tests[i].name);
	}
}

