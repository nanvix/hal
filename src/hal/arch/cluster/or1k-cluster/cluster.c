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
#include <nanvix/hal/target/stdout.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo  ALIGN(OR1K_CACHE_LINE_SIZE) cores[OR1K_CLUSTER_NUM_CORES] = {
#if (defined(__or1k_cluster__))
	{ true,  CORE_RUNNING,   0, NULL, OR1K_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ false, CORE_RESETTING, 0, NULL, OR1K_SPINLOCK_LOCKED   }, /* Slave Core 1  */
#elif (defined(__optimsoc_cluster__))
	{ true,  CORE_RUNNING,   0, NULL, OR1K_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ false, CORE_RESETTING, 0, NULL, OR1K_SPINLOCK_LOCKED   }, /* Slave Core 1  */
	{ false, CORE_RESETTING, 0, NULL, OR1K_SPINLOCK_LOCKED   }, /* Slave Core 2  */
	{ false, CORE_RESETTING, 0, NULL, OR1K_SPINLOCK_LOCKED   }, /* Slave Core 3  */
#endif
};

/*============================================================================*
 * or1k_cluster_setup()                                                       *
 *============================================================================*/

/**
 * Initializes the underlying cluster.
 */
PUBLIC void or1k_cluster_setup(void)
{
	int coreid;

	coreid = or1k_core_get_id();

	if (coreid == OR1K_CLUSTER_COREID_MASTER)
		kprintf("[hal] booting up cluster...");

	/* Configure Memory Layout. */
	or1k_cluster_mem_setup();

	/* Setup MMU. */
	or1k_core_setup();

	/* Setup events. */
	#if (!CLUSTER_HAS_EVENTS)
	if (coreid == OR1K_CLUSTER_COREID_MASTER)
		event_setup();
	#else
		or1k_cluster_ompic_init();
		or1k_pic_unmask(OR1K_INT_OMPIC);
		or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR) | OR1K_SPR_SR_IEE);
	#endif
}

/*============================================================================*
 * or1k_cluster_slave_setup()                                                  *
 *============================================================================*/

/**
 * @brief Initializes a slave core.
 *
 * The or1k_slave_setup() function initializes the underlying slave
 * core.  It setups the stack and then call the kernel main function.
 * Architectural structures are initialized by the master core and
 * registered later on, when the slave core is started effectively.
 *
 * @note This function does not return.
 *
 * @see or1k_core_setup() and or1k_master_setup().
 *
 * @author Davidson Francis
 */
PUBLIC NORETURN void or1k_cluster_slave_setup(void)
{
	cluster_fence_wait();

	or1k_cluster_setup();

	while (true)
	{
		core_idle();
		core_run();
	}
}

/*============================================================================*
 * or1k_cluster_master_setup()                                                 *
 *============================================================================*/

/**
 * @brief Initializes the master core.
 *
 * The or1k_master_setup() function initializes the underlying
 * master core. It setups the stack and then call the kernel
 * main function.
 *
 * @note This function does not return.
 *
 * @author Davidson Francis
 */
PUBLIC NORETURN void or1k_cluster_master_setup(void)
{
	/* Clear BSS section. */
	kmemset(&__BSS_START, 0, &__BSS_END - &__BSS_START);

	/*
	 * Early initialization of
	 * stdout to help us debugging.
	 */
	stdout_init();

	or1k_cluster_setup();

	cluster_fence_release();

	/* Kernel main. */
	kmain(0, NULL);
}
