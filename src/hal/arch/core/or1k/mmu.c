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

#include <arch/core/or1k/mmu.h>
#include <nanvix/const.h>
#include <posix/errno.h>

/**
 * @brief For a given page table, addresses and permissions, maps a virtual
 * address into a physical address by the specified page table with the
 * permissions required. The address will be mapped using the default
 * architecture page size.
 *
 * @param pgtab Page table to be used.
 * @param paddr Physical address target.
 * @param vaddr Virtual address to be mapped.
 * @param w Write permission.
 * @param x Execute permission.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgtab == NULL))
		return (-EINVAL);

	idx = pte_idx_get(vaddr);

	pgtab[idx].cc = 1;
	pgtab[idx].present = 1;
	pgtab[idx].wbc = 1;
	pgtab[idx].wom = 0;
	pgtab[idx].accessed = 0;
	pgtab[idx].dirty = 0;
	pgtab[idx].last = 1;
	pgtab[idx].frame = OR1K_FRAME(paddr >> OR1K_PAGE_SHIFT);

	/* Permissions. */
	pgtab[idx].ppi = 0;

	if (w)
		pgtab[idx].ppi  = (OR1K_PT_PPI_USR_RDWR >> OR1K_PT_PPI_OFFSET);

	if (x)
		pgtab[idx].ppi |= ((OR1K_PT_PPI_USR_EX | OR1K_PT_PPI_SPV_EX)
			>> OR1K_PT_PPI_OFFSET);

	return (0);
}

/**
 * @brief For a given page directory, addresses and permissions, maps a virtual
 * address into a physical address by the specified page directory with the
 * permissions required. The address will be mapped using the biggest page
 * size available, i.e: huge pages and thus, using only one level page table.
 *
 * @param pgtab Page table to be used.
 * @param paddr Physical address target.
 * @param vaddr Virtual address to be mapped.
 * @param w Write permission.
 * @param x Execute permission.
 *
 * @author Davidson Francis
 */
PUBLIC int or1k_huge_page_map(struct pte *pgdir, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgdir == NULL))
		return (-EINVAL);

	idx = pde_idx_get(vaddr);

	pgdir[idx].cc = 1;
	pgdir[idx].present = 1;
	pgdir[idx].wbc = 1;
	pgdir[idx].wom = 0;
	pgdir[idx].accessed = 0;
	pgdir[idx].dirty = 0;
	pgdir[idx].last = 1;
	pgdir[idx].frame = OR1K_FRAME(paddr >> OR1K_PAGE_SHIFT);

	/* Permissions. */
	pgdir[idx].ppi = 0;

	if (w)
		pgdir[idx].ppi  = (OR1K_PT_PPI_USR_RDWR >> OR1K_PT_PPI_OFFSET);

	if (x)
		pgdir[idx].ppi |= ((OR1K_PT_PPI_USR_EX | OR1K_PT_PPI_SPV_EX)
			>> OR1K_PT_PPI_OFFSET);

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
 * @author Davidson Francis
 */
PUBLIC int or1k_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
{
	int idx;

	/* Invalid page directory. */
	if (UNLIKELY(pgdir == NULL))
		return (-EINVAL);

	idx = pde_idx_get(vaddr);

	pgdir[idx].cc = 1;
	pgdir[idx].present = 1;
	pgdir[idx].wbc = 1;
	pgdir[idx].wom = 0;
	pgdir[idx].accessed = 0;
	pgdir[idx].dirty = 0;
	pgdir[idx].last = 0;
	pgdir[idx].frame = OR1K_FRAME(paddr >> OR1K_PAGE_SHIFT);

	/* Permissions. */
	pgdir[idx].ppi = 0;

	return (0);
}
