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

#ifndef ARCH_CLUSTER_OPTIMSOC_CLUSTER_IPI_H_
#define ARCH_CLUSTER_OPTIMSOC_CLUSTER_IPI_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

/**
 * @addtogroup optimsoc-cluster-ipi IPI
 * @ingroup optimsoc-cluster
 *
 * @brief IPI Interface
 */
/**@{*/

	#include <arch/cluster/optimsoc-cluster/ompic.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Sends an interrupt to another core.
	 *
	 * @param coreid ID of target core.
	 */
	static inline void optimsoc_cluster_ipi_send(int coreid)
	{
		optimsoc_cluster_ompic_send_ipi(coreid, 0);
	}

	/**
	 * @brief Complete the interrupt that came from another core.
	 */
	static inline void optimsoc_cluster_ipi_ack(void)
	{
		optimsoc_cluster_ompic_ack_ipi();
	}

	/**
	 * @brief Waits for an IPI interrupt.
	 */
	static inline void optimsoc_cluster_ipi_wait(void)
	{
		or1k_int_wait();
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond optimsoc_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __cluster_ipi_send_fn /**< cluster_ipi_send() */
	#define __cluster_ipi_ack_fn  /**< cluster_ipi_ack()  */
	#define __cluster_ipi_wait_fn /**< cluster_ipi_wait() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see optimsoc_cluster_ipi_send().
	 */
	static inline void cluster_ipi_send(int coreid)
	{
		optimsoc_cluster_ipi_send(coreid);
	}

	/**
	 * @see optimsoc_cluster_ipi_ack().
	 */
	static inline void cluster_ipi_ack(void)
	{
		optimsoc_cluster_ipi_ack();
	}

	/**
	 * @see optimsoc_cluster_ipi_wait().
	 */
	static inline void cluster_ipi_wait(void)
	{
		optimsoc_cluster_ipi_wait();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_OPTIMSOC_CLUSTER_IPI_H_ */

