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
#define __NEED_HAL_CLUSTER
#include <nanvix/hal/cluster.h>
#include <arch/cluster/linux64-cluster.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <arch/stdout/tty-virt.h>

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
 * @brief Setup the cluster.
 */
PUBLIC void linux64_cluster_setup(void)
{
	kprintf("[hal] booting up cluster...");
	
	for(int i = 1; i < LINUX64_CLUSTER_NUM_CORES; i++)
		linux64_spinlock_lock(&cores[i].lock);
}

/**
 * @brief Setup a slave core of the cluster.
 */
PUBLIC NORETURN void linux64_cluster_slave_setup(void)
{
	linux64_core_setup();
	while(1)
	{
		core_idle();
		core_run();
	}
}

/**
 * @brief Setup the master core of the cluster.
 */
PUBLIC NORETURN void linux64_cluster_master_setup(void)
{
	/*
	 * Early initialization of Virtual
	 * TTY device to help us debugging.
	 */
	tty_virt_init();

	/* cluster setup. */
	linux64_cluster_setup();

	/* Core setup */
	linux64_core_setup();

	kmain(0, NULL);
}
