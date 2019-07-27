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

#include <nanvix/hal/cluster/memory.h>
#include <arch/cluster/x86-cluster/cores.h>
#include <arch/cluster/x86-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Memory layout.
 */
PUBLIC struct memory_region mem_layout[X86_CLUSTER_MEM_REGIONS] = {
	{
		.pbase = X86_CLUSTER_KERNEL_BASE_PHYS,
		.vbase = X86_CLUSTER_KERNEL_BASE_VIRT,
		.pend  = X86_CLUSTER_KERNEL_END_PHYS,
		.vend  = X86_CLUSTER_KERNEL_END_VIRT,
		.size  = X86_CLUSTER_KMEM_SIZE,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	},
	{
		.pbase = X86_CLUSTER_KPOOL_BASE_PHYS,
		.vbase = X86_CLUSTER_KPOOL_BASE_VIRT,
		.pend  = X86_CLUSTER_KPOOL_END_PHYS,
		.vend  = X86_CLUSTER_KPOOL_END_VIRT,
		.size  = X86_CLUSTER_KPOOL_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 1,
		.desc = "kpool"
	},
};
