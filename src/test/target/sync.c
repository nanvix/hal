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

#if (__TARGET_HAS_SYNC)

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Synchronization Point Create Unlink
 */
PRIVATE void test_sync_create_unlink(void)
{
	int tmp;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	/* These numbers must be valid. */
	KASSERT((NODENUM_SLAVE + 1) < PROCESSOR_NOC_NODES_NUM);
	KASSERT((NODENUM_SLAVE + 2) < PROCESSOR_NOC_NODES_NUM);

	KASSERT((syncid = sync_create(nodes, NODES_AMOUNT, SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(sync_unlink(syncid) == 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT((syncid = sync_create(nodes, NODES_AMOUNT, SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(sync_unlink(syncid) == 0);
}

/**
 * @brief API Test: Synchronization Point Open Close
 */
PRIVATE void test_sync_open_close(void)
{
	int tmp;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_open(nodes, NODES_AMOUNT, SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(sync_close(syncid) == 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT((syncid = sync_open(nodes, NODES_AMOUNT, SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(sync_close(syncid) == 0);
}

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Create
 */
PRIVATE void test_sync_invalid_create(void)
{
	int tmp;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT(sync_create(NULL, NODES_AMOUNT, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, -1, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, 0, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, 1, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, NODES_AMOUNT, -1) == -EINVAL);
	KASSERT(sync_create(nodes, NODES_AMOUNT, 1000) == -EINVAL);
	KASSERT(sync_create(nodes, (PROCESSOR_NOC_NODES_NUM + 1), SYNC_ALL_TO_ONE) == -EINVAL);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT(sync_create(NULL, NODES_AMOUNT, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, -1, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, 0, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, 1, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, 1000, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, NODES_AMOUNT, -1) == -EINVAL);
	KASSERT(sync_create(nodes, NODES_AMOUNT, 1000) == -EINVAL);
	KASSERT(sync_create(nodes, (PROCESSOR_NOC_NODES_NUM + 1), SYNC_ONE_TO_ALL) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Create
 */
PRIVATE void test_sync_bad_create(void)
{
	int nodes[NODES_AMOUNT + 1];

	/* Invalid list of NoC nodes. */
	for (int i = 0; i < (NODES_AMOUNT + 1); ++i)
		nodes[i] = -1;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);

	/* Underlying NoC node is the sender. */
	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;
	nodes[2] = NODENUM_SLAVE + 1;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	/* Underlying NoC node is the receiver. */
	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_SLAVE + 1;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);

	/* Underlying NoC node is not listed. */
	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_SLAVE + 1;
	nodes[2] = NODENUM_SLAVE + 2;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	/* Underlying NoC node appears twice in the list. */
	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_SLAVE;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_MASTER;

	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_create(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Open
 */
PRIVATE void test_sync_invalid_open(void)
{
	int tmp;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT(sync_open(NULL, NODES_AMOUNT, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, -1, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, 0, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, 1, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, 1000, SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, NODES_AMOUNT, -1) == -EINVAL);
	KASSERT(sync_open(nodes, NODES_AMOUNT, 1000) == -EINVAL);
	KASSERT(sync_open(nodes, (PROCESSOR_NOC_NODES_NUM + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT(sync_open(NULL, NODES_AMOUNT, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, -1, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, 0, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, 1, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, 1000, SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, NODES_AMOUNT, -1) == -EINVAL);
	KASSERT(sync_open(nodes, NODES_AMOUNT, 1000) == -EINVAL);
	KASSERT(sync_open(nodes, (PROCESSOR_NOC_NODES_NUM + 1), SYNC_ALL_TO_ONE) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Open
 */
PRIVATE void test_sync_bad_open(void)
{
	int nodes[NODES_AMOUNT + 1];

	/* Invalid list of NoC nodes. */
	for (int i = 0; i < (NODES_AMOUNT + 1); ++i)
		nodes[i] = -1;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);
	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);

	/* Underlying NoC node is the sender. */
	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_SLAVE + 1;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	/* Underlying NoC node is the receiver. */
	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;
	nodes[2] = NODENUM_SLAVE + 1;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);

	/* Underlying NoC node is not listed. */
	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_SLAVE + 1;
	nodes[2] = NODENUM_SLAVE + 2;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	/* Underlying NoC node appears twice in the list. */
	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_SLAVE;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);

	nodes[0] = NODENUM_SLAVE;
	nodes[1] = NODENUM_MASTER;
	nodes[2] = NODENUM_MASTER;

	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ALL_TO_ONE) == -EINVAL);
	KASSERT(sync_open(nodes, (NODES_AMOUNT + 1), SYNC_ONE_TO_ALL) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Unlink
 */
PRIVATE void test_sync_invalid_unlink(void)
{
	KASSERT(sync_unlink(-1) == -EBADF);
	KASSERT(sync_unlink(1) == -EBADF);
	KASSERT(sync_unlink(1000000) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Unlink
 */
PRIVATE void test_sync_bad_unlink(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_open(nodes, NODES_AMOUNT, SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(sync_unlink(syncid) == -EBADF);
	KASSERT(sync_close(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Double Unlink
 */
PRIVATE void test_sync_double_unlink(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_create(nodes, NODES_AMOUNT, SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(sync_unlink(syncid) == 0);
	KASSERT(sync_unlink(syncid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Close
 */
PRIVATE void test_sync_invalid_close(void)
{
	KASSERT(sync_close(-1) == -EBADF);
	KASSERT(sync_close(1) == -EBADF);
	KASSERT(sync_close(1000000) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Close
 */
PRIVATE void test_sync_bad_close(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_create(nodes, NODES_AMOUNT, SYNC_ALL_TO_ONE)) >= 0);

		KASSERT(sync_close(syncid) == -EBADF);

	KASSERT(sync_unlink(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Double Close
 */
PRIVATE void test_sync_double_close(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_open(nodes, NODES_AMOUNT, SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(sync_close(syncid) == 0);
	KASSERT(sync_close(syncid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Signal
 */
PRIVATE void test_sync_invalid_signal(void)
{
	KASSERT(sync_signal(-1) == -EBADF);
	KASSERT(sync_signal(1000) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Signal
 */
PRIVATE void test_sync_bad_signal(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_create(nodes, NODES_AMOUNT, SYNC_ALL_TO_ONE)) >= 0);

		KASSERT(sync_signal(syncid) == -EBADF);

	KASSERT(sync_unlink(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Wait
 */
PRIVATE void test_sync_invalid_wait(void)
{
	KASSERT(sync_wait(-1) == -EBADF);
	KASSERT(sync_wait(1000) == -EBADF);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Signal
 */
PRIVATE void test_sync_bad_wait(void)
{
	int syncid;
	int nodes[NODES_AMOUNT];

	nodes[0] = NODENUM_MASTER;
	nodes[1] = NODENUM_SLAVE;

	KASSERT((syncid = sync_open(nodes, NODES_AMOUNT, SYNC_ONE_TO_ALL)) >= 0);

		KASSERT(sync_wait(syncid) == -EBADF);

	KASSERT(sync_close(syncid) == 0);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test sync_tests_api[] = {
	{ test_sync_create_unlink, "create unlink" },
	{ test_sync_open_close,    "open close   " },
	{ NULL,                     NULL           },
};

/**
 * @brief Unit tests.
 */
PRIVATE struct test sync_tests_fault[] = {
	{ test_sync_invalid_create, "invalid create" },
	{ test_sync_bad_create,     "bad create    " },
	{ test_sync_invalid_open,   "invalid open  " },
	{ test_sync_bad_open,       "bad open      " },
	{ test_sync_bad_unlink,     "bad unlink    " },
	{ test_sync_double_unlink,  "double unlink " },
	{ test_sync_invalid_close,  "invalid close " },
	{ test_sync_bad_close,      "bad close     " },
	{ test_sync_double_close,   "double close  " },
	{ test_sync_invalid_signal, "invalid signal" },
	{ test_sync_bad_signal,     "bad signal    " },
	{ test_sync_invalid_wait,   "invalid wait  " },
	{ test_sync_bad_wait,       "bad wait      " },
	{ NULL,                      NULL            },
};

/**
 * The test_sync() function launches testing units on the sync
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_sync(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; sync_tests_api[i].test_fn != NULL; i++)
	{
		sync_tests_api[i].test_fn();
		kprintf("[test][api][sync] %s [passed]", sync_tests_api[i].name);
	}

	/* FAULT Tests */
	kprintf(HLINE);
	for (int i = 0; sync_tests_fault[i].test_fn != NULL; i++)
	{
		sync_tests_fault[i].test_fn();
		kprintf("[test][fault][sync] %s [passed]", sync_tests_fault[i].name);
	}
}

#endif /* !__TARGET_HAS_SYNC */
