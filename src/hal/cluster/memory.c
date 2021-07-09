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

/* Must come first. */
#define __NEED_HAL_CLUSTER

#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * ROOT_PGTAB_NUM should be <= MEM_REGIONS.
 */
#if (ROOT_PGTAB_NUM > MEM_REGIONS)
	#error "ROOT_PGTAB_NUM should be less than or equal MEM_REGIONS"
#endif

/**
 * @brief Root page directory.
 */
PRIVATE struct pde cluster_root_pgdir[PGDIR_LENGTH] ALIGN(PAGE_SIZE);

/**
 * @brief Root page tables.
 */
PRIVATE struct pte cluster_root_pgtabs[ROOT_PGTAB_NUM][PGTAB_LENGTH] ALIGN(PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &cluster_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = cluster_root_pgtabs[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = cluster_root_pgtabs[1];


#if (!CORE_HAS_TLB_HW)

/*============================================================================*
 * tlb_lookup_vaddr()                                                         *
 *============================================================================*/

/**
 * The tlb_lookup_vaddr() function searches the architectural TLB
 * for an entry that matches the virtual address @p vaddr.
 * type should be used.
 *
 * @author Davidson Francis
 */
PUBLIC const struct tlbe *tlb_lookup_vaddr(int tlb_type, vaddr_t vaddr)
{
	vaddr_t addr;            /* Aligned address.   */
	struct tlbe *utlb;       /* Underlying TLB.    */
	const struct tlbe *tlbe; /* TLB Entry Pointer. */

	/* Invalid TLB type. */
	if ((tlb_type != TLB_INSTRUCTION) && (tlb_type != TLB_DATA))
		return (NULL);

	addr   = vaddr & TLB_VADDR_MASK;
	utlb   = tlb_get_utlb(tlb_type);

	for (int i = 0; i < TLB_LENGTH; i++)
	{
		tlbe = &utlb[i];

		/* Found */
		if (tlbe_vaddr_get(tlbe) == addr)
		{
			if (tlbe_is_valid(tlbe))
				return (tlbe);
		}
	}

	return (NULL);
}

/*============================================================================*
 * tlb_lookup_paddr()                                                         *
 *============================================================================*/

/**
 * The tlb_lookup_paddr() function searches the architectural TLB
 * for an entry that matches the physical address @p paddr.
 *
 * @returns Upon successful completion, the matching TLB
 * entry for the address @p vaddr is returned. If none
 * is found, @p NULL is returned.
 *
 * @author Davidson Francis
 */
PUBLIC const struct tlbe *tlb_lookup_paddr(int tlb_type, paddr_t paddr)
{
	paddr_t addr;            /* Aligned address.   */
	struct tlbe *utlb;       /* Underlying TLB.    */
	const struct tlbe *tlbe; /* TLB Entry Pointer. */

	/* Invalid TLB type. */
	if ((tlb_type != TLB_INSTRUCTION) && (tlb_type != TLB_DATA))
		return (NULL);

	addr   = paddr & TLB_VADDR_MASK;
	utlb   = tlb_get_utlb(tlb_type);

	for (int i = 0; i < TLB_LENGTH; i++)
	{
		tlbe = &utlb[i];

		/* Found */
		if (tlbe_paddr_get(tlbe) == addr)
		{
			if (tlbe_is_valid(tlbe))
				return (tlbe);
		}
	}

	return (NULL);
}

/*============================================================================*
 * tlb_write()                                                                *
 *============================================================================*/

/**
 * The tlb_write() function writes an entry into the architectural
 * TLB. If the new entry conflicts to an old one, the old one is
 * overwritten.
 *
 * @note Although the OpenRISC specification states that the TLB can
 * have up to 4-ways, there is no known implementation that uses more
 * than 1-way, i.e: direct mapping. Therefore, this function will use
 * only 1-way at the moment.
 *
 * @author Davidson Francis
 */
PUBLIC int tlb_write(int tlb_type, vaddr_t vaddr, paddr_t paddr)
{
	int config;        /* Configuration flag. */
	unsigned idx;      /* TLB Index.          */
	struct tlbe *utlb; /* Underlying TLB.     */

	/* Invalid TLB type. */
	if ((tlb_type != TLB_INSTRUCTION) && (tlb_type != TLB_DATA))
		return (-EINVAL);

	utlb   = tlb_get_utlb(tlb_type);
	idx    = tlbe_get_index(vaddr);
	config = tlb_get_vaddr_info(vaddr);

	if (tlbe_write(&utlb[idx], tlb_type, vaddr, paddr, config) != 0)
		return (-EAGAIN);

	return (0);
}

/*============================================================================*
 * tlb_inval()                                                                *
 *============================================================================*/

/**
 * The tlb_inval() function invalidates the TLB entry that
 * encodes the virtual address @p vaddr.
 *
 * @author Davidson Francis
 */
PUBLIC int tlb_inval(int tlb_type, vaddr_t vaddr)
{
	int idx;           /* TLB Index.      */
	struct tlbe *utlb; /* Underlying TLB. */

	/* Invalid TLB type. */
	if ((tlb_type != TLB_INSTRUCTION) && (tlb_type != TLB_DATA))
		return (-EINVAL);

	idx  = tlbe_get_index(vaddr);
	utlb = tlb_get_utlb(tlb_type);

	if (tlbe_inval(&utlb[idx], tlb_type, vaddr) != 0)
		return (-EAGAIN);

	return (0);
}

/*============================================================================*
 * tlb_dump()                                                                 *
 *============================================================================*/

/**
 * The tlb_dump() function dumps the contents of the TLB on the standard
 * output device.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void tlb_dump(void)
{
	/* Dump D-TLB */
	for (unsigned  i = 0; i < TLB_LENGTH; i++)
		tlbe_dump(TLB_DATA, i);

	/* Dump I-TLB */
	for (unsigned i = 0; i < TLB_LENGTH; i++)
		tlbe_dump(TLB_INSTRUCTION, i);
}

#endif /* !CORE_HAS_TLB_HW */

/*============================================================================*
 * mem_info()                                                                 *
 *============================================================================*/

/**
 * @brief Prints information about memory layout.
 *
 * The mem_info() prints information about the virtual
 * memory layout.
 *
 * @author Davidson Francis
 */
PUBLIC void mem_info(void)
{
	int i; /* Loop index. */

	kprintf("[hal][cluster] text = %d KB data = %d KB bss = %d KB",
		__div((&__TEXT_END - &__TEXT_START), KB),
		__div((&__DATA_END - &__DATA_START), KB),
		__div((&__BSS_END  - &__BSS_START), KB)
	);

	for (i = 0; i < MEM_REGIONS; i++)
	{
		kprintf("[hal][cluster] %s_base=%lx %s_end=%lx",
			mem_layout[i].desc,
			mem_layout[i].vbase,
			mem_layout[i].desc,
			mem_layout[i].vend
		);
	}
	kprintf("[hal][cluster] user_base=%lx   user_end=%lx",
		UBASE_VIRT,
		UEND_VIRT
	);
	kprintf("[hal][cluster] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
		MEMORY_SIZE/MB,
		KMEM_SIZE/KB,
		KPOOL_SIZE/KB,
		UMEM_SIZE/KB
	);
}

/*============================================================================*
 * mem_check_align()                                                          *
 *============================================================================*/

#ifndef __mem_check_align_fn

/**
 * @brief Asserts the memory alignment.
 *
 * Checks the memory alignment for regions that are aligned at page
 * boundaries and regions that must be aligned at page table
 * boundaries.
 *
 * @author Davidson Francis
 */
PUBLIC void mem_check_align(void)
{
	int i; /* Loop index. */

	/* These should be aligned at page boundaries. */
	for (i = MREGION_PG_ALIGN_START; i < MREGION_PG_ALIGN_END; i++)
	{
		if (mem_layout[i].vbase & (KPAGE_SIZE - 1))
			kpanic("%s base address misaligned", mem_layout[i].desc);
		if (mem_layout[i].vend  & (KPAGE_SIZE - 1))
			kpanic("Page %s end address misaligned", mem_layout[i].desc);
	}

	/* These should be aligned at page table boundaries. */
	for (i = MREGION_PT_ALIGN_START; i < MREGION_PT_ALIGN_END; i++)
	{
		if (mem_layout[i].vbase & (PGTAB_SIZE - 1))
			kpanic("%s base address misaligned", mem_layout[i].desc);
		if (mem_layout[i].vend  & (PGTAB_SIZE - 1))
			kpanic("Table %s end address misaligned", mem_layout[i].desc);
	}

	if (UBASE_VIRT & (PGTAB_SIZE - 1))
		kpanic("user base address misaligned");
	if (UEND_VIRT & (PGTAB_SIZE - 1))
		kpanic("user end address misaligned");
}

#endif /* !__mem_check_align_fn */

/*============================================================================*
 * mem_check_layout()                                                         *
 *============================================================================*/

#ifndef __mem_check_layout_fn

/**
 * @brief Asserts the memory layout.
 *
 * Checks if the memory layout is as expected, i.e: if the virtual
 * and physical memories are identity mapped.
 *
 * @author Davidson Francis
 */
PUBLIC void mem_check_layout(void)
{
	int i; /* Loop index. */

	/*
	 * These should be identity mapped, because the this is called
	 * with paging disabled.
	 */
	for (i = 0; i < MEM_REGIONS; i++)
	{
		if (mem_layout[i].vbase != mem_layout[i].pbase)
		{
			kpanic("%s base address is not identity mapped",
				mem_layout[i].desc
			);
		}
		if (mem_layout[i].vend != mem_layout[i].pend)
		{
			kpanic("%s end address is not identity mapped",
				mem_layout[i].desc
			);
		}
	}
}

#endif /* !__mem_check_layout_fn */

/*============================================================================*
 * mem_map()                                                                  *
 *============================================================================*/

/**
 * @brief Builds the memory layout.
 *
 * For each memory region, maps each page into the underlying
 * page table and maps the page table.
 *
 * @author Davidson Francis
 */
PUBLIC void mem_map(void)
{
	/* Clean root page directory. */
	for (int i = 0; i < PGDIR_LENGTH; i++)
		pde_clear(&cluster_root_pgdir[i]);

	/* Build root address space. */
	for (int i = 0; i < MEM_REGIONS; i++)
	{
		paddr_t j;
		vaddr_t k;
		paddr_t pbase = mem_layout[i].pbase;
		vaddr_t vbase = mem_layout[i].vbase;
		size_t size = mem_layout[i].size;
		int w = mem_layout[i].writable;
		int x = mem_layout[i].executable;

		/* Map underlying pages. */
		for (j = pbase, k = vbase;
			 k < (pbase + size);
			 j += PAGE_SIZE, k += PAGE_SIZE)
		{
			mmu_page_map(
				cluster_root_pgtabs[mem_layout[i].root_pgtab_num], j, k, w, x
			);
		}
		/*
		 * Map underlying page table.
		 *
		 * It is important to note that there are no problems to
		 * map multiple times the same page table.
		 */
		mmu_pgtab_map(
				cluster_root_pgdir,
				PADDR(cluster_root_pgtabs[mem_layout[i].root_pgtab_num]),
				TRUNCATE(vbase, PGTAB_SIZE)
		);
		kprintf(
			"PGTAB_map args for %s:\npgdir = %lx\npadr = %lx\nvadr trunc = %lx",
			mem_layout[i].desc,
			cluster_root_pgdir,
			PADDR(cluster_root_pgtabs[mem_layout[i].root_pgtab_num]),
			TRUNCATE(vbase, PGTAB_SIZE)
		);
	}

	/* Load virtual address space and enable MMU. */
	tlb_load(PADDR(cluster_root_pgdir));
}

/*============================================================================*
 * mem_setup()                                                                *
 *============================================================================*/

/**
 * The mem_setup() function initializes the Memory
 * Interface of the underlying cluster.
 *
 * @author Davidson Francis
 */
PUBLIC void  mem_setup(void)
{
	int coreid;

	coreid = core_get_id();

	/* Master core builds root virtual address space. */
	if (coreid == COREID_MASTER)
	{
		kprintf("[hal][cluster] initializing memory layout...");

		mem_info();

		/* Check for memory layout. */
		mem_check_align();
		mem_check_layout();

		mem_map();
	}

	mem_warmup();

	tlb_init();
}
