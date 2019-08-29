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
#include "../test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_NOC_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Query Logical NoC Node ID                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Query Logical NoC Node ID
 */
PRIVATE void test_node_get_num(void)
{
	int nodenum;

	nodenum = cluster_get_num();

#if (TEST_NOC_VERBOSE)
	kprintf("[test][processor][node][api] noc node %d online", nodenum);
#endif

	KASSERT(nodenum == PROCESSOR_NODENUM_MASTER);
}

/*----------------------------------------------------------------------------*
 * Query NoC Node Type                                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Query NoC Node Type
 */
PRIVATE void test_node_get_type(void)
{
	KASSERT(processor_noc_is_ionode(PROCESSOR_NODENUM_MASTER));
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief API Tests.
 */
PRIVATE struct test test_api_node[] = {
	{ test_node_get_num,  "get logical noc node id" },
	{ test_node_get_type, "get noc node type      " },
	{ NULL,                NULL                     },
};

/**
 * The test_noc() function launches regression tests on the NoC
 * Interface of the Processor Abstraction Layer.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_noc(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; test_api_node[i].test_fn != NULL; i++)
	{
		test_api_node[i].test_fn();
		kprintf("[test][processor][node][api] %s [passed]",
			test_api_node[i].name
		);
	}
}

