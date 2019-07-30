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

/* Must come first. */
#define __NEED_HAL_CLUSTER

#include <nanvix/hal/cluster.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

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

	tlbe = &utlb[tlbe_get_index(addr)];

	/* Found. */
	if (tlbe_vaddr_get(tlbe) == addr)
	{
		if (tlbe_is_valid(tlbe))
			return (tlbe);
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
 * tlb_inval()                                                   *
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

#endif /* !CORE_HAS_TLB_HW */
