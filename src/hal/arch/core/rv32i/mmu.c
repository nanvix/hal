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

#include <arch/core/rv32i/mmu.h>
#include <nanvix/const.h>
#include <errno.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32i_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgtab == NULL))
		return (-EINVAL);

	idx = pte_idx_get(vaddr);

	pgtab[idx].valid = 1;
	pgtab[idx].readable = 1;
	pgtab[idx].writable = (w) ? 1 : 0;
	pgtab[idx].executable = (x) ? 1 : 0;
	pgtab[idx].frame = RV32I_FRAME(paddr >> RV32I_PAGE_SHIFT);

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32i_huge_page_map(struct pte *pgdir, paddr_t paddr, vaddr_t vaddr, int w, int x)
{
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgdir == NULL))
		return (-EINVAL);

	idx = pde_idx_get(vaddr);

	pgdir[idx].valid = 1;
	pgdir[idx].readable = 1;
	pgdir[idx].writable = (w) ? 1 : 0;
	pgdir[idx].executable = (x) ? 1 : 0;
	pgdir[idx].frame = RV32I_FRAME(paddr >> RV32I_PAGE_SHIFT);

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32i_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
{
	int idx;

	/* Invalid page directory. */
	if (UNLIKELY(pgdir == NULL))
		return (-EINVAL);

	idx = pde_idx_get(vaddr);

	pgdir[idx].valid = 1;
	pgdir[idx].readable = 0;
	pgdir[idx].writable = 0;
	pgdir[idx].executable = 0;
	pgdir[idx].frame = RV32I_FRAME(paddr >> RV32I_PAGE_SHIFT);

	return (0);
}

