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

#include <arch/core/or1k/tlb.h>
#include <arch/core/or1k/tlb.h>
#include <nanvix/const.h>
#include <errno.h>

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
	int tlb_type,
	int user,
	int inst,
	vaddr_t vaddr,
	paddr_t paddr,
	struct tlbe *tlbe
)
{
	unsigned idx; /* TLB Index.         */

	kmemset(tlbe, 0, OR1K_TLBE_SIZE);

	/* Address belonging to the kernel. */
	if (!user)
	{
		if (tlb_type == OR1K_TLB_INSTRUCTION)
		{
			if (inst)
				tlbe->perms = OR1K_ITLBE_SXE;

			/*
			 * Kernel trying to execute data segments
			 * should leads to error.
			 */
			else
				tlbe->perms = 0;
		}

		else
		{
			/*
			 * Kernel trying to read code segment.
			 */
			if (inst)
				tlbe->perms = OR1K_DTLBE_SRE;

			else
				tlbe->perms = OR1K_DTLBE_SRE | OR1K_DTLBE_SwE;
		}

	}

	/* Address belonging to the user. */
	else
	{
		if (tlb_type == OR1K_TLB_INSTRUCTION)
		{
			if (inst)
				tlbe->perms = OR1K_ITLBE_UXE;

			/*
			 * User trying to execute data segments
			 * should leads to error.
			 */
			else
				tlbe->perms = 0;
		}

		else
		{
			/*
			 * User trying to read code segment.
			 * Kernel always have permissions to R/W user data.
			 */
			if (inst)
				tlbe->perms = OR1K_DTLBE_URE | OR1K_DTLBE_SRE |
					OR1K_DTLBE_SwE;

			else
				tlbe->perms = OR1K_DTLBE_URE | OR1K_DTLBE_UWE |
					OR1K_DTLBE_SRE | OR1K_DTLBE_SwE;
		}
	}

	/* Remaining fields. */
	tlbe->vpn = vaddr >> PAGE_SHIFT;
	tlbe->lru = 0;
	tlbe->cid = 0;
	tlbe->pl = OR1K_TLBE_PL2;
	tlbe->valid = OR1K_TLBE_VALID;

	tlbe->ppn = paddr >> PAGE_SHIFT;
	tlbe->dirty = !OR1K_TLBE_DIRTY;
	tlbe->accessed = !OR1K_TLBE_ACCESSED;
	tlbe->wom = OR1K_TLBE_MEMORY_MODEL_STRONG;
	tlbe->wbc = OR1K_TLBE_CACHE_POLICY_WRBACK;
	tlbe->ci = !OR1K_TLBE_CACHE_INHIBIT;
	tlbe->cc = OR1K_TLBE_CACHE_COHERENCY;

	/* TLB index. */
	idx = (vaddr >> PAGE_SHIFT) & (OR1K_TLB_LENGTH - 1);

	or1k_tlbe_update(tlb_type, idx, tlbe);

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
