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

#include <arch/core/or1k/tlb.h>
#include <arch/core/or1k/tlb.h>
#include <nanvix/const.h>
#include <posix/errno.h>

/**
 * @brief TLB Entry Value
 *
 * Converts between a structure value and a single value
 * writable into xTLBMR and xTLBTR.
 */
struct tlbe_value
{
	union
	{
		/* TLB Entry. */
		struct tlbe tlbe;

		/* TLB Value. */
		uint64_t value;
	} u;
} PACK;

/**
 * TLBE xTLBMR
 */
#define OR1K_TLBE_xTLBMR(x) ((x) >> 32)

/**
 * TLBE xTLBTR
 */
#define OR1K_TLBE_xTLBTR(x) ((x) & 0xFFFFFFFF)

/**
 * @brief Execution flags
 */
/**@{*/
#define OR1K_KERNEL_EXEC_CODE OR1K_ITLBE_SXE /**< Kernel executes code segment         */
#define OR1K_KERNEL_EXEC_DATA 0              /**< Kernel executes data segment (error) */
#define OR1K_USER_EXEC_CODE   OR1K_ITLBE_UXE /**< User executes code segment           */
#define OR1K_USER_EXEC_DATA   0              /**< User executes data segment (error)   */
/**@}*/

/**
 * @brief Read flags
 */
/**@{*/
#define OR1K_KERNEL_READ_CODE (OR1K_DTLBE_SRE)                                                    /**< Kernel reads code segment */
#define OR1K_KERNEL_READ_DATA (OR1K_DTLBE_SRE | OR1K_DTLBE_SwE)                                   /**< Kernel reads data segment */
#define OR1K_USER_READ_CODE   (OR1K_DTLBE_URE | OR1K_DTLBE_SRE | OR1K_DTLBE_SwE)                  /**< User reads code segment   */
#define OR1K_USER_READ_DATA   (OR1K_DTLBE_URE | OR1K_DTLBE_UWE | OR1K_DTLBE_SRE | OR1K_DTLBE_SwE) /**< User reads data segment   */
/**@}*/

/*============================================================================*
 * or1k_tlbe_update()                                                         *
 *============================================================================*/

/**
 * The or1k_tlbe_update() function updates the HW TLB using the
 * given parameters.
 *
 * @author João Vicente Souto
 */
PUBLIC void or1k_tlbe_update(int tlb_type, int idx, const struct tlbe *tlbe)
{
	struct tlbe_value tlbev; /* TLB Entry value.   */

	kmemcpy(&tlbev, tlbe, OR1K_TLBE_SIZE);

	if (tlb_type == OR1K_TLB_INSTRUCTION)
	{
		/* Copy to HW TLB. */
		or1k_mtspr(OR1K_SPR_ITLBTR_BASE(0) | idx, OR1K_TLBE_xTLBTR(tlbev.u.value));
		or1k_mtspr(OR1K_SPR_ITLBMR_BASE(0) | idx, OR1K_TLBE_xTLBMR(tlbev.u.value));
	}
	else
	{
		/* Copy to HW TLB. */
		or1k_mtspr(OR1K_SPR_DTLBTR_BASE(0) | idx, OR1K_TLBE_xTLBTR(tlbev.u.value));
		or1k_mtspr(OR1K_SPR_DTLBMR_BASE(0) | idx, OR1K_TLBE_xTLBMR(tlbev.u.value));
	}
}

/*============================================================================*
 * or1k_tlbe_write()                                                           *
 *============================================================================*/

/**
 * The or1k_tlbe_write() function writes an entry into the architectural
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
PUBLIC int or1k_tlbe_write(
	struct tlbe *tlbe,
	int tlb_type,
	vaddr_t vaddr,
	paddr_t paddr,
	int user,
	int inst
)
{
	unsigned idx;      /* TLB Index.           */
	struct tlbe _tlbe; /* Temporary TLB Entry. */

	/* Invalid TLB entry. */
	if (tlbe == NULL)
		return (-EINVAL);

	/* Address belonging to the kernel. */
	if (!user)
	{
		if (tlb_type == OR1K_TLB_INSTRUCTION)
			_tlbe.perms = (inst) ? OR1K_KERNEL_EXEC_CODE : OR1K_KERNEL_EXEC_DATA;
		else
			_tlbe.perms = (inst) ? OR1K_KERNEL_READ_CODE : OR1K_KERNEL_READ_DATA;
	}

	/* Address belonging to the user. */
	else
	{
		if (tlb_type == OR1K_TLB_INSTRUCTION)
			_tlbe.perms = (inst) ? OR1K_USER_EXEC_CODE : OR1K_USER_EXEC_DATA;
		else
			_tlbe.perms = (inst) ? OR1K_USER_READ_CODE : OR1K_USER_READ_DATA;
	}

	/* Remaining fields. */
	_tlbe.vpn = vaddr >> PAGE_SHIFT;
	_tlbe.lru = 0;
	_tlbe.cid = 0;
	_tlbe.pl = OR1K_TLBE_PL2;
	_tlbe.valid = OR1K_TLBE_VALID;

	_tlbe.ppn = paddr >> PAGE_SHIFT;
	_tlbe.dirty = !OR1K_TLBE_DIRTY;
	_tlbe.accessed = !OR1K_TLBE_ACCESSED;
	_tlbe.wom = OR1K_TLBE_MEMORY_MODEL_STRONG;
	_tlbe.wbc = OR1K_TLBE_CACHE_POLICY_WRBACK;
	_tlbe.ci = !OR1K_TLBE_CACHE_INHIBIT;
	_tlbe.cc = OR1K_TLBE_CACHE_COHERENCY;

	/* TLB index. */
	idx = or1k_tlbe_get_index(vaddr);

	or1k_tlbe_update(tlb_type, idx, &_tlbe);

	kmemcpy(tlbe, &_tlbe, OR1K_TLBE_SIZE);

	return (0);
}

PUBLIC void or1k_tlbe_init(
	int idx,
	unsigned dtlbtr,
	unsigned itlbtr,
	unsigned xtlbmr,
	struct tlbe *dtlb,
	struct tlbe *itlb
)
{
	struct tlbe_value tlbev;

	or1k_mtspr(OR1K_SPR_DTLBTR_BASE(0) | idx, dtlbtr);
	or1k_mtspr(OR1K_SPR_ITLBTR_BASE(0) | idx, itlbtr);
	or1k_mtspr(OR1K_SPR_DTLBMR_BASE(0) | idx, xtlbmr);
	or1k_mtspr(OR1K_SPR_ITLBMR_BASE(0) | idx, xtlbmr);

	tlbev.u.value = ((uint64_t)xtlbmr << 32) | dtlbtr;
	kmemcpy(dtlb, &tlbev, OR1K_TLBE_SIZE);

	tlbev.u.value = ((uint64_t)xtlbmr << 32) | itlbtr;
	kmemcpy(itlb, &tlbev, OR1K_TLBE_SIZE);
}
