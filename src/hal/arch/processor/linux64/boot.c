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

/* Must come fist. */
#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <unistd.h>

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int linux64_processor_boot(int nclusters)
{
	kprintf("[hal][processor] powering on processor...");

	linux64_processor_clusters_boot();
	linux64_processor_noc_boot();

	/* Power on clusters. */
	for (int i = 1; i < LINUX64_PROCESSOR_CLUSTERS_NUM; i++)
	{
		pid_t pid;

		/* Enough clusters are powered on. */
		if (i == nclusters)
			break;

		/* Cannot power on slave cluster. */
		if ((pid = fork()) < 0)
			return (pid);

		/* Slave cluster. */
		if (pid == 0)
			break;
	}

	return (linux64_cluster_boot());
}

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC NORETURN void linux64_processor_poweroff(void)
{
	if (cluster_get_num() == PROCESSOR_CLUSTERID_MASTER)
		kprintf("[hal][processor] powering off...");

	linux64_processor_noc_shutdown();
	linux64_processor_clusters_shutdown();

	cluster_poweroff();
}
