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
#define __NEED_CORE_IVT

#include <arch/core/rv32gc/ivt.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/* Import definitions. */
EXTERN void rv32gc_do_strap(void);

/*============================================================================*
 * rv32gc_core_poweroff()                                                      *
 *============================================================================*/

/**
 * The rv32gc_core_poweroff() function powers off the underlying core.
 * Afeter powering off a core, instruction execution cannot be
 * resumed.
 *
 * @author Davidson Francis
 */
PUBLIC NORETURN void rv32gc_core_poweroff(void)
{
	while (TRUE)
		/* noop(). */;

	UNREACHABLE();
}

/*============================================================================*
 * rv32gc_core_setup()                                                         *
 *============================================================================*/

/**
 * The rv32gc_core_setup() function initializes all architectural
 * structures of the underlying core. It setups the Interrupt Vector
 * Table (IVT) and the Memory Management Unit (MMU) tables.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_core_setup(void)
{
	kprintf("[hal] booting up core...");

	rv32gc_ivt_setup(&rv32gc_do_strap);
}
