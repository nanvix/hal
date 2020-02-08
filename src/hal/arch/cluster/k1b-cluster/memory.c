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
#include <arch/cluster/k1b-cluster/cores.h>
#include <arch/cluster/k1b-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Memory Layout.
 */
PUBLIC struct memory_region mem_layout[K1B_CLUSTER_MEM_REGIONS] = {
	{
		.pbase = K1B_CLUSTER_HYPER_LOW_BASE_PHYS,
		.vbase = K1B_CLUSTER_HYPER_LOW_BASE_VIRT,
		.pend  = K1B_CLUSTER_HYPER_LOW_END_PHYS,
		.vend  = K1B_CLUSTER_HYPER_LOW_END_VIRT,
		.size  = K1B_CLUSTER_HYPER_SIZE,
		.writable = false,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "hyper_low"
	},
	{
		.pbase = K1B_CLUSTER_HYPER_HIGH_BASE_PHYS,
		.vbase = K1B_CLUSTER_HYPER_HIGH_BASE_VIRT,
		.pend  = K1B_CLUSTER_HYPER_HIGH_END_PHYS,
		.vend  = K1B_CLUSTER_HYPER_HIGH_END_VIRT,
		.size  = K1B_CLUSTER_HYPER_SIZE,
		.writable = false,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "hyper_high"
	},
	{
		.pbase = K1B_CLUSTER_HYPER_LOW_END_PHYS,                      /**< @see K1B_CLUSTER_KERNEL_BASE_PHYS */
		.vbase = K1B_CLUSTER_HYPER_LOW_END_VIRT,                      /**< @see K1B_CLUSTER_KERNEL_BASE_VIRT */
		.pend  = K1B_PADDR(&_kend),                                   /**< @see K1B_CLUSTER_KERNEL_END_PHYS  */
		.vend  = K1B_VADDR(&_kend),                                   /**< @see K1B_CLUSTER_KERNEL_END_VIRT  */
		.size  = K1B_PADDR(&_kend) - K1B_CLUSTER_HYPER_LOW_END_PHYS,  /**< @see K1B_CLUSTER_KMEM_SIZE        */
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	},
	{
		.pbase = K1B_PADDR(&_kend),                          /**< @see K1B_CLUSTER_KPOOL_BASE_PHYS */
		.vbase = K1B_VADDR(&_kend),                          /**< @see K1B_CLUSTER_KPOOL_BASE_VIRT */
		.pend  = K1B_PADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE, /**< @see K1B_CLUSTER_KPOOL_END_PHYS  */
		.vend  = K1B_VADDR(&_kend) + K1B_CLUSTER_KPOOL_SIZE, /**< @see K1B_CLUSTER_KPOOL_END_VIRT  */
		.size  = K1B_CLUSTER_KPOOL_SIZE,
		.writable = true,
		.executable = false,
		.root_pgtab_num = 0,
		.desc = "kpool"
	},
};

/*
 * Addresses should be aligned to huge page boundaries.
 */
#if (K1B_CLUSTER_HYPER_LOW_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
#error "bad memory layout"
#endif
#if (K1B_CLUSTER_HYPER_HIGH_BASE_VIRT & (K1B_HUGE_PAGE_SIZE - 1))
#error "bad memory layout"
#endif

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
struct
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
 * k1b_cluster_tlb_get_utlb()                                                 *
 *============================================================================*/

/**
 * @brief Gets the underlying TLB entries.
 *
 * @param _tlb     The TLB of a core.
 * @param tlb_type Type of the underlying TLB entries that we want get.
 *
 * The k1b_cluster_tlb_get_utlb() function returns the architectural
 * TLB entries of a specific TLB type.
 *
 * @returns Initial position of the specific underlying tlb entries.
 *
 * @author João Vicente Souto
 */
PUBLIC struct tlbe *k1b_cluster_tlb_get_utlb()
{
	int coreid;

	coreid = core_get_id();

	return (k1b_tlb[coreid].jtlb);
}

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
 * k1b_cluster_tlb_flush()                                                    *
 *============================================================================*/

/**
 * The k1b_cluster_tlb_flush() function writes all JTLB entries cached in
 * memory into the architectural TLB of the underlying core.
 *
 * @todo Implement this function.
 */
PUBLIC int k1b_cluster_tlb_flush(void)
{
	return (0);
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
 * k1b_cluster_tlb_init()                                                     *
 *============================================================================*/

/**
 * The k1b_cluster_tlb_init() initializes the architectural TLB of the
 * underlying k1b core.
 */
PUBLIC void k1b_cluster_tlb_init(void)
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
 * k1b_cluster_tlb_write()                                                    *
 *============================================================================*/

/**
 * THe k1b_cluster_tlb_write() function writes an entry into the architectural
 * TLB. If the new entry conflicts to an old one, the old one is
 * overwritten.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE int k1b_cluster_tlb_write(
		vaddr_t vaddr,
		paddr_t paddr,
		unsigned shift,
		unsigned way,
		unsigned protection
)
{
	int coreid;
	unsigned idx;

	coreid = k1b_core_get_id();
	idx = k1b_tlbe_get_index(vaddr, shift, way);

	return k1b_tlbe_write(
		&k1b_tlb[coreid].jtlb[idx],
		vaddr,
		paddr,
		shift,
		way,
		protection
	);
}

/*============================================================================*
 * k1b_cluster_tlb_inval()                                                    *
 *============================================================================*/

/**
 * The k1b_tlb_inval() function invalidates the TLB entry that
 * encodes the virtual address @p vaddr.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE int k1b_cluster_tlb_inval(vaddr_t vaddr, unsigned shift, unsigned way)
{
	int coreid;
	unsigned idx;

	coreid = k1b_core_get_id();
	idx = k1b_tlbe_get_index(vaddr, shift, way);

	/* Write to hardware TLB. */
	return (k1b_tlbe_inval(&k1b_tlb[coreid].jtlb[idx], vaddr, shift, way));
}

/*============================================================================*
 * k1b_cluster_tlb_shootdown()                                                *
 *============================================================================*/

/**
 * The k1b_cluster_tlb_shootdown() function invalidates the TLB entry
 * that encodes the virtual address @p vaddr in all cores.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_cluster_tlb_shootdown(vaddr_t vaddr)
{
	unsigned idx;
	struct tlbe tlbe;

	idx = k1b_tlbe_get_index(vaddr, K1B_PAGE_SHIFT, 0);

	/* Shootdown hardware TLBs */
	if (k1b_tlbe_shootdown(&tlbe, vaddr, K1B_PAGE_SHIFT))
		return (-EAGAIN);

	/* Shootdown software TLBs. */
	for (int i = 0; i < K1B_CLUSTER_NUM_CORES; i++)
		kmemcpy(&k1b_tlb[i].jtlb[idx], &tlbe, K1B_TLBE_SIZE);

	return (0);
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
PUBLIC void k1b_cluster_mem_warmup(void)
{
	vaddr_t start, end;

	/* Load Hypervisor entries into the TLB. */
	k1b_cluster_tlb_write(
		K1B_CLUSTER_HYPER_LOW_BASE_VIRT,
		K1B_CLUSTER_HYPER_LOW_BASE_VIRT,
		K1B_HUGE_PAGE_SHIFT,
		1,
		K1B_TLBE_PROT_RWX
	);
	k1b_cluster_tlb_write(
		K1B_CLUSTER_HYPER_HIGH_BASE_VIRT,
		K1B_CLUSTER_HYPER_HIGH_BASE_VIRT,
		K1B_HUGE_PAGE_SHIFT,
		1,
		K1B_TLBE_PROT_RWX
	);

	/* Load Kernel entries into the TLB. */
	start = K1B_CLUSTER_KERNEL_BASE_VIRT; end = K1B_CLUSTER_KERNEL_END_VIRT;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_HUGE_PAGE_SIZE)
		k1b_cluster_tlb_write(vaddr, vaddr, K1B_HUGE_PAGE_SHIFT, 1, K1B_TLBE_PROT_RWX);

	/* Load Kernel Page Pool entries into the TLB. */
	start = K1B_CLUSTER_KPOOL_BASE_VIRT; end = K1B_CLUSTER_KPOOL_END_VIRT;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_HUGE_PAGE_SIZE)
		k1b_cluster_tlb_write(vaddr, vaddr, K1B_HUGE_PAGE_SHIFT, 1, K1B_TLBE_PROT_RW);

	/* Invalidate all entries in way 0. */
	start = 0; end = MEMORY_SIZE;
	for (vaddr_t vaddr = start; vaddr < end; vaddr += K1B_PAGE_SIZE)
		k1b_cluster_tlb_inval(vaddr, K1B_PAGE_SHIFT, 0);
}

/*============================================================================*
 * k1b_cluster_mem_check_alignment()                                          *
 *============================================================================*/

/**
 * @brief Assert memory alignment.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void k1b_cluster_mem_check_alignment(void)
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
PUBLIC void k1b_cluster_mem_check_layout(void)
{
	size_t kstack_size;

	kstack_size = (vaddr_t)(&_user_stack_start) - (vaddr_t)(&_user_stack_end);
	kstack_size /= K1B_CLUSTER_NUM_CORES;

	if (K1B_CLUSTER_KSTACK_BASE_VIRT != (vaddr_t)(&_user_stack_start))
		kpanic("bad kernel stack base address");
	if (kstack_size != K1B_CLUSTER_KSTACK_SIZE)
		kpanic("bad kernel stack size");
	if (K1B_CLUSTER_KMEM_SIZE != ((K1B_PADDR(&_kend) - K1B_CLUSTER_HYPER_LOW_END_PHYS)))
		kpanic("bad kernel memory size");
}
