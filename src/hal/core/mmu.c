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

#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <nanvix/hal/core/_core.h>
#include <nanvix/hal/core/mmu.h>

/**
 * @brief Megabyte shift
 */
#define MEGABYTE_SHIFT 20

/**
 * @brief Page directory address offset.
 */
#define PGDIR_ADDR_OFFSET                   \
	(((1UL << (VADDR_BIT - MEGABYTE_SHIFT)) \
		>> (VADDR_BIT - PGTAB_SHIFT))       \
		<< MEGABYTE_SHIFT)

/**
 * @brief Page directory last valid address.
 */
#define PGDIR_ADDR_END                           \
	(((1UL << (VADDR_BIT - MEGABYTE_SHIFT))      \
		- (PGDIR_ADDR_OFFSET >> MEGABYTE_SHIFT)) \
		<< MEGABYTE_SHIFT)

/**
 * @brief Page table last valid address.
 */
#define PGTAB_ADDR_END ((PGDIR_ADDR_OFFSET) - (PAGE_SIZE))

/**
 * @brief Searches for a page belonging to a given physical
 * address.
 *
 * The mmu_page_walk function does a page walk in the system
 * and returns the virtual address of the page belonging the
 * given physical address.
 *
 * @param paddr Physical address.
 * @return Returns the virtual address of the page, if not found,
 * null.
 *
 * @author Davidson Francis
 */
PUBLIC void* mmu_page_walk(paddr_t paddr)
{
	paddr_t paddr_aligned; /* Physical address aligned.       */
	vaddr_t vaddr_pgdir;   /* Page dir loop index.            */
	vaddr_t vaddr_pgtab;   /* Page tab loop index.            */
	vaddr_t vaddr;         /* Virtual address found.          */

	struct pte *pte;       /* Working page table table entry. */
	struct pde *pde;       /* Working page directory entry.   */
	struct pte *pgtab;     /* Working page table.             */

	vaddr = 0;
	paddr_aligned = (paddr & PAGE_MASK);

	for (vaddr_pgdir = 0; vaddr_pgdir < PGDIR_ADDR_END;
		vaddr_pgdir += PGDIR_ADDR_OFFSET)
	{
		pde = pde_get(root_pgdir, vaddr_pgdir);
		if (!pde_is_present(pde))
			continue;

		pgtab = (struct pte *)(pde_frame_get(pde) << OR1K_PAGE_SHIFT);
		
		for (vaddr_pgtab = 0; vaddr_pgtab < PGTAB_ADDR_END;
			vaddr_pgtab += PAGE_SIZE)
		{
			pte = pte_get(pgtab, vaddr_pgdir + vaddr_pgtab);
			if (!pte_is_present(pte))
				continue;

			if ( (pte_frame_get(pte) << PAGE_SHIFT) == paddr_aligned )
			{
				vaddr = vaddr_pgdir + vaddr_pgtab;
				goto out;
			}
		}
	}

out:
	if (!vaddr)
		return (NULL);
	else
		return (void*)(vaddr + (paddr - paddr_aligned));
}
