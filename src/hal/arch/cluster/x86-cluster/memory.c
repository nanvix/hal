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

#include <arch/cluster/x86-cluster/cores.h>
#include <arch/cluster/x86-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Number of memory regions.
 */
#define X86_CLUSTER_MEM_REGIONS 2

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
PRIVATE struct memory_region x86_cluster_mem_layout[X86_CLUSTER_MEM_REGIONS] = {
	{ X86_CLUSTER_KERNEL_BASE_PHYS, X86_CLUSTER_KERNEL_BASE_VIRT, X86_CLUSTER_KMEM_SIZE,  true, true  },
	{ X86_CLUSTER_KPOOL_BASE_PHYS,  X86_CLUSTER_KPOOL_BASE_VIRT,  X86_CLUSTER_KPOOL_SIZE, true, false },
};

/**
 * @brief Root page directory.
 */
PRIVATE struct pde x86_cluster_root_pgdir[I486_PGDIR_LENGTH] ALIGN(PAGE_SIZE);

/**
 * @brief Root page tables.
 */
PRIVATE struct pte x86_cluster_root_pgtabs[X86_CLUSTER_MEM_REGIONS][I486_PGTAB_LENGTH] ALIGN(PAGE_SIZE);

/**
 * Alias to root page directory.
 */
PUBLIC struct pde *root_pgdir = &x86_cluster_root_pgdir[0];

/**
 * Alias to kernel page table.
 */
PUBLIC struct pte *kernel_pgtab = x86_cluster_root_pgtabs[0];

/**
 * Alias to kernel page pool page table.
 */
PUBLIC struct pte *kpool_pgtab = x86_cluster_root_pgtabs[1];

/*============================================================================*
 * x86_cluster_mem_info()                                                     *
 *============================================================================*/

/**
 * @brief Prints information about memory layout.
 *
 * The x86_cluster_mem_info() prints information about the virtual
 * memory layout.
 *
 * @author Davidson Francis
 */
PRIVATE void x86_cluster_mem_info(void)
{
	kprintf("[hal] text = %d KB data = %d KB bss = %d KB",
		(&__TEXT_END - &__TEXT_START)/KB,
		(&__DATA_END - &__DATA_START)/KB,
		(&__BSS_END  - &__BSS_START)/KB
	);
	kprintf("[hal] kernel_base=%x kernel_end=%x",
		X86_CLUSTER_KERNEL_BASE_VIRT,
		X86_CLUSTER_KERNEL_END_VIRT
	);
	kprintf("[hal]  kpool_base=%x  kpool_end=%x",
		X86_CLUSTER_KPOOL_BASE_VIRT,
		X86_CLUSTER_KPOOL_END_VIRT
	);
	kprintf("[hal]   user_base=%x   user_end=%x",
		X86_CLUSTER_USER_BASE_VIRT,
		X86_CLUSTER_USER_END_VIRT
	);
	kprintf("[hal] memsize=%d MB kmem=%d KB kpool=%d KB umem=%d KB",
		X86_CLUSTER_MEM_SIZE/MB,
		X86_CLUSTER_KMEM_SIZE/KB,
		X86_CLUSTER_KPOOL_SIZE/KB,
		X86_CLUSTER_UMEM_SIZE/KB
	);
}

/*============================================================================*
 * x86_cluster_mem_check_align()                                              *
 *============================================================================*/

/**
 * @brief Asserts the memory alignment.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void x86_cluster_mem_check_align(void)
{
	/* These should be aligned at page table boundaries. */
	if (X86_CLUSTER_KERNEL_BASE_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("kernel base address misaligned");
	if (X86_CLUSTER_KERNEL_END_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("kernel end address misaligned");
	if (X86_CLUSTER_KPOOL_BASE_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("kernel pool base address misaligned");
	if (X86_CLUSTER_KPOOL_END_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("kernel pool end address misaligned");
	if (X86_CLUSTER_USER_BASE_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("user base address misaligned");
	if (X86_CLUSTER_USER_END_VIRT & (I486_PGTAB_SIZE - 1))
		kpanic("user end address misaligned");
}

/*============================================================================*
 * x86_cluster_mem_check_layout()                                             *
 *============================================================================*/

/**
 * @brief Asserts the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void x86_cluster_mem_check_layout(void)
{
	/*
	 * These should be identity mapped, because the this is called
	 * with paging disabled.
	 */
	if (X86_CLUSTER_KERNEL_BASE_VIRT != X86_CLUSTER_KERNEL_BASE_PHYS)
		kpanic("kernel base address is not identity mapped");
	if (X86_CLUSTER_KERNEL_END_VIRT != X86_CLUSTER_KERNEL_END_PHYS)
		kpanic("kernel end address is not identity mapped");
	if (X86_CLUSTER_KPOOL_BASE_VIRT != X86_CLUSTER_KPOOL_BASE_PHYS)
		kpanic("kernel pool base address is identity mapped");
	if (X86_CLUSTER_KPOOL_END_VIRT != X86_CLUSTER_KPOOL_END_PHYS)
		kpanic("kernel pool end address is not identity mapped");
}

/*============================================================================*
 * x86_cluster_mem_map()                                                     *
 *============================================================================*/

/**
 * @brief Builds the memory layout.
 *
 * @todo TODO provide a detailed description for this function.
 *
 * @author Davidson Francis
 */
PRIVATE void x86_cluster_mem_map(void)
{
	/* Clean root page directory. */
	for (int i = 0; i < I486_PGDIR_LENGTH; i++)
		pde_clear(&x86_cluster_root_pgdir[i]);

	/* Build root address space. */
	for (int i = 0; i < X86_CLUSTER_MEM_REGIONS; i++)
	{
		paddr_t j;
		vaddr_t k;
		paddr_t pbase = x86_cluster_mem_layout[i].pbase;
		vaddr_t vbase = x86_cluster_mem_layout[i].vbase;
		size_t size = x86_cluster_mem_layout[i].size;
		int w = x86_cluster_mem_layout[i].writable;
		int x = x86_cluster_mem_layout[i].executable;

		/* Map underlying pages. */
		for (j = pbase, k = vbase;
			 k < (pbase + size);
			 j += I486_PAGE_SIZE, k += I486_PAGE_SIZE)
		{
			i486_page_map(x86_cluster_root_pgtabs[i], j, k, w, x);
		}

		/* Map underlying page table. */
		i486_pgtab_map(
				x86_cluster_root_pgdir,
				I486_PADDR(x86_cluster_root_pgtabs[i]),
				vbase
		);
	}
}

/*============================================================================*
 * x86_cluster_mem_setup()                                                    *
 *============================================================================*/

/**
 * The x86_cluster_mem_setup() function initializes the Memory
 * Interface of the underlying x86 Cluster.
 *
 * @author Davidson Francis
 */
PUBLIC void x86_cluster_mem_setup(void)
{
	int coreid;

	coreid = i486_core_get_id();

	kprintf("[hal] initializing memory layout...");

	/* Master core builds root virtual address space. */
	if (coreid == X86_CLUSTER_COREID_MASTER)
	{
		x86_cluster_mem_info();

		/* Check for memory layout. */
		x86_cluster_mem_check_layout();
		x86_cluster_mem_check_align();

		x86_cluster_mem_map();
	}

	/* Enable paging. */
	_x86_cluster_enable_paging();
}
