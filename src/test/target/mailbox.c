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

#if (__TARGET_HAS_MAILBOX)

/**
 * @brief ID of master NoC node.
 */
#define NODES_AMOUNT   2
#define NODENUM_MASTER 0
#define NODENUM_SLAVE  1

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Mailbox Create Unlink
 */
static void test_mailbox_create_unlink(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);
	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief API Test: Mailbox Open Close
 */
static void test_mailbox_open_close(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);
	KASSERT(mailbox_close(mbxid) == 0);
}

/*============================================================================*/

/**
 * @brief Unit tests.
 */
struct test mailbox_tests_api[] = {
	/* Intra-Cluster API Tests */
	{test_mailbox_create_unlink, "Create Unlink" },
	{test_mailbox_open_close,    "Open Close"    },
	{NULL,                       NULL            },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/**
 * @brief Fault Injection Test: Mailbox Invalid Create
 */
static void test_mailbox_invalid_create(void)
{
	KASSERT(mailbox_create(-1) == -EINVAL);
	KASSERT(mailbox_create(PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Open
 */
static void test_mailbox_invalid_open(void)
{
	KASSERT(mailbox_open(-1) == -EINVAL);
	KASSERT(mailbox_open(PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Unlink
 */
static void test_mailbox_invalid_unlink(void)
{
	KASSERT(mailbox_unlink(-1) == -EINVAL);
	KASSERT(mailbox_unlink(MAILBOX_CREATE_MAX) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Close
 */
static void test_mailbox_invalid_close(void)
{
	KASSERT(mailbox_close(-1) == -EINVAL);
	KASSERT(mailbox_close(MAILBOX_OPEN_MAX) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Read
 */
static void test_mailbox_invalid_read(void)
{
	char msg[MAILBOX_MSG_SIZE];

	KASSERT(mailbox_read(-1, msg, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_read(0, NULL, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_read(0, msg, 0) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Invalid Write
 */
static void test_mailbox_invalid_write(void)
{
	char msg[MAILBOX_MSG_SIZE];

	KASSERT(mailbox_write(-1, msg, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_write(0, NULL, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_write(0, msg, 0) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Create
 */
static void test_mailbox_bad_create(void)
{
	KASSERT(mailbox_create(NODENUM_SLAVE) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Open
 */
static void test_mailbox_bad_open(void)
{
	KASSERT(mailbox_open(NODENUM_MASTER) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Unlink
 */
static void test_mailbox_bad_unlink(void)
{
	KASSERT(mailbox_unlink(0) == -EINVAL);
	KASSERT(mailbox_unlink(MAILBOX_CREATE_MAX - 1) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Close
 */
static void test_mailbox_bad_close(void)
{
	KASSERT(mailbox_close(0) == -EINVAL);
	KASSERT(mailbox_close(MAILBOX_OPEN_MAX - 1) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Double Unlink
 */
static void test_mailbox_double_unlink(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);
	KASSERT(mailbox_unlink(mbxid) == 0);
	KASSERT(mailbox_unlink(mbxid) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Double Close
 */
static void test_mailbox_double_close(void)
{
	int mbxid;

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);
	KASSERT(mailbox_close(mbxid) == 0);
	KASSERT(mailbox_close(mbxid) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Write
 */
static void test_mailbox_bad_read(void)
{
	int mbxid;
	char msg[MAILBOX_MSG_SIZE];

	KASSERT((mbxid = mailbox_create(NODENUM_MASTER)) >= 0);
	KASSERT(mailbox_write(mbxid, NULL, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_write(mbxid, msg, 0) == -EINVAL);
	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief Fault Injection Test: Mailbox Bad Write
 */
static void test_mailbox_bad_write(void)
{
	int mbxid;
	char msg[MAILBOX_MSG_SIZE];

	KASSERT((mbxid = mailbox_open(NODENUM_SLAVE)) >= 0);
	KASSERT(mailbox_read(mbxid, NULL, MAILBOX_MSG_SIZE) == -EINVAL);
	KASSERT(mailbox_read(mbxid, msg, 0) == -EINVAL);
	KASSERT(mailbox_close(mbxid) == 0);
}

/**
 * @brief Unit tests.
 */
struct test mailbox_tests_fault[] = {
	{test_mailbox_invalid_create, "Invalid Create" },
	{test_mailbox_invalid_open,   "Invalid Open"   },
	{test_mailbox_invalid_unlink, "Invalid Unlink" },
	{test_mailbox_invalid_close,  "Invalid Close"  },
	{test_mailbox_invalid_read,   "Invalid Read"   },
	{test_mailbox_invalid_write,  "Invalid Write"  },
	{test_mailbox_bad_create,     "Bad Create"     },
	{test_mailbox_bad_open,       "Bad Open"       },
	{test_mailbox_bad_unlink,     "Bad Unlink"     },
	{test_mailbox_bad_close,      "Bad Close"      },
	{test_mailbox_double_unlink,  "Double Unlink"  },
	{test_mailbox_double_close,   "Double Close"   },
	{test_mailbox_bad_read,       "Bad Read"       },
	{test_mailbox_bad_write,      "Bad Write"      },
	{NULL,                         NULL            },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_mailbox() function launches testing units on the mailbox
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_mailbox(void)
{
	/* API Tests */
	for (int i = 0; mailbox_tests_api[i].test_fn != NULL; i++)
	{
		mailbox_tests_api[i].test_fn();
		kprintf("[test][api][mailbox] %s [passed]", mailbox_tests_api[i].name);
	}

	/* FAULT Tests */
	for (int i = 0; mailbox_tests_fault[i].test_fn != NULL; i++)
	{
		mailbox_tests_fault[i].test_fn();
		kprintf("[test][fault][mailbox] %s [passed]", mailbox_tests_fault[i].name);
	}
}

#endif /* __TARGET_HAS_MAILBOX */
