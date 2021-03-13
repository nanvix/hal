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

#define __NEED_HAL_CLUSTER
#include <nanvix/hal/clus ter.h>
#include <nanvix/const.h>

EXTERN NORETURN void _main(int, const char *[]);

// /**
//  * @brief Cores table.
//  */
// PUBLIC struct coreinfo cores[ARM64_CLUSTER_NUM_CORES] = {
// 	{ true,  CORE_RUNNING,   0, NULL, RV32GC_SPINLOCK_UNLOCKED }, /* Master Core   */
// 	{ false, CORE_RESETTING, 0, NULL, RV32GC_SPINLOCK_LOCKED   }, /* Slave Core 1  */
// 	{ false, CORE_RESETTING, 0, NULL, RV32GC_SPINLOCK_LOCKED   }, /* Slave Core 2  */
// 	{ false, CORE_RESETTING, 0, NULL, RV32GC_SPINLOCK_LOCKED   }, /* Slave Core 3  */
// };


/*============================================================================*
 * arm64_cluster_setup()                                                    *
 *============================================================================*/

PUBLIC void arm64_cluster_setup(void)
{
	int coreid;

	coreid = arm64_core_get_id();

	if (coreid == ARM64_CLUSTER_COREID_MASTER)
		kprintf("[hal] booting up cluster...");

	//mem_setup();
	//core_setup(NULL);
}

/*============================================================================*
 * arm64_slave_setup()                                                      *
 *============================================================================*/

PUBLIC NORETURN void arm64_cluster_slave_setup(void)
{
	arm64_cluster_setup();
	while (true)
	{
	}
}

/*============================================================================*
 * arm64_master_setup()                                                      *
 *============================================================================*/

PUBLIC NORETURN void arm64_cluster_master_setup(void)
{	
	arm64_cluster_setup();
	_main(0, NULL);
	while (true) {};
}