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

#include <arch/cluster/linux64-cluster/cores.h>

/**
 * @brief Root page directory.
 */
PRIVATE struct pde linux64_root_pgdir[LINUX64_PGDIR_LENGTH];
/**
 * @brief Root page table.
 */
PRIVATE struct pte linux64_root_pgtab[LINUX64_PGTAB_LENGTH];

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &linux64_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = &linux64_root_pgtab[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = &linux64_root_pgtab[0];

/**
 * @brief TLB
 */
PRIVATE struct tlbe linux64_root_tlb[LINUX64_CLUSTER_NUM_CORES][LINUX64_TLB_LENGTH];

/**
 * Alias to TLB
 */
PUBLIC struct tlbe *root_tlb = &linux64_root_tlb[0][0];

/**
 * @ brief counter for the flush function
 */
PRIVATE unsigned linux64_cluster_tlb_flush_count = 0;

/**
 * @brief Gets the underlying TLB entries.
 *
 * The linux64_cluster_tlb_get_utlb() function returns the architectural
 * TLB entries.
 *
 * @returns Initial position of the specific underlying tlb entries.
 */
PUBLIC struct tlbe *linux64_cluster_tlb_get_utlb()
{
	return linux64_root_tlb[core_get_id()];
}

/**
 * @brief
 */
PUBLIC int linux64_cluster_tlb_flush(void)
{
	linux64_cluster_tlb_flush_count++;
	return (0);
}

/**
 * @brief Dumps a TLB entry.
 *
 * @param idx Index of target entry in the TLB.
 */
PUBLIC void linux64_cluster_tlbe_dump(int idx)
{
	const struct tlbe *tlbe = &linux64_root_tlb[core_get_id()][idx];

	if (tlbe == NULL)
	{
		kprintf("no tlb entry");
		return;
	}

	kprintf("[%d] frame=%x page=%x",
		idx,
		linux64_tlbe_paddr_get(tlbe),
		linux64_tlbe_vaddr_get(tlbe)
	);
}
