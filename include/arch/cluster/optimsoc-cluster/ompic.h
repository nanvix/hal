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

#ifndef CLUSTER_OPTIMSOC_CLUSTER_OMPIC_H_
#define CLUSTER_OPTIMSOC_CLUSTER_OMPIC_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/optimsoc-cluster/_optimsoc-cluster.h>

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <posix/stdint.h>

	/* External functions. */
	EXTERN void or1k_cluster_ompic_init(void);
	EXTERN void or1k_cluster_ompic_send_ipi(uint32_t dstcore, uint16_t data);
	EXTERN void or1k_cluster_ompic_ack_ipi(void);

	/**
	 * @see or1k_cluster_ompic_init().
	 */
	static inline void optimsoc_cluster_ompic_init(void)
	{
		or1k_cluster_ompic_init();
	}

	/**
	 * @see or1k_cluster_ompic_send_ipi()
	 */
	static inline void optimsoc_cluster_ompic_send_ipi(uint32_t dstcore, uint16_t data)
	{
		or1k_cluster_ompic_send_ipi(dstcore, data);
	}

	/**
	 * @see or1k_cluster_ompic_ipi_ack()
	 */
	static inline void optimsoc_cluster_ompic_ack_ipi(void)
	{
		or1k_cluster_ompic_ack_ipi();
	}

#endif /* _ASM_FILE_ */

#endif /* CLUSTER_OPTIMSOC_CLUSTER_OMPIC_H_ */
