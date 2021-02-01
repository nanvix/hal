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

#define __NEED_RESOURCE

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include "../test.h"

/**
 * @brief Number of resource used.
 */
#define TEST_RESOURCE_MAX 10

/**
 * @brief Resource Node.
 */
struct test_resource
{
	struct resource resource;
	int id;
} resources[TEST_RESOURCE_MAX];

/**
 * @brief API: Test queue behaviour.
 */
PRIVATE void test_resource_queue(void)
{
	struct test_resource * r;
	struct resource_arrangement arr = RESOURCE_ARRANGEMENT_INITIALIZER;

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		resources[i].resource = RESOURCE_INITIALIZER;
		resources[i].id = i;

		KASSERT(resource_enqueue(&arr, &resources[i].resource) >= 0);
	}

	KASSERT(arr.size == TEST_RESOURCE_MAX);
	KASSERT(arr.head == &resources[0].resource);
	KASSERT(arr.tail == &resources[TEST_RESOURCE_MAX - 1].resource);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		KASSERT((r = (struct test_resource *) resource_dequeue(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);
}

/**
 * @brief API: Test list behaviour.
 */
PRIVATE void test_resource_list(void)
{
	struct test_resource * r;
	struct resource_arrangement arr = RESOURCE_ARRANGEMENT_INITIALIZER;

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		resources[i].resource = RESOURCE_INITIALIZER;
		resources[i].id = i;

		KASSERT(resource_push_front(&arr, &resources[i].resource) >= 0);
	}

	KASSERT(arr.size == TEST_RESOURCE_MAX);
	KASSERT(arr.head == &resources[TEST_RESOURCE_MAX - 1].resource);
	KASSERT(arr.tail == &resources[0].resource);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		KASSERT((r = (struct test_resource *) resource_pop_back(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
		KASSERT(resource_push_back(&arr, &resources[i].resource) >= 0);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		KASSERT((r = (struct test_resource *) resource_pop_front(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
		KASSERT(resource_push_front(&arr, &resources[i].resource) >= 0);

	for (int i = TEST_RESOURCE_MAX - 1; i >= 0; --i)
	{
		KASSERT((r = (struct test_resource *) resource_pop_front(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
		KASSERT(resource_push_back(&arr, &resources[i].resource) >= 0);

	for (int i = TEST_RESOURCE_MAX - 1; i >= 0; --i)
	{
		KASSERT((r = (struct test_resource *) resource_pop_back(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	KASSERT(resource_insert(&arr, &resources[0].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[1].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[2].resource, 2) >= 0);

	KASSERT((r = (struct test_resource *) resource_pop_front(&arr)) != NULL);
	KASSERT(r->id == 1);
	KASSERT((r = (struct test_resource *) resource_pop_front(&arr)) != NULL);
	KASSERT(r->id == 0);
	KASSERT((r = (struct test_resource *) resource_pop_front(&arr)) != NULL);
	KASSERT(r->id == 2);

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	KASSERT(resource_insert(&arr, &resources[0].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[1].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[2].resource, 2) >= 0);

	KASSERT(resource_search(&arr, &resources[1].resource) >= 0);
	KASSERT(resource_search(&arr, &resources[0].resource) >= 1);
	KASSERT(resource_search(&arr, &resources[2].resource) >= 2);

	KASSERT((r = (struct test_resource *) resource_remove(&arr, 1)) != NULL);
	KASSERT(r->id == 0);
	KASSERT((r = (struct test_resource *) resource_remove(&arr, 0)) != NULL);
	KASSERT(r->id == 1);
	KASSERT((r = (struct test_resource *) resource_remove(&arr, 0)) != NULL);
	KASSERT(r->id == 2);

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);

	KASSERT(resource_insert(&arr, &resources[0].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[1].resource, 0) >= 0);
	KASSERT(resource_insert(&arr, &resources[2].resource, 2) >= 0);

	KASSERT(resource_pop(&arr, &resources[2].resource) == 2);
	KASSERT(resource_pop(&arr, &resources[0].resource) == 1);
	KASSERT(resource_pop(&arr, &resources[1].resource) == 0);

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL && arr.tail == NULL);
}

/**
 * @brief Auxiliar: Compare two resource and verify if a < b.
 */
PRIVATE int test_incresing(struct resource * a, struct resource * b)
{
	int _a = ((struct test_resource *) a)->id;
	int _b = ((struct test_resource *) b)->id;

	return (_a - _b);
}

/**
 * @brief Auxiliar: Compare two resource and verify if a > b.
 */
PRIVATE int test_decreasing(struct resource * a, struct resource * b)
{
	int _a = ((struct test_resource *) a)->id;
	int _b = ((struct test_resource *) b)->id;

	return (_b - _a);
}

/**
 * @brief Auxiliar: Verify if a is odd.
 */
PRIVATE bool test_verify_id_odd(struct resource * a)
{
	return (((struct test_resource *) a)->id & 1);
}

/**
 * @brief Auxiliar: Return true. 
 */
PRIVATE bool test_verify_id_all(struct resource * a)
{
	return (a != NULL);
}

/**
 * @brief API: Verify compare/verify to insert and remove.. 
 */
PRIVATE void test_resource_cmp_vrf(void)
{
	struct test_resource * r;
	struct resource_arrangement arr = RESOURCE_ARRANGEMENT_INITIALIZER;

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		resources[i].resource = RESOURCE_INITIALIZER;
		resources[i].id = i;

		KASSERT(resource_insert_ordered(&arr, &resources[i].resource, test_incresing) >= 0);
	}

	KASSERT(arr.size == TEST_RESOURCE_MAX);
	KASSERT(arr.head == &resources[0].resource);
	KASSERT(arr.tail == &resources[TEST_RESOURCE_MAX - 1].resource);

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
	{
		KASSERT((r = (struct test_resource *) resource_dequeue(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	for (int i = 0; i < TEST_RESOURCE_MAX; ++i)
		KASSERT(resource_insert_ordered(&arr, &resources[i].resource, test_decreasing) >= 0);

	KASSERT(arr.size == TEST_RESOURCE_MAX);
	KASSERT(arr.head == &resources[TEST_RESOURCE_MAX - 1].resource);
	KASSERT(arr.tail == &resources[0].resource);

	for (int i = TEST_RESOURCE_MAX - 1; i >= 0; --i)
	{
		KASSERT((r = (struct test_resource *) resource_dequeue(&arr)) != NULL);
		KASSERT(r->id == i);
	}

	KASSERT(resource_insert_ordered(&arr, &resources[0].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[1].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[2].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[3].resource, test_incresing) >= 0);

	KASSERT(arr.size == 4);
	KASSERT(arr.head == &resources[0].resource);
	KASSERT(arr.tail == &resources[3].resource);

	KASSERT(resource_remove_verify(&arr, test_verify_id_odd) == &resources[1].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_odd) == &resources[3].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_odd) == NULL);

	KASSERT(arr.size == 2);
	KASSERT(arr.head == &resources[0].resource);
	KASSERT(arr.tail == &resources[2].resource);

	for (int i = 0; i < 2; ++i)
	{
		KASSERT((r = (struct test_resource *) resource_dequeue(&arr)) != NULL);
		KASSERT((r->id & 1) == 0);
	}

	KASSERT(resource_insert_ordered(&arr, &resources[0].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[1].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[2].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[3].resource, test_incresing) >= 0);

	KASSERT(arr.size == 4);
	KASSERT(arr.head == &resources[0].resource);
	KASSERT(arr.tail == &resources[3].resource);

	KASSERT(resource_remove_verify(&arr, test_verify_id_all) == &resources[0].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_all) == &resources[1].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_all) == &resources[2].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_all) == &resources[3].resource);
	KASSERT(resource_remove_verify(&arr, test_verify_id_all) == NULL);

	KASSERT(arr.size == 0);
	KASSERT(arr.head == NULL);
	KASSERT(arr.tail == NULL);

	resources[0].id = 1;
	resources[1].id = 1;
	resources[2].id = 1;
	KASSERT(resource_insert_ordered(&arr, &resources[0].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[2].resource, test_incresing) >= 0);
	KASSERT(resource_insert_ordered(&arr, &resources[1].resource, test_incresing) >= 0);
	KASSERT(resource_dequeue(&arr) == &resources[0].resource);
	KASSERT(resource_dequeue(&arr) == &resources[2].resource);
	KASSERT(resource_dequeue(&arr) == &resources[1].resource);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief API Tests.
 */
PRIVATE struct test resource_tests_api[] = {
	{ test_resource_queue,   "Resource queue behaviour " },
	{ test_resource_list,    "Resource list behaviour  " },
	{ test_resource_cmp_vrf, "Resource compare/verify  " },
	{ NULL,                  NULL                        },
};

/**
 * The test_abstract_resource() function launches testing units on the
 * abstractions of the nanvix.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_abstract_resource(void)
{
	/* API Tests */
	CLUSTER_KPRINTF(HLINE);
	for (int i = 0; resource_tests_api[i].test_fn != NULL; i++)
	{
		resource_tests_api[i].test_fn();
		CLUSTER_KPRINTF("[test][hal][resource] %s [passed]", resource_tests_api[i].name);
	}
}
