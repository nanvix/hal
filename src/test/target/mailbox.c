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
 * @brief ID of master NoC node.
 */
#define NODES_AMOUNT    2
#define NODENUM_MASTER 16
#define NODENUM_SLAVE   0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Mailbox Create Unlink
 */
static void test_mailbox_create_unlink(void)
{
	int mbxid;
	int local;

	local = processor_node_get_num(processor_node_get_id());

	KASSERT((mbxid = mailbox_create(local)) >= 0);
	KASSERT(mailbox_unlink(mbxid) == 0);
}

/**
 * @brief API Test: Mailbox Open Close
 */
static void test_mailbox_open_close(void)
{
	int mbxid;
	int remote;

	if (processor_node_get_num(processor_node_get_id()) == NODENUM_MASTER)
		remote = NODENUM_SLAVE;
	else
		remote = NODENUM_MASTER;

	KASSERT((mbxid = mailbox_open(remote)) >= 0);
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

}

/**
 * @brief Fault Injection Test: Mailbox Invalid Open
 */
static void test_mailbox_invalid_open(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Invalid Unlink
 */
static void test_mailbox_invalid_unlink(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Bad Unlink
 */
static void test_mailbox_bad_unlink(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Double Unlink
 */
static void test_mailbox_double_unlink(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Invalid Close
 */
static void test_mailbox_invalid_close(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Bad Close
 */
static void test_mailbox_bad_close(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Double Close
 */
static void test_mailbox_double_close(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Invalid Signal
 */
static void test_mailbox_invalid_signal(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Bad Signal
 */
static void test_mailbox_bad_signal(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Invalid Wait
 */
static void test_mailbox_invalid_wait(void)
{

}

/**
 * @brief Fault Injection Test: Mailbox Bad Signal
 */
static void test_mailbox_bad_wait(void)
{

}

/**
 * @brief Unit tests.
 */
struct test mailbox_tests_fault[] = {
	{NULL,                     NULL             },
	{test_mailbox_invalid_create, "Invalid Create" },
	{test_mailbox_invalid_open,   "Invalid Open"   },
	{test_mailbox_invalid_unlink, "Invalid Unlink" },
	{test_mailbox_bad_unlink,     "Bad Unlink"     },
	{test_mailbox_double_unlink,  "Double Unlink"  },
	{test_mailbox_invalid_close,  "Invalid Close"  },
	{test_mailbox_bad_close,      "Bad Close"      },
	{test_mailbox_double_close,   "Double Close"   },
	{test_mailbox_invalid_signal, "Invalid Signal" },
	{test_mailbox_bad_signal,     "Bad Signal"     },
	{test_mailbox_invalid_wait,   "Invalid Wait"   },
	{test_mailbox_bad_wait,       "Bad Wait"       },
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
