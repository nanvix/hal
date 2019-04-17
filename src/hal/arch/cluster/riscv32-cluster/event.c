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
#define __NEED_CLUSTER_CLINT

#include <arch/cluster/riscv32-cluster/clint.h>
#include <arch/cluster/riscv32-cluster/cores.h>
#include <nanvix/const.h>

/**
 * @brief Table of locks.
 */
PRIVATE rv32gc_spinlock_t locks[RISCV32_CLUSTER_NUM_CORES] = {
	RV32GC_SPINLOCK_UNLOCKED, /* Master Core  */
	RV32GC_SPINLOCK_UNLOCKED, /* Slave Core 1 */
	RV32GC_SPINLOCK_UNLOCKED, /* Slave Core 2 */
	RV32GC_SPINLOCK_UNLOCKED, /* Slave Core 3 */
	RV32GC_SPINLOCK_UNLOCKED, /* Slave Core 4 */
};

/**
 * Table of pending events.
 */
PRIVATE int volatile events[RISCV32_CLUSTER_NUM_CORES] ALIGN(RV32GC_CACHE_LINE_SIZE) = {
	0, /* Master Core  */
	0, /* Slave Core 1 */
	0, /* Slave Core 2 */
	0, /* Slave Core 3 */
	0, /* Slave Core 4 */
};

/*============================================================================*
 * riscv32_cluster_event_send()                                               *
 *============================================================================*/

/**
 * @brief Handles IPIs.
 *
 * @note This executes in machine mode.
 */
PUBLIC void riscv32_cluster_do_ipi(const struct context *ctx)
{
	rv32gc_word_t mie;

	/* Clear Machine IPI. */
	mie = rv32gc_mie_read();
	mie = BITS_SET(mie, RV32GC_MIE_MSIE, 0);
	rv32gc_mie_write(mie);

	UNUSED(ctx);
}

/*============================================================================*
 * riscv32_cluster_event_send()                                               *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 */
PUBLIC int riscv32_cluster_event_send(int coreid)
{
	int mycoreid;

	/* Invalid core. */
	if (UNLIKELY(coreid < 0))
		return (-EINVAL);

	mycoreid = rv32gc_core_get_id();

	/* Bad core. */
	if (UNLIKELY(coreid == mycoreid))
		return (-EINVAL);

	rv32gc_spinlock_lock(&locks[coreid]);

		/* Set the pending IPI flag. */
		events[coreid] |= (1 << mycoreid);
		riscv32_cluster_ipi_send(coreid);

	rv32gc_spinlock_unlock(&locks[coreid]);

	return (0);
}

/*============================================================================*
 * riscv32_cluster_event_ack()                                                *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 */
PUBLIC int riscv32_cluster_event_ack(void)
{
	return (0);
}

/*============================================================================*
 * rv32gc_core_waitclear()                                                      *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 */
PUBLIC int riscv32_cluster_event_waitclear(void)
{
	int mycoreid = rv32gc_core_get_id();

	while (TRUE)
	{
		rv32gc_spinlock_lock(&locks[mycoreid]);

			if (events[mycoreid])
			{
				riscv32_cluster_ipi_ack(mycoreid);
				break;
			}

		rv32gc_spinlock_unlock(&locks[mycoreid]);

		rv32gc_int_wait();
	}

		/* Clear IPI. */
		for (int i = 0; i < RISCV32_CLUSTER_NUM_CORES; i++)
		{
			if (events[mycoreid] & (1 << i))
			{
				events[mycoreid] &= ~(1 << i);
				break;
			}
		}

	rv32gc_spinlock_unlock(&locks[mycoreid]);

	return (0);
}
