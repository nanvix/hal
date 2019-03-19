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

#include <nanvix/hal/cluster/cpu.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

PUBLIC int pending_ipis[OR1K_NUM_CORES] = {0};

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo  ALIGN(OR1K_CACHE_LINE_SIZE) cores[OR1K_NUM_CORES] = {
	{ TRUE,  CORE_RUNNING,   0, NULL, OR1K_SPINLOCK_LOCKED }, /* Master Core   */
	{ FALSE, CORE_RESETTING, 0, NULL, OR1K_SPINLOCK_LOCKED }, /* Slave Core 1  */
};

/*============================================================================*
 * or1k_core_waitclear()                                                      *
 *============================================================================*/

/**
 * @brief Wait and clears the current IPIs pending of the underlying core.
 *
 * @author Davidson Francis
 */
PUBLIC void or1k_core_waitclear(void)
{
	int mycoreid = or1k_core_get_id();

	while (TRUE)
	{
		or1k_spinlock_lock(&cores[mycoreid].lock);

			if (pending_ipis[mycoreid])
				break;

		or1k_spinlock_unlock(&cores[mycoreid].lock);
	}

		/* Clear IPI. */
		for (int i = 0; i < OR1K_NUM_CORES; i++)
		{
			if (pending_ipis[mycoreid] & (1 << i))
			{
				pending_ipis[mycoreid] &= ~(1 << i);
				break;
			}
		}

	or1k_spinlock_unlock(&cores[mycoreid].lock);
}

/*============================================================================*
 * or1k_core_poweroff()                                                       *
 *============================================================================*/

/**
 * The or1k_core_poweroff() function powers off the underlying core.
 * Afeter powering off a core, instruction execution cannot be
 * resumed.
 *
 * @author Davidson Francis
 */
PUBLIC void or1k_core_poweroff(void)
{
	/* Disable all interrupts. */
	or1k_pic_lvl_set(OR1K_INTLVL_0);

	/* If Power Management Present. */
	if (or1k_mfspr(OR1K_SPR_UPR) & OR1K_SPR_UPR_PMP)
		or1k_mtspr(OR1K_SPR_PMR, OR1K_SPR_PMR_DME);
	else
	{
		while (TRUE)
			/* noop(). */;
	}
}
