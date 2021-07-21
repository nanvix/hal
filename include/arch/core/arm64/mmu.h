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
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES


	#include <arch/core/arm64/types.h>
	#include <nanvix/hlib.h>
	#include <posix/errno.h>


	/**
	 * Support 64K granule and 39 VA_BITS, and 2 translation level
	 * With 64KB pages, there are 2 levels of page tables. Each level has
	 * 8192 entries of 8 bytes each, occupying a 64KB page. Levels 0 and 1 are not
	 * used. The 2nd level table (PGD for Linux) can cover a range of 4TB, each
	 * entry representing 512MB. The user and kernel address spaces are limited to
	 * 4TB in the 64KB page configuration.
 	*/
	#define CONFIG_PGTABLE_LEVELS	2

	/**
	 * @name Page Shifts and Masks
	 */
	/**@{*/
	#define ARM64_PAGE_SHIFT  		16                          /**< Page Shift        */
	#define ARM64_PGTAB_SHIFT 		29                          /**< Page Table Shift  */
	#define ARM64_PAGE_MASK   		(~(ARM64_PAGE_SIZE - 1))  	/**< Page Mask         */
	#define ARM64_PGTAB_MASK  		(~(ARM64_PGTAB_SIZE - 1)) 	/**< Page Table Mask   */
	/**@}*/

	/**
	 * @name Size of Pages and Page Tables
	 */
	/**@{*/
	#define ARM64_HUGE_PAGE_SIZE 	(1 << ARM64_PAGE_SHIFT)  	/**< Huge Page Size                       */
	#define ARM64_PAGE_SIZE      	(1 << ARM64_PAGE_SHIFT)  	/**< Page Size                            */
	#define ARM64_PGTAB_SIZE     	(1 << ARM64_PGTAB_SHIFT) 	/**< Page Table Size                      */
	#define ARM64_PTE_SIZE        	8                           /**< Page Table Entry Size (in bytes)     */
	#define ARM64_PDE_SIZE        	8                           /**< Page Directory Entry Size (in bytes) */
	/**@}*/

	#define ARM64_VADDR_LENGTH 		ARM64_VADDR_BIT

	/**
	 * @brief Page Directory length.
	 *
	 * Number of Page Directory Entries (PDEs) per Page Directory.
	 */
	#define ARM64_PGDIR_LENGTH 		(1 << (ARM64_VADDR_LENGTH - ARM64_PGTAB_SHIFT))

	/**
	 * @brief Page Table length.
	 *
	 * Number of Page Table Entries (PTEs) per Page Table.
	 */
	#define ARM64_PGTAB_LENGTH 		(1 << (ARM64_PGTAB_SHIFT - ARM64_PAGE_SHIFT))


	#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
	#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
	#define SCTLR_I_CACHE_DISABLED          (0 << 12)
	#define SCTLR_I_CACHE_ENABLE          	(1 << 12)
	#define SCTLR_D_CACHE_DISABLED          (0 << 2)
	#define SCTLR_D_CACHE_ENABLE          	(1 << 2)
	#define SCTLR_MMU_DISABLED              (0 << 0)
	#define SCTLR_MMU_ENABLED               (1 << 0)

	#define SCTLR_VALUE_MMU_DISABLED    (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)
	#define SCTLR_VALUE_MMU_ENABLE		(SCTLR_MMU_ENABLED | SCTLR_D_CACHE_ENABLE | SCTLR_I_CACHE_ENABLE)


	/*
	Cachabilty for Outer/Inner memory
	*/

	#define c0 0UL //Normal memory, Non-cacheable
	#define c1 1UL //Normal memory, Write-Back Read-Allocate Write-Allocate Cacheable
	#define c2 2UL //Normal memory, Write-Through Read-Allocate NoWrite-Allocate Cacheable.
	#define c3 3UL //Normal memory, Write-Back Read-Allocate No Write-Allocate Cacheable

	/*
	* TCR flags.
	*/
	#define TCR_T0SZ			((64 - (ARM64_VADDR_BIT)) << 0)
	#define TCR_T1SZ			((64 - (ARM64_VADDR_BIT)) << 16)
	#define TCR_TxSZ			(TCR_T0SZ | TCR_T1SZ)

	#define TCR_IRGN0_WBWA		(c1 << 8)
	#define TCR_ORGN0_WBWA		(c1 << 10)

	#define TCR_IRGN1_WBWA		(c1 << 24)
	#define TCR_ORGN1_WBWA		(c1 << 26)

	/* PTWs cacheable, inner/outer WBWA */
	#define TCR_IRGN_WBWA		(TCR_IRGN0_WBWA | TCR_IRGN1_WBWA)
	#define TCR_ORGN_WBWA		(TCR_ORGN0_WBWA | TCR_ORGN1_WBWA)
	#define TCR_CACHE_FLAGS		TCR_IRGN_WBWA | TCR_ORGN_WBWA

	#define TCR_SH0_INNER		(c3 << 12)
	#define TCR_SH1_INNER		(c3 << 28)
	#define TCR_SHARED			(TCR_SH0_INNER | TCR_SH1_INNER)

	#define TCR_TG0_64K			(1UL << 14)
	#define TCR_TG1_64K			(3UL << 30)
	#define TCR_TG_FLAGS		TCR_TG0_64K | TCR_TG1_64K

	#define TCR_ASID16			(1UL << 36)
	#define TCR_TBI0			(1UL << 37)
	#define TCR_HA				(1UL << 39)
	#define TCR_HD				(1UL << 40)

	#define TCR_VALUE           (TCR_TxSZ | TCR_CACHE_FLAGS | TCR_SHARED | \
									TCR_TG_FLAGS | TCR_ASID16 | TCR_TBI0)


	/*
	* Memory types
	*/

	#define MT_DEVICE_NGNRNE	0
	#define MT_DEVICE_NGNRE		1
	#define MT_DEVICE_GRE		2
	#define MT_NORMAL_NC		3
	#define MT_NORMAL			4

	#define MAIR_VALUE	((0x0 << (MT_DEVICE_NGNRNE * 8)) |	\
					(0x04 << (MT_DEVICE_NGNRE * 8))   |	\
					(0x0c << (MT_DEVICE_GRE * 8))     |	\
					(0x44 << (MT_NORMAL_NC * 8))      |	\
					(0xffLL << (MT_NORMAL * 8)))


	#define MAIR_ATTR_SET(attr, index)	(attr << (index << 3))

	/*
	* ID_AA64MMFR0_EL1
	*/
	#define TG4 	(0xF << 28) // Not Supported
	#define TG16 	(0 << 20) 	// Not Supported
	#define TG64	(0 << 24)	// Supported
	#define SNSM	(1 << 12)	//Disctinction between Secure and non Secure
	#define BE		(1 << 8)	//Mixed Endian Support
	#define ASIDb	(2 << 4)	// 16 Bits
	#define PAr		(3 << 0)	// 42 bits

	#define ID_AA64MMFR0_EL1_VALUE \
		(TG4 | TG16 | TG64 | ASIDb | PAr | BE | SNSM)




#ifndef _ASM_FILE_
	/**
	 * @brief Page directory entry.
	 *
	* Hardware page table definitions.
	*
	* Level 1 descriptor (PDE).
	 */
	struct pde
	{
		unsigned present  	:  	1; 	/**< Present in memory? 			*/
		unsigned table		:	1;	/**< 1 -> table, 0 -> block			*/
		unsigned			:	14;	/**< Unused							*/
		unsigned frame 		: 	32; /**< Frame number.      			*/
		unsigned res0		:	4;	/**< Res0							*/
		unsigned			:	7;	/**< Unused							*/
		unsigned pxn		:	1;	/**< Privileged execute-never 		*/
		unsigned uxn		:	1;	/**< User execute-never				*/
		//The data access permission controls
		unsigned user		:	1;	/**< EL0 and EL1 control			*/
		unsigned rdonly		:	1;	/**< read-only and read/write acces */
		//Security state for subsequent levels of lookup
		unsigned ns			:	1;	/**< NSTable						*/
	} PACK;

	/**
	 * @brief Page table entry.
	 * Level 2 descriptor (PTE).
	*/
	struct pte
	{
		unsigned present  	:  	1; 	/**< @0 Present in memory? 				*/
		unsigned table		:	1;	/**< @1 0 -> makes entry invalid		*/
		unsigned type		:	4;	/**< @2-5 Memory Type					*/
		// access permissions bits
		unsigned user		:  	1; 	/**< @6 Data Access Permissions Ap[1] 	*/
		unsigned rdonly		:	1; 	/**< @7 Data Access Permissions AP[2]	*/
		// shareability field
		unsigned shared	 	:  	2; 	/**< @8-9 SH[1:0], inner shareable		*/
		unsigned accessed 	:  	1; 	/**< @10 AF, Accessed?          		*/
		unsigned ng			:	1;	/**< @11 Global Bit						*/
		unsigned			:	4;	/**< @12-15	Res0						*/
		unsigned frame 		: 	32; /**< @16-47	Frame number.      			*/
		unsigned 			:	4;	/**< @48-51	Res0						*/
		unsigned cont		:	1; 	/**< @52 Contiguous ?					*/
		unsigned pxn		:	1;	/**< @53 Privileged execute-never 		*/
		unsigned uxn		:	1;	/**< @54 User execute-never				*/
		unsigned dirty      : 	1; 	/**< @55 Dirty.            				*/
		unsigned			: 	8;	/**< @56-63	Unused.						*/
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
	#define KPAGE_SIZE   ARM64_PAGE_SIZE 	/**< @ref ARM64_HUGE_PAGE_SIZE */
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
	EXTERN int arm64_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x);

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
	EXTERN int arm64_huge_page_map(struct pte *pgtab, paddr_t paddr, vaddr_t vaddr, int w, int x);

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
	EXTERN int arm64_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr);

	EXTERN int arm64_enable_mmu();
	EXTERN void arm64_mmu_setup();
	EXTERN int arm64_invalidate_d_cache();
	EXTERN int arm64_invalidate_tlb();

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

		kmemset(pde, 0, PDE_SIZE);

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
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

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
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return 1;
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

		pde->rdonly = (set) ? 0 : 1;

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

		return (!pde->rdonly);
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
	 * @brief Sets/clears the exec bit of a page table.
	 *
	 * @param pde Page directory entry of target page table.
	 * @param set Set bit?
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline int pde_exec_set(struct pde *pde, int set)
	{
		/* If the uxn field is set to 0, then the instruction execution is granted.	*/
		pde->uxn = ~set;
		pde->pxn = ~set;

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
		/* Invalid PDE. */
		if (pde == NULL)
			return (-EINVAL);

		return pde->uxn;
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

		pte->user = (set) ? 1: 0;

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

		pte->rdonly = set;

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
		return (pte->rdonly);
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

		pte->rdonly = ~set;

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

		return (!pte->rdonly);
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

		/* If the uxn field is set to 0, then the instruction execution is granted.	*/
		pte->uxn = ~set;
		pte->pxn = ~set;

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
		/* If the uxn field is set to 0, then the instruction execution is granted. */
		return ~pte->uxn;
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
		return (arm64_page_map(pgtab, paddr, vaddr, w, x));
	}

	/**
	 * @see arm64_huge_page_map().
	 */
	static inline int mmu_huge_page_map(struct pte *pgdir, paddr_t paddr, vaddr_t vaddr, int w, int x)
	{
		return (arm64_huge_page_map(pgdir, paddr, vaddr, w, x));
	}

	/**
	 * @see arm64_pgtab_map().
	 */
	static inline int mmu_pgtab_map(struct pde *pgdir, paddr_t paddr, vaddr_t vaddr)
	{
		return (arm64_pgtab_map(pgdir, paddr, vaddr));
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
		uint32_t sctlr;

		__asm__ __volatile__("mrs %0, SCTLR_EL1\n\t" : "=r" (sctlr) :  : "memory");

		return (sctlr & SCTLR_MMU_ENABLED);
	}

	static inline int arm64_disable_mmu(void)
	{

		__asm__ __volatile__("msr SCTLR_EL1, %0\n\t" : : "r" (SCTLR_VALUE_MMU_DISABLED) : "memory");

		return (0);
	}

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the MMU.
	 */
	static inline void mmu_setup(void)
	{
		arm64_mmu_setup();
	}

#endif /* __NANVIX_HAL */

#endif /* _ASM_FILE_*/

/**@endcond*/

#endif /* ARCH_CORE_ARM64_MMU_H_ */
