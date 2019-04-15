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

#include <arch/cluster/or1k-cluster/memory.h>
#include <arch/cluster/or1k-cluster/cores.h>
#include <nanvix/hal/core/exception.h>
#include <nanvix/klib.h>
#include <nanvix/const.h>

/**
 * @brief Length of virtual addresses.
 *
 * Number of bits in a virtual address.
 *
 * @author Davidson Francis
 */
#define OR1K_VADDR_LENGTH 32

/**
 * @brief Number of memory regions.
 */
#define OR1K_CLUSTER_MEM_REGIONS 4

/**
 * @brief Memory region.
 */
struct memory_region
{
	paddr_t pbase;  /**< Base physical address. */
	vaddr_t vbase;  /**< Base virtual address.  */
	size_t size;    /**< Size.                  */
	int writable;   /**< Writable?              */
	int executable; /**< Executable?            */
};

/**
 * @brief Memory layout.
 */
PRIVATE struct memory_region or1k_cluster_mem_layout[OR1K_CLUSTER_MEM_REGIONS] = {
	{ OR1K_CLUSTER_KERNEL_BASE_PHYS, OR1K_CLUSTER_KERNEL_BASE_VIRT, OR1K_CLUSTER_KMEM_SIZE,      TRUE, TRUE  },
	{ OR1K_CLUSTER_KPOOL_BASE_PHYS,  OR1K_CLUSTER_KPOOL_BASE_VIRT,  OR1K_CLUSTER_KPOOL_SIZE,     TRUE, FALSE },
	{ OR1K_CLUSTER_UART_BASE_PHYS,   OR1K_CLUSTER_UART_BASE_VIRT,   OR1K_CLUSTER_UART_MEM_SIZE,  TRUE, FALSE },
	{ OR1K_CLUSTER_OMPIC_BASE_PHYS,  OR1K_CLUSTER_OMPIC_BASE_VIRT,  OR1K_CLUSTER_OMPIC_MEM_SIZE, TRUE, FALSE },
};

/**
 * @brief Root page directory.
 */
PRIVATE struct pde or1k_cluster_root_pgdir[OR1K_PGDIR_LENGTH] ALIGN(PAGE_SIZE);

/**
 * @brief Root page tables.
 */
PRIVATE struct pte or1k_cluster_root_pgtabs[OR1K_CLUSTER_MEM_REGIONS][OR1K_PGTAB_LENGTH] ALIGN(PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &or1k_cluster_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = or1k_cluster_root_pgtabs[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = or1k_cluster_root_pgtabs[1];

/**
 * @brief Handles a TLB fault.
 *
 * The or1k_do_tlb_fault() function handles a early TLB faults. It
 * checks the current page directory for a virtual-to-physical address
 * mapping, and if it finds one, it writes this mapping to the TLB. If
 * the faulting address is not currently mapped in the current page
 * directory, it panics the kernel.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted execution context.
 *
 * @author Davidson Francis
 * @author Pedro Henrique Penna
 */
PRIVATE void or1k_do_tlb_fault(
	const struct exception *excp,
	const struct context *ctx
)
{
	int tlb;           /* Target TLB.                     */
	paddr_t paddr;     /* Physical address.               */
	vaddr_t vaddr;     /* Faulting address.               */
	struct pte *pte;   /* Working page table table entry. */
	struct pde *pde;   /* Working page directory entry.   */
	struct pte *pgtab; /* Working page table.             */

	/* Get page address of faulting address. */
	vaddr = or1k_excp_get_addr(excp);
	vaddr &= OR1K_PAGE_MASK;

	/* Lookup PDE. */
	pde = pde_get(root_pgdir, vaddr);
	if (!pde_is_present(pde))
	{
		or1k_context_dump(ctx);
		kpanic("[hal] page fault at %x", exception_get_addr(excp));
	}

	/* Lookup PTE. */
	pgtab = (struct pte *)(pde_frame_get(pde) << OR1K_PAGE_SHIFT);
	pte = pte_get(pgtab, vaddr);
	if (!pte_is_present(pte))
	{
		or1k_context_dump(ctx);
		kpanic("[hal] page fault at %x", exception_get_addr(excp));
	}

	/* Writing mapping to TLB. */
	paddr = pte_frame_get(pte) << OR1K_PAGE_SHIFT;
	tlb = (excp->num == OR1K_EXCP_ITLB_FAULT) ?
		OR1K_TLB_INSTRUCTION : OR1K_TLB_DATA;
	if (or1k_tlb_write(tlb, vaddr, paddr) < 0)
		kpanic("[hal] cannot write to tlb");
}

/**
 * The or1k_mmu_enable() function enables the MMU of the underlying
 * or1k core.
 */
PUBLIC void or1k_enable_mmu(void)
{
	or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR)
		| OR1K_SPR_SR_DME | OR1K_SPR_SR_IME);
}

/**
 * The or1k_mmu_setup() function initializes the Memory Management Unit
 * (MMU) of the underlying or1k core.
 */
PUBLIC void or1k_mmu_setup(void)
{
	/* TLB Handler. */
	exception_register(EXCEPTION_DTLB_FAULT, or1k_do_tlb_fault);
	exception_register(EXCEPTION_ITLB_FAULT, or1k_do_tlb_fault);

	/* Initial TLB. */
	or1k_tlb_init();

	/* Enable MMU. */
	or1k_enable_mmu();
}

/*============================================================================*
 * or1k_cluster_mem_info()                                                    *
 *============================================================================*/

/**
 * @brief Prints information about memory layout.
 *
 * The or1k_cluster_mem_info() prints information about the virtual
 * memory layout.
 *
 * @author Davidson Francis
 */
PRIVATE void or1k_cluster_mem_info(void)
{
	kprintf("[hal] kernel_base=%x kernel_end=%x",
		OR1K_CLUSTER_KERNEL_BASE_VIRT,
		OR1K_CLUSTER_KERNEL_END_VIRT
	);
	kprintf("[hal] kpool_base=%x  kpool_end=%x",
		OR1K_CLUSTER_KPOOL_BASE_VIRT,
		OR1K_CLUSTER_KPOOL_END_VIRT
	);
	kprintf("[hal] user_base=%x   user_end=%x",
		OR1K_CLUSTER_USER_BASE_VIRT,
		OR1K_CLUSTER_USER_END_VIRT
	);
	kprintf("[hal] uart_base=%x   uart_end=%x",
		OR1K_CLUSTER_UART_BASE_VIRT,
		OR1K_CLUSTER_UART_END_VIRT
	);
	kprintf("[hal] ompic_base=%x  ompic_end=%x",
		OR1K_CLUSTER_OMPIC_BASE_VIRT,
		OR1K_CLUSTER_OMPIC_END_VIRT
	);
	kprintf("[hal] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
		OR1K_CLUSTER_MEM_SIZE/MB,
		OR1K_CLUSTER_KMEM_SIZE/KB,
		OR1K_CLUSTER_KPOOL_SIZE/KB,
		OR1K_CLUSTER_UMEM_SIZE/KB
	);
}

/*============================================================================*
 * or1k_cluster_mem_check_align()                                             *
 *============================================================================*/

/**
 * @brief Asserts the memory alignment.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void or1k_cluster_mem_check_align(void)
{
	/* These should be aligned at page boundaries. */
	if (OR1K_CLUSTER_UART_BASE_VIRT & (OR1K_PAGE_SIZE - 1))
		kpanic("uart base address misaligned");
	if (OR1K_CLUSTER_UART_END_VIRT & (OR1K_PAGE_SIZE - 1))
		kpanic("uart end address misaligned");
	if (OR1K_CLUSTER_OMPIC_BASE_VIRT & (OR1K_PAGE_SIZE - 1))
		kpanic("ompic base address misaligned");
	if (OR1K_CLUSTER_OMPIC_END_VIRT & (OR1K_PAGE_SIZE - 1))
		kpanic("ompic end address misaligned");

	/* These should be aligned at page table boundaries. */
	if (OR1K_CLUSTER_KERNEL_BASE_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("kernel base address misaligned");
	if (OR1K_CLUSTER_KERNEL_END_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("kernel end address misaligned");
	if (OR1K_CLUSTER_KPOOL_BASE_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("kernel pool base address misaligned");
	if (OR1K_CLUSTER_KPOOL_END_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("kernel pool end address misaligned");
	if (OR1K_CLUSTER_USER_BASE_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("user base address misaligned");
	if (OR1K_CLUSTER_USER_END_VIRT & (OR1K_PGTAB_SIZE - 1))
		kpanic("user end address misaligned");
}

/*============================================================================*
 * or1k_cluster_mem_check_layout()                                            *
 *============================================================================*/

/**
 * @brief Asserts the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void or1k_cluster_mem_check_layout(void)
{
	/*
	 * These should be identity mapped, becasuse the underlying
	 * hypervisor runs with virtual memory disabled.
	 */
	if (OR1K_CLUSTER_UART_BASE_VIRT != OR1K_CLUSTER_UART_BASE_PHYS)
		kpanic("uart base address is not identity mapped");
	if (OR1K_CLUSTER_UART_END_VIRT != OR1K_CLUSTER_UART_END_PHYS)
		kpanic("uart end address is not identity mapped");
	if (OR1K_CLUSTER_OMPIC_BASE_VIRT != OR1K_CLUSTER_OMPIC_BASE_PHYS)
		kpanic("ompic base address is not identity mapped");
	if (OR1K_CLUSTER_OMPIC_END_VIRT != OR1K_CLUSTER_OMPIC_END_PHYS)
		kpanic("ompic end address is not identity mapped");
	if (OR1K_CLUSTER_KERNEL_BASE_VIRT != OR1K_CLUSTER_KERNEL_BASE_PHYS)
		kpanic("kernel base address is not identity mapped");
	if (OR1K_CLUSTER_KERNEL_END_VIRT != OR1K_CLUSTER_KERNEL_END_PHYS)
		kpanic("kernel end address is not identity mapped");
	if (OR1K_CLUSTER_KPOOL_BASE_VIRT != OR1K_CLUSTER_KPOOL_BASE_PHYS)
		kpanic("kernel pool base address is not identity mapped");
	if (OR1K_CLUSTER_KPOOL_END_VIRT != OR1K_CLUSTER_KPOOL_END_PHYS)
		kpanic("kernel pool end address is not identity mapped");
}

/*============================================================================*
 * or1k_cluster_mem_map()                                                     *
 *============================================================================*/

/**
 * @brief Builds the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void or1k_cluster_mem_map(void)
{
	/* Clean root page directory. */
	for (int i = 0; i < OR1K_PGDIR_LENGTH; i++)
		pde_clear(&or1k_cluster_root_pgdir[i]);

	/* Build root address space. */
	for (int i = 0; i < OR1K_CLUSTER_MEM_REGIONS; i++)
	{
		paddr_t j;
		vaddr_t k;
		paddr_t pbase = or1k_cluster_mem_layout[i].pbase;
		vaddr_t vbase = or1k_cluster_mem_layout[i].vbase;
		size_t size = or1k_cluster_mem_layout[i].size;
		int w = or1k_cluster_mem_layout[i].writable;
		int x = or1k_cluster_mem_layout[i].executable;

		/* Map underlying pages. */
		for (j = pbase, k = vbase;
			 k < (pbase + size);
			 j += OR1K_PAGE_SIZE, k += OR1K_PAGE_SIZE)
		{
			or1k_page_map(or1k_cluster_root_pgtabs[i], j, k, w, x);
		}

		/* Map underlying page table. */
		or1k_pgtab_map(
				or1k_cluster_root_pgdir,
				OR1K_PADDR(or1k_cluster_root_pgtabs[i]),
				vbase
		);
	}
}

/*============================================================================*
 * or1k_cluster_mem_setup()                                                   *
 *============================================================================*/

/**
 * The or1k_cluster_mem_setup() function initializes the Memory
 * Interface of the underlying OpenRISC Cluster.
 *
 * @author Davidson Francis
 */
PUBLIC void or1k_cluster_mem_setup(void)
{
	int coreid;

	coreid = or1k_core_get_id();

	kprintf("[hal] initializing memory layout...");

	/* Master core builds root virtual address space. */
	if (coreid == OR1K_CLUSTER_COREID_MASTER)
	{
		or1k_cluster_mem_info();

		/* Check for memory layout. */
		or1k_cluster_mem_check_layout();
		or1k_cluster_mem_check_align();

		or1k_cluster_mem_map();
	}

	/*
	 * TODO: slave cores should warmup the TLB.
	 */
}
