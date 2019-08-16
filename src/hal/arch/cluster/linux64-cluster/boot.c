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
#define __NEED_CLUSTER_LINUX64

#include <arch/cluster/linux64-cluster.h>
#include <nanvix/const.h>
#include <pthread.h>

/**
 * @brief Lookup table for thread IDs.
 */
PUBLIC pthread_t linux64_cores_tab[LINUX64_CLUSTER_NUM_CORES];

/**
 * @brief Entry point for slave core.
 */
PRIVATE void *linux64_do_slave(void *args)
{
	UNUSED(args);
	linux64_cluster_setup();
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int linux64_cluster_boot(void)
{
	/* Save ID of master core. */
	linux64_cores_tab[0] = pthread_self();

	for (int i = 1; i < LINUX64_CLUSTER_NUM_CORES; i++)
	{
		if (pthread_create(&linux64_cores_tab[i], NULL, linux64_do_slave, NULL))
			return (-EINVAL);
	}

	return (0);
}
