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
 * @brief Lock for the array of the cores.
 */
PUBLIC linux64_spinlock_t linux64_cores_lock;

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
