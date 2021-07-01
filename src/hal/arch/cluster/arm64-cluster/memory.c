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

#include <nanvix/hal/cluster/memory.h>
#include <arch/cluster/arm64-cluster/cores.h>
#include <arch/cluster/arm64-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Memory layout.
 */
PUBLIC struct memory_region mem_layout[MEM_REGIONS] = {
	{
		.pbase = ARM64_CLUSTER_KERNEL_BASE_PHYS,
		.vbase = ARM64_CLUSTER_KERNEL_BASE_VIRT,
		.pend  = ARM64_CLUSTER_KERNEL_END_PHYS,
		.vend  = ARM64_CLUSTER_KERNEL_END_VIRT,
		.size  = ARM64_CLUSTER_KMEM_SIZE,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	},
	{
		.pbase = ARM64_CLUSTER_KPOOL_BASE_PHYS,
		.vbase = ARM64_CLUSTER_KPOOL_BASE_VIRT,
		.pend  = ARM64_CLUSTER_KPOOL_END_PHYS,
		.vend  = ARM64_CLUSTER_KPOOL_END_VIRT,
		.size  = ARM64_CLUSTER_KPOOL_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 1,
		.desc = "kpool"
	},
	{
		.pbase = ARM64_CLUSTER_UART_BASE_PHYS,
		.vbase = ARM64_CLUSTER_UART_BASE_VIRT,
		.pend  = ARM64_CLUSTER_UART_END_PHYS,
		.vend  = ARM64_CLUSTER_UART_END_VIRT,
		.size  = ARM64_CLUSTER_UART_MEM_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 2,
		.desc = "uart"
	},
	{
		.pbase = ARM64_CLUSTER_GIC_BASE_PHYS,
		.vbase = ARM64_CLUSTER_GIC_BASE_VIRT,
		.pend  = ARM64_CLUSTER_GIC_END_PHYS,
		.vend  = ARM64_CLUSTER_GIC_END_VIRT,
		.size  = ARM64_CLUSTER_GIC_MEM_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 3,
		.desc = "gic"
	},
};


PUBLIC int arm64_enable_mmu(void)
{
	uint64_t sctlr;

	/* Ensure that the mmu is disabled before doing any change */
	arm64_disable_mmu();

	/**
	 * SCTLR Register
	 * This register is responsible for configuring the mmu, especially if the mmu is enabled,
	 * and if caches are enabled
	*/

	// Read actual value of sctlr register
	__asm__ __volatile__(
		"mrs %0, sctlr_el1	\n\t"
		: "=r" (sctlr)
		:
		: "memory"
	);

	//enable mmu
	//sctlr |= SCTLR_MMU_ENABLED;
	sctlr |= SCTLR_I_CACHE_ENABLE;
	sctlr |= SCTLR_D_CACHE_ENABLE;

	asm volatile(
		"msr sctlr_el1, %0	\n\t"
		"dsb sy				\n\t"
		"isb"
		:
		: "r" (sctlr) 
		: "memory"
	);
	return (0);
}

/*
 *	arm64_cpu_setup
 *
 *	Initialise the processor for turning the MMU on.
 */
void arm64_cpu_setup(void)
{
	uint64_t tmp;
	uint64_t val = TCR_VALUE;

	__asm__ __volatile__(
		"tlbi	vmalle1 	\n\t"		// Invalidate local TLB
		"dsb nsh"
		: :	: "memory"
	);

	__asm__ __volatile__(
		"msr 	cpacr_el1, %0 	\n\t"		// Enable FP/ASIMD
		"msr	mdscr_el1, %1	\n\t"		// Reset mdscr_el1 and disable access to the DCC from EL0
		"isb"
		:
		: "r" (3<<20), "r" (1 << 12)
		: "memory"
	);


	/*
	 * Read the PARange bits from ID_AA64MMFR0_EL1 and set the IPS bits in
	 * TCR_EL1.
	 */
	__asm__ __volatile__(
		"mrs	%0, ID_AA64MMFR0_EL1 	\n\t"
		"bfi	%1, %0, #32, #3\n\t"
		"isb"
		: "=r" (tmp)
		: "r" (TCR_VALUE)
		: "memory"
	);
	/*
	 * Hardware update of the Access and Dirty bits.
	 */
	__asm__ __volatile__(
		"mrs	%0, ID_AA64MMFR1_EL1 	\n\t"
		"and	%0, %0, #0xf		\n\t"
		: "=r" (tmp)
		:
		: "memory"
	);

	if (tmp != 0xf)
	{
		if (tmp >= 2)
			val |=TCR_HD;		// hardware Dirty flag update
		val |= TCR_HA;		// hardware Access flag update
	}
	/**
	*	MAIR Register
	*	We are using only 2 out of 8 available slots in the mair registers.
	*	The first one corresponds to device memory and second to normal non-cachable memory
	*
	*	TCR Register
	*	tcr_el1 of Translation Control Register is responsible for configuring some general
	*	parameters of the MMU. (For example, here we configure that both 
	*	kernel and user page tables should use 4 KB pages.)
	*/
	asm volatile (
		"msr tcr_el1, %0	\n\t"
		"msr mair_el1, %1	\n\t"
		:
		: 	"r"(val),
			"r"(MAIR_VALUE)
		: "memory"
	);
}
/*============================================================================*
 * arm64_mmu_setup()                                                           *
 *============================================================================*/

/**
 * The arm64_mmu_setup() function initializes the Memory Management Unit
 * (MMU) of the underlying arm64 core.
 */

PUBLIC void arm64_mmu_setup(void)
{

	__asm__ __volatile__(
		"msr ttbr0_el1, %0 \n\t"
		"msr ttbr1_el1, %0 \n\t"
		:
		: "r" (root_pgdir)
		: "memory"
	);

	arm64_cpu_setup();

	/* Enable MMU. */
	arm64_enable_mmu();

	if (mmu_is_enabled()) {
        kprintf("[hal][mmu] MMU enable");
    } else {
        kprintf("[hal][mmu] MMU enable failed");
    }
}


