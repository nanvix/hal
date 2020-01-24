/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
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
#define __NEED_CORE_MACHINE

#include <nanvix/hal/cluster.h>
#include <arch/stdout/16550a.h>
#include <nanvix/const.h>
#include <posix/errno.h>

/**
 * @brief Initializes machine mode.
 *
 * @param pc Target program counter.
 */
PUBLIC NORETURN void rv32gc_machine_master_setup(rv32gc_word_t pc)
{
	rv32gc_word_t mie;

	/* TODO: do this even earlier. */
	kmemset(&__BSS_START, 0, &__BSS_END - &__BSS_START);

	/*
	 * Early initialization of
	 * UART to help us debugging.
	 */
	uart_16550a_init();

	cluster_fence_release();

	/* Enable machine IRQs. */
	mie = rv32gc_mie_read();
	mie = BITS_SET(mie, RV32GC_MIE_MSIE, 1);
	mie = BITS_SET(mie, RV32GC_MIE_MTIE, 1);
	mie = BITS_SET(mie, RV32GC_MIE_MEIE, 1);
	rv32gc_mie_write(mie);

	rv32gc_machine_delegate_traps();

	rv32gc_supervisor_enter(pc);
}

/**
 * @brief Initializes machine mode.
 *
 * @param pc Target program counter.
 */
PUBLIC NORETURN void rv32gc_machine_slave_setup(rv32gc_word_t pc)
{
	rv32gc_word_t mie;

	/* Enable machine IRQs. */
	mie = rv32gc_mie_read();
	mie = BITS_SET(mie, RV32GC_MIE_MSIE, 1);
	mie = BITS_SET(mie, RV32GC_MIE_MTIE, 0);
	mie = BITS_SET(mie, RV32GC_MIE_MEIE, 0);
	rv32gc_mie_write(mie);

	rv32gc_machine_delegate_traps();

	cluster_fence_wait();

	rv32gc_supervisor_enter(pc);
}
