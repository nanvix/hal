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

#include <nanvix/hal/cluster/memory.h>
#include <arch/cluster/arm64-cluster/cores.h>
#include <arch/cluster/arm64-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Memory layout.
 */
PUBLIC struct memory_region mem_layout[MEM_REGIONS] = {
	// {
	// 	.pbase = 0,
	// 	.vbase = 0,
	// 	.pend  = 0,
	// 	.vend  = 0,
	// 	.size  = 0,
	// 	.writable = true,
	// 	.executable = true,
	// 	.root_pgtab_num = 0,
	// 	.desc = "kernel"
	// }
	{
		.pbase = ARM64_CLUSTER_KERNEL_BASE_PHYS,
		.vbase = ARM64_CLUSTER_KERNEL_BASE_VIRT,
		.pend  = ARM64_CLUSTER_KERNEL_END_PHYS,
		.vend  = ARM64_CLUSTER_KERNEL_END_VIRT,
		.size  = ARM64_CLUSTER_KMEM_SIZE,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	},
	// {
	// 	.pbase = ARM64_CLUSTER_USER_BASE_PHYS,
	// 	.vbase = ARM64_CLUSTER_USER_BASE_VIRT,
	// 	.pend  = ARM64_CLUSTER_USER_END_PHYS,
	// 	.vend  = ARM64_CLUSTER_USER_END_VIRT,
	// 	.size  = ARM64_CLUSTER_UMEM_SIZE,
	// 	.writable = true,
	// 	.executable = true,
	// 	.root_pgtab_num = 1,
	// 	.desc = "user"
	// },
	{
		.pbase = ARM64_CLUSTER_MODULE_START_PHYS,
		.vbase = ARM64_CLUSTER_MODULE_START_VIRT,
		.pend  = ARM64_CLUSTER_MODULE_END_PHYS,
		.vend  = ARM64_CLUSTER_MODULE_END_VIRT,
		.size  = ARM64_CLUSTER_MODULE_SIZE,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 2,
		.desc = "module"
	}
};
