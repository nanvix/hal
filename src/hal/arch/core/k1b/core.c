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

#include <nanvix/hal/core/interrupt.h>
#include <arch/core/k1b/excp.h>
#include <arch/core/k1b/int.h>
#include <arch/core/k1b/ivt.h>
#include <arch/core/k1b/mOS.h>
#include <arch/core/k1b/trap.h>
#include <nanvix/const.h>

/*============================================================================*
 * k1b_core_setup()                                                           *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_core_setup(void *stack)
{
	kprintf("[hal] booting up core");

	k1b_ivt_setup(
		do_interrupt,
		_k1b_do_syscall,
		_k1b_do_excp,
		stack
	);
}

/*============================================================================*
 * k1b_core_poweroff()                                                        *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_core_poweroff(void)
{
	mOS_exit(__k1_spawn_type() != __MPPA_MPPA_SPAWN, 0);
}
