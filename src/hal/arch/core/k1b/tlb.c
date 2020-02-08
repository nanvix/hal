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

#include <arch/core/k1b/tlb.h>
#include <nanvix/const.h>
#include <posix/errno.h>

/*============================================================================*
 * k1b_tlb_write()                                                            *
 *============================================================================*/

/**
 * The k1b_tlbe_write() function writes an entry into the architectural
 * TLB. If the new entry conflicts to an old one, the old one is
 * overwritten.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_tlbe_write(
		struct tlbe *tlbe,
		vaddr_t vaddr,
		paddr_t paddr,
		unsigned shift,
		unsigned way,
		unsigned protection
)
{
	struct tlbe _tlbe;      /**< Temporary tlbe.  */
	__k1_tlb_entry_t utlbe; /**< Underlying tlbe. */

	/* Invalid TLB entry. */
	if (tlbe == NULL)
		return (-EINVAL);

	_tlbe.addr_ext = 0;
	_tlbe.addrspace = 0;
	_tlbe.cache_policy = K1B_DTLBE_CACHE_POLICY_WRTHROUGH;
	_tlbe.frame = paddr >> 12;
	_tlbe.global = 1;
	_tlbe.page = (vaddr >> 12) | (1 << (shift - 12 - 1));
	_tlbe.protection = protection;
	_tlbe.size = (shift == 12) ? 1 : 0;
	_tlbe.status = K1B_TLBE_STATUS_AMODIFIED;

	kmemcpy(&utlbe, &_tlbe, K1B_TLBE_SIZE);

	/* Write to hardware TLB. */
	if (mOS_mem_write_jtlb(utlbe, way) != 0)
	{
		kprintf("[hal] failed to write tlb %x", vaddr);
		return (-EAGAIN);
	}

	kmemcpy(tlbe, &_tlbe, K1B_TLBE_SIZE);

	return (0);
}

/**
 * The k1b_tlbe_inval() function invalidates the TLB entry that
 * encodes the virtual address @p vaddr.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_tlbe_inval(
	struct tlbe *tlbe,
	vaddr_t vaddr,
	unsigned shift,
	unsigned way
)
{
	struct tlbe _tlbe;      /**< Temporary tlbe.  */
	__k1_tlb_entry_t utlbe; /**< Underlying tlbe. */

	/* Invalid TLB entry. */
	if (tlbe == NULL)
		return (-EINVAL);

	_tlbe.addr_ext = 0;
	_tlbe.addrspace = 0;
	_tlbe.cache_policy = 0;
	_tlbe.frame = 0;
	_tlbe.global = 0;
	_tlbe.page = (vaddr >> 12) | (1 << (shift - 12 - 1));
	_tlbe.protection = 0;
	_tlbe.size = (shift == 12) ? 1 : 0;
	_tlbe.status = K1B_TLBE_STATUS_INVALID;

	kmemcpy(&utlbe, &_tlbe, K1B_TLBE_SIZE);

	/* Write to hardware TLB. */
	if (mOS_mem_write_jtlb(utlbe, way) != 0)
	{
		kprintf("[hal] failed to invalidate tlb %x", vaddr);
		return (-EAGAIN);
	}

	kmemcpy(tlbe, &_tlbe, K1B_TLBE_SIZE);

	return (0);
}

/**
 * The k1b_tlbe_shootdown() function invalidates the TLB entry that
 * encodes the virtual address @p vaddr in all cores.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_tlbe_shootdown(
	struct tlbe *tlbe,
	vaddr_t vaddr,
	unsigned shift
)
{
	struct tlbe _tlbe;      /* Temporary tlbe.    */
	uint32_t inval_list;    /* Invalidation list. */
	__k1_tlb_entry_t utlbe; /* Underlying tlbe.   */

	/* Invalid TLB entry. */
	if (tlbe == NULL)
		return (-EINVAL);

	_tlbe.addr_ext = 0;
	_tlbe.addrspace = 0;
	_tlbe.cache_policy = 0;
	_tlbe.frame = 0;
	_tlbe.global = 0;
	_tlbe.page = (vaddr >> 12) | (1 << (shift - 12 - 1));
	_tlbe.protection = 0;
	_tlbe.size = (shift == 12) ? 1 : 0;
	_tlbe.status = K1B_TLBE_STATUS_INVALID;

#if defined(__k1bdp__)
	inval_list = 0xffff;
#else
	inval_list = 0xf;
#endif

	kmemcpy(&utlbe, &_tlbe, K1B_TLBE_SIZE);

	/* Write to hardware TLB. */
	if (mOS_mem_shootdown_jtlb(utlbe, inval_list) != 0)
	{
		kprintf("[hal] failed to shootdown tlb %x", vaddr);
		return (-EAGAIN);
	}

	kmemcpy(tlbe, &_tlbe, K1B_TLBE_SIZE);

	return (0);
}
