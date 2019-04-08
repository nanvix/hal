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
#define __NEED_CORE_MACHINE

#include <arch/cluster/riscv32-cluster/memory.h>
#include <arch/cluster/riscv32-cluster/cores.h>
#include <arch/stdout/16550a.h>
#include <nanvix/const.h>
#include <errno.h>

/**
 * @brief Startup fence.
 */
PRIVATE struct
{
	int master_alive;
	rv32i_spinlock_t lock;
} fence = { FALSE , RV32I_SPINLOCK_UNLOCKED};

/**
 * @brief Releases the startup fence.
 */
PRIVATE void rv32i_fence_release(void)
{
	rv32i_spinlock_lock(&fence.lock);
		fence.master_alive = TRUE;
	rv32i_spinlock_unlock(&fence.lock);
}

/**
 * @brief Waits on the startup fence.
 */
PRIVATE void rv32i_fence_wait(void)
{
	while (TRUE)
	{
		rv32i_spinlock_lock(&fence.lock);

			/* Fence is released. */
			if (fence.master_alive)
			{
				rv32i_spinlock_unlock(&fence.lock);
				break;
			}

			noop();
		rv32i_spinlock_unlock(&fence.lock);
	}
}

/**
 * @brief Initializes machine mode.
 *
 * @param pc Target program counter.
 */
PUBLIC NORETURN void rv32i_machine_master_setup(rv32i_word_t pc)
{
	rv32i_word_t mie;

	/* TODO: do this even earlier. */
	kmemset(&__BSS_START, 0, &__BSS_END - &__BSS_START);

	/*
	 * Early initialization of
	 * UART to help us debugging.
	 */
	uart_16550a_init();

	rv32i_fence_release();

	/* Enable machine IRQs. */
	mie = rv32i_mie_read();
	mie = BITS_SET(mie, RV32I_MIE_MSIE, 1);
	mie = BITS_SET(mie, RV32I_MIE_MTIE, 1);
	mie = BITS_SET(mie, RV32I_MIE_MEIE, 1);
	rv32i_mie_write(mie);

	rv32i_machine_delegate_traps();

	rv32i_supervisor_enter(pc);
}

/**
 * @brief Initializes machine mode.
 *
 * @param pc Target program counter.
 */
PUBLIC NORETURN void rv32i_machine_slave_setup(rv32i_word_t pc)
{
	rv32i_word_t mie;

	/* Enable machine IRQs. */
	mie = rv32i_mie_read();
	mie = BITS_SET(mie, RV32I_MIE_MSIE, 0);
	mie = BITS_SET(mie, RV32I_MIE_MTIE, 0);
	mie = BITS_SET(mie, RV32I_MIE_MEIE, 0);
	rv32i_mie_write(mie);

	rv32i_machine_delegate_traps();

	rv32i_fence_wait();

	rv32i_supervisor_enter(pc);
}
