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
#include <HAL/hal/core/legacy.h>
#include <HAL/hal/board/boot_args.h>
#include <mOS_common_types_c.h>
#include <mOS_vcore_u.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo ALIGN(K1B_CACHE_LINE_SIZE) cores[K1B_CLUSTER_NUM_CORES] = {
	{ TRUE,  CORE_RUNNING,   0, NULL, K1B_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 1  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 2  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 3  */
#if defined(__node__)
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 4  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 5  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 6  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 7  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 8  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 9  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 10 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 11 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 12 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 13 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 14 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 15 */
#endif
};

/*============================================================================*
 * k1b_stack_setup()                                                          *
 *============================================================================*/

/**
 * @brief Setups the stack.
 *
 * The k1b_stack_setup() function setups the stack of the underlying
 * core by reseting the stack pointer register to the location defined
 * in the link scripts.
 *
 * It would be safier to do this in asembly code, early in boot.
 * However, we are relying on VBSP and we cannot do so. Thus, we make
 * this function inline and call it as early as possible. Hopefully,
 * it will work.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE inline void k1b_stack_setup(void)
{
	__k1_uint8_t *stack_base;

	stack_base = __k1_tls_pe_base_address(k1b_core_get_id());
	__k1_setup_tls_pe(stack_base);
}

/*============================================================================*
 * k1b_cluster_master_setup()                                                 *
 *============================================================================*/

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

	k1b_stack_setup();

	get_k1_boot_args(&args);

	k1b_cluster_setup();

	kmain(args.argc, (const char **)args.argv);
}

/*============================================================================*
 * k1b_cluster_slave_setup()                                                          *
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
	k1b_stack_setup();

	while (TRUE)
	{
		core_idle();
		core_run();
	}
}

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
	kprintf("[hal] booting up cluster...");

	k1b_core_setup();
	k1b_cluster_mem_setup();
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
