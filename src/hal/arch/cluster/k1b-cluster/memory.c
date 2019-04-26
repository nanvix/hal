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
PUBLIC const vaddr_t K1B_CLUSTER_KERNEL_BASE_VIRT  = K1B_CLUSTER_HYPER_LOW_END_VIRT;             /* Kernel Base        */
PUBLIC const vaddr_t K1B_CLUSTER_KERNEL_END_VIRT   = K1B_VADDR(&_kend);                          /* Kernel End         */
PUBLIC const vaddr_t K1B_CLUSTER_KPOOL_BASE_VIRT   = K1B_VADDR(&_kend);                          /* Kernel Pool Base   */
PUBLIC const vaddr_t K1B_CLUSTER_KPOOL_END_VIRT    = K1B_VADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE; /* Kernel Pool End    */
PUBLIC const vaddr_t K1B_CLUSTER_KSTACK_BASE_VIRT  = K1B_VADDR(&_user_stack_start);              /* Kernel Stack Base  */
/**@}*/

/**
 * @brief TLB
 *
 * Architectural Translation Lookaside Buffer (TLB).
 *
 * @note This structure is accessed by all cores in the processor.
 * Therefore, it must be aligned at a cache line boundary, otherwise
 * we could trash the data.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE struct
{
	/**
	 * @brief Join TLB.
	 *
	 * The Join TLB (JTLB) is a 2-way set-associative TLB intended for
	 * caching not-so-used physical to virtual address translations. A
	 * page of 2^n (with n >= 12) may be encoded either in the entry
	 * 2^(2p) or 2^(2p+1), where p is the value of the bit slice
	 * comprising bits n to n+5 of the virtual address corresponding
	 * to the start of the page.
	 */
	struct tlbe jtlb[K1B_JTLB_LENGTH];

	/**
	 * @brief Locked TLB.
	 *
	 * The Locked TLB (LTLB) is a fully associative TLB intended for
	 * caching always-used physical to virtual address translations.
	 * It may may encode in any entry, any virtual to physical address
	 * translation.
	 */
	struct tlbe ltlb[K1B_LTLB_LENGTH];
} ALIGN(K1B_CACHE_LINE_SIZE) k1b_tlb[K1B_CLUSTER_NUM_CORES];

/*============================================================================*
 * k1b_tlbe_idx_get()                                                         *
 *============================================================================*/

/**
 * @brief Gets a TLB entry.
 *
 * @param idx Index of the target entry in the TLB.
 *
 * The k1b_tlbe_get() function returns the architectural TLB entry at
 * position @p idx.
 *
 * @returns Upon successful completion, the architectural TLB entry at
 * position @p idx is returned. Upon failure, @p NULL is returned
 * instead.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE inline const struct tlbe *k1b_tlbe_get(int idx)
{
	int coreid;

	coreid = k1b_core_get_id();

	if ((idx >= 0) && (idx < K1B_JTLB_LENGTH))
		return (&k1b_tlb[coreid].jtlb[idx]);

	else if (idx < (K1B_JTLB_LENGTH + K1B_LTLB_LENGTH))
		return (&k1b_tlb[coreid].ltlb[(idx - K1B_JTLB_LENGTH)]);

	return (NULL);
}

/*============================================================================*
 * k1b_tlbe_dump()                                                            *
 *============================================================================*/

/**
 * The k1b_tlbe_dump() function dumps information about the TLB entry
 * @p idx on the kernel output device.
 */
PUBLIC void k1b_tlbe_dump(int idx)
{
	const struct tlbe *tlbe;
	static const char *status[4] = {"i", "p", "m", "a"};

	if ((tlbe = k1b_tlbe_get(idx)) == NULL)
	{
		kprintf("no tlb entry");
		return;
	}

	kprintf("[%d][%s] frame=%x page=%x size=%x",
		idx,
		status[tlbe->status],
		k1b_tlbe_paddr_get(tlbe),
		k1b_tlbe_vaddr_get(tlbe),
		k1b_tlbe_pgsize_get(tlbe)
	);
}

/*============================================================================*
 * k1b_tlb_lookup_vaddr()                                                     *
 *============================================================================*/

/**
 * The k1b_tlb_lookup_vaddr() function searches the architectural TLB
 * for an entry that matches the virtual address @p vaddr.
 *
 * @todo Search in the JTLB may be performed using a faster method
 * based on tag lookup.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC const struct tlbe *k1b_tlb_lookup_vaddr(vaddr_t vaddr)
{
	int coreid;
	const struct tlbe *tlbe;

	coreid = k1b_core_get_id();

	/* Search in JTLB. */
	for (int i = 0; i < K1B_JTLB_LENGTH; i++)
	{
		tlbe = &k1b_tlb[coreid].jtlb[i];

		/* Found */
		if (k1b_tlbe_vaddr_get(tlbe) == vaddr)
		{
			if (tlbe->status != K1B_TLBE_STATUS_INVALID)
				return (tlbe);
		}
	}

	/* Search in LTLB. */
	for (int i = 0; i < K1B_LTLB_LENGTH; i++)
	{
		tlbe = &k1b_tlb[coreid].ltlb[i];

		/* Found */
		if (k1b_tlbe_vaddr_get(tlbe) == vaddr)
		{
			if (tlbe->status != K1B_TLBE_STATUS_INVALID)
				return (tlbe);
		}
	}

	return (NULL);
}

/*============================================================================*
 * k1b_tlb_lookup_paddr()                                                     *
 *============================================================================*/

/**
 * The k1b_tlb_lookup_paddr() function searches the architectural TLB
 * for an entry that matches the physical address @p paddr.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC const struct tlbe *k1b_tlb_lookup_paddr(paddr_t paddr)
{
	int coreid;
	const struct tlbe *tlbe;

	coreid = k1b_core_get_id();

	/* Search in JTLB. */
	for (int i = 0; i < K1B_JTLB_LENGTH; i++)
	{
		tlbe = &k1b_tlb[coreid].jtlb[i];

		/* Found */
		if (k1b_tlbe_paddr_get(tlbe) == paddr)
		{
			if (tlbe->status != K1B_TLBE_STATUS_INVALID)
				return (tlbe);
		}
	}

	/* Search in LTLB. */
	for (int i = 0; i < K1B_LTLB_LENGTH; i++)
	{
		tlbe = &k1b_tlb[coreid].ltlb[i];

		/* Found */
		if (k1b_tlbe_paddr_get(tlbe) == paddr)
		{
			if (tlbe->status != K1B_TLBE_STATUS_INVALID)
				return (tlbe);
		}
	}

	return (NULL);
}

/*============================================================================*
 * k1b_tlb_write()                                                            *
 *============================================================================*/

/**
 * THe k1b_tlb_write() function writes an entry into the architectural
 * TLB. If the new entry conflicts to an old one, the old one is
 * overwritten.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_tlb_write(
		vaddr_t vaddr,
		paddr_t paddr,
		unsigned shift,
		unsigned way,
		unsigned protection
)
{
	int coreid;
	unsigned idx;
	struct tlbe tlbe;
	
	coreid = k1b_core_get_id();
	idx = 2*((vaddr >> shift) & 0x3f) + way;

	if (k1b_tlbe_write(vaddr, paddr, shift, way, protection, &tlbe) != 0)
		return (-EAGAIN);

	kmemcpy(&k1b_tlb[coreid].jtlb[idx], &tlbe, K1B_TLBE_SIZE);

	return (0);
}

/*============================================================================*
 * k1b_tlb_inval()                                                            *
 *============================================================================*/

/**
 * The k1b_tlb_inval() function invalidates the TLB entry that
 * encodes the virtual address @p vaddr.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_tlb_inval(vaddr_t vaddr, unsigned shift, unsigned way)
{
	int coreid;
	unsigned idx;
	struct tlbe tlbe;

	coreid = k1b_core_get_id();
	idx = 2*((vaddr >> shift) & 0x3f) + way;

	/* Write to hardware TLB. */
	if (k1b_tlbe_inval(vaddr, shift, way, &tlbe) != 0)
		return (-EAGAIN);

	kmemcpy(&k1b_tlb[coreid].jtlb[idx], &tlbe, K1B_TLBE_SIZE);

	return (0);
}

/*============================================================================*
 * k1b_tlb_flush()                                                            *
 *============================================================================*/

/**
 * The k1b_tlb_flush() function writes all JTLB entries cached in
 * memory into the architectural TLB of the underlying core.
 *
 * @todo Implement this function.
 */
PUBLIC int k1b_tlb_flush(void)
{
	return (0);
}

/*============================================================================*
 * k1b_tlb_init()                                                             *
 *============================================================================*/

/**
 * The k1b_tlb_init() initializes the architectural TLB of the
 * underlying k1b core.
 */
PRIVATE void k1b_tlb_init(void)
{
	int coreid;

	coreid = k1b_core_get_id();

	kprintf("[core %d][hal] initializing tlb", coreid);

	/* Read JTLB into memory. */
	for (int i = 0; i < K1B_JTLB_LENGTH; i++)
		k1b_tlbe_read(&k1b_tlb[coreid].jtlb[i], K1B_JTLB_OFFSET + i);

	/* Read LTLB into memory. */
	for (int i = 0; i < K1B_LTLB_LENGTH; i++)
		k1b_tlbe_read(&k1b_tlb[coreid].ltlb[i], K1B_LTLB_OFFSET + i);
}

/*============================================================================*
 * k1b_cluster_mem_warmup()                                                   *
 *============================================================================*/

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

/*============================================================================*
 * k1b_cluster_mem_info()                                                     *
 *============================================================================*/

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

/*============================================================================*
 * k1b_cluster_mem_check_alignment()                                          *
 *============================================================================*/

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

/*============================================================================*
 * k1b_cluster_mem_check_layout()                                             *
 *============================================================================*/

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

/*============================================================================*
 * k1b_cluster_mem_map()                                                      *
 *============================================================================*/

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

/*============================================================================*
 * k1b_cluster_mem_setup()                                                    *
 *============================================================================*/

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
