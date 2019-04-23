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

#ifndef NANVIX_HAL_CLUSTER_MEMORY_H_
#define NANVIX_HAL_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_MEMORY)

	/* Constants */
	#ifndef UBASE_VIRT
	#error "UBASE_VIRT not defined"
	#endif
	#ifndef USTACK_VIRT
	#error "USTACK_VIRT not defined"
	#endif
	#ifndef KBASE_VIRT
	#error "KBASE_VIRT not defined"
	#endif
	#ifndef KPOOL_VIRT
	#error "KPOOL_VIRT not defined"
	#endif
	#ifndef KBASE_PHYS
	#error "KBASE_PHYS not defined"
	#endif
	#ifndef KPOOL_PHYS
	#error "KPOOL_PHYS not defined"
	#endif
	#ifndef UBASE_PHYS
	#error "UBASE_PHYS not defined"
	#endif
	#ifndef MEMORY_SIZE
	#error "MEMORY_SIZE not defined"
	#endif
	#ifndef KMEM_SIZE
	#error "KMEM_SIZE not defined"
	#endif
	#ifndef KPOOL_SIZE
	#error "KPOOL_SIZE not defined"
	#endif
	#ifndef UMEM_SIZE
	#error "UMEM_SIZE not defined"
	#endif

#endif

/*============================================================================*
 * Memory Interface                                                           *
 *============================================================================*/

/**
 * @defgroup kernel-hal-cluster-mem Memory
 * @ingroup kernel-hal-cluster
 *
 * @brief Memory HAL Interface
 */
/**@{*/

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_MEMORY_H_ */
