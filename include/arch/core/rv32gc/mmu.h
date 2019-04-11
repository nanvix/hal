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

#ifndef ARCH_CORE_RV32GC_MMU_H_
#define ARCH_CORE_RV32GC_MMU_H_

/**
 * @addtogroup rv32gc-core-mmu MMU
 * @ingroup rv32gc-core
 *
 * @brief Memory Management Unit
 */
/**@{*/

	/* Must comme first. */
	#define __NEED_MEMORY_TYPES

	#include <arch/core/rv32gc/types.h>
#ifndef _ASM_FILE_
	#include <nanvix/klib.h>
	#include <errno.h>
#endif

	/**
	 * @name Page Shifts and Masks
	 */
	/**@{*/
	#define RV32GC_PAGE_SHIFT  12                        /**< Page Shift       */
	#define RV32GC_PGTAB_SHIFT 22                        /**< Page Table Shift */
	#define RV32GC_PAGE_MASK   (~(RV32GC_PAGE_SIZE - 1))  /**< Page Mask        */
	#define RV32GC_PGTAB_MASK  (~(RV32GC_PGTAB_SIZE - 1)) /**< Page Table Mask  */
	/**@}*/

	/**
	 * @name Size of Pages and Page Tables
	 */
	/**@{*/
	#define RV32GC_PAGE_SIZE  (1 << RV32GC_PAGE_SHIFT)  /**< Page Size                 */
	#define RV32GC_PGTAB_SIZE (1 << RV32GC_PGTAB_SHIFT) /**< Page Table Size           */
	#define RV32GC_PTE_SIZE   4                        /**< Page Table Entry Size     */
	#define RV32GC_PDE_SIZE   4                        /**< Page Directory Entry Size */
	/**@}*/

	/**
	 * @brief Length of virtual addresses.
	 *
	 * Number of bits in a virtual address.
	 *
	 * @author Pedro Henrique Penna
	 */
	#define RV32GC_VADDR_LENGTH 32

	/**
	 * @brief Page Directory length.
	 *
	 * Number of Page Directory Entries (PDEs) per Page Directory.
	 *
	 * @author Pedro Henrique Penna
	 */
	#define RV32GC_PGDIR_LENGTH (1 << (RV32GC_VADDR_LENGTH - RV32GC_PGTAB_SHIFT))

	/**
	 * @brief Page Table length.
	 *
	 * Number of Page Table Entries (PTEs) per Page Table.
	 *
	 * @author Pedro Henrique Penna
	 */
	#define RV32GC_PGTAB_LENGTH (1 << (RV32GC_PGTAB_SHIFT - RV32GC_PAGE_SHIFT))

	/**
	 * @name Page Table Entry Shifts and Masks
	 */
	/**@{*/
	#define RV32GC_PTE_V (1 << 0) /**< Valid PTE?   */
	#define RV32GC_PTE_R (1 << 1) /**< Readable?    */
	#define RV32GC_PTE_W (1 << 2) /**< Writable?    */
	#define RV32GC_PTE_X (1 << 3) /**< Executable?  */
	#define RV32GC_PTE_U (1 << 4) /**< User Page?   */
	#define RV32GC_PTE_G (1 << 5) /**< Global Page? */
	#define RV32GC_PTE_A (1 << 6) /**< Accessed?    */
	#define RV32GC_PTE_D (1 << 7) /**< Dirty?       */
	/**@}*/

	/**
	 * @name Access Modes for a Page.
	 */
	/**@{*/
	#define RV32GC_PTE_RW  (RV32GC_PTE_R | RV32GC_PTE_W)               /**< Read-Write Page         */
	#define RV32GC_PTE_RX  (RV32GC_PTE_R | RV32GC_PTE_X)               /**< Read-Execute Page       */
	#define RV32GC_PTE_RWX (RV32GC_PTE_R | RV32GC_PTE_W | RV32GC_PTE_X) /**< Read-Write-Execute Page */
	/**@}*/

	/**
	 * @brief Computes the PTE index of a virtual address.
	 *
	 * @param x Target virtual address.
	 */
	#define RV32GC_PTE_IDX(x) \
		(((x) & (RV32GC_PGTAB_MASK^RV32GC_PAGE_MASK)) >> (RV32GC_PAGE_SHIFT))

	/**
	 * @brief Computes the PDE index of a virtual address.
	 *
	 * @param x Target virtual address.
	 */
	#define RV32GC_PDE_IDX(x) \
		((x) >> (RV32GC_PGTAB_SHIFT))

#ifndef _ASM_FILE_

	/**
	 * @brief Page directory entry.
	 */
	struct pde
	{
		unsigned valid      :  1; /**< Valid?       */
		unsigned readable   :  1; /**< Readable?    */
		unsigned writable   :  1; /**< Writable?    */
		unsigned executable :  1; /**< Executable?  */
		unsigned user       :  1; /**< User page?   */
		unsigned global     :  1; /**< Global Page? */
		unsigned            :  1; /**< Reserved     */
		unsigned            :  1; /**< Reserved     */
		unsigned            :  2; /**< Reserved     */
		unsigned frame      : 22; /**< Frame Number */
	};

	/**
	 * @brief Page table entry.
	 */
	struct pte
	{
		unsigned valid      :  1; /**< Valid?       */
		unsigned readable   :  1; /**< Readable?    */
		unsigned writable   :  1; /**< Writable?    */
		unsigned executable :  1; /**< Executable?  */
		unsigned user       :  1; /**< User page?   */
		unsigned global     :  1; /**< Global Page? */
		unsigned accessed   :  1; /**< Accessed?    */
		unsigned dirty      :  1; /**< Dirty?       */
		unsigned            :  2; /**< Reserved     */
		unsigned frame      : 22; /**< Frame Number */
	};

	/**
	 * @brief Maps a page.
	 *
	 * @param pgtab Target page table.
	 * @param paddr Physical address of the target page frame.
	 * @param vaddr Virtual address of the target page.
	 * @param w     Writable page?
	 * @param x     Executable page?
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x);

	/**
	 * @brief Maps a huge page.
	 *
	 * @param pgtab Target page directory.
	 * @param paddr Physical address of the target huge page frame.
	 * @param vaddr Virtual address of the target huge page.
	 * @param w     Writable huge page?
	 * @param x     Executable huge page?
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_huge_page_map(struct pte *pgdir, paddr_t paddr, vaddr_t vaddr, int w, int x);

	/**
	 * @brief Maps a page table.
	 *
	 * @param pgdir Target page directory.
	 * @param paddr Physical address of the target page table frame.
	 * @param vaddr Virtual address of the target page table.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int rv32gc_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define KPAGE_SIZE  RV32GC_PAGE_SIZE   /**< @ref RV32GC_PAGE_SIZE   */
	#define PAGE_SIZE   RV32GC_PAGE_SIZE   /**< @ref RV32GC_PAGE_SIZE   */
	#define PGTAB_SIZE  RV32GC_PGTAB_SIZE  /**< @ref RV32GC_PGTAB_BIT   */
	#define PTE_SIZE    RV32GC_PTE_SIZE    /**< @ref RV32GC_PTE_SIZE    */
	#define PDE_SIZE    RV32GC_PDE_SIZE    /**< @ref RV32GC_PDE_SIZE    */
	#define PAGE_SHIFT  RV32GC_PAGE_SHIFT  /**< @ref RV32GC_PAGE_SHIFT  */
	#define PGTAB_SHIFT RV32GC_PGTAB_SHIFT /**< @ref RV32GC_PGTAB_SHIFT */
	#define PAGE_MASK   RV32GC_PAGE_MASK   /**< @ref RV32GC_PAGE_MASK   */
	#define PGTAB_MASK  RV32GC_PGTAB_MASK  /**< @ref RV32GC_PGTAB_MASK  */
	#define PADDR_BIT   RV32GC_PADDR_BIT   /**< @ref RV32GC_PADDR_BIT   */
	#define VADDR_BIT   RV32GC_VADDR_BIT   /**< @ref RV32GC_VADDR_BIT   */
	#define PADDR_BYTE  RV32GC_PADDR_BYTE  /**< @ref RV32GC_PADDR_BYTE  */
	#define VADDR_BYTE  RV32GC_VADDR_BYTE  /**< @ref RV32GC_VADDR_BYTE  */
	/**@}*/

	/**
	 * @name Exported Types
	 */
	/**@{*/
	#define __frame_t /**< @ref frame_t */
	#define __paddr_t /**< @ref paddr_t */
	#define __vaddr_t /**< @ref vaddr_t */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __root_pgdir   /**< @ref root_pgdir   */
	#define __kernel_pgtab /**< @ref kernel_pgtab */
	#define __kpool_pgtab  /**< @ref kpool_pgtab  */
	/**@}*/

	/**
	 * @brief Exported Functions
	 */
	/**@{*/
	#define __pde_clear_fn       /**< pde_clear()       */
	#define __pde_frame_get_fn   /**< pde_frame_get()   */
	#define __pde_frame_set_fn   /**< pde_frame_set()   */
	#define __pde_get_fn         /**< pde_get()         */
	#define __pde_is_present_fn  /**< pde_is_present()  */
	#define __pde_is_user_fn     /**< pde_is_user()     */
	#define __pde_is_write_fn    /**< pde_is_write()    */
	#define __pde_present_set_fn /**< pde_present_set() */
	#define __pde_user_set_fn    /**< pde_user_set()    */
	#define __pde_write_set_fn   /**< pde_write_set()   */
	#define __pte_clear_fn       /**< pte_clear()       */
	#define __pte_frame_get_fn   /**< pte_frame_get()   */
	#define __pte_frame_set_fn   /**< pte_frame_set()   */
	#define __pte_get_fn         /**< pte_get()         */
	#define __pte_is_present_fn  /**< pte_is_present()  */
	#define __pte_is_user_fn     /**< pte_is_user()     */
	#define __pte_is_write_fn    /**< pte_is_write()    */
	#define __pte_present_set_fn /**< pte_present_set() */
	#define __pte_user_set_fn    /**< pte_user_set()    */
	#define __pte_write_set_fn   /**< pte_write_set()   */
	#define __mmu_is_enabled_fn  /**< mmu_is_enabled()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Clears a page directory entry.
	 *
	 * @param pde Target page directory entry.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_clear(struct pde *pde)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		kmemset(pde, 0, PTE_SIZE);

		return (0);
	}

	/**
	 * @brief Sets the frame of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param frame Frame number.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_frame_set(struct pde *pde, frame_t frame)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		/* Invalid frame. */
		if (frame > ~(frame_t)((1 << (VADDR_BIT - PAGE_SHIFT)) - 1))
			return (-EINVAL);

		pde->frame = frame;

		return (0);
	}

	/**
	 * @brief Sets/clears the present bit of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_present_set(struct pde *pde, int set)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		pde->valid = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the present bit of a page table is set.
	 *
	 * @param pde Page directory entry of target page table.
	 *
	 * @returns If the present bit of the target page table is set,
	 * non zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_is_present(struct pde *pde)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return (pde->valid);
	}

	/**
	 * @brief Gets the frame number of a page directory entry.
	 *
	 * @param pde Target page directory entry.
	 *
	 * @returns The frame number of the target page directory entry.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline frame_t pde_frame_get(struct pde *pde)
	{
		return (pde->frame);
	}

	/**
	 * @brief Sets/clears the read bit of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_read_set(struct pde *pde, int set)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		pde->readable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the read bit of a page table is set.
	 *
	 * @param pde Page directory entry of target page table.
	 *
	 * @returns If the read bit of the target page table is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_is_read(struct pde *pde)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return (pde->writable);
	}

	/**
	 * @brief Sets/clears the write bit of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_write_set(struct pde *pde, int set)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		/*
		 * Set read and write bits,
		 * otherwise the behavior is
		 * undefined.
		 */
		pde->readable = (set) ? 1 : 0;
		pde->writable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the write bit of a page table is set.
	 *
	 * @param pde Page directory entry of target page table.
	 *
	 * @returns If the write bit of the target page table is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_is_write(struct pde *pde)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return (pde->writable);
	}

	/**
	 * @brief Sets/clears the exec bit of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_exec_set(struct pde *pde, int set)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		/*
		 * Set read and executable
		 * bits otherwise the behavior
		 * is undefined.
		 */
		pde->readable = (set) ? 1 : 0;
		pde->executable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the exec bit of a page table is set.
	 *
	 * @param pde Page directory entry of target page table.
	 *
	 * @returns If the exec bit of the target page table is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_is_exec(struct pde *pde)
	{
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return (pde->executable);
	}

	/**
	 * @brief Sets/clears the user bit of a page.
	 *
	 * @param pde Page directory entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_user_set(struct pde *pde, int set)
	{
		/* Invalid PTE. */
		if (pde == NULL)
			return (-EINVAL);

		pde->user = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the user bit of a page.
	 *
	 * @param pde Page directory entry of target page.
	 *
	 * @returns If the user bit of the target page, non zero is
	 * returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_is_user(struct pde *pde)
	{
		/* Invalid PTE. */
		if (pde == NULL)
			return (-EINVAL);

		return (pde->user);
	}

	/**
	 * @brief Clears a page table entry.
	 *
	 * @param pte Target page table entry.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_clear(struct pte *pte)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		kmemset(pte, 0, PTE_SIZE);

		return (0);
	}

	/**
	 * @brief Sets/clears the present bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_present_set(struct pte *pte, int set)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		pte->valid = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the present bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 *
	 * @returns If the present bit of the target page, non zero is
	 * returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_is_present(struct pte *pte)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		return (pte->valid);
	}

	/**
	 * @brief Sets the frame of a page.
	 *
	 * @param pte   Page table entry of target page.
	 * @param frame Frame number.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_frame_set(struct pte *pte, frame_t frame)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		/* Invalid frame. */
		if (frame > ~(frame_t)((1 << (VADDR_BIT - PAGE_SHIFT)) - 1))
			return (-EINVAL);

		pte->frame = frame;

		return (0);
	}

	/**
	 * @brief Gets the frame linked to page.
	 *
	 * @param pte   Page table entry of target page.
	 *
	 * @returns Number of the frame that is linked to the target page.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline frame_t pte_frame_get(struct pte *pte)
	{
		return (pte->frame);
	}

	/**
	 * @brief Sets/clears the read bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_read_set(struct pte *pte, int set)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		pte->readable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the read bit of a page is set.
	 *
	 * @param pte Page table entry of target page.
	 *
	 * @returns If the read bit of the target page is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_is_read(struct pte *pte)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		return (pte->writable);
	}

	/**
	 * @brief Sets/clears the write bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_write_set(struct pte *pte, int set)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		/*
		 * Set read and write bits,
		 * otherwise the behavior is
		 * undefined.
		 */
		pte->readable = (set) ? 1 : 0;
		pte->writable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the write bit of a page is set.
	 *
	 * @param pte Page table entry of target page.
	 *
	 * @returns If the write bit of the target page is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_is_write(struct pte *pte)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		return (pte->writable);
	}

	/**
	 * @brief Sets/clears the exec bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_exec_set(struct pte *pte, int set)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		/*
		 * Set read and executable
		 * bits otherwise the behavior
		 * is undefined.
		 */
		pte->readable = (set) ? 1 : 0;
		pte->executable = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Sets/clears the user bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_user_set(struct pte *pte, int set)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		pte->user = (set) ? 1 : 0;

		return (0);
	}

	/**
	 * @brief Asserts if the user bit of a page.
	 *
	 * @param pte Page table entry of target page.
	 *
	 * @returns If the user bit of the target page, non zero is
	 * returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_is_user(struct pte *pte)
	{
		/* Invalid PTE. */
		if (pte == NULL)
			return (-EINVAL);

		return (pte->user);
	}

	/**
	 * @brief Gets the page table index of a page.
	 *
	 * @param vaddr Target virtual address.
	 *
	 * @returns Returns the page table index of the page that lies at
	 * address @p vaddr.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline unsigned pte_idx_get(vaddr_t vaddr)
	{
		return (RV32GC_PTE_IDX(RV32GC_VADDR(vaddr)));
	}

	/**
	 * @brief Gets the page directory index of a page.
	 *
	 * @param vaddr Target virtual address.
	 *
	 * @returns Returns the page directory index of the page that lies
	 * at address @p vaddr.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline unsigned pde_idx_get(vaddr_t vaddr)
	{
		return (RV32GC_PDE_IDX(RV32GC_VADDR(vaddr)));
	}

	/**
	 * @brief Gets a page directory entry.
	 *
	 * @param pgdir Target page directory.
	 * @param vaddr  Target virtual address.
	 *
	 * @returns The requested page directory entry.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline struct pde *pde_get(struct pde *pgdir, vaddr_t vaddr)
	{
		/* Invalid page directory. */
		if (pgdir == NULL)
			return (NULL);

		return (&pgdir[pde_idx_get(vaddr)]);
	}

	/**
	 * @brief Gets a page table entry.
	 *
	 * @param pgtab Target page table.
	 * @param vaddr Target virtual address.
	 *
	 * @returns The requested page table entry.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline struct pte *pte_get(struct pte *pgtab, vaddr_t vaddr)
	{
		/* Invalid page table. */
		if (pgtab == NULL)
			return (NULL);

		return (&pgtab[pte_idx_get(vaddr)]);
	}

	/**
	 * @brief Checks if the MMU is enabled.
	 *
	 * @returns A non-zero value if the MMU is enabled, and
	 * 0 otherwise.
	 *
	 * @FIXME: pgramatically get this.
	 */
	static inline int mmu_is_enabled(void)
	{
		return (0);
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_MMU_H_ */
