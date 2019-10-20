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

#include <nanvix/hal/core.h>
#include <arch/core/or1k/regs.h>
#include <arch/core/or1k/lpic.h>
#include <arch/core/or1k/mmu.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/*============================================================================*
 * or1k_core_poweroff()                                                       *
 *============================================================================*/

/**
 * The or1k_core_poweroff() function powers off the underlying core.
 * After powering off a core, instruction execution cannot be
 * resumed on it.
 *
 * @author Davidson Francis
 * @author João Vicente Souto
 */
PUBLIC NORETURN void or1k_core_poweroff(void)
{
	/* Disable all interrupts. */
	or1k_pic_lvl_set(OR1K_IRQLVL_0);

	/* Is Power Management Unit present? */
	if (or1k_mfspr(OR1K_SPR_UPR) & OR1K_SPR_UPR_PMP)
	{
		kprintf("[hal] powering off...");

		or1k_mtspr(OR1K_SPR_PMR, OR1K_SPR_PMR_DME);

		/* Stay here forever. */
		UNREACHABLE();
	}
	else
		core_halt();
}
