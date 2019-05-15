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

#ifndef _NANVIX_HAL_CLUSTER_CLUSTER_H_
#define _NANVIX_HAL_CLUSTER_CLUSTER_H_

	/**
	 * @defgroup clusters Clusters
	 */

	#if (defined(__k1bdp__) || defined(__k1bio__))

		#undef  __NEED_CLUSTER_K1B
		#define __NEED_CLUSTER_K1B
		#include <arch/cluster/k1b-cluster.h>

	#elif (defined(__x86_cluster__))

		#undef  __NEED_CLUSTER_X86
		#define __NEED_CLUSTER_X86
		#include <arch/cluster/x86-cluster.h>

	#elif (defined(__or1k_cluster__))

		#undef  __NEED_CLUSTER_OR1K
		#define __NEED_CLUSTER_OR1K
		#include <arch/cluster/or1k-cluster.h>

	#elif (defined(__optimsoc_cluster__))

		#undef  __NEED_CLUSTER_OPTIMSOC
		#define __NEED_CLUSTER_OPTIMSOC
		#include <arch/cluster/optimsoc-cluster.h>

	#elif (defined(__riscv32_cluster__))

		#undef  __NEED_CLUSTER_RISCV32
		#define __NEED_CLUSTER_RISCV32
		#include <arch/cluster/riscv32-cluster.h>

	#elif (defined(__linux64_cluster__))

		#undef  __NEED_CLUSTER_LINUX64
		#define __NEED_CLUSTER_LINUX64
		// TODO: include cluster-specific header.

	#else

		#error "unkonwn cluster"

	#endif

	#undef  __NEED_HAL_CORE
	#define __NEED_HAL_CORE
	#include <nanvix/hal/core.h>

#endif /* _NANVIX_HAL_CLUSTER_CLUSTER_H_ */
