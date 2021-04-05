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

#ifndef ARCH_CORE_ARM64_MMU_H_
#define ARCH_CORE_ARM64_MMU_H_

/**
 * @addtogroup arm64-core-mmu MMU
 * @ingroup arm64-core
 *
 * @brief Memory Management Unit
 */
/**@{*/

	/* Must comme first. */
	#define __NEED_MEMORY_TYPES

	#include <arch/core/arm64/types.h>
	#include <nanvix/hlib.h>
	#include <posix/errno.h>

#ifndef _ASM_FILE_

	/**
	 * @name Page Shifts and Masks
	 */
	/**@{*/
	#define ARM64_PAGE_SHIFT  16                          /**< Page Shift        */
	#define ARM64_PGTAB_SHIFT 24                          /**< Page Table Shift  */
	#define ARM64_PAGE_MASK   (~(ARM64_PAGE_SIZE - 1))  /**< Page Mask         */
	#define ARM64_PGTAB_MASK  (~(ARM64_PGTAB_SIZE - 1)) /**< Page Table Mask   */
	/**@}*/

	/**
	 * @name Size of Pages and Page Tables
	 */
	/**@{*/
	#define ARM64_HUGE_PAGE_SIZE (0x1UL << ARM64_PAGE_SHIFT)  /**< Huge Page Size                       */
	#define ARM64_PAGE_SIZE      (0x1UL << ARM64_PAGE_SHIFT)  /**< Page Size                            */
	#define ARM64_PGTAB_SIZE     (0x1UL << ARM64_PGTAB_SHIFT) /**< Page Table Size                      */
	#define ARM64_PTE_SIZE        6                             /**< Page Table Entry Size (in bytes)     */
	#define ARM64_PDE_SIZE        6                             /**< Page Directory Entry Size (in bytes) */
	/**@}*/

	#define ARM64_VADDR_LENGTH 48

	#define ARM64_PGDIR_LENGTH (1 << (ARM64_VADDR_LENGTH - ARM64_PGTAB_SHIFT))

	#define ARM64_PGTAB_LENGTH (1 << (ARM64_PGTAB_SHIFT - ARM64_PAGE_SHIFT))


	/**
	 * @brief Page directory entry.
	 */
	struct pde
	{
		unsigned present  :  1; /**< Present in memory? */
		unsigned writable :  1; /**< Writable page?     */
		unsigned user     :  1; /**< User page?         */
		unsigned accessed :  1; /**< Accessed?          */
		unsigned          : 12; /**< Unused.            */
		unsigned frame    : 32; /**< Frame number.      */
	} PACK;

	/**
	 * @brief Page table entry.
	 */
	struct pte
	{
		unsigned present  :  1; /**< Present in memory? */
		unsigned writable :  1; /**< Writable page?     */
		unsigned user     :  1; /**< User page?         */
		unsigned accessed :  1; /**< Accessed?          */
		unsigned          : 12; /**< Unused.            */
		unsigned frame    : 32; /**< Frame number.      */
	} PACK;

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define KPAGE_SIZE   ARM64_HUGE_PAGE_SIZE /**< @ref ARM64_HUGE_PAGE_SIZE */
	#define PAGE_SIZE    ARM64_PAGE_SIZE      /**< @ref ARM64_PAGE_SIZE      */
	#define PGTAB_SIZE   ARM64_PGTAB_SIZE     /**< @ref ARM64_PGTAB_SIZE     */
	#define PGTAB_LENGTH ARM64_PGTAB_LENGTH   /**< @ref ARM64_PGTAB_LENGTH   */
	#define PGDIR_LENGTH ARM64_PGDIR_LENGTH   /**< @ref ARM64_PGDIR_LENGTH   */
	#define PTE_SIZE     ARM64_PTE_SIZE       /**< @ref ARM64_PTE_SIZE       */
	#define PDE_SIZE     ARM64_PDE_SIZE       /**< @ref ARM64_PDE_SIZE       */
	#define PAGE_SHIFT   ARM64_PAGE_SHIFT     /**< @ref ARM64_PAGE_SHIFT     */
	#define PGTAB_SHIFT  ARM64_PGTAB_SHIFT    /**< @ref ARM64_PGTAB_SHIFT    */
	#define PAGE_MASK    ARM64_PAGE_MASK      /**< @ref ARM64_PAGE_MASK      */
	#define PGTAB_MASK   ARM64_PGTAB_MASK     /**< @ref ARM64_PGTAB_MASK     */
	#define PADDR_BIT    ARM64_PADDR_BIT      /**< @ref ARM64_PADDR_BIT      */
	#define VADDR_BIT    ARM64_VADDR_BIT      /**< @ref ARM64_VADDR_BIT      */
	#define PADDR_BYTE   ARM64_PADDR_BYTE     /**< @ref ARM64_PADDR_BYTE     */
	#define VADDR_BYTE   ARM64_VADDR_BYTE     /**< @ref ARM64_VADDR_BYTE     */
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
	#define __pde_clear_fn         /**< pde_clear()         */
	#define __pde_frame_get_fn     /**< pde_frame_get()     */
	#define __pde_frame_set_fn     /**< pde_frame_set()     */
	#define __pde_get_fn           /**< pde_get()           */
	#define __pde_is_present_fn    /**< pde_is_present()    */
	#define __pde_is_user_fn       /**< pde_is_user()       */
	#define __pde_is_read_fn       /**< pde_is_read()       */
	#define __pde_is_write_fn      /**< pde_is_write()      */
	#define __pde_is_exec_fn       /**< pde_is_exec()       */
	#define __pde_present_set_fn   /**< pde_present_set()   */
	#define __pde_user_set_fn      /**< pde_user_set()      */
	#define __pde_read_set_fn      /**< pde_read_set()      */
	#define __pde_write_set_fn     /**< pde_write_set()     */
	#define __pde_exec_set_fn      /**< pde_exec_set()      */
	#define __pte_clear_fn         /**< pte_clear()         */
	#define __pte_frame_get_fn     /**< pte_frame_get()     */
	#define __pte_frame_set_fn     /**< pte_frame_set()     */
	#define __pte_get_fn           /**< pte_get()           */
	#define __pte_is_present_fn    /**< pte_is_present()    */
	#define __pte_is_user_fn       /**< pte_is_user()       */
	#define __pte_is_read_fn       /**< pte_is_read()       */
	#define __pte_is_write_fn      /**< pte_is_write()      */
	#define __pte_is_exec_fn       /**< pte_is_exec()       */
	#define __pte_present_set_fn   /**< pte_present_set()   */
	#define __pte_user_set_fn      /**< pte_user_set()      */
	#define __pte_read_set_fn      /**< pte_read_set()      */
	#define __pte_write_set_fn     /**< pte_write_set()     */
	#define __pte_exec_set_fn      /**< pte_exec_set()      */
	#define __mmu_page_map_fn      /**< mmu_page_map()      */
	#define __mmu_huge_page_map_fn /**< mmu_huge_page_map() */
	#define __mmu_pgtab_map_fn     /**< mmu_pgtab_map()     */
	#define __mmu_is_enabled_fn    /**< mmu_is_enabled()    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Memory Types
	 */
	/**@{*/
	typedef arm64_paddr_t paddr_t; /**< Physical Address */
	typedef arm64_frame_t frame_t; /**< Frame Number     */
	typedef arm64_vaddr_t vaddr_t; /**< Virtual Address  */
	/**@}*/

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
		if (frame > ~(frame_t)((0X1UL << (VADDR_BIT - PAGE_SHIFT)) - 1))
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

		pde->present = (set) ? 1 : 0;

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

		return (pde->present);
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
		UNUSED(pde);
		UNUSED(set);
		return 0;
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
		UNUSED(pde);
		return 0;
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
		UNUSED(pde);
		UNUSED(set);
		return 0;
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
		UNUSED(pde);
		return 0;
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
		/* Invalid PDE. */
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
		/* Invalid PDE. */
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

		pte->present = (set) ? 1 : 0;

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

		return (pte->present);
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
		if (frame > ~(frame_t)((0X1UL << (VADDR_BIT - PAGE_SHIFT)) - 1))
			return (-EINVAL);

		pte->frame = frame;
		return 0;
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
		UNUSED(pte);
		UNUSED(set);

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
		UNUSED(pte);
		return 0;
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
		UNUSED(pte);
		UNUSED(set);
		return (0);
	}

	/**
	 * @brief Asserts if the exec bit of a page is set.
	 *
	 * @param pte Page table entry of target page.
	 *
	 * @returns If the exec bit of the target page is set, non
	 * zero is returned. Otherwise, zero is returned instead.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pte_is_exec(struct pte *pte)
	{
		UNUSED(pte);
		return 0;
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
	static inline unsigned pte_idx_get(arm64_vaddr_t vaddr)
	{
		return (((unsigned)(vaddr) & (ARM64_PGTAB_MASK^ARM64_PAGE_MASK)) >> ARM64_PAGE_SHIFT);
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
	static inline unsigned pde_idx_get(arm64_vaddr_t vaddr)
	{
		return ((vaddr) >> ARM64_PGTAB_SHIFT);
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
	static inline struct pde *pde_get(struct pde *pgdir, arm64_vaddr_t vaddr)
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
	static inline struct pte *pte_get(struct pte *pgtab, arm64_vaddr_t vaddr)
	{
		/* Invalid page table. */
		if (pgtab == NULL)
			return (NULL);

		return (&pgtab[pte_idx_get(vaddr)]);
	}

	/**
	 * @see arm64_page_map().
	 */
	static inline int mmu_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x)
	{
		UNUSED(pgtab);
		UNUSED(paddr);
		UNUSED(vaddr);
		UNUSED(w);
		UNUSED(x);
		return 0;
	}

	/**
	 * @see arm64_huge_page_map().
	 */
	static inline int mmu_huge_page_map(struct pte *pgdir, paddr_t paddr, vaddr_t vaddr, int w, int x)
	{
		UNUSED(pgdir);
		UNUSED(paddr);
		UNUSED(vaddr);
		UNUSED(w);
		UNUSED(x);
		return 0;
	}

	/**
	 * @see arm64_pgtab_map().
	 */
	static inline int mmu_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
	{
		UNUSED(pgdir);
		UNUSED(paddr);
		UNUSED(vaddr);
		return 0;
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

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the MMU.
	 */
	static inline void mmu_setup(void)
	{
		/* noop */
	}

#endif /* __NANVIX_HAL */

#endif

/**@endcond*/

#endif /* ARCH_CORE_ARM64_MMU_H_ */
