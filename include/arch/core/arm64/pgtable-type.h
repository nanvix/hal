
#ifndef __ASM_PGTABLE_TYPES_H
#define __ASM_PGTABLE_TYPES_H

    #define __NEED_CORE_TYPES
    #include <arch/core/arm64/types.h>

    typedef arm64_word_t pteval_t;
    typedef arm64_word_t pmdval_t;
    typedef arm64_word_t pdeval_t;

    /*
    * These are used to make use of C type-checking..
    */
    typedef struct { pteval_t pte; } pte_t;
    #define pte_val(x)	((x).pte)
    #define __pte(x)	((pte_t) { (x) } )
    /*Get index value of page table entry*/
    #define pte_index(addr)     (((addr) >> ARM64_PAGE_SHIFT) & (PTRS_PER_PTE - 1))

    typedef struct { pmdval_t pmd; } pmd_t;
    #define pmd_val(x)	((x).pmd)
    #define __pmd(x)	((pmd_t) { (x) } )
    /*Get index value of page table entry*/
    #define pmd_index(addr)     (((addr) >> ARM64_PMD_SHIFT) & (PTRS_PER_PMD - 1))

    typedef struct { pdeval_t pde; } pde_t;
    #define pde_val(x)	((x).pde)
    #define __pde(x)	((pde_t) { (x) } )
    /*Get index value of page table entry*/
    #define pgd_index(addr)     (((addr) >> ARM64_PGDIR_SHIFT) & (PTRS_PER_PDE - 1))

    typedef struct { pteval_t pgprot; } pgprot_t;
    #define pgprot_val(x)	((x).pgprot)
    #define __pgprot(x)	((pgprot_t) { (x) } )


    #define __pte_to_phys(pte)	(pte_val(pte) & PTE_ADDR_MASK)
    #define __phys_to_pte_val(phys)	(phys)

    #define pte_pfn(pte)		(__pte_to_phys(pte) >> PAGE_SHIFT)
    #define pfn_pte(pfn,prot)	\
        __pte(__phys_to_pte_val((phys_addr_t)(pfn) << PAGE_SHIFT) | pgprot_val(prot))

    #define pte_none(pte)		(!pte_val(pte))
    #define pte_clear(mm,addr,ptep)	set_pte(ptep, __pte(0))

    /*
    * The following only work if pte_present(). Undefined behaviour otherwise.
    */
    #define pte_present(pte)	(!!(pte_val(pte) & (PTE_VALID | PTE_PROT_NONE)))
    #define pte_young(pte)		(!!(pte_val(pte) & PTE_AF))
    #define pte_special(pte)	(!!(pte_val(pte) & PTE_SPECIAL))
    #define pte_write(pte)		(!!(pte_val(pte) & PTE_WRITE))
    #define pte_user_exec(pte)	(!(pte_val(pte) & PTE_UXN))
    #define pte_cont(pte)		(!!(pte_val(pte) & PTE_CONT))
    #define pte_devmap(pte)		(!!(pte_val(pte) & PTE_DEVMAP))
    #define pte_tagged(pte)		((pte_val(pte) & PTE_ATTRINDX_MASK) == \
                    PTE_ATTRINDX(MT_NORMAL_TAGGED))


    #define pmd_none(pmd)       (!pmd_val(pmd))
    #define pmd_present(pmd)    (pmd_val(pmd))
    #define pmd_bad(pmd)        (!(pmd_val(pmd) & 2))

    #define pde_none(pde)       (!pud_val(pde))
    #define pde_bad(pde)        (!(pud_val(pde) & 2))
    #define pde_present(pde)    (pud_val(pde))


    static inline void set_pte(pte_t *ptep, pte_t pte) {
       *ptep = pte;
    }

#endif	/* __ASM_PGTABLE_TYPES_H */
