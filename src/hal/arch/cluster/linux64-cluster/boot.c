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


#include <arch/cluster/linux64-cluster/cores.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <stdlib.h>
#include <stdio.h>

/* Import definitions. */
EXTERN NORETURN void linux64_cluster_master_setup(void);
EXTERN NORETURN void linux64_cluster_slave_setup(void);

/**
 * @brief Lookup table for thread IDs.
 */
PUBLIC pthread_t linux64_cores_tab[LINUX64_CLUSTER_NUM_CORES];

/**
 * @brief Entry point.
 *
 * @param argc Number of arguments.
 * @param argv Arguments list.
 */
int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	linux64_core_dcache_setup();
	linux64_core_icache_setup();
	perf_setup();

	/**
	 * Initialize the lookup table for Threads IDs.
	 */
	linux64_cores_tab[0] = pthread_self();

	linux64_cluster_master_setup();

	/*
	 * TODO: grab some memory.
	 */

	/*
	 * TODO: spawn slave threads.
	 */

	return (0);
}
