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

#if (__TARGET_HAS_MAILBOX)

/**
 * @brief ID of master NoC node.
 */
#define NODES_AMOUNT   2
#define NODENUM_MASTER PROCESSOR_NODENUM_MASTER
#ifdef __mppa256__
	#define NODENUM_SLAVE (PROCESSOR_NODENUM_MASTER + PROCESSOR_NOC_IONODES_NUM)
#else
	#define NODENUM_SLAVE (PROCESSOR_NODENUM_MASTER + 1)
#endif

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Mailbox Create Unlink
 */
PRIVATE void test_mailbox_create_unlink(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);
	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief API Test: Mailbox Open Close
 */
PRIVATE void test_mailbox_open_close(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);
	KASSERT(mailbox_close(mbxid) == 0);
}

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/**
 * @brief Fault Injection Test: Mailbox Invalid Create
 */
PRIVATE void test_mailbox_invalid_create(void)
{
	KASSERT(mailbox_create(-1) == -EINVAL);
	KASSERT(mailbox_create(PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Open
 */
PRIVATE void test_mailbox_invalid_open(void)
{
	KASSERT(mailbox_open(-1) == -EINVAL);
	KASSERT(mailbox_open(PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Unlink
 */
PRIVATE void test_mailbox_invalid_unlink(void)
{
	KASSERT(mailbox_unlink(-1) == -EBADF);
	KASSERT(mailbox_unlink(HAL_MAILBOX_CREATE_MAX) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Close
 */
PRIVATE void test_mailbox_invalid_close(void)
{
	KASSERT(mailbox_close(-1) == -EBADF);
	KASSERT(mailbox_close(HAL_MAILBOX_OPEN_MAX) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Read
 */
PRIVATE void test_mailbox_invalid_read(void)
{
	int mbxid;
	char msg[HAL_MAILBOX_MSG_SIZE];

	KASSERT(mailbox_aread(-1, msg, HAL_MAILBOX_MSG_SIZE) == -EBADF);

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);

		KASSERT(mailbox_aread(mbxid, NULL, HAL_MAILBOX_MSG_SIZE) == -EINVAL);
		KASSERT(mailbox_aread(mbxid, msg, 0) == -EINVAL);

	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Write
 */
PRIVATE void test_mailbox_invalid_write(void)
{
	int mbxid;
	char msg[HAL_MAILBOX_MSG_SIZE];

	KASSERT(mailbox_awrite(-1, msg, HAL_MAILBOX_MSG_SIZE) == -EBADF);

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);

		KASSERT(mailbox_awrite(mbxid, NULL, HAL_MAILBOX_MSG_SIZE) == -EINVAL);
		KASSERT(mailbox_awrite(mbxid, msg, 0) == -EINVAL);

	KASSERT(mailbox_close(mbxid) == 0);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Create
 */
PRIVATE void test_mailbox_bad_create(void)
{
	KASSERT(mailbox_create(NODENUM_SLAVE) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Open
 */
PRIVATE void test_mailbox_bad_open(void)
{
#ifdef __mppa256__
	/**
	 * This doesn't fail on MPPA-256 because the IOCluster is able to open
	 * locally (multiples nodes inside the cluster).
	 */
	if (cluster_is_ccluster(cluster_get_num()))
#endif
		KASSERT(mailbox_open(NODENUM_MASTER) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Unlink
 */
PRIVATE void test_mailbox_bad_unlink(void)
{
	KASSERT(mailbox_unlink(0) == -EBADF);
	KASSERT(mailbox_unlink(HAL_MAILBOX_CREATE_MAX - 1) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Close
 */
PRIVATE void test_mailbox_bad_close(void)
{
	KASSERT(mailbox_close(0) == -EBADF);
	KASSERT(mailbox_close(HAL_MAILBOX_OPEN_MAX - 1) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Double Unlink
 */
PRIVATE void test_mailbox_double_unlink(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);
	KASSERT(mailbox_unlink(mbxid) == 0);
	KASSERT(mailbox_unlink(mbxid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Double Close
 */
PRIVATE void test_mailbox_double_close(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);
	KASSERT(mailbox_close(mbxid) == 0);
	KASSERT(mailbox_close(mbxid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Write
 */
PRIVATE void test_mailbox_bad_read(void)
{
	int mbxid;
	char msg[HAL_MAILBOX_MSG_SIZE];

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);

		KASSERT(mailbox_awrite(mbxid, NULL, HAL_MAILBOX_MSG_SIZE) == -EINVAL);
		KASSERT(mailbox_awrite(mbxid, msg, 0) == -EINVAL);

	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Write
 */
PRIVATE void test_mailbox_bad_write(void)
{
	int mbxid;
	char msg[HAL_MAILBOX_MSG_SIZE];

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);

		KASSERT(mailbox_aread(mbxid, NULL, HAL_MAILBOX_MSG_SIZE) == -EINVAL);
		KASSERT(mailbox_aread(mbxid, msg, 0) == -EINVAL);

	KASSERT(mailbox_close(mbxid) == 0);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test mailbox_tests_api[] = {
	/* Intra-Cluster API Tests */
	{ test_mailbox_create_unlink, "create unlink" },
	{ test_mailbox_open_close,    "open close   " },
	{ NULL,                        NULL           },
};

/**
 * @brief Unit tests.
 */
PRIVATE struct test mailbox_tests_fault[] = {
	{ test_mailbox_invalid_create, "invalid create" },
	{ test_mailbox_invalid_open,   "invalid open  " },
	{ test_mailbox_invalid_unlink, "invalid unlink" },
	{ test_mailbox_invalid_close,  "invalid close " },
	{ test_mailbox_invalid_read,   "invalid read  " },
	{ test_mailbox_invalid_write,  "invalid write " },
	{ test_mailbox_bad_create,     "bad create    " },
	{ test_mailbox_bad_open,       "bad open      " },
	{ test_mailbox_bad_unlink,     "bad unlink    " },
	{ test_mailbox_bad_close,      "bad close     " },
	{ test_mailbox_double_unlink,  "double unlink " },
	{ test_mailbox_double_close,   "double close  " },
	{ test_mailbox_bad_read,       "bad read      " },
	{ test_mailbox_bad_write,      "bad write     " },
	{ NULL,                         NULL            },
};

/**
 * The test_mailbox() function launches testing units on the mailbox
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_mailbox(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; mailbox_tests_api[i].test_fn != NULL; i++)
	{
		mailbox_tests_api[i].test_fn();
		kprintf("[test][api][mailbox] %s [passed]", mailbox_tests_api[i].name);
	}

	/* FAULT Tests */
	kprintf(HLINE);
	for (int i = 0; mailbox_tests_fault[i].test_fn != NULL; i++)
	{
		mailbox_tests_fault[i].test_fn();
		kprintf("[test][fault][mailbox] %s [passed]", mailbox_tests_fault[i].name);
	}
}

#endif /* __TARGET_HAS_MAILBOX */
