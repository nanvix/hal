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
#include "vsyscall.h"

#if (__TARGET_HAS_SYNC)

/**
 * @name Synchronization points.
 */
/**@{*/
static int _syncin  = -1;
static int _syncout = -1;
/**@}*/

/*============================================================================*
 * Auxiliar Function                                                          *
 *============================================================================*/

/**
 * @brief Dummy interrupt handler.
 */
PRIVATE void dummy_handler(int num)
{
	UNUSED(num);
}

/**
 * @brief Setup interrupts.
 */
PUBLIC void test_stress_interrupt_setup(void)
{
	/* Ignores spurious interrupts. */
	if (core_get_id() == 0)
		KASSERT(interrupt_register(INTERRUPT_TIMER, dummy_handler) == 0);

	interrupts_enable();

	interrupt_mask(INTERRUPT_TIMER);
}

/**
 * @brief Cleanup interrupts.
 */
PUBLIC void test_stress_interrupt_cleanup(void)
{
	interrupt_unmask(INTERRUPT_TIMER);

	interrupts_disable();

	/* Unregister interrupt handler. */
	if (core_get_id() == 0)
		KASSERT(interrupt_unregister(INTERRUPT_TIMER) == 0);
}

/**
 * @brief Setup synchronization points.
 */
PUBLIC void test_stress_setup(void)
{
	int local;
	int remote;
	int nodes[2];

	test_stress_interrupt_setup();

	local  = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	/* Create sync. */
	nodes[0] = remote;
	nodes[1] = local;
	KASSERT((_syncin = vsys_sync_create(nodes, 2, SYNC_ONE_TO_ALL)) >= 0);

	/* Open sync. */
	nodes[0] = local;
	nodes[1] = remote;
	KASSERT((_syncout = vsys_sync_open(nodes, 2, SYNC_ONE_TO_ALL)) >= 0);
}

/**
 * @brief Cleanup synchronization points.
 */
PUBLIC void test_stress_cleanup(void)
{
	KASSERT(vsys_sync_unlink(_syncin) == 0);
	KASSERT(vsys_sync_close(_syncout) == 0);

	_syncin  = -1;
	_syncout = -1;

	test_stress_interrupt_cleanup();
}

/**
 * @brief Synchronization step.
 */
PUBLIC void test_stress_barrier(void)
{
	int ret;

	if (processor_node_get_num() == NODENUM_MASTER)
	{
		do
			ret = vsys_sync_signal(_syncout);
		while (ret == (-EAGAIN));
		KASSERT(ret == 0);

		KASSERT(vsys_sync_wait(_syncin) == 0);
	}
	else
	{
		KASSERT(vsys_sync_wait(_syncin) == 0);

		do
			ret = vsys_sync_signal(_syncout);
		while (ret == (-EAGAIN));
		KASSERT(ret == 0);
	}
}

#endif /* __TARGET_HAS_SYNC */
