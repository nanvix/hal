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
 * @brief ID of master NoC node.
 */
#define NODES_AMOUNT 2
#define NODENUM_MASTER 16
#define NODENUM_SLAVE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Synchronization Point Create Unlink
 */
static void hal_test_sync_create_unlink(void)
{
	int tmp;
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(hal_sync_unlink(syncid) == 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT((syncid = hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(hal_sync_unlink(syncid) == 0);
}

/**
 * @brief API Test: Synchronization Point Open Close
 */
static void hal_test_sync_open_close(void)
{
	int tmp;
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(hal_sync_close(syncid) == 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT((syncid = hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(hal_sync_close(syncid) == 0);
}

/*============================================================================*/

/**
 * @brief Unit tests.
 */
struct test sync_tests_api[] = {
	/* Intra-Cluster API Tests */
	{hal_test_sync_create_unlink, "Create Unlink" },
	{hal_test_sync_open_close,    "Open Close"    },
	{NULL,                        NULL            },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Create
 */
static void test_hal_sync_invalid_create(void)
{
	int tmp;
	int nodenum;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;
	nodes[1] = nodenum;

	KASSERT(hal_sync_create(NULL, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, -1, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, 0, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, 1, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, 1000, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, 1000) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, 1000) < 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT(hal_sync_create(NULL, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, -1, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, 0, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, 1, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, 1000, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, 1000) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_create(nodes, NODES_AMOUNT, 1000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Open
 */
static void test_hal_sync_invalid_open(void)
{
	int tmp;
	int nodenum;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT(hal_sync_open(NULL, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, -1, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, 0, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, 1, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, 1000, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, 1000) < 0);

	tmp = nodes[0];
	nodes[0] = nodes[1];
	nodes[1] = tmp;

	KASSERT(hal_sync_open(NULL, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, -1, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, 0, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, 1, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, 1000, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, -1) < 0);
	KASSERT(hal_sync_open(nodes, NODES_AMOUNT, 1000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Unlink
 */
static void test_hal_sync_invalid_unlink(void)
{
	KASSERT(hal_sync_unlink(-1) < 0);
	KASSERT(hal_sync_unlink(1) < 0);
	KASSERT(hal_sync_unlink(1000000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Unlink
 */
static void test_hal_sync_bad_unlink(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(hal_sync_unlink(syncid) < 0);
	KASSERT(hal_sync_close(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Double Unlink
 */
static void test_hal_sync_double_unlink(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(hal_sync_unlink(syncid) == 0);
	KASSERT(hal_sync_unlink(syncid) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Close
 */
static void test_hal_sync_invalid_close(void)
{
	KASSERT(hal_sync_close(-1) < 0);
	KASSERT(hal_sync_close(1) < 0);
	KASSERT(hal_sync_close(1000000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Close
 */
static void test_hal_sync_bad_close(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(hal_sync_close(syncid) < 0);
	KASSERT(hal_sync_unlink(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Double Close
 */
static void test_hal_sync_double_close(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(hal_sync_close(syncid) == 0);
	KASSERT(hal_sync_close(syncid) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Signal
 */
static void test_hal_sync_invalid_signal(void)
{
	KASSERT(hal_sync_signal(-1) < 0);
	KASSERT(hal_sync_signal(1) < 0);
	KASSERT(hal_sync_signal(1000000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Signal
 */
static void test_hal_sync_bad_signal(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_create(nodes, NODES_AMOUNT, HAL_SYNC_ALL_TO_ONE)) >= 0);
	KASSERT(hal_sync_signal(syncid) < 0);
	KASSERT(hal_sync_unlink(syncid) == 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Invalid Wait
 */
static void test_hal_sync_invalid_wait(void)
{
	KASSERT(hal_sync_wait(-1) < 0);
	KASSERT(hal_sync_wait(1) < 0);
	KASSERT(hal_sync_wait(1000000) < 0);
}

/**
 * @brief Fault Injection Test: Synchronization Point Bad Signal
 */
static void test_hal_sync_bad_wait(void)
{
	int nodenum;
	int syncid;
	int nodes[NODES_AMOUNT];

	nodenum = processor_node_get_num(processor_node_get_id());
	nodes[0] = nodenum;
	nodes[1] = nodenum == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	KASSERT((syncid = hal_sync_open(nodes, NODES_AMOUNT, HAL_SYNC_ONE_TO_ALL)) >= 0);
	KASSERT(hal_sync_wait(syncid) < 0);
	KASSERT(hal_sync_close(syncid) == 0);
}

/**
 * @brief Unit tests.
 */
struct test sync_tests_fault[] = {
	{test_hal_sync_invalid_create, "Invalid Create" },
	{test_hal_sync_invalid_open,   "Invalid Open"   },
	{test_hal_sync_invalid_unlink, "Invalid Unlink" },
	{test_hal_sync_bad_unlink,     "Bad Unlink"     },
	{test_hal_sync_double_unlink,  "Double Unlink"  },
	{test_hal_sync_invalid_close,  "Invalid Close"  },
	{test_hal_sync_bad_close,      "Bad Close"      },
	{test_hal_sync_double_close,   "Double Close"   },
	{test_hal_sync_invalid_signal, "Invalid Signal" },
	{test_hal_sync_bad_signal,     "Bad Signal"     },
	{test_hal_sync_invalid_wait,   "Invalid Wait"   },
	{test_hal_sync_bad_wait,       "Bad Wait"       },
	{NULL,                         NULL             },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_sync() function launches testing units on the sync
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_sync(void)
{
	/* API Tests */
	for (int i = 0; sync_tests_api[i].test_fn != NULL; i++)
	{
		sync_tests_api[i].test_fn();
		kprintf("[test][api][sync] %s [passed]", sync_tests_api[i].name);
	}

	/* FAULT Tests */
	for (int i = 0; sync_tests_fault[i].test_fn != NULL; i++)
	{
		sync_tests_fault[i].test_fn();
		kprintf("[test][fault][sync] %s [passed]", sync_tests_fault[i].name);
	}
}
