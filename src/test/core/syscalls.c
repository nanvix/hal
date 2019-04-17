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
#include "../test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_TRAP_VERBOSE 0

/**
 * @brief Magic Numbers.
 */
/**@{*/
#define SYSCALL0_NR          1
#define SYSCALL1_NR          2
#define SYSCALL2_NR          3
#define SYSCALL3_NR          4
#define SYSCALL4_NR          5
#define SYSCALL5_NR          6
#define MAGIC0       0xbeefcace
#define MAGIC1       0xC00010FF
#define MAGIC2       0xcafebabe
#define MAGIC3       0xcafed00d
#define MAGIC4       0x0d15ea5e
#define MAGIC5       0xdead10cc
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
	unsigned arg0,
	unsigned arg1,
	unsigned arg2,
	unsigned arg3,
	unsigned arg4,
	unsigned syscall_nr)
{

#if (TEST_TRAP_VERBOSE)
	kprintf("syscall() nr=%x");
	kprintf("syscall() arg0=%x arg1=%x", arg0, arg1);
	kprintf("syscall() arg2=%x arg3=%x", arg2, arg3);
	kprintf("syscall() arg4=%x", arg4);
#endif

	if (syscall_nr >= SYSCALL1_NR)
		KASSERT(arg0 == MAGIC0);
	if (syscall_nr >= SYSCALL2_NR)
		KASSERT(arg1 == MAGIC1);
	if (syscall_nr >= SYSCALL3_NR)
		KASSERT(arg2 == MAGIC2);
	if (syscall_nr >= SYSCALL4_NR)
		KASSERT(arg3 == MAGIC3);
	if (syscall_nr >= SYSCALL5_NR)
		KASSERT(arg4 == MAGIC4);

	return (MAGIC5);
}

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Issue a Trap with No Arguments                                             *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with No Arguments
 */
PRIVATE void test_trap_issue0(void)
{
	KASSERT(syscall0(
		SYSCALL0_NR
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Issue a Trap with One Argument                                             *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with One Argument
 */
PRIVATE void test_trap_issue1(void)
{
	KASSERT(syscall1(
		SYSCALL1_NR,
		MAGIC0
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Issue a Trap with Two Arguments                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with Two Arguments
 */
PRIVATE void test_trap_issue2(void)
{
	KASSERT(syscall2(
		SYSCALL2_NR,
		MAGIC0,
		MAGIC1
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Issue a Trap with Three Arguments                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with Three Arguments
 */
PRIVATE void test_trap_issue3(void)
{
	KASSERT(syscall3(
		SYSCALL3_NR,
		MAGIC0,
		MAGIC1,
		MAGIC2
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Issue a Trap with Four Arguments                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with Four Arguments
 */
PRIVATE void test_trap_issue4(void)
{
	KASSERT(syscall4(
		SYSCALL4_NR,
		MAGIC0,
		MAGIC1,
		MAGIC2,
		MAGIC3
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Issue a Trap with Five Arguments                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Issue a Trap with Five Arguments
 */
PRIVATE void test_trap_issue5(void)
{
	KASSERT(syscall5(
		SYSCALL5_NR,
		MAGIC0,
		MAGIC1,
		MAGIC2,
		MAGIC3,
		MAGIC4
	  ) == MAGIC5
	);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test trap_tests_api[] = {
	{ test_trap_issue0, "issue a trap no arguments"    },
	{ test_trap_issue1, "issue a trap one argument"    },
	{ test_trap_issue2, "issue a trap two arguments"   },
	{ test_trap_issue3, "issue a trap three arguments" },
	{ test_trap_issue4, "issue a trap four arguments"  },
	{ test_trap_issue5, "issue a trap five arguments"  },
	{ NULL,             NULL                           },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

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

