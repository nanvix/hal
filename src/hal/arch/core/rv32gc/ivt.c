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
#define __NEED_CORE_REGS
#define __NEED_CORE_TYPES

#include <arch/core/rv32gc/ivt.h>
#include <arch/core/rv32gc/regs.h>
#include <arch/core/rv32gc/types.h>
#include <nanvix/const.h>

/* Import definitions. */
EXTERN void rv32gc_do_strap(void);

/**
 * @brief Initializes the interrupt vector table.
 *
 * @param do_trap Trap handler.
 *
 * The rv32gc_ivt_setup() function initializes the interrupt vector
 * table in the rv32gc core.
 */
PRIVATE void rv32gc_ivt_setup(void (*do_trap)(void))
{
	rv32gc_stvec_write(RV32GC_WORD(do_trap));
}

/**
 * @see rv32gc_ivt_setup().
 */
PUBLIC void ivt_setup(void *stack)
{
	((void) stack);
	rv32gc_ivt_setup(&rv32gc_do_strap);
}
