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
extern volatile unsigned char _data;
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

	// unsigned long data_page = (unsigned long)&_data/ARM64_PAGE_SIZE;
    // unsigned long r, b, *paging=(unsigned long*)&KERNEL_DATA_END;

    // /* create MMU translation tables at _end */

    // // TTBR0, identity L1
    // paging[0]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+2*ARM64_PAGE_SIZE) |    // physical address
    //     PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
    //     PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
    //     PT_USER |     // non-privileged
    //     PT_ISH |      // inner shareable
    //     PT_MEM;       // normal memory

    // // identity L2, first 2M block
    // paging[2*512]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+3*ARM64_PAGE_SIZE) | // physical address
    //     PT_PAGE |     // we have area in it mapped by pages
    //     PT_AF |       // accessed flag
    //     PT_USER |     // non-privileged
    //     PT_ISH |      // inner shareable
    //     PT_MEM;       // normal memory

    // // identity L2 2M blocks
    // b=MMIO_BASE>>21;
    // // skip 0th, as we're about to map it by L3
    // for(r=1;r<512;r++)
    //     paging[2*512+r]=(unsigned long)((r<<21)) |  // physical address
    //     PT_BLOCK |    // map 2M block
    //     PT_AF |       // accessed flag
    //     PT_NX |       // no execute
    //     PT_USER |     // non-privileged
    //     (r>=b? PT_OSH|PT_DEV : PT_ISH|PT_MEM); // different attributes for device memory

    // // identity L3
    // for(r=0;r<512;r++)
    //     paging[3*512+r]=(unsigned long)(r*ARM64_PAGE_SIZE) |   // physical address
    //     PT_PAGE |     // map 4k
    //     PT_AF |       // accessed flag
    //     PT_USER |     // non-privileged
    //     PT_ISH |      // inner shareable
    //     ((r<0x80||r>=data_page)? PT_RW|PT_NX : PT_RO); // different for code and data

    // // TTBR1, kernel L1
    // paging[512+511]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+4*ARM64_PAGE_SIZE) | // physical address
    //     PT_PAGE |     // we have area in it mapped by pages
    //     PT_AF |       // accessed flag
    //     PT_KERNEL |   // privileged
    //     PT_ISH |      // inner shareable
    //     PT_MEM;       // normal memory

    // // kernel L2
    // paging[4*512+511]=(unsigned long)((unsigned char*)&KERNEL_DATA_END+5*ARM64_PAGE_SIZE) |   // physical address
    //     PT_PAGE |     // we have area in it mapped by pages
    //     PT_AF |       // accessed flag
    //     PT_KERNEL |   // privileged
    //     PT_ISH |      // inner shareable
    //     PT_MEM;       // normal memory

    // // kernel L3
    // paging[5*512]=(unsigned long)(MMIO_BASE+0x00201000) |   // physical address
    //     PT_PAGE |     // map 4k
    //     PT_AF |       // accessed flag
    //     PT_NX |       // no execute
    //     PT_KERNEL |   // privileged
    //     PT_OSH |      // outter shareable
    //     PT_DEV;       // device memory

    // /* okay, now we have to set system registers to enable MMU */

    // // check for 4k granule and at least 36 bits physical address bus */
    // asm volatile ("mrs %0, id_aa64mmfr0_el1" : "=r" (r));
    // b=r&0xF;

    // // if(r&(0xF<<28)/*4k*/ || b<1/*36 bits*/) {
    // //     uart_puts("ERROR: 4k granule or 36 bit address space not supported\n");
    // //     return;
    // // }

    // // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our example
    // r=  (0xFF << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
    //     (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
    //     (0x44 <<16);     // AttrIdx=2: non cacheable
    // asm volatile ("msr mair_el1, %0" : : "r" (r));

    // // next, specify mapping characteristics in translate control register
    // r=  (0LL << 36) | // TBI=0, no tagging
    //     (b << 32) |      // IPS=autodetected
    //     (2 << 30) | // TG1=4k
    //     (3 << 28) | // SH1=3 inner
    //     (1 << 26) | // ORGN1=1 write back
    //     (1 << 24) | // IRGN1=1 write back
    //     (0 << 23) | // EPD1 enable higher half
    //     (25LL   << 16) | // T1SZ=25, 3 levels (512G)
    //     (0 << 14) | // TG0=4k
    //     (3 << 12) | // SH0=3 inner
    //     (1 << 10) | // ORGN0=1 write back
    //     (1 << 8) |  // IRGN0=1 write back
    //     (0  << 7) |  // EPD0 enable lower half
    //     (25LL   << 0);   // T0SZ=25, 3 levels (512G)
    // asm volatile ("msr tcr_el1, %0; isb" : : "r" (r));

    // // tell the MMU where our translation tables are. TTBR_CNP bit not documented, but required
    // // lower half, user space
    // asm volatile ("msr ttbr0_el1, %0" : : "r" ((unsigned long)&KERNEL_DATA_END + TTBR_CNP));
    // // upper half, kernel space
    // asm volatile ("msr ttbr1_el1, %0" : : "r" ((unsigned long)&KERNEL_DATA_END + TTBR_CNP + ARM64_PAGE_SIZE));

    // // finally, toggle some bits in system control register to enable page translation
    // asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));
    // r|=0xC00800;     // set mandatory reserved bits
    // r&=~((1<<25) |   // clear EE, little endian translation tables
    //      (1<<24) |   // clear E0E
    //      (1<<19) |   // clear WXN
    //      (1<<12) |   // clear I, no instruction cache
    //      (1<<4) |    // clear SA0
    //      (1<<3) |    // clear SA
    //      (1<<2) |    // clear C, no cache at all
    //      (1<<1));    // clear A, no aligment check
    // r|=  (1<<0);     // set M, enable MMU
    // asm volatile ("msr sctlr_el1, %0; isb" : : "r" (r));


    uint32_t sctlr;

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

