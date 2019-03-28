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
#define __NEED_IVT
#define __NEED_MEMORY_TYPES

#include <arch/core/rv32i/types.h>
#include <arch/core/rv32i/ivt.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);
EXTERN unsigned char __BSS_START;
EXTERN unsigned char __BSS_END;
EXTERN void rv32i_do_event(void);

/*============================================================================*
 * rv32i_core_setup()                                                         *
 *============================================================================*/

/**
 * The rv32i_core_setup() function initializes all architectural
 * structures of the underlying core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_core_setup(void)
{
	kprintf("[hal] booting up master core...");

	rv32i_ivt_setup(&rv32i_do_event);
}

/*============================================================================*
 * rv32i_master_setup()                                                       *
 *============================================================================*/

/**
 * @brief Initializes the master core.
 *
 * The rv32i_master_setup() function initializes the underlying
 * master core. It setups the stack and then call the kernel
 * main function.
 *
 * @note This function does not return.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32i_master_setup(void)
{
	kmemset(&__BSS_START, 0, &__BSS_END - &__BSS_START);

	/* Core setup. */
	rv32i_core_setup();

	/* Say hello. */
	kprintf("[hal] Hello Word!");
	kprintf("[hal] Nanvix for RISC-V is comming soon!");
	kprintf("[hal] halting...");

	kmain(0, NULL);
}
