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

#ifndef NANVIX_HAL_CLUSTER_IPI_H_
#define NANVIX_HAL_CLUSTER_IPI_H_

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if (CLUSTER_HAS_IPI)
#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_IPI)

	/* Functions */
	#ifndef __cluster_ipi_send_fn
	#error "cluster_ipi_send() not defined?"
	#endif
	#ifndef __cluster_ipi_ack_fn
	#error "cluster_ipi_ack() not defined?"
	#endif
	#ifndef __cluster_ipi_wait_fn
	#error "cluster_ipi_wait() not defined?"
	#endif

#endif
#endif

/*============================================================================*
 * IPI Interface                                                              *
 *============================================================================*/

/**
 * @defgroup kernel-hal-cluster-event IPI
 * @ingroup kernel-hal-cluster
 *
 * @brief IPI Interface
 */
/**@{*/

	/**
	 * @brief Sends an interrupt to another core.
	 *
	 * @param coreid ID of target core.
	 */
#if (CLUSTER_HAS_IPI)
	EXTERN void cluster_ipi_send(int coreid);
#else
	static inline void cluster_ipi_send(int coreid)
	{
		UNUSED(coreid);
	}
#endif

	/**
	 * @brief Complete the interrupt that came from another core.
	 */
#if (CLUSTER_HAS_IPI)
	EXTERN void cluster_ipi_ack(void);
#else
	static inline void cluster_ipi_ack(void)
	{
		/* noop. */
	}
#endif

	/**
	 * @brief Waits for an IPI interrupt.
	 */
#if (CLUSTER_HAS_IPI)
	EXTERN void cluster_ipi_wait(void);
#else
	static inline void cluster_ipi_wait(void)
	{
		/* noop. */
	}
#endif

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_IPI_H_ */

