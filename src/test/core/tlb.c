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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>
#include "../test.h"

/**
 * @brief Run destructive tests?
 */
#define TEST_TLB_DESTRUCTIVE 1

/**
 * @brief Launch verbose tests?
 */
#define TEST_TLB_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Lookup a Virtual Address                                                   *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Lookup a Virtual Address
 */
PRIVATE void test_tlb_lookup_vaddr(void)
{
	vaddr_t vaddr;

	vaddr = TRUNCATE(VADDR(test_tlb_lookup_vaddr), KPAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_vaddr() vaddr = %x", vaddr);
#endif

	KASSERT(tlb_lookup_vaddr(TLB_INSTRUCTION, vaddr) != NULL);
}

/*----------------------------------------------------------------------------*
 * Lookup a Physical Address                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Lookup a Physical Address
 */
PRIVATE void test_tlb_lookup_paddr(void)
{
	paddr_t paddr;

	paddr = TRUNCATE(PADDR(test_tlb_lookup_paddr), KPAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_paddr() paddr = %x", paddr);
#endif

	KASSERT(tlb_lookup_paddr(TLB_INSTRUCTION, paddr) != NULL);
}

/*----------------------------------------------------------------------------*
 * Write an Entry to the TLB (nondestructive)                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Write an Entry to the TLB (nondestructive)
 */
PRIVATE void test_tlb_write(void)
{
	vaddr_t vaddr;
	paddr_t paddr;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);
	paddr = TRUNCATE(PADDR(UBASE_PHYS) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_write() vaddr = %x, paddr = %x", vaddr, paddr);
#endif

	/* Write TLB entry. */
	KASSERT(tlb_write(TLB_DATA, vaddr, paddr) == 0);
	KASSERT(tlb_flush() == 0);

	/*
	 * TLB entry will be invalidated in
	 * test_tlb_invalidate().
	 */
}

/*----------------------------------------------------------------------------*
 * Invalidate an Entry of the TLB                                             *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Invalidate an Entry of the TLB
 */
PRIVATE void test_tlb_invalidate(void)
{
	vaddr_t vaddr;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_inval() vaddr = %x", vaddr);
#endif

	/* This entry should exist. */
	KASSERT(tlb_lookup_vaddr(TLB_DATA, vaddr) != NULL);

	/* Invalidate TLB entry. */
	KASSERT(tlb_inval(TLB_DATA, vaddr) == 0);
	KASSERT(tlb_flush() == 0);

	/* This entry should no longer exist. */
	KASSERT(tlb_lookup_vaddr(TLB_DATA, vaddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Write an Entry to the TLB (destructive)                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Write an Entry to the TLB (destructive)
 */
PRIVATE void test_tlb_write_destructive(void)
{
	vaddr_t vaddr;
	paddr_t paddr;

	/* Don't run this test. */
	if (!TEST_TLB_DESTRUCTIVE)
		return;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);
	paddr = TRUNCATE(PADDR(UBASE_PHYS) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_write() vaddr = %x, paddr = %x", vaddr, paddr);
#endif

	/* Write TLB entry. */
	KASSERT(tlb_write(TLB_DATA, vaddr, paddr) == 0);
	KASSERT(tlb_flush() == 0);

	/* This entry should exist. */
	KASSERT(tlb_lookup_vaddr(TLB_DATA, vaddr) != NULL);

	/* Write to the page. */
	kmemset((void *) vaddr, -1, PAGE_SIZE);

	/* Invalidate TLB entry. */
	KASSERT(tlb_inval(TLB_DATA, vaddr) == 0);
	KASSERT(tlb_flush() == 0);

	/* This entry should no longer exist. */
	KASSERT(tlb_lookup_vaddr(TLB_DATA, vaddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test tlb_api_tests[] = {
	{ test_tlb_lookup_vaddr,      "lookup virtual address"  },
	{ test_tlb_lookup_paddr,      "lookup physical address" },
	{ test_tlb_write,             "write"                   },
	{ test_tlb_invalidate,        "invalidate"              },
	{ test_tlb_write_destructive, "write destructive"       },
	{ NULL,                        NULL                     },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Lookup a Virtual Address in Invalid TLB Type                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Lookup a Virtual Address in Invalid TLB Type
 */
PRIVATE void test_tlb_lookup_vaddr_tlb_type_inval(void)
{
	vaddr_t vaddr;

	vaddr = VADDR(KBASE_VIRT);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_vaddr() vaddr = %x", vaddr);
#endif

	KASSERT(tlb_lookup_vaddr(-1, vaddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Lookup an Bad Virtual Address                                              *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection Test: Lookup Bad Virtual Address
 */
PRIVATE void test_tlb_lookup_vaddr_bad(void)
{
	vaddr_t vaddr;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_vaddr() vaddr = %x", vaddr);
#endif

	KASSERT(tlb_lookup_vaddr(TLB_INSTRUCTION, vaddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Lookup a Physical Address in Invalid TLB Type                              *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Lookup a Physical Address in Invalid TLB Type
 */
PRIVATE void test_tlb_lookup_paddr_tlb_type_inval(void)
{
	paddr_t paddr;

	paddr = PADDR(KBASE_PHYS);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_paddr() paddr = %x", paddr);
#endif

	KASSERT(tlb_lookup_paddr(-1, paddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Lookup an Bad Physical Address                                             *
 *----------------------------------------------------------------------------*/

/**
 * Fault Injection Test: Lookup Bad Physical Address
 */
PRIVATE void test_tlb_lookup_paddr_bad(void)
{
	paddr_t paddr;

	paddr = TRUNCATE(VADDR(UBASE_PHYS) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_lookup_paddr() paddr = %x", paddr);
#endif

	KASSERT(tlb_lookup_paddr(TLB_INSTRUCTION, paddr) == NULL);
}

/*----------------------------------------------------------------------------*
 * Write an Invalid TLB Entry                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Write an Invalid TLB Entry
 */
PRIVATE void test_tlb_write_inval(void)
{
	vaddr_t vaddr;
	paddr_t paddr;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);
	paddr = TRUNCATE(PADDR(UBASE_PHYS) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_write() vaddr = %x, paddr = %x", vaddr, paddr);
#endif

	/* Write TLB entry. */
	KASSERT(tlb_write(-1, vaddr, paddr) == -EINVAL);
	KASSERT(tlb_flush() == 0);

	/*
	 * TLB entry will be invalidated in
	 * test_tlb_invalidate().
	 */
}

/*----------------------------------------------------------------------------*
 * Invalidate an Invalid TLB Entry                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Invalidate an Invalid TLB Entry
 */
PRIVATE void test_tlb_invalidate_inval(void)
{
	vaddr_t vaddr;

	vaddr = TRUNCATE(VADDR(UBASE_VIRT) + PAGE_SIZE, PAGE_SIZE);

#if (TEST_TLB_VERBOSE)
	kprintf("tlb_write() vaddr = %x, paddr = %x", vaddr, paddr);
#endif

	/* Write TLB entry. */
	KASSERT(tlb_inval(-1, vaddr) == -EINVAL);
	KASSERT(tlb_flush() == 0);

	/*
	 * TLB entry will be invalidated in
	 * test_tlb_invalidate().
	 */
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test tlb_fault_tests[] = {
	{ test_tlb_lookup_vaddr_tlb_type_inval, "lookup invalid virtual address"  },
	{ test_tlb_lookup_vaddr_bad,            "lookup bad virtual address"      },
	{ test_tlb_lookup_paddr_tlb_type_inval, "lookup invalid physical address" },
	{ test_tlb_lookup_paddr_bad,            "lookup bad physical address"     },
	{ test_tlb_write_inval,                 "write invalid entry"             },
	{ test_tlb_invalidate_inval,            "invalidate invalid entry"        },
	{ NULL,                                  NULL                             },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_tlb() function launches testing units on the
 * TLB Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_tlb(void)
{
	/* Test not applicable. */
#if (CORE_HAS_TLB_HW)
		return;
#endif

	for (int i = 0; tlb_api_tests[i].test_fn != NULL; i++)
	{
		tlb_api_tests[i].test_fn();
		kprintf("[test][api][tlb] %s [passed]", tlb_api_tests[i].name);
	}

	for (int i = 0; tlb_fault_tests[i].test_fn != NULL; i++)
	{
		tlb_fault_tests[i].test_fn();
		kprintf("[test][fault][tlb] %s [passed]", tlb_fault_tests[i].name);
	}
}

