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
#include "test.h"

/**
 * @brief Launch verbose tests?
 */
#define TEST_CORE_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Number of cores started.
 */
PRIVATE int cores_started = 1;

/**
 * @brief API Test: Spinlock for number of cores.
 */
PRIVATE spinlock_t core_start_lock = SPINLOCK_UNLOCKED;

/**
 * @brief API Test: Core suspend and wakeup flags.
 */
#define TEST_CORE_RUNNING 0xDEAD /**< Indicates that the core was suspended. */
#define TEST_CORE_AWAKEN  0xC0DE /**< Indicates that the core is awaken.     */

/*----------------------------------------------------------------------------*
 * Get Core ID                                                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Query Core ID
 */
PRIVATE void test_core_get_id(void)
{
	int coreid;

	coreid = core_get_id();

#if (TEST_CORE_VERBOSE)
	kprintf("coreid = %d", coreid);
#endif

	KASSERT(coreid == COREID_MASTER);
}

/*----------------------------------------------------------------------------*
 * Start Execution Slave                                                      *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Slave Core Entry Point.
 */
PRIVATE void test_core_slave_entry(void)
{
#if (TEST_CORE_VERBOSE)
	kprintf("core %d running", core_get_id());
#endif

	spinlock_lock(&core_start_lock);
		cores_started++;
	spinlock_unlock(&core_start_lock);

#if (TEST_CORE_VERBOSE)
	kprintf("core %d stopping", core_get_id());
#endif
}

/**
 * @brief API Test: Start Execution in Slave Cores
 */
PRIVATE void test_core_start_slave(void)
{
	/* Unit test not applicable. */
	if (!CLUSTER_IS_MULTICORE)
		return;

	/**
	 * Start each slave core.
	 */
	for (int i = 0; i < CORES_NUM; i++)
	{
		if (i != COREID_MASTER)
			core_start(i, test_core_slave_entry);
	}

	/**
	 * Wait indefinitely for all cores start.
	 *
	 * @note: If for some reason not all cores were started,
	 * the master core will hang forever.
	 */
	while (TRUE)
	{
		dcache_invalidate();

		if (cores_started == CORES_NUM)
			break;
	}
}

/*----------------------------------------------------------------------------*
 * Suspend and Resume from Master                                             *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Slave Core, Suspend/Resume entry point.
 */
PRIVATE void test_core_suspend_slave_entry(void)
{
#if (TEST_CORE_VERBOSE)
	kprintf("core %d running", core_get_id());
#endif

	/* Running. */
	dcache_invalidate();
	cores_started = TEST_CORE_RUNNING;

#if (TEST_CORE_VERBOSE)
	kprintf("core %d suspending", core_get_id());
#endif

	/* Sleep. */
	core_sleep();

	/* Wakeup. */
	dcache_invalidate();
	cores_started = TEST_CORE_AWAKEN;

#if (TEST_CORE_VERBOSE)
	kprintf("core %d waking up", core_get_id());
#endif
}

/**
 * @brief API Test: Suspend and resume a slave core.
 */
PRIVATE void test_core_suspend_resume_master(void)
{
	int i;   /* Loop index. */

	/* Unit test not applicable. */
	if (!CLUSTER_IS_MULTICORE)
		return;

	/*
	 * Start one slave core.
	 */
	for (i = 0; i < CORES_NUM; i++)
	{
		if (i != COREID_MASTER)
		{
			core_start(i, test_core_suspend_slave_entry);
			break;
		}
	}

	/*
	 * Send a wakeup signal to the slave core.
	 *
	 * Note: It's important to note that the wakeup signal is
	 * not atomic, i.e, the signal can arrives before the core
	 * slept. The HAL covers this scenario using a wakeups
	 * counter that is able to prevent a core from sleeping if
	 * it has already received a wakeup signal.
	 */
	while (TRUE)
	{
		dcache_invalidate();

		if (cores_started == TEST_CORE_RUNNING)
		{
			core_wakeup(i);
			break;
		}
	}

	/*
	 * Wait for the slave wake up.
	 */
	while (TRUE)
	{
		dcache_invalidate();

		if (cores_started == TEST_CORE_AWAKEN)
			break;
	}
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test core_tests_api[] = {
	{ test_core_get_id,                "Get Core ID"                    },
	{ test_core_start_slave,           "Start Execution Slave"          },
	{ test_core_suspend_resume_master, "Suspend and Resume from Master" },
	{ NULL,                            NULL                             },
};

/**
 * The test_core() function launches testing units on the core
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_core(void)
{
	/* API Tests */
	for (int i = 0; core_tests_api[i].test_fn != NULL; i++)
	{
		core_tests_api[i].test_fn();
		kprintf("[test][api][core] %s [passed]", core_tests_api[i].name);
	}
}
