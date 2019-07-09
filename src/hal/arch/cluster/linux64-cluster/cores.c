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

/**
 * @brief Returns the ID of the underlying core.
 */
PUBLIC int linux64_core_get_id(void)
{
	/* Search for target core. */
	for (int i = 0; i < LINUX64_CLUSTER_NUM_CORES ; i++)
	{
		/* Found. */
		if (linux64_cores_tab[i] == pthread_self())
			return (i);
	}

	return (-1);
}

/**
 * @brief Shutdown the core that execute this function
 * and remove his pid from the core_tab.
 */
PUBLIC NORETURN void linux64_core_poweroff(void)
{
	/* Search for target core. */
	for (int i = 0; i < LINUX64_CLUSTER_NUM_CORES; i++)
	{
		/* Found. */
		if (linux64_cores_tab[i] == pthread_self())
		{
			linux64_cores_tab[i] = 0;
			pthread_exit(NULL);
		}
	}

	UNREACHABLE();
}

/**
 * @brief Create an context struct.
 * 
 * @return Created context struct.
 */
PUBLIC struct context linux64_create_context(void)
{
	struct context ctx;
	ctx.id = linux64_core_get_id();
	return ctx;
}
