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
#include <nanvix/const.h>
#include "test.h"

/**
 * @brief Launch unit tests on performance interface?
 */
#define TEST_PERF 0

#ifndef __unix64__

/**
 * @brief Timer frequency (in Hz).
 */
#define TIMER_FREQ 100

/**
 * @brief Launches unit tests on Core AL.
 */
PRIVATE void test_core_al(void)
{
	test_core();
	test_exception();
	test_interrupt();
	test_mmu();
	test_tlb();
	test_trap();
	test_upcall();
#if (CORE_HAS_PERF)
	test_perf();
#endif
}

/**
 * @brief Launches unit tests on Cluster AL.
 */
PRIVATE void test_cluster_al(void)
{
#if (CLUSTER_IS_MULTICORE)
	test_cluster_cores();
#endif
}

/**
 * @brief Launches unit tests on Processor AL.
 */
PRIVATE void test_processor_al(void)
{
#ifdef __mppa256__
	test_cnoc();
	test_dnoc();
#endif
}

/**
 * @brief Launches unit tests on Target AL.
 */
PRIVATE void test_target_al(void)
{
#if (TARGET_HAS_SYNC)
	test_sync();
#endif
}

/**
 * @brief Dummy main function.
 */
PUBLIC int main(int argc, const char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	return (0);
}

#endif

/**
 * @brief Initializes the kernel.
 */
PUBLIC NORETURN void kmain(int argc, const char *argv[])
{
#ifndef __unix64__

	const char *arg;

	arg = (argc < 2) ? "--all" : argv[1];

	/*
	 * Initializes the HAL. Must come
	 * before everything else.
	 */
	hal_init();

	timer_init(TIMER_FREQ);

	/* Run unit tests. */
	if ((!kstrcmp(arg, "--all")) || (!kstrcmp(arg, "--core")))
		test_core_al();
	if ((!kstrcmp(arg, "--all")) || (!kstrcmp(arg, "--cluster")))
		test_cluster_al();
	if ((!kstrcmp(arg, "--all")) || (!kstrcmp(arg, "--processor")))
		test_processor_al();
	if ((!kstrcmp(arg, "--all")) || (!kstrcmp(arg, "--target")))
		test_target_al();

#else

	UNUSED(argc);
	UNUSED(argv);

	test_core();

	linux64_core_dcache_dump_info();
	linux64_core_icache_dump_stats();
	linux64_core_dcache_dump_stats();

	test_mmu();
	test_spinlock();
	test_timer();

	test_tlb();

	test_trap();

	test_exception();
	test_interrupt();

#if (defined(TEST_PERF) && (TEST_PERF))
	test_perf();
#endif

#endif

	core_poweroff();
}
