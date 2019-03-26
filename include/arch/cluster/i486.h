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

#ifndef CLUSTER_I486_H_
#define CLUSTER_I486_H_

	#ifndef __NEED_CLUSTER_I486
		#error "bad cluster configuration?"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/i486/_i486.h>

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @addtogroup i486-cluster x86 Cluster
 * @ingroup clusters
 *
 * @brief x86 Cluster
 */
/**@{*/

	#include <arch/cluster/i486/cores.h>
	#include <arch/cluster/i486/memory.h>

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define CLUSTER_IS_MULTICORE  1 /**< Multicore Cluster */
	#define CLUSTER_IS_IO         1 /**< I/O Cluster       */
	#define CLUSTER_IS_COMPUTE    0 /**< Compute Cluster   */
	#define CLUSTER_SUPPORTS_MMIO 0 /**< MMIO Support      */
	/**@}*/

/**@}*/

#endif /* CLUSTER_I486_H_ */
