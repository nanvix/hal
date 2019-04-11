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
#include <stdint.h>
#include "../test.h"

/**
 * @brief Launch verbose tests?
 */
#define UPCALL_TEST_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief Magic number for checksum.
 */
#define MAGIC 0xdeadbeef

#define FAKE_STACK_HALO 128

/**
 * @brief Size of fake stack.
 */
#define FAKE_STACK_SIZE (CONTEXT_SIZE + 2*WORD_SIZE + DWORD_SIZE)

/**
 * @name Helpers for Fake Stack
 */
/**@{*/
#define FAKE_STACK_BASE (((word_t) &fake_stack) + FAKE_STACK_HALO + FAKE_STACK_SIZE)
#define FAKE_STACK_TOP (((word_t) &fake_stack) + FAKE_STACK_HALO)
#define FAKE_STACK(x) (&fake_stack[FAKE_STACK_HALO + (x)])
/**@}*/

/*/
 * @brief Issues a fake upcall.
 *
 * @param sp Fake stack pointer.
 */
EXTERN void _upcall_issue(word_t sp);

/**
 * @brief Return hook from _upcall_issue().
 */
EXTERN void _upcall_issue_ret(void);

/**
 * @brief Fake stack.
 */
PRIVATE uint8_t fake_stack[FAKE_STACK_HALO + FAKE_STACK_SIZE] ALIGN(DWORD_SIZE);

/**
 * @brief Saved stack pointer.
 */
PUBLIC word_t saved_sp;

/**
 * @brief Fake context.
 */
PRIVATE struct context fake_context;

/**
 * @brief Dumb upcall function.
 */
void dumb_upcall(void *arg)
{
	KASSERT(*((dword_t*) arg) == MAGIC);
}

/**
 * @brief API Test: Upcall Forge
 */
PRIVATE void test_upcall_forge(void)
{
	dword_t arg = MAGIC;

	/* These should agree. */
	KASSERT(FAKE_STACK_SIZE == UPCALL_STACK_FRAME_SIZE(DWORD_SIZE));

	/* Build fake stack. */
	kmemset(&fake_stack, 0, FAKE_STACK_SIZE);

	/* Build fake context. */
	kmemset(&fake_context, 0, sizeof(struct context));
	for (int i = 0; i < CONTEXT_SIZE/WORD_SIZE; i++)
		((word_t *)&fake_context)[i] = (word_t) i;
	context_set_sp(&fake_context, FAKE_STACK_BASE);
	context_set_pc(&fake_context, ((word_t) &_upcall_issue_ret));

	/* Forge upcall. */
	upcall_forge(
		&fake_context,
		&dumb_upcall,
		&arg,
		sizeof(dword_t)
	);

#if (UPCALL_TEST_VERBOSE)

	/* Dump fake stack. */
	for (int i = 0; i < FAKE_STACK_SIZE; i += WORD_SIZE)
	{
		kprintf("%x %x %d[fake_stack]",
			((word_t *)(&fake_stack[FAKE_STACK_HALO + i])),
			*((word_t *)(&fake_stack[FAKE_STACK_HALO + i])),
			FAKE_STACK_SIZE - i - WORD_SIZE
		);
	}

	/* Dump fake stack. */
	for (int i = 0; i < CONTEXT_SIZE/WORD_SIZE; i++)
		kprintf("%x %d[fake_context]", ((word_t *)(&fake_context))[i], i);

#endif /* UPCAL_TEST_VERBOSE */

	/* Check fake stack. */
	KASSERT(*((dword_t *)FAKE_STACK(UPCALL_STACK_FRAME_ARG_OFF)) == (dword_t)MAGIC);
	KASSERT(*((word_t *)FAKE_STACK(UPCALL_STACK_FRAME_ARGSIZE_OFF)) == (word_t)sizeof(dword_t));
	KASSERT(*((word_t *)FAKE_STACK(UPCALL_STACK_FRAME_FN_OFF)) == (word_t)&dumb_upcall);

	/* Check fake context. */
	KASSERT(context_get_sp(&fake_context) == FAKE_STACK_TOP);
	KASSERT(context_get_pc(&fake_context) == ((word_t) &upcall_ret));
}

/**
 * @brief API Test: Issue an Upcall
 */
PRIVATE void test_upcall_issue(void)
{
	dword_t arg = MAGIC;

	/* These should agree. */
	KASSERT(FAKE_STACK_SIZE == UPCALL_STACK_FRAME_SIZE(sizeof(dword_t)));

	/* Build fake stack. */
	kmemset(&fake_stack, 0, FAKE_STACK_SIZE);

	/* Build fake context. */
	kmemset(&fake_context, 0, sizeof(struct context));
	for (int i = 0; i < CONTEXT_SIZE/WORD_SIZE; i++)
		((word_t *)&fake_context)[i] = (word_t) i;
	context_set_sp(&fake_context, FAKE_STACK_BASE);
	context_set_pc(&fake_context, ((word_t) &_upcall_issue_ret));

	/* Forge upcall. */
	upcall_forge(
		&fake_context,
		&dumb_upcall,
		&arg,
		sizeof(dword_t)
	);

	/* Issue upcall. */
	_upcall_issue(FAKE_STACK_TOP);
}

/**
 * @brief Unit tests.
 */
PRIVATE struct test upcall_tests_api[] = {
	{ test_upcall_forge, "Forge Upcall" },
	{ test_upcall_issue, "Issue Upcall" },
	{ NULL,               NULL          },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_upcall() function launches testing units on the upcall
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_upcall(void)
{
	kprintf("[test][upcall] launching api tests...");

	/* API Tests */
	for (int i = 0; upcall_tests_api[i].test_fn != NULL; i++)
	{
		upcall_tests_api[i].test_fn();
		kprintf("[test][upcall][api] %s [passed]", upcall_tests_api[i].name);
	}
}
