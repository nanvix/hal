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

#include <arch/core/arm64/mmu.h>
#include <arch/core/arm64/tlb.h>
#include <nanvix/const.h>
#include <posix/errno.h>
#include <nanvix/hlib.h>

// get addresses from linker
extern volatile unsigned char KERNEL_DATA_START;
extern volatile unsigned char KERNEL_DATA_END;

#define TTBR_CNP    	1
#define MMIO_BASE       0x3F000000

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
	pte_write_set(&pgtab[idx],w);
	pte_exec_set(&pgtab[idx], x);

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

	/* Permissions. */
	pte_write_set(&pgtab[idx],w);
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
	// pgdir[idx].dirty = 0;
	// pgdir[idx].accessed = 0;

	return (0);
}

/*============================================================================*
 * arm64_mmu_setup()                                                           *
 *============================================================================*/

/**
 * The arm_mmu_setup() function initializes the Memory Management Unit
 * (MMU) of the underlying arm64 core.
 */
PUBLIC int arm64_mmu_setup(void) {

	unsigned long data_page = (unsigned long)&KERNEL_DATA_START/ARM64_PAGE_SIZE;
    unsigned long r, b, *paging=(unsigned long*)&KERNEL_DATA_END;
    uint32_t sctlr;

    kprintf("Pagging %ld", paging);

    /* create MMU translation tables at _end */

    // TTBR0, identity L1
    paging[0]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+2*ARM64_PAGE_SIZE) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // identity L2, first 2M block
    paging[2*512]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+3*ARM64_PAGE_SIZE) | // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // identity L2 2M blocks
    b=MMIO_BASE>>21;
    // skip 0th, as we're about to map it by L3
    for(r=1;r<512;r++)
        paging[2*512+r]=(unsigned long)((r<<21)) |  // physical address
        PT_BLOCK |    // map 2M block
        PT_AF |       // accessed flag
        PT_NX |       // no execute
        PT_USER |     // non-privileged
        (r>=b? PT_OSH|PT_DEV : PT_ISH|PT_MEM); // different attributes for device memory

    // identity L3
    for(r=0;r<512;r++)
        paging[3*512+r]=(unsigned long)(r*ARM64_PAGE_SIZE) |   // physical address
        PT_PAGE |     // map 4k
        PT_AF |       // accessed flag
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        ((r<0x80||r>=data_page)? PT_RW|PT_NX : PT_RO); // different for code and data

    // TTBR1, kernel L1
    paging[512+511]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+4*ARM64_PAGE_SIZE) | // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_KERNEL |   // privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // kernel L2
    paging[4*512+511]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+5*ARM64_PAGE_SIZE) |   // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_KERNEL |   // privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // kernel L3
    paging[5*512]=(unsigned long)(MMIO_BASE+0x00201000) |   // physical address
        PT_PAGE |     // map 4k
        PT_AF |       // accessed flag
        PT_NX |       // no execute
        PT_KERNEL |   // privileged
        PT_OSH |      // outter shareable
        PT_DEV;       // device memory

	/* Initialize MAIR indices */
	__asm__ __volatile__("msr MAIR_EL1, %0\n\t" : : "r" (MAIR_ATTRIBUTES) : "memory");
	kprintf("[MMU] MAIR init");

	/* Invalidate TLBs */
	tlb_flush();
	kprintf("[MMU] tlb flush");

	/* Initialize TCR flags */
	__asm__ __volatile__("msr TCR_EL1, %0\n\t" : : "r" (TCR_MMU_ENABLE) : "memory");
	kprintf("[MMU] TCR init");

	/* Initialize TTBR */
	//__asm__ __volatile__("msr TTBR0_EL1, %0\n\t" : : "r" ((uintptr_t)xlat_addr) : "memory");

	// tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // lower half, user space
    asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)&KERNEL_DATA_END + TTBR_CNP));
    // upper half, kernel space
    asm volatile ("msr ttbr1_el1, %0" : : "r" ((unsigned long)&KERNEL_DATA_END + TTBR_CNP + ARM64_PAGE_SIZE));
	kprintf("[MMU] TTBR init");

	/* Ensure system register writes are committed before enabling MMU */
	isb();
	kprintf("[MMU] ISB");

	/* Enable MMU */
	__asm__ __volatile__("mrs %0, SCTLR_EL1\n\t" : "=r" (sctlr) :  : "memory");
	kprintf("[MMU] SCTLR_EL1 read");
	sctlr |= SCTLR_C | SCTLR_M | SCTLR_I;
	kprintf("[MMU] SCTLR_EL1 update");
	__asm__ __volatile__("msr SCTLR_EL1, %0\n\t" : : "r" (sctlr) : "memory");
	kprintf("[MMU] MMU enable");

	isb();
	kprintf("[MMU] ISB");

	return 0;
}

