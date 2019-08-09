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
#define __NEED_HAL_CORE
#define __NEED_CORE_LPIC
#define __NEED_OR1K_REGS

#include <arch/core/i486/gdt.h>
#include <arch/core/i486/idt.h>
#include <arch/core/i486/int.h>
#include <arch/core/i486/tss.h>
#include <nanvix/hal/core.h>
#include <nanvix/const.h>

/*============================================================================*
 * i486_core_poweroff()                                                       *
 *============================================================================*/

/**
 * The i486_core_poweroff() function powers off the underlying core.
 * After powering off a core, instruction execution cannot be
 * resumed on it.
 *
 * @author Pedro Henrique Penna
 * @author João Vicente Souto
 */
PUBLIC NORETURN void i486_core_poweroff(void)
{
	core_halt();
}

/*============================================================================*
 * i486_core_setup()                                                          *
 *============================================================================*/

/**
 * The i486_core_setup() function initializes all architectural
 * structures of the underlying core. It setups the GDT, TSS and IDT.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void i486_core_setup(void)
{
	gdt_setup();
	tss_setup();
	i486_lpic_setup(0x20, 0x28);
	idt_setup();
}
