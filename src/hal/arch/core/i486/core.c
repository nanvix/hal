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

#define __NEED_HAL_CLUSTER
#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo ALIGN(I486_CACHE_LINE_SIZE) cores[X86_CLUSTER_NUM_CORES] = {
	{ true,  CORE_RUNNING,   0, NULL, I486_SPINLOCK_LOCKED }, /* Master Core   */
};

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
