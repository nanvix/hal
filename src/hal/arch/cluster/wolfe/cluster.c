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
#define __NEED_WOLFE_CTRL
#define __NEED_WOLFE_STDOUT
#define __NEED_CLUSTER_WOLFE

#include <arch/cluster/wolfe/ctrl.h>
#include <arch/cluster/wolfe/stdout.h>
#include <arch/cluster/wolfe.h>
#include <nanvix/const.h>

/*============================================================================*
 * wolfe_poweroff()                                                           *
 *============================================================================*/

/**
 * @brief Powers off the underlying cluster.
 */
PRIVATE NORETURN void wolfe_poweroff(void)
{
	wolfe_ctrl_corestatus_write(WOLFE_CTRL_ADDR);

	UNREACHABLE();
}

/*============================================================================*
 * wolfe_core_master_setup()                                                  *
 *============================================================================*/

/**
 * @brief Initializes the master core.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void wolfe_master_setup(void)
{
	wolfe_stdout_init(WOLFE_STDOUT_ADDR);
	wolfe_stdout_write("Hello World from BigPULP!\n", 26);

	wolfe_poweroff();
}
