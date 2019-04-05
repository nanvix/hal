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
 * @brief Length of virtual addresses.
 *
 * Number of bits in a virtual address.
 *
 * @author Pedro Henrique Penna
 */
#define RV32I_VADDR_LENGTH 32

/**
 * @brief Page Directory length.
 *
 * Number of Page Directory Entries (PDEs) per Page Directory.
 *
 * @author Pedro Henrique Penna
 */
#define RV32I_PGDIR_LENGTH (1 << (RV32I_VADDR_LENGTH - RV32I_PGTAB_SHIFT))

/**
 * @brief Page Table length.
 *
 * Number of Page Table Entries (PTEs) per Page Table.
 *
 * @author Pedro Henrique Penna
 */
#define RV32I_PGTAB_LENGTH (1 << (RV32I_PGTAB_SHIFT - RV32I_PAGE_SHIFT))

/**
 * @brief Root page directory.
 */
PUBLIC struct pde rv32i_root_pgdir[RV32I_PAGE_SIZE/RV32I_PTE_SIZE] ALIGN(PAGE_SIZE);

/**
 * @brief PIC page table.
 */
PUBLIC struct pte rv32i_pic_pgtab[RV32I_PAGE_SIZE/RV32I_PTE_SIZE] ALIGN(PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &rv32i_root_pgdir[0];

/**
 * Alias to kernel page table.
 *
 * @todo FIXME properly initialize this.
 */
PUBLIC struct pte *kernel_pgtab = NULL;

/**
 * Alias to kernel page pool page table.
 *
 * @todo FIXME properly initialize this.
 */
PUBLIC struct pte *kpool_pgtab = NULL;

/**
 * @brief Assert memory alignment.
 */
PRIVATE void riscv32_cluster_mmu_check_alignment(void)
{
	if (RISCV32_CLUSTER_KERNEL_BASE_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("kernel base address misaligned");
	if (RISCV32_CLUSTER_KERNEL_END_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("kernel end address misaligned");
	if (RISCV32_CLUSTER_KPOOL_BASE_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("kernel pool base address misaligned");
	if (RISCV32_CLUSTER_KPOOL_END_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("kernel pool end address misaligned");
	if (RISCV32_CLUSTER_USER_BASE_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("user base address misaligned");
	if (RISCV32_CLUSTER_USER_END_VIRT & (RV32I_PAGE_SIZE - 1))
		kpanic("user end address misaligned");
}

/**
 * @brief The rv32i_mmu_setup() function initializes the Memory
 * Management Unit (MMU) of the underlying rv32i core.
 */
PUBLIC void riscv32_mmu_setup(void)
{
	int coreid;

	coreid = rv32i_core_get_id();

	kprintf("[hal] initializing mmu");

	/*
	 * Master core builds
	 * root page directory.
	 */
	if (coreid == RISCV32_CLUSTER_COREID_MASTER)
	{
		int pde_idx_kernel;
		int pde_idx_kpool;
		int pde_idx_uart;
		int pde_idx_pic;

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
		kprintf("[hal] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
			_MEMORY_SIZE/MB,
			_KMEM_SIZE/KB,
			_KPOOL_SIZE/KB,
			_UMEM_SIZE/KB
		);

		/* Check for memory layout. */
		riscv32_cluster_mmu_check_alignment();

		/* Clean root page directory. */
		for (int i = 0; i < RV32I_PGDIR_LENGTH; i++)
			pde_clear(&rv32i_root_pgdir[i]);

		pde_idx_kernel = pde_idx_get(RISCV32_CLUSTER_KERNEL_BASE_VIRT);
		pde_idx_kpool = pde_idx_get(RISCV32_CLUSTER_KPOOL_BASE_VIRT);
		pde_idx_uart = pde_idx_get(RISCV32_CLUSTER_UART_VIRT);
		pde_idx_pic = pde_idx_get(RISCV32_CLUSTER_PIC_VIRT);

		/* Map kernel code and data. */
		rv32i_root_pgdir[pde_idx_kernel].valid = 1;
		rv32i_root_pgdir[pde_idx_kernel].readable = 1;
		rv32i_root_pgdir[pde_idx_kernel].writable = 1;
		rv32i_root_pgdir[pde_idx_kernel].executable = 1;
		rv32i_root_pgdir[pde_idx_kernel].frame =
			RV32I_FRAME(RISCV32_CLUSTER_KERNEL_BASE_VIRT >> RV32I_PAGE_SHIFT);

		/* Map kernel pool. */
		rv32i_root_pgdir[pde_idx_kpool].valid = 1;
		rv32i_root_pgdir[pde_idx_kpool].readable = 1;
		rv32i_root_pgdir[pde_idx_kpool].writable = 1;
		rv32i_root_pgdir[pde_idx_kpool].executable = 0;
		rv32i_root_pgdir[pde_idx_kpool].frame =
			RV32I_FRAME(RISCV32_CLUSTER_KPOOL_BASE_VIRT >> RV32I_PAGE_SHIFT);

		/* Map PIC. */
		rv32i_root_pgdir[pde_idx_pic].valid = 1;
		rv32i_root_pgdir[pde_idx_pic].readable = 1;
		rv32i_root_pgdir[pde_idx_pic].writable = 1;
		rv32i_root_pgdir[pde_idx_pic].executable = 0;
		rv32i_root_pgdir[pde_idx_pic].frame =
			RV32I_FRAME(RISCV32_CLUSTER_PIC_VIRT >> RV32I_PAGE_SHIFT);

		/* Map UART. */
		rv32i_root_pgdir[pde_idx_uart].valid = 1;
		rv32i_root_pgdir[pde_idx_uart].readable = 1;
		rv32i_root_pgdir[pde_idx_uart].writable = 1;
		rv32i_root_pgdir[pde_idx_uart].executable = 0;
		rv32i_root_pgdir[pde_idx_uart].frame =
			RV32I_FRAME(RISCV32_CLUSTER_UART_VIRT >> RV32I_PAGE_SHIFT);

		rv32i_tlb_load(RV32I_PADDR(rv32i_root_pgdir));
	}
}
