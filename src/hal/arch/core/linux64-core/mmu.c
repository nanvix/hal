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

#include <arch/core/linux64/mmu.h>
#include <nanvix/const.h>
#include <errno.h>

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Daniel Coscia
 */
PUBLIC int linux64_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w)
{

	if(pgtab == NULL)
		return (-EINVAL);
	int i = pte_idx_get(vaddr);
	pgtab[i].writable = w;
	pgtab[i].present = 1;
	pgtab[i].frame = LINUX64_FRAME(paddr >> LINUX64_PAGE_SHIFT);
	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Daniel Coscia
 */
PUBLIC int linux64_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
{
	if(pgdir == NULL)
		return (-EINVAL);

	int i = pde_idx_get(vaddr);
	pgdir[i].present = 1;
	pgdir[i].frame = LINUX64_FRAME(paddr >> LINUX64_PAGE_SHIFT);

	return (0);
}
