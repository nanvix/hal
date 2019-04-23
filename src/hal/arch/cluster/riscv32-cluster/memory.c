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

#include <arch/cluster/riscv32-cluster/cores.h>
#include <arch/cluster/riscv32-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Number of memory regions.
 */
#define RISCV32_CLUSTER_MEM_REGIONS 4

/**
 * @brief Memory region.
 */
struct memory_region
{
	paddr_t pbase;   /**< Base physical address. */
	vaddr_t vbase;   /**< Base virtual address.  */
	size_t size;     /**< Size.                  */
	bool writable;   /**< Writable?              */
	bool executable; /**< Executable?            */
};

/**
 * @brief Memory layout.
 */
PRIVATE struct memory_region riscv32_cluster_mem_layout[RISCV32_CLUSTER_MEM_REGIONS] = {
	{ RISCV32_CLUSTER_KERNEL_BASE_PHYS, RISCV32_CLUSTER_KERNEL_BASE_VIRT,  RISCV32_CLUSTER_KMEM_SIZE,     true, true  },
	{ RISCV32_CLUSTER_KPOOL_BASE_PHYS,  RISCV32_CLUSTER_KPOOL_BASE_VIRT,   RISCV32_CLUSTER_KPOOL_SIZE,    true, false },
	{ RISCV32_CLUSTER_PIC_BASE_PHYS,    RISCV32_CLUSTER_PIC_BASE_VIRT,     RISCV32_CLUSTER_PIC_MEM_SIZE,  true, false },
	{ RISCV32_CLUSTER_UART_BASE_PHYS,   RISCV32_CLUSTER_UART_BASE_VIRT,    RISCV32_CLUSTER_UART_MEM_SIZE, true, false },
};

/**
 * @brief Root page directory.
 */
PRIVATE struct pde riscv32_cluster_root_pgdir[RV32GC_PGDIR_LENGTH] ALIGN(PAGE_SIZE);

/**
 * @brief Root page tables.
 */
PRIVATE struct pte riscv32_cluster_root_pgtabs[RISCV32_CLUSTER_MEM_REGIONS][RV32GC_PGTAB_LENGTH] ALIGN(PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &riscv32_cluster_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = riscv32_cluster_root_pgtabs[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = riscv32_cluster_root_pgtabs[1];

/*============================================================================*
 * riscv32_cluster_mem_info()                                                 *
 *============================================================================*/

/**
 * @brief Prints information about memory layout.
 *
 * The riscv32_cluster_mem_info() prints information about the virtual
 * memory layout.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void riscv32_cluster_mem_info(void)
{
	kprintf("[hal] kernel_base=%x kernel_end=%x",
		RISCV32_CLUSTER_KERNEL_BASE_VIRT,
		RISCV32_CLUSTER_KERNEL_END_VIRT
	);
	kprintf("[hal]  kpool_base=%x  kpool_end=%x",
		RISCV32_CLUSTER_KPOOL_BASE_VIRT,
		RISCV32_CLUSTER_KPOOL_END_VIRT
	);
	kprintf("[hal]   user_base=%x   user_end=%x",
		RISCV32_CLUSTER_USER_BASE_VIRT,
		RISCV32_CLUSTER_USER_END_VIRT
	);
	kprintf("[hal]   uart_base=%x   uart_end=%x",
		RISCV32_CLUSTER_UART_BASE_VIRT,
		RISCV32_CLUSTER_UART_END_VIRT
	);
	kprintf("[hal]    pic_base=%x    pic_end=%x",
		RISCV32_CLUSTER_PIC_BASE_VIRT,
		RISCV32_CLUSTER_PIC_END_VIRT
	);
	kprintf("[hal] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
		RISCV32_CLUSTER_MEM_SIZE/MB,
		RISCV32_CLUSTER_KMEM_SIZE/KB,
		RISCV32_CLUSTER_KPOOL_SIZE/KB,
		RISCV32_CLUSTER_UMEM_SIZE/KB
	);
}

/*============================================================================*
 * riscv32_cluster_mem_check_align()                                          *
 *============================================================================*/

/**
 * @brief Asserts the memory alignment.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void riscv32_cluster_mem_check_align(void)
{
	/* These should be aligned at page boundaries. */
	if (RISCV32_CLUSTER_PIC_BASE_VIRT & (RV32GC_PAGE_SIZE - 1))
		kpanic("pic base address misaligned");
	if (RISCV32_CLUSTER_PIC_END_VIRT & (RV32GC_PAGE_SIZE - 1))
		kpanic("pic end address misaligned");
	if (RISCV32_CLUSTER_UART_BASE_VIRT & (RV32GC_PAGE_SIZE - 1))
		kpanic("uart base address misaligned");
	if (RISCV32_CLUSTER_UART_END_VIRT & (RV32GC_PAGE_SIZE - 1))
		kpanic("uart end address misaligned");

	/* These should be aligned at page table boundaries. */
	if (RISCV32_CLUSTER_KERNEL_BASE_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("kernel base address misaligned");
	if (RISCV32_CLUSTER_KERNEL_END_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("kernel end address misaligned");
	if (RISCV32_CLUSTER_KPOOL_BASE_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("kernel pool base address misaligned");
	if (RISCV32_CLUSTER_KPOOL_END_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("kernel pool end address misaligned");
	if (RISCV32_CLUSTER_USER_BASE_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("user base address misaligned");
	if (RISCV32_CLUSTER_USER_END_VIRT & (RV32GC_PGTAB_SIZE - 1))
		kpanic("user end address misaligned");
}

/*============================================================================*
 * riscv32_cluster_mem_check_layout()                                         *
 *============================================================================*/

/**
 * @brief Asserts the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void riscv32_cluster_mem_check_layout(void)
{
	/*
	 * These should be identity mapped, becasuse the underlying
	 * hypervisor runs with virtual memory disabled.
	 */
	if (RISCV32_CLUSTER_PIC_BASE_VIRT != RISCV32_CLUSTER_PIC_BASE_PHYS)
		kpanic("pic base address is not identity mapped");
	if (RISCV32_CLUSTER_PIC_END_VIRT != RISCV32_CLUSTER_PIC_END_PHYS)
		kpanic("pic end address is not identity mapped");
	if (RISCV32_CLUSTER_UART_BASE_VIRT != RISCV32_CLUSTER_UART_BASE_PHYS)
		kpanic("uart base address is not identity mapped");
	if (RISCV32_CLUSTER_UART_END_VIRT != RISCV32_CLUSTER_UART_END_PHYS)
		kpanic("uart end address is not identity mapped");
	if (RISCV32_CLUSTER_KERNEL_BASE_VIRT != RISCV32_CLUSTER_KERNEL_BASE_PHYS)
		kpanic("kernel base address is not identity mapped");
	if (RISCV32_CLUSTER_KERNEL_END_VIRT != RISCV32_CLUSTER_KERNEL_END_PHYS)
		kpanic("kernel end address is not identity mapped");
	if (RISCV32_CLUSTER_KPOOL_BASE_VIRT != RISCV32_CLUSTER_KPOOL_BASE_PHYS)
		kpanic("kernel pool base address is not identity mapped");
	if (RISCV32_CLUSTER_KPOOL_END_VIRT != RISCV32_CLUSTER_KPOOL_END_PHYS)
		kpanic("kernel pool end address is not identity mapped");
}

/*============================================================================*
 * riscv32_cluster_mem_map()                                                  *
 *============================================================================*/

/**
 * @brief Builds the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void riscv32_cluster_mem_map(void)
{
	/* Clean root page directory. */
	for (int i = 0; i < RV32GC_PGDIR_LENGTH; i++)
		pde_clear(&riscv32_cluster_root_pgdir[i]);

	/* Build root address space. */
	for (int i = 0; i < RISCV32_CLUSTER_MEM_REGIONS; i++)
	{
		paddr_t j;
		vaddr_t k;
		paddr_t pbase = riscv32_cluster_mem_layout[i].pbase;
		vaddr_t vbase = riscv32_cluster_mem_layout[i].vbase;
		size_t size = riscv32_cluster_mem_layout[i].size;
		int w = riscv32_cluster_mem_layout[i].writable;
		int x = riscv32_cluster_mem_layout[i].executable;

		/* Map underlying pages. */
		for (j = pbase, k = vbase;
			 k < (pbase + size);
			 j += RV32GC_PAGE_SIZE, k += RV32GC_PAGE_SIZE)
		{
			rv32gc_page_map(riscv32_cluster_root_pgtabs[i], j, k, w, x);
		}

		/* Map underlying page table. */
		rv32gc_pgtab_map(
				riscv32_cluster_root_pgdir,
				RV32GC_PADDR(riscv32_cluster_root_pgtabs[i]),
				vbase
		);
	}

	/* Load virtual address space and enable MMU. */
	rv32gc_tlb_load(RV32GC_PADDR(riscv32_cluster_root_pgdir));
}

/*============================================================================*
 * riscv32_cluster_mem_setup()                                                *
 *============================================================================*/

/**
 * The rv32gc_cluster_mem_setup() function initializes the Memory
 * Interface of the underlying RISC-V 32-Bit Cluster.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void riscv32_cluster_mem_setup(void)
{
	int coreid;

	coreid = rv32gc_core_get_id();

	kprintf("[hal] initializing memory layout...");

	/* Master core builds root virtual address space. */
	if (coreid == RISCV32_CLUSTER_COREID_MASTER)
	{
		riscv32_cluster_mem_info();

		/* Check for memory layout. */
		riscv32_cluster_mem_check_layout();
		riscv32_cluster_mem_check_align();

		riscv32_cluster_mem_map();
	}

	/*
	 * TODO: slave cores should warmup the TLB.
	 */
}
