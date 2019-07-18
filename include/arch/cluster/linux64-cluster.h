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

#ifndef CLUSTER_LINUX64_CLUSTER_H_
#define CLUSTER_LINUX64_CLUSTER_H_


	#ifndef __NEED_CLUSTER_LINUX64
		#error "bad cluster configuration?"
	#endif

	/* Cluster Interface Implementation */
	#include <arch/cluster/linux64-cluster/_linux64-cluster.h>

/**
 * @addtogroup linux64-cluster Linux64 Cluster
 * @ingroup clusters
 *
 * @brief Linux64 Cluster
 */
/**@{*/

	#include <arch/cluster/linux64-cluster/memory.h>
	#include <arch/cluster/linux64-cluster/timer.h>
	#include <arch/cluster/linux64-cluster/cores.h>
	#include <arch/cluster/linux64-cluster/event.h>

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define CLUSTER_IS_MULTICORE  0 /**< Multicore Cluster */
	#define CLUSTER_IS_IO         1 /**< I/O Cluster       */
	#define CLUSTER_IS_COMPUTE    0 /**< Compute Cluster   */
	#define CLUSTER_HAS_EVENTS    0 /**< Event Support?    */
	#define CLUSTER_HAS_RTC       1 /**< RTC Support?      */
	/**@}*/

/**@}*/

#endif /* CLUSTER_LINUX64_CLUSTER_H_ */
