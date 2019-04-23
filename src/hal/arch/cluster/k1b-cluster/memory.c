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

#include <arch/cluster/k1b-cluster/cores.h>
#include <arch/cluster/k1b-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Number of memory regions.
 */
#define K1B_CLUSTER_MEM_REGIONS 4

/**
 * @brief Memory region.
 */
struct memory_region
{
	paddr_t pbase; /**< Base physical address. */
	vaddr_t vbase; /**< Base virtual address.  */
	size_t size;   /**< Size.                  */
	bool writable; /**< Writable?              */
};

/*
 * Addresses should be alined to huge page boundaries.
 */
#if (K1B_CLUSTER_HYPER_LOW_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
#error "bad memory layout"
#endif
#if (K1B_CLUSTER_HYPER_HIGH_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
#error "bad memory layout"
#endif

/**
 * @brief Root page directory.
 */
PRIVATE struct pde k1b_root_pgdir[K1B_PGDIR_LENGTH] ALIGN(K1B_PAGE_SIZE);

/**
 * @brief Root page table.
 */
PRIVATE struct pte k1b_root_pgtab[K1B_PGTAB_LENGTH] ALIGN(K1B_PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &k1b_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = &k1b_root_pgtab[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = &k1b_root_pgtab[0];

/*
 * Physical Memory Layout
 */
/**@{*/
PUBLIC const paddr_t K1B_CLUSTER_KERNEL_BASE_PHYS  = K1B_CLUSTER_HYPER_LOW_END_PHYS;                                     /* Kernel Base        */
PUBLIC const paddr_t K1B_CLUSTER_KERNEL_END_PHYS   = K1B_PADDR(&_kend);                                                  /* Kernel End         */
PUBLIC const paddr_t K1B_CLUSTER_KPOOL_BASE_PHYS   = K1B_PADDR(&_kend);                                                  /* Kernel Pool Base   */
PUBLIC const paddr_t K1B_CLUSTER_KPOOL_END_PHYS    = K1B_PADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE;                         /* Kernel Pool End    */
PUBLIC const paddr_t K1B_CLUSTER_USER_BASE_PHYS    = K1B_PADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE;                         /* User Base          */
PUBLIC const paddr_t K1B_CLUSTER_USER_END_PHYS     = K1B_PADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE + K1B_CLUSTER_UMEM_SIZE; /* User End           */
PUBLIC const paddr_t K1B_CLUSTER_KSTACK_BASE_PHYS  = K1B_PADDR(&_user_stack_start);                                      /* Kernel Stack Base  */
/**@}*/

/**
 * Virtual Memory Layout
 */
/**{*/
PUBLIC const vaddr_t K1B_CLUSTER_KERNEL_BASE_VIRT  = K1B_CLUSTER_HYPER_LOW_END_VIRT;                                     /* Kernel Base        */
PUBLIC const vaddr_t K1B_CLUSTER_KERNEL_END_VIRT   = K1B_VADDR(&_kend);                                                  /* Kernel End         */
PUBLIC const vaddr_t K1B_CLUSTER_KPOOL_BASE_VIRT   = K1B_VADDR(&_kend);                                                  /* Kernel Pool Base   */
PUBLIC const vaddr_t K1B_CLUSTER_KPOOL_END_VIRT    = K1B_VADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE;                         /* Kernel Pool End    */
PUBLIC const vaddr_t K1B_CLUSTER_USER_BASE_VIRT    = K1B_VADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE;                         /* User Base          */
PUBLIC const vaddr_t K1B_CLUSTER_USER_END_VIRT     = K1B_VADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE + K1B_CLUSTER_UMEM_SIZE; /* User End           */
PUBLIC const vaddr_t K1B_CLUSTER_KSTACK_BASE_VIRT  = K1B_VADDR(&_user_stack_start);                                      /* Kernel Stack Base  */
/**@}*/

/**
 * @brief Warmups the MMU.
 *
 * The k1b_cluster_mem_warmup() function loads the TLB of the underlying core with
 * the initial maping of the system. Overall, it loads Hypervisor the
 * Hypervisor, Kernel and Kernel Page Pool mappings into way one of
 * the architectural TLB, and then it invalidates all entries in way
 * zero.
 *
 * @bug FIXME we cannot invalidate entries in way 0 of the TLB.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void k1b_cluster_mem_warmup(void)
{
	vaddr_t start, end;

	/* Load Hypervisor entries into the TLB. */
	k1b_tlb_write(
		K1B_CLUSTER_HYPER_LOW_BASE_VIRT,
		K1B_CLUSTER_HYPER_LOW_BASE_VIRT,
		K1B_HUGE_PAGE_SHIFT,
		1,
		K1B_TLBE_PROT_RWX
	);
	k1b_tlb_write(
		K1B_CLUSTER_HYPER_HIGH_BASE_VIRT,
		K1B_CLUSTER_HYPER_HIGH_BASE_VIRT,
		K1B_HUGE_PAGE_SHIFT,
		1,
		K1B_TLBE_PROT_RWX
	);

	/* Load Kernel entries into the TLB. */
	start = K1B_CLUSTER_KERNEL_BASE_VIRT; end = K1B_CLUSTER_KERNEL_END_VIRT;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_HUGE_PAGE_SIZE)
		k1b_tlb_write(vaddr, vaddr, K1B_HUGE_PAGE_SHIFT, 1, K1B_TLBE_PROT_RWX);

	/* Load Kernel Page Pool entries into the TLB. */
	start = K1B_CLUSTER_KPOOL_BASE_VIRT; end = K1B_CLUSTER_KPOOL_END_VIRT;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_HUGE_PAGE_SIZE)
		k1b_tlb_write(vaddr, vaddr, K1B_HUGE_PAGE_SHIFT, 1, K1B_TLBE_PROT_RW);

	/* Invalidate all entries in way 0. */
	start = 0; end = MEMORY_SIZE;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_PAGE_SIZE)
		k1b_tlb_inval(vaddr, K1B_PAGE_SHIFT, 0);
}

/**
 * @brief Print information about memory layout.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void k1b_cluster_mem_info(void)
{
	kprintf("[hal] kernel_base=%x kernel_end=%x",
		K1B_CLUSTER_KERNEL_BASE_VIRT,
		K1B_CLUSTER_KERNEL_END_VIRT
	);
	kprintf("[hal]  kpool_base=%x  kpool_end=%x",
		K1B_CLUSTER_KPOOL_BASE_VIRT,
		K1B_CLUSTER_KPOOL_END_VIRT
	);
	kprintf("[hal]   user_base=%x   user_end=%x",
		K1B_CLUSTER_USER_BASE_VIRT,
		K1B_CLUSTER_USER_END_VIRT
	);

	kprintf("[hal] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
		K1B_CLUSTER_MEM_SIZE/MB,
		K1B_CLUSTER_KMEM_SIZE/KB,
		K1B_CLUSTER_KPOOL_SIZE/KB,
		K1B_CLUSTER_UMEM_SIZE/KB
	);
}

/**
 * @brief Assert memory alignment.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void k1b_cluster_mem_check_alignment(void)
{
	if (K1B_CLUSTER_KERNEL_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("kernel base address misaligned");
	if (K1B_CLUSTER_KERNEL_END_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("kernel end address misaligned");
	if (K1B_CLUSTER_KPOOL_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("kernel pool base address misaligned");
	if (K1B_CLUSTER_KPOOL_END_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("kernel pool end address misaligned");
	if (K1B_CLUSTER_USER_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("user base address misaligned");
	if (K1B_CLUSTER_USER_END_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
		kpanic("user end address misaligned");
}

/**
 * @brief Asserts memory layout.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void k1b_cluster_mem_check_layout(void)
{
	size_t kstack_size;

	kstack_size = (vaddr_t)(&_user_stack_start) - (vaddr_t)(&_user_stack_end);
	kstack_size /= K1B_CLUSTER_NUM_CORES;

	if (K1B_CLUSTER_KSTACK_BASE_VIRT != (vaddr_t)(&_user_stack_start))
		kpanic("bad kernel stack base address");
	if (kstack_size != K1B_CLUSTER_KSTACK_SIZE)
		kpanic("bad kernel stack size");
}

/**
 * @brief Builds the memory layout.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void k1b_cluster_mem_map(void)
{
	struct memory_region k1b_cluster_mem_layout[K1B_CLUSTER_MEM_REGIONS] = {
		{ K1B_CLUSTER_HYPER_LOW_BASE_PHYS,  K1B_CLUSTER_HYPER_LOW_BASE_VIRT,  K1B_CLUSTER_HYPER_SIZE, false },
		{ K1B_CLUSTER_HYPER_HIGH_BASE_PHYS, K1B_CLUSTER_HYPER_HIGH_BASE_VIRT, K1B_CLUSTER_HYPER_SIZE, false },
		{ K1B_CLUSTER_KERNEL_BASE_PHYS,     K1B_CLUSTER_KERNEL_BASE_VIRT,     K1B_CLUSTER_KMEM_SIZE,  true  },
		{ K1B_CLUSTER_KPOOL_BASE_PHYS,      K1B_CLUSTER_KPOOL_BASE_VIRT,      K1B_CLUSTER_KPOOL_SIZE, true  },
	};

	/* Clean root page table. */
	for (int i = 0; i < K1B_PGTAB_LENGTH; i++)
		pte_clear(&k1b_root_pgtab[i]);

	/* Clean root page directory. */
	for (int i = 0; i < K1B_PGDIR_LENGTH; i++)
		pde_clear(&k1b_root_pgdir[i]);

	/* Build page tables. */
	for (int i = 0; i < K1B_CLUSTER_MEM_REGIONS; i++)
	{
		paddr_t pbase = k1b_cluster_mem_layout[i].pbase;
		vaddr_t vbase = k1b_cluster_mem_layout[i].vbase;
		size_t size = k1b_cluster_mem_layout[i].size;
		int w = k1b_cluster_mem_layout[i].writable;
		paddr_t j = pbase;
		vaddr_t k = vbase;

		/* Map underlying pages. */
		for (/* */; k < (pbase + size); j += K1B_PAGE_SIZE, k += K1B_PAGE_SIZE)
			k1b_page_map(k1b_root_pgtab, j, k, w);
	}

	/* Build page directory. */
	k1b_pgtab_map(
			k1b_root_pgdir,
			K1B_PADDR(k1b_root_pgtab),
			K1B_CLUSTER_HYPER_LOW_BASE_VIRT
	);
}

/**
 * The k1b_cluster_mem_setup() function initializes the Memory
 * Interface of the underlying Bostan Cluster.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_cluster_mem_setup(void)
{
	int coreid;

	coreid = k1b_core_get_id();

	/*
	 * Master core builds
	 * root page directory.
	 */
	if (coreid == 0)
	{
		kprintf("[hal] initializing memory layout...");

		k1b_cluster_mem_info();

		/* Check for memory layout. */
		k1b_cluster_mem_check_alignment();
		k1b_cluster_mem_check_layout();

		/* Build memory layout. */
		k1b_cluster_mem_map();
	}

	k1b_cluster_mem_warmup();

	k1b_tlb_init();
}
