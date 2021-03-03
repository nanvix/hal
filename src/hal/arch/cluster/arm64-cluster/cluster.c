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

#include <nanvix/const.h>
#include <arch/cluster/arm64-cluster/cores.h>
#include <arch/cluster/arm64-cluster/psci.h>
#include <arch/stdout/pl011.h>
#include <arch/core/arm64-core/core.h>

EXTERN NORETURN void _main(int, const char *[]);


/*============================================================================*
 * arm64_cluster_setup()                                                    *
 *============================================================================*/

PUBLIC void arm64_cluster_setup(void)
{
	int coreid;

	coreid = arm64_core_get_id();
	switch (coreid)
    {
    case 0:
        uart_pl011_write("CPU ID 0!\n", 20);
        break;
    case 1:
        uart_pl011_write("CPU ID 1!\n", 20);
        break;
    case 2:
        uart_pl011_write("CPU ID 2!\n", 20);
        break;
    case 3:
        uart_pl011_write("CPU ID 3!\n", 20);
        break;
	default:
		uart_pl011_write("CPU ID ?!\n", 20);
		break;
	}

	//if (coreid == ARM64_CLUSTER_COREID_MASTER)
		//kparm64_mcall_cpu_idrintf("[hal] booting up cluster...");

	//mem_setup();
	//core_setup(NULL);
}

/*============================================================================*
 * arm64_slave_setup()                                                      *
 *============================================================================*/

PUBLIC NORETURN void arm64_cluster_slave_setup(void)
{
	//arm64_cluster_setup();
	for(int i = 0; i < 1000000; i++);
	_main(0, NULL);
	while (true)
	{
	}
}

/*============================================================================*
 * arm64_master_setup()                                                      *
 *============================================================================*/

PUBLIC NORETURN void arm64_cluster_master_setup(void)
{
	for(int i = 0; i < 1000000; i++);
	psci_cpu_turn_on_print(PSCI_v0_2_CPU1);
	for(int i = 0; i < 1000000; i++);
	psci_cpu_turn_on_print(PSCI_v0_2_CPU1);
	for(int i = 0; i < 1000000; i++);
	//_main(0, NULL);
	while (true) {};
}
