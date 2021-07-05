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

#define __NEED_CLUSTER_MEMMAP

#include <arch/cluster/arm64-cluster/memmap.h>
#include <arch/core/arm64/mmu.h>
#include <arch/core/arm64/tlb.h>
#include <nanvix/const.h>
#include <posix/errno.h>
#include <nanvix/hlib.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @param pgtab Page table to be used.
 * @param paddr Physical address target.
 * @param vaddr Virtual address to be mapped.
 * @param w Write permission.
 * @param x Execute permission.
 *
 * @author Alexis Lanquetin
 */
PUBLIC int arm64_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgtab == NULL))
		return (-EINVAL);

	idx = pte_idx_get(vaddr);
	pgtab[idx].present = 1;
	pgtab[idx].table = 1;
	pgtab[idx].accessed = 0;
	pgtab[idx].type = 3;
	pgtab[idx].user = 1;
	pte_write_set(&pgtab[idx], w);
	pte_exec_set(&pgtab[idx], x);
	pgtab[idx].dirty = 0;
	pgtab[idx].frame = ARM64_FRAME(paddr >> ARM64_PAGE_SHIFT);

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @param pgtab Page table to be used.
 * @param paddr Physical address target.
 * @param vaddr Virtual address to be mapped.
 * @param w Write permission.
 * @param x Execute permission.
 *
 * @author Alexis Lanquetin
 */
PUBLIC int arm64_huge_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	arm64_page_map(pgtab, paddr, vaddr, w, x);

	return (0);
}

/**
 * @brief For a given page directory, virtual and physical addresses, maps a
 * specified page table into a page directory entry.
 *
 * @param pgdir Page table to be used.
 * @param paddr Physical address target.
 * @param vaddr Virtual address to be mapped.
 *
 * @author Alexis Lanquetin
 */
PUBLIC int arm64_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
{
	int idx;

	/* Invalid page directory. */
	if (UNLIKELY(pgdir == NULL))
		return (-EINVAL);

	idx = pde_idx_get(vaddr);
	kprintf("Idx : %d", idx);
	pgdir[idx].present = 1;
	pgdir[idx].table = 1;
	pgdir[idx].ns = 1;
	pgdir[idx].accessed = 0;
	pgdir[idx].user = 1;
	pgdir[idx].rdonly = 0;
	pgdir[idx].frame = ARM64_FRAME(paddr >> ARM64_PAGE_SHIFT);
	pgdir[idx].cont = 0;

	return (0);
}

