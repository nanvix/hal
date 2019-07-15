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
#include <arch/cluster/linux64-cluster/_linux64-cluster.h>
#include <arch/cluster/linux64-cluster/cores.h>

/**
 * @brief Return the Id of the underlying core.
 */
PUBLIC int linux64_core_get_id(void)
{
	linux64_spinlock_lock(&linux64_cores_lock);

	/* Search for target core. */
	for (int i = 0; i < LINUX64_CLUSTER_NUM_CORES ; i++)
	{
		/* Found. */
		if (linux64_cores_tab[i] == pthread_self())
		{
			linux64_spinlock_unlock(&linux64_cores_lock);
			return (i);
		}
	}
	linux64_spinlock_unlock(&linux64_cores_lock);
	return (-1);
}

/**
 * @brief Shutdown the underlying core.
 */
PUBLIC NORETURN void linux64_core_poweroff(void)
{
	int is_master = false;

	kprintf("[hal] powering off...");

	linux64_spinlock_lock(&linux64_cores_lock);

		/* Search for target core. */
		for (int i = 0; i < LINUX64_CLUSTER_NUM_CORES; i++)
		{
			/* Found. */
			if (linux64_cores_tab[i] == pthread_self())
			{
				linux64_cores_tab[i] = 0;
				linux64_spinlock_unlock(&linux64_cores_lock);
				is_master = (i == LINUX64_CLUSTER_COREID_MASTER) ? true : false;
			}
		}

	linux64_spinlock_unlock(&linux64_cores_lock);

	if (is_master)
		exit(0);
	else
		pthread_exit(NULL);

	UNREACHABLE();
}

/**
 * @brief Create an context struct.
 */
PUBLIC struct context linux64_create_context(void)
{
	struct context ctx;
	ctx.id = linux64_core_get_id();
	return ctx;
}

/**
 * @brief Setup a core.
 */
PUBLIC void linux64_core_setup(void)
{
	linux64_core_dcache_setup();
	linux64_core_icache_setup();
	linux64_excp_setup();
	linux64_interrupts_enable();
	linux64_perf_setup();
}
