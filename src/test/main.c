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
 * @brief Clock frequency (in Hz).
 */
#define CLOCK_FREQ 100

/**
 * @brief Dummy main function.
 */
PUBLIC int main(int argc, const char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	while (TRUE)
		noop();
}

/**
 * @brief Initializes the kernel.
 */
PUBLIC void kmain(int argc, const char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	/*
	 * Initializes the HAL. Must come
	 * before everything else.
	 */
	hal_init();

	clock_init(CLOCK_FREQ);

	test_exception();
	test_interrupt();
	test_mmu();
	test_tlb();
#if (CLUSTER_IS_MULTICORE)
	test_core();
#endif

#if !defined(__rv32i__)
	test_trap();
	test_upcall();

#endif

#if (TARGET_HAS_SYNC)
	test_sync();
#endif

	kprintf("[hal] halting...");

	main(0, NULL);
}
