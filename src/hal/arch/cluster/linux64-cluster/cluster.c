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

/* Must come first. */
#define __NEED_HAL_CLUSTER

#include <nanvix/hal/cluster.h>
#include <arch/cluster/linux64-cluster.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <setjmp.h>

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo cores[LINUX64_CLUSTER_NUM_CORES] = {
	{ true,  CORE_RUNNING,   0, NULL, LINUX64_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ false, CORE_RESETTING, 0, NULL, LINUX64_SPINLOCK_UNLOCKED }, /* Slave Core 1  */
	{ false, CORE_RESETTING, 0, NULL, LINUX64_SPINLOCK_UNLOCKED }, /* Slave Core 2  */
	{ false, CORE_RESETTING, 0, NULL, LINUX64_SPINLOCK_UNLOCKED }, /* Slave Core 3  */
};

/**
 * @brief Reset buffers
 */
PRIVATE jmp_buf reset_buffers[LINUX64_CLUSTER_NUM_CORES];

/**
 * @todo TODO: provide a detailed description for this function.
 */
PRIVATE NORETURN void linux64_cluster_slave_setup(void)
{
	int coreid;

	cluster_fence_wait();

	linux64_core_setup();

	coreid = linux64_core_get_id();

	while(1)
	{
		setjmp(reset_buffers[coreid]);
		core_idle();
		core_run();
	}

	UNREACHABLE();
}

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC NORETURN void _linux64_core_reset(void)
{
	int coreid;

	coreid = linux64_core_get_id();
	longjmp(reset_buffers[coreid], 0);
}


/**
 * @todo TODO: provide a detailed description for this function.
 */
PRIVATE NORETURN void linux64_cluster_master_setup(void)
{
	kprintf("[hal][cluster] initializing cluster...");

	for (int i = 1; i < LINUX64_CLUSTER_NUM_CORES; i++)
		linux64_spinlock_lock(&cores[i].lock);

	mem_setup();
	event_setup();
	linux64_core_setup();

	cluster_fence_release();
	kmain(0, NULL);
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC NORETURN void linux64_cluster_setup(void)
{
	int coreid;

	coreid = linux64_core_get_id();

	if (coreid == LINUX64_CLUSTER_COREID_MASTER)
		linux64_cluster_master_setup();

	linux64_cluster_slave_setup();
}

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC NORETURN void linux64_core_poweroff(bool panic)
{
	/* Panic. */
	if (panic)
	{
		kprintf("[hal] core panic");
		exit(0);
	}

	/* Poweroff. */
	if (linux64_core_get_id() == LINUX64_CLUSTER_COREID_MASTER)
	{
		kprintf("[hal] powering off...");
		exit(0);
	}

	pthread_exit(NULL);
}
