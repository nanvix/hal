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

#if (defined(__or1k_cluster__))
	#include <arch/cluster/or1k-cluster/memory.h>
	#include <arch/cluster/or1k-cluster/cores.h>
#elif (defined(__optimsoc_cluster__))
	#include <arch/cluster/optimsoc-cluster/memory.h>
	#include <arch/cluster/optimsoc-cluster/cores.h>
#endif

#include <nanvix/hal/cluster/memory.h>
#include <nanvix/hal/core/exception.h>
#include <nanvix/hlib.h>
#include <nanvix/const.h>

/**
 * @brief Memory layout.
 *
 * @note KERNEL_BASE and KPOOL_BASE *must* be the first two positions,
 * since they are aligned at page table boundaries. The remaining
 * positions left are intended for other devices.
 */
PUBLIC struct memory_region mem_layout[OR1K_CLUSTER_MEM_REGIONS] = {
	{
		.pbase = OR1K_CLUSTER_KERNEL_BASE_PHYS,
		.vbase = OR1K_CLUSTER_KERNEL_BASE_VIRT,
		.pend  = OR1K_CLUSTER_KERNEL_END_PHYS,
		.vend  = OR1K_CLUSTER_KERNEL_END_VIRT,
		.size  = OR1K_CLUSTER_KMEM_SIZE,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	},
	{
		.pbase = OR1K_CLUSTER_KPOOL_BASE_PHYS,
		.vbase = OR1K_CLUSTER_KPOOL_BASE_VIRT,
		.pend  = OR1K_CLUSTER_KPOOL_END_PHYS,
		.vend  = OR1K_CLUSTER_KPOOL_END_VIRT,
		.size  = OR1K_CLUSTER_KPOOL_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 1,
		.desc = "kpool"
	},
	{
		.pbase = OR1K_CLUSTER_OMPIC_BASE_PHYS,
		.vbase = OR1K_CLUSTER_OMPIC_BASE_VIRT,
		.pend  = OR1K_CLUSTER_OMPIC_END_PHYS,
		.vend  = OR1K_CLUSTER_OMPIC_END_VIRT,
		.size  = OR1K_CLUSTER_OMPIC_MEM_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 2,
		.desc = "ompic"
	},

#if (defined(__or1k_cluster__))
	{
		.pbase = OR1K_CLUSTER_UART_BASE_PHYS,
		.vbase = OR1K_CLUSTER_UART_BASE_VIRT,
		.pend  = OR1K_CLUSTER_UART_END_PHYS,
		.vend  = OR1K_CLUSTER_UART_END_VIRT,
		.size  = OR1K_CLUSTER_UART_MEM_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 3,
		.desc = "uart"
	},
	{
		.pbase = OR1K_CLUSTER_ETH_BASE_PHYS,
		.vbase = OR1K_CLUSTER_ETH_BASE_VIRT,
		.pend  = OR1K_CLUSTER_ETH_END_PHYS,
		.vend  = OR1K_CLUSTER_ETH_END_VIRT,
		.size  = OR1K_CLUSTER_ETH_MEM_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 4,
		.desc = "eth"
	},
#endif
};

/**
 * @brief TLB
 *
 * Architectural Translation Lookaside Buffer (TLB).
 *
 * @author Davidson Francis
 */
PRIVATE struct
{
	/**
	 * @brief Data TLB.
	 */
	struct tlbe dtlb[OR1K_TLB_LENGTH];

	/**
	 * @brief Instruction TLB.
	 */
	struct tlbe itlb[OR1K_TLB_LENGTH];
} or1k_tlb[OR1K_CLUSTER_NUM_CORES];

/*============================================================================*
 * or1k_cluster_tlb_get_utlb()                                                *
 *============================================================================*/

/**
 * @brief Gets the underlying TLB entries.
 *
 * @param _tlb     The TLB of a core.
 * @param tlb_type Type of the underlying TLB entries that we want get.
 *
 * The or1k_cluster_tlb_get_utlb() function returns the architectural
 * TLB entries of a specific TLB type.
 *
 * @returns Initial position of the specific underlying tlb entries.
 *
 * @author João Vicente Souto
 */
PUBLIC struct tlbe *or1k_cluster_tlb_get_utlb(int tlb_type)
{
	int coreid;

	coreid = core_get_id();

	if (tlb_type == OR1K_TLB_INSTRUCTION)
		return (or1k_tlb[coreid].itlb);
	else
		return (or1k_tlb[coreid].dtlb);
}

/*============================================================================*
 * or1k_tlb_get_tlbe_config()                                                 *
 *============================================================================*/

/**
 * @brief Gets the configuration of a TLB Entry.
 *
 * @param vaddr Target virtual address.
 *
 * @return K1B TLB entry does not need configuration.
 */
PUBLIC int or1k_cluster_tlb_get_vaddr_info(vaddr_t vaddr)
{
	volatile vaddr_t kcode; /* Kernel text start address. */
	volatile vaddr_t kdata; /* Kernel data start address. */
	int info;               /* User address.              */

	kcode = (vaddr_t)&KERNEL_CODE_START;
	kdata = (vaddr_t)&KERNEL_DATA_START;

	/* Kernel address. */
	if (WITHIN(vaddr, kcode, KMEM_SIZE))
	{
		info = OR1K_KERNEL_VADDR;

		if (WITHIN(vaddr, kcode, kdata))
			info |= OR1K_CODE_VADDR;
		else
			info |= OR1K_DATA_VADDR;
	}

	/* User address. */
	else
	{
		info = OR1K_USER_VADDR;

		if (WITHIN(vaddr, UBASE_VIRT, USTACK_VIRT))
			info |= OR1K_CODE_VADDR;
		else
			info |= OR1K_DATA_VADDR;
	}

	return info;
}

/*============================================================================*
 * or1k_cluster_tlb_flush()                                                   *
 *============================================================================*/

/**
 * The or1k_cluster_tlb_flush() function flushes the changes made to the TLB
 * of the underlying or1k core.
 */
PUBLIC int or1k_cluster_tlb_flush(void)
{
	int coreid;       /* Core ID.   */

	coreid = or1k_core_get_id();

	for (int i = 0; i < OR1K_TLB_LENGTH; i++)
	{
		/* Instruction TLB. */
		or1k_tlbe_update(OR1K_TLB_INSTRUCTION, i, &or1k_tlb[coreid].itlb[i]);

		/* Data TLB. */
		or1k_tlbe_update(OR1K_TLB_DATA, i, &or1k_tlb[coreid].dtlb[i]);
	}

	return (0);
}

/*============================================================================*
 * or1k_cluster_tlb_init()                                                    *
 *============================================================================*/

/**
 * The or1k_tlb_init() initializes the architectural TLB.
 */
PUBLIC void or1k_cluster_tlb_init(void)
{
	unsigned dtlbtr; /* Data TLB Translate Register.         */
	unsigned itlbtr; /* Instruction TLB Translate Register.  */
	unsigned xtlbmr; /* Data/Instruction TLB Match Register. */
	int coreid;      /* Core ID.                             */

	dtlbtr = (OR1K_SPR_DTLBTR_CC | OR1K_SPR_DTLBTR_WBC | OR1K_SPR_DTLBTR_SRE
			| OR1K_SPR_DTLBTR_SWE);

	itlbtr = (OR1K_SPR_ITLBTR_CC | OR1K_SPR_ITLBTR_WBC | OR1K_SPR_ITLBTR_SXE);
	xtlbmr = 1;
	coreid = or1k_core_get_id();

	if (!coreid)
		kprintf("[hal] initializing tlb");

	/* Write into DTLB/ITLB. */
	for (int i = 0; i < OR1K_TLB_LENGTH; i++)
	{
		or1k_tlbe_init(
			i,
			dtlbtr,
			itlbtr,
			xtlbmr,
			&or1k_tlb[coreid].dtlb[i],
			&or1k_tlb[coreid].itlb[i]
		);

		dtlbtr += OR1K_PAGE_SIZE;
		itlbtr += OR1K_PAGE_SIZE;
		xtlbmr += OR1K_PAGE_SIZE;
	}
}

/*============================================================================*
 * or1k_do_tlb_fault()                                                        *
 *============================================================================*/

/**
 * @brief Handles a TLB fault.
 *
 * The or1k_cluster_do_tlb_fault() function handles a early TLB faults. It
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
PRIVATE void or1k_cluster_do_tlb_fault(
	const struct exception *excp,
	const struct context *ctx
)
{
	int tlb_type;      /* Target TLB.                     */
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

	tlb_type = (excp->num == OR1K_EXCP_ITLB_FAULT) ?
		OR1K_TLB_INSTRUCTION : OR1K_TLB_DATA;

	if (tlb_write(tlb_type, vaddr, paddr) < 0)
		kpanic("[hal] cannot write to tlb");
}

/*============================================================================*
 * or1k_enable_mmu()                                                          *
 *============================================================================*/

/**
 * The or1k_mmu_enable() function enables the MMU of the underlying
 * or1k core.
 */
PUBLIC void or1k_enable_mmu(void)
{
	or1k_mtspr(OR1K_SPR_SR, or1k_mfspr(OR1K_SPR_SR)
		| OR1K_SPR_SR_DME | OR1K_SPR_SR_IME);
}

/*============================================================================*
 * or1k_mmu_setup()                                                           *
 *============================================================================*/

/**
 * The or1k_mmu_setup() function initializes the Memory Management Unit
 * (MMU) of the underlying or1k core.
 */
PUBLIC void or1k_mmu_setup(void)
{
	/* TLB Handler. */
	if (core_get_id() == OR1K_CLUSTER_COREID_MASTER)
	{
		exception_register(EXCEPTION_DTLB_FAULT, or1k_cluster_do_tlb_fault);
		exception_register(EXCEPTION_ITLB_FAULT, or1k_cluster_do_tlb_fault);
	}

	/* Enable MMU. */
	or1k_enable_mmu();
}
