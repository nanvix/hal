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

#include <arch/stdout/jtag.h>
#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * @brief Size of exception stack (in double words).
 */
#define EXCEPTION_STACK_SIZE K1B_PAGE_SIZE/sizeof(uint64_t)

/**
 * @brief Kernel stack.
 */
PRIVATE uint64_t excp_stacks[K1B_CLUSTER_NUM_CORES][EXCEPTION_STACK_SIZE] ALIGN(K1B_PAGE_SIZE);

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo ALIGN(K1B_CACHE_LINE_SIZE) cores[K1B_CLUSTER_NUM_CORES] = {
	{ true,  CORE_RUNNING,   0, NULL, K1B_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 1  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 2  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 3  */
#if defined(__node__)
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 4  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 5  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 6  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 7  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 8  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 9  */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 10 */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 11 */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 12 */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 13 */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 14 */
	{ false, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 15 */
#endif
};

/*============================================================================*
 * k1b_cluster_setup()                                                        *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_cluster_setup(void)
{
	int coreid;

	coreid = k1b_core_get_id();

	if (coreid == K1B_CLUSTER_COREID_MASTER)
		kprintf("[hal] booting up cluster...");

	mem_setup();
	core_setup(&excp_stacks[k1b_core_get_id()][EXCEPTION_STACK_SIZE]);
}

/*============================================================================*
 * k1b_cluster_slave_setup()                                                  *
 *============================================================================*/

/**
 * @brief Initializes a slave core.
 *
 * The k1b_cluster_slave_setup() function initializes the underlying
 * slave core.  It setups the stack and then call the kernel main
 * function.  Architectural structures are initialized by the master
 * core and registered later on, when the slave core is started
 * effectively.
 *
 * @note This function does not return.
 *
 * @see k1b_cluster_setup() and k1b_cluster_master_setup().
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void k1b_cluster_slave_setup(void)
{
	k1b_cluster_setup();

	while (true)
	{
		core_idle();
		core_run();
	}
}

/*============================================================================*
 * k1b_cluster_master_setup()                                                 *
 *============================================================================*/

/**
 * @name Default boot arguments.
 */
/**@{*/
PRIVATE const char *noargv[] = {"kmain", NULL };
PRIVATE const char *noenvp[] = { NULL };
/**@}*/

/**
 * @brief Gets boot arguments.
 *
 * @param args Location where boot arguments should be stored.
 */
PRIVATE void k1b_get_boot_args(k1_boot_args_t *args)
{
	extern uint8_t __image_start;
	extern uint8_t __image_end;
	__k1_boot_args_mmap_t *_args = (__k1_boot_args_mmap_t*) &MPPA_ARGAREA_START;

again:

	/* This is a valid boort argument. */
	if ((args != NULL) && (_args->magic_value == __MPPA_MAGIC_BOOT_ARGS_VALUE))
	{
		args->argc = _args->argc;
		args->envc = _args->envc;
		args->argv = (char**) _args->argv_ptr;
		args->envp = (char**) _args->envp_ptr;
		return;
	}

	/* Search in the memory for boot arguments. */
	for (uint8_t *p = &__image_start; p < &__image_end; p++)
	{
		if (*((uint32_t *) p) == __MPPA_MAGIC_BOOT_ARGS_VALUE)
		{
			_args = (__k1_boot_args_mmap_t *) p;
			goto again;
		}
	}

	args->argc  = 1;
	args->envc  = 0;
	args->argv  = (char **) noargv;
	args->envp  = (char **) noenvp;
}


/**
 * @brief Initializes the master core.
 *
 * The k1b_cluster_master_setup() function initializes the underlying
 * master core.  It setups the stack, retrieves the boot arguments,
 * initializes architectural structures and then call the kernel main
 * function.
 *
 * @note This function does not return.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE NORETURN void k1b_cluster_master_setup(void)
{
	k1_boot_args_t args;

	kmemset(&__BSS_START, 0, &__BSS_END - &__BSS_START);

	/*
	 * Early initialization of
	 * JTAG to help us debugging.
	 */
	jtag_init();

	k1b_get_boot_args(&args);

	k1b_cluster_setup();

	cluster_fence_release();

	kmain(args.argc, (const char **)args.argv);
}

/*============================================================================*
 * _do_slave_pe()                                                             *
 *============================================================================*/

/**
 * @brief Starts a slave core.
 *
 * @param oldsp Old stack pointer (unused).
 *
 * The _do_slave_pe() function is the entry point for a slave core. It
 * is called by the VBSP routines, once a bare environment is setup.
 *
 * @note This function does not return.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void SECTION_TEXT NORETURN _do_slave_pe(uint32_t oldsp)
{
	UNUSED(oldsp);

	cluster_fence_wait();

	k1b_cluster_slave_setup();
}

/*============================================================================*
 * _do_master_pe()                                                            *
 *============================================================================*/

/**
 * @brief Starts a master core.
 *
 * @param oldsp Old stack pointer (unused).
 *
 * The _do_master_pe() function is the entry point for a master core. It
 * is called by the VBSP routines, once a bare environment is setup.
 *
 * @note This function does not return.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void SECTION_TEXT NORETURN _do_master_pe(uint32_t oldsp)
{
	UNUSED(oldsp);

	k1b_cluster_master_setup();
}
