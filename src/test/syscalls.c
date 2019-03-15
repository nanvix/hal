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

#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <nanvix/hal/core/trap.h>
#include "test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_TRAP_VERBOSE 0

/**
 * @brief Magic Numbers.
 */
/**@{*/
#define SYSCALL_NR         0
#define MAGIC0    0xBEEFCACE
#define MAGIC1    0xC00010FF
#define MAGIC2    0xCAFEBABE
#define MAGIC3    0xCAFED00D
#define MAGIC4    0x0D15EA5E
#define MAGIC5    0xDEAD10CC
/**@}*/

/**
 * @brief System call dispatcher.
 *
 * @param arg0       First system call argument.
 * @param arg1       Second system call argument.
 * @param arg2       Third system call argument.
 * @param arg3       Fourth system call argument.
 * @param arg4       Fifth system call argument.
 * @param arg5       Sixth system call argument.
 * @param syscall_nr System call number.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int do_syscall(
	int arg0,
	int arg1,
	int arg2,
	int arg3,
	int arg4,
	int arg5,
	int syscall_nr)
{
	((void) arg0);
	((void) arg1);
	((void) arg2);
	((void) arg3);
	((void) arg4);
	((void) arg5);
	((void) syscall_nr);

#if (TEST_TRAP_VERBOSE)
	kprintf("syscall() nr=%x");
	kprintf("syscall() arg0=%x arg1=%x", arg0, arg1);
	kprintf("syscall() arg2=%x arg3=%x", arg2, arg3);
	kprintf("syscall() arg4=%x arg5=%x", arg4, arg5);
#endif

	return (MAGIC5);
}

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Issue a Trap
 */
PRIVATE void test_trap_issue(void)
{
	KASSERT(syscall5(
		SYSCALL_NR,
		MAGIC0,
		MAGIC1,
		MAGIC2,
		MAGIC3,
		MAGIC4
	  ) == MAGIC5
	);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test trap_tests_api[] = {
	{ test_trap_issue, "Issue a Trap" },
	{ NULL,             NULL          },
};

/**
 * The test_trap() function launches testing units on the trap
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_trap(void)
{
	/* API Tests */
	for (int i = 0; trap_tests_api[i].test_fn != NULL; i++)
	{
		trap_tests_api[i].test_fn();
		kprintf("[test][api][trap] %s [passed]", trap_tests_api[i].name);
	}
}

