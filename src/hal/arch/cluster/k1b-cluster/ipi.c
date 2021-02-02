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

#include <nanvix/hal/cluster.h>

/**
 * @brief Get correspond BSP line.
 */
#define K1B_BSP_IPI_LINE (BSP_IT_PE_0)

/**
 * @brief Sends an interrupt to another core.
 *
 * @param coreid ID of target core.
 *
 * @details We must use BSP as an intermediary to emmit interrupt to another
 * core. The BSP has many inter-pe interrupt lines but they are all virtuals
 * over K1B_INT_IPI.
 */
PUBLIC void k1b_cluster_ipi_send(int coreid)
{
	KASSERT(WITHIN(coreid, 0, CORES_NUM));

	bsp_inter_pe_interrupt_raise((1 << coreid), K1B_BSP_IPI_LINE);
	__event_notify(coreid);
}

/**
 * @brief Waits for an IPI interrupt.
 *
 * @details The clear of the mOS interrupt line used
 * is automatically done by the interrupt dispatcher.
 */
PUBLIC void k1b_cluster_ipi_wait(void)
{
	__event_wait();
}

