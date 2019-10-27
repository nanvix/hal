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
#include <arch/cluster/linux64-cluster/cores.h>
#include <arch/cluster/linux64-cluster/memory.h>
#include <stdlib.h>

PRIVATE void *linux64_user_base_virt_ptr;
PRIVATE void *linux64_ustack_base_virt_ptr;
PRIVATE void *linux64_kernel_base_virt_ptr;
PRIVATE void *linux64_kpool_base_virt_ptr;

/**
 * Virtual memory layout.
 */
/**@{*/
PUBLIC vaddr_t LINUX64_USER_BASE_VIRT;
PUBLIC vaddr_t LINUX64_USER_END_VIRT;
PUBLIC vaddr_t LINUX64_USTACK_BASE_VIRT;
PUBLIC vaddr_t LINUX64_KERNEL_BASE_VIRT;
PUBLIC vaddr_t LINUX64_KERNEL_END_VIRT;
PUBLIC vaddr_t LINUX64_KPOOL_BASE_VIRT;
PUBLIC vaddr_t LINUX64_KPOOL_END_VIRT;
/**@}*/

/**
 * @brief Binary sections.
 *
 * Since the unix64 target lacks a custom linker
 * script, these values for now are dummy.
 */
PUBLIC unsigned char __TEXT_START = 0;
PUBLIC unsigned char __TEXT_END   = 0;
PUBLIC unsigned char __DATA_START = 0;
PUBLIC unsigned char __DATA_END   = 0;
PUBLIC unsigned char __BSS_START  = 0;
PUBLIC unsigned char __BSS_END    = 0;

/**
 * @brief Memory layout.
 */
PUBLIC struct memory_region mem_layout[LINUX64_CLUSTER_MEM_REGIONS];

/**
 * @ brief counter for the flush function
 */
PRIVATE unsigned linux64_cluster_tlb_flush_count[LINUX64_CLUSTER_NUM_CORES];

/**
 * @brief Flushes the TLB.
 */
PUBLIC int linux64_cluster_tlb_flush(void)
{
	linux64_cluster_tlb_flush_count[linux64_core_get_id()]++;

	return (0);
}


/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_cluster_memory_boot(void)
{
	kprintf("[hal][cluster] powering on memory...");

	/* Grab some memory. */
	KASSERT(posix_memalign(&linux64_user_base_virt_ptr,   LINUX64_PGTAB_SIZE, LINUX64_UMEM_SIZE) == 0);
	KASSERT(posix_memalign(&linux64_ustack_base_virt_ptr, LINUX64_PGTAB_SIZE, LINUX64_PAGE_SIZE) == 0);
	KASSERT(posix_memalign(&linux64_kernel_base_virt_ptr, LINUX64_PGTAB_SIZE, LINUX64_KMEM_SIZE) == 0);
	KASSERT(posix_memalign(&linux64_kpool_base_virt_ptr,  LINUX64_PGTAB_SIZE, LINUX64_KPOOL_SIZE) == 0);

	/* Build memory layout. */
	LINUX64_USER_BASE_VIRT   = (vaddr_t) linux64_user_base_virt_ptr;
	LINUX64_USTACK_BASE_VIRT = (vaddr_t) linux64_ustack_base_virt_ptr;
	LINUX64_KERNEL_BASE_VIRT = (vaddr_t) linux64_kernel_base_virt_ptr;
	LINUX64_KPOOL_BASE_VIRT  = (vaddr_t) linux64_kpool_base_virt_ptr;
	LINUX64_USER_END_VIRT    = LINUX64_USER_BASE_VIRT + LINUX64_UMEM_SIZE;
	LINUX64_KERNEL_END_VIRT  = LINUX64_KERNEL_BASE_VIRT + LINUX64_KMEM_SIZE;
	LINUX64_KPOOL_END_VIRT   = LINUX64_KPOOL_BASE_VIRT + LINUX64_KPOOL_SIZE;

	mem_layout[0].pbase = LINUX64_KERNEL_BASE_VIRT;
	mem_layout[0].vbase = LINUX64_KERNEL_BASE_VIRT;
	mem_layout[0].pend  = LINUX64_KERNEL_END_VIRT;
	mem_layout[0].vend  = LINUX64_KERNEL_END_VIRT;
	mem_layout[0].size  = LINUX64_KMEM_SIZE;
	mem_layout[0].writable = true;
	mem_layout[0].executable = true;
	mem_layout[0].root_pgtab_num = 0;
	mem_layout[0].desc = "kernel";

	mem_layout[1].pbase = LINUX64_KPOOL_BASE_VIRT;
	mem_layout[1].vbase = LINUX64_KPOOL_BASE_VIRT;
	mem_layout[1].pend  = LINUX64_KPOOL_END_VIRT;
	mem_layout[1].vend  = LINUX64_KPOOL_END_VIRT;
	mem_layout[1].size  = LINUX64_KPOOL_SIZE;
	mem_layout[1].writable = true;
	mem_layout[1].executable = false;
	mem_layout[1].root_pgtab_num = 1;
	mem_layout[1].desc = "kpool";
}
