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

	pte_present_set(&pgtab[idx], 1);
	pte_frame_set(&pgtab[idx], ARM64_FRAME(paddr >> ARM64_PAGE_SHIFT));
	pgtab[idx].table = 1;

	/* Permissions. */
	pte_exec_set(&pgtab[idx], x);
	pte_write_set(&pgtab[idx],w);
	

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
	int idx;

	/* Invalid page table. */
	if (UNLIKELY(pgtab == NULL))
		return (-EINVAL);

	idx = pte_idx_get(vaddr);

	pgtab[idx].present = 1;
	pgtab[idx].frame = ARM64_FRAME(paddr >> ARM64_PAGE_SHIFT);
    pgtab[idx].dirty = 0;
    pgtab[idx].accessed = 0;
    pgtab[idx].table = 1;

	/* Permissions. */
	pte_write_set(&pgtab[idx], w);
	pte_exec_set(&pgtab[idx], x);

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

	pgdir[idx].present = 1;
	pgdir[idx].frame = ARM64_FRAME(paddr >> ARM64_PAGE_SHIFT);
	pgdir[idx].accessed = 0;
    pgdir[idx].table = 1;

	return (0);
}



PUBLIC int arm64_enable_mmu(void)
{
	uint32_t sctlr;


	unsigned long r, b;

    asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    b=r&0xF;
    if(r&(0xF<<28)/*4k*/ || b<1/*36 bits*/) {
        kprintf("ERROR: 4k granule or 36 bit address space not supported\n");
        return 0;
    }

	asm volatile (
		"msr tcr_el1, %0	\n\t"
		"msr mair_el1, %1	\n\t"
		:
		: 	"r"(TCR_VALUE),
			"r"(MAIR_VALUE)
		: "memory"
	);

	__asm__ __volatile__("mrs %0, SCTLR_EL1\n\t" : "=r" (sctlr) :  : "memory");

	sctlr |= SCTLR_MMU_ENABLED;
	sctlr |= SCTLR_I_CACHE_ENABLE;
	sctlr |= SCTLR_D_CACHE_ENABLE;

	asm volatile(
		"msr SCTLR_EL1, %0	\n\t"
		"dsb sy				\n\t"
		"isb"
		:
		: "r" (sctlr) 
		: "memory"
	);
	if (mmu_is_enabled()) {
        kprintf("[MMU] MMU enable");
    } else {
        kprintf("[MMU] MMU enable failed");
    }
	return (0);
}


PUBLIC int arm64_config_mmu(void)
{
	/*	MMU kernel map	(DRAM + DEVICE regs)
		map 0x4000 0000 				to 	0xffff 0000 0000 0000 			(1020MB)
		map 0xUART_PA_BASE 				to 	0xffff 0000 0000 0000 + 1020MB 	(2MB)
		map 0xQEMU_VIRT_GIC_PA_BASE 	to 	0xffff 0000 0000 0000 + 1022MB 	(2MB)
	*/
	asm volatile("mov x0, %[kvs]" :: [kvs]"r"(KERNEL_VA_START): "x0");

	//populate first PGD
	populate((pa_pud_base | MM_TYPE_PAGE_TABLE), (pa_pgd_entry));
	//populate first PUD
	populate((pa_pmd_base | MM_TYPE_PAGE_TABLE), (pa_pud_entry));
	//populate 512 PMD
	for (int i = 0; i < ARM64_PGMDIR_LENGTH - 2 ; i ++)
	{
		populate(((ARM64_CLUSTER_DRAM_BASE_PHY + i * ARM64_PMD_SIZE) | MMU_FLAGS), (pa_pmd_base + (i << 3)));
	}

	//UART REG，511号entry，对应VA = VA_START + (511-1) * 2 * 1024 * 1024
	populate((UART_PA_BASE | MMU_DEVICE_FLAGS), (pa_pmd_base + ((ARM64_PGMDIR_LENGTH - 2) << 3)));

	//GIC REG, 512号entry，对应VA = VA_START + (512-1)*2*1024*1024
	populate((QEMU_VIRT_GIC_PA_BASE | MMU_DEVICE_FLAGS), (pa_pmd_base + ((ARM64_PGMDIR_LENGTH - 1) << 3)));

	/*
    	MMU DRAM temporary map
    	map 0x4000 0000 - 0x4020 0000(DRAM) to 0x4000 0000 - 0x4020 0000(VA)
	*/
	asm volatile("mov x0, %[kvs]" :: [kvs]"r"(ARM64_CLUSTER_DRAM_BASE_PHY): "x0");
	//populate second PGD, 1G-2G
	populate((pa_pud_base_ram | MM_TYPE_PAGE_TABLE),(pa_pgd_entry_ram));
	//populate first PUD
	populate((pa_pmd_base_ram | MM_TYPE_PAGE_TABLE),(pa_pud_entry_ram));

	//populate 1 PMD, map 0x4000 0000 - 0x4020 0000(DRAM) to 0x4000 0000 - 0x4020 0000(VA)
    populate((ARM64_CLUSTER_DRAM_BASE_PHY | MMU_FLAGS), (pa_pmd_base_ram));

	/* Invalidate TLBs */
	asm volatile(
		"ic iallu 		\n"                           // I+BTB cache invalidate
    	"tlbi vmalle1   \n"                      // invalidate I + D TLBs
    	"dsb sy"
		: : :
	);
	return 0;
}