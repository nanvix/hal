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
 * @brief Number of iterations for stress tests.
 */
#define NITERATIONS 10

/*----------------------------------------------------------------------------*
 * Barrier                                                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief A simple barrier.
 */
struct barrier
{
	int nreached;    /**< Number of cores that reached the barrier */
	int ncores;      /**< Number of cores in the barrier.          */
	spinlock_t lock; /**< Lock.                                    */
};

/**
 * @brief Initializes a barrier.
 *
 * @param b      Target barrier.
 * @param ncores Number of cores in the barrier.
 */
PRIVATE void barrier_init(struct barrier *b, int ncores)
{
	b->nreached = 0;
	b->ncores = ncores;
	b->lock = SPINLOCK_UNLOCKED;
	dcache_invalidate();
}

/**
 * @brief Waits in a barrier.
 */
PRIVATE void barrier_wait(struct barrier *b)
{
	again:

		spinlock_lock(&b->lock);

			if (b->nreached < b->ncores)
			{
				spinlock_unlock(&b->lock);
				goto again;
			}

		spinlock_unlock(&b->lock);
}

/*============================================================================*
 * Stress Tests                                                               *
 *============================================================================*/

/**
 * @brief Joins a barrier.
 *
 * @param b Target barrier.
 */
PRIVATE void barrier_join(struct barrier *b)
{
	spinlock_lock(&b->lock);

		b->nreached++;

	spinlock_unlock(&b->lock);
}

/*----------------------------------------------------------------------------*
 * Dummy Task                                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief Join barrier for task 0.
 */
PRIVATE struct barrier task0_barrier;

/**
 * @brief Dummy task.
 */
PRIVATE void task0(void)
{
	const int n = 1000;
	volatile int k = 0;

	/* Waste some cycles. */
	for (int i = 0; i < n; i++)
		k++;

	KASSERT(k == n);
	barrier_join(&task0_barrier);
}

/*----------------------------------------------------------------------------*
 * Dummy Task 1                                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief Join barrier for task 1.
 */
PRIVATE struct barrier task1_barrier;

/**
 * @brief Dummy task.
 */
PRIVATE void task1(void)
{
	int mycoreid;

	mycoreid = core_get_id();

	for (int k = 0; k < NITERATIONS; k++)
	{
		barrier_init(&task0_barrier, CORES_NUM - 2);

		/* Spawn execution in all cores. */
		for (int i = 0; i < CORES_NUM; i++)
		{
			if ((i != COREID_MASTER) && (i != mycoreid))
			{
				int ret;

				do
				{
					ret = core_start(i, task0);
					KASSERT((ret == 0) || (ret == -EBUSY));
				} while (ret != 0);
			}
		}

		/* Wait for slave cores. */
		barrier_wait(&task0_barrier);
	}

	barrier_join(&task1_barrier);
}

/*----------------------------------------------------------------------------*
 * Spawn Execution from Master Core                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief Stress Test: Spawn Execution from Master Core
 */
PRIVATE void test_cluster_cores_stress_master_spawn(void)
{
	for (int k = 0; k < NITERATIONS; k++)
	{
		barrier_init(&task0_barrier, CORES_NUM - 1);

		/* Spawn execution in all cores. */
		for (int i = 0; i < CORES_NUM; i++)
		{
			if (i != COREID_MASTER)
			{
				int ret;

				do
				{
					ret = core_start(i, task0);
					KASSERT((ret == 0) || (ret == -EBUSY));
				} while (ret != 0);
			}
		}

		/* Wait for slave cores. */
		barrier_wait(&task0_barrier);
	}
}

/*----------------------------------------------------------------------------*
 * Spawn Execution from Leader Core                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief Stress Test: Spawn Execution from Leader Core
 */
PRIVATE void test_cluster_cores_stress_leader_spawn(void)
{
	for (int k = 0; k < NITERATIONS; k++)
	{
		barrier_init(&task1_barrier, 1);

		/* Spawn execution in all cores. */
		for (int i = 0; i < CORES_NUM; i++)
		{
			if (i != COREID_MASTER)
			{
				int ret;

				do
				{
					ret = core_start(i, task1);
					KASSERT((ret == 0) || (ret == -EBUSY));
				} while (ret != 0);

				break;
			}
		}

		/* Wait for slave cores. */
		barrier_wait(&task1_barrier);
	}
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Stress Injection Tests.
 */
PRIVATE struct test stress_tests_api[] = {
	{ test_cluster_cores_stress_master_spawn, "spawn from master core" },
	{ test_cluster_cores_stress_leader_spawn, "spawn from leader core" },
	{ NULL,                                   NULL                     },
};

/**
 * The test_cluster_cores() function launches testing units on the
 * Cores Interface of the Cluster AL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_cluster_cores(void)
{
	/* Stress Tests */
	for (int i = 0; stress_tests_api[i].test_fn != NULL; i++)
	{
		stress_tests_api[i].test_fn();
		kprintf("[test][cluster][stress][cores] %s [passed]", stress_tests_api[i].name);
	}
}
