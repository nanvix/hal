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
#include "test.h"

/**
 * @brief Run destructive tests?
 */
#define TEST_MMU_DESTRUCTIVE 0

/**
 * @brief Launch verbose tests?
 */
#define TEST_MMU_VERBOSE 0

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Clear a PTE (nondestructive)                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Clear a PTE (nondestructive)
 */
PRIVATE void mmu_pte_clear(void)
{
	struct pte pte;

	kmemset(&pte, 1, sizeof(struct pte));
	pte_clear(&pte);
	KASSERT(
		(pte_frame_get(&pte) == 0) &&
		(pte_is_present(&pte) == 0)
	);
}

/*----------------------------------------------------------------------------*
 * Clear a PDE (nondestructive)                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Clear a PDE
 */
PRIVATE void mmu_pde_clear(void)
{
	struct pde pde;

	kmemset(&pde, 1, sizeof(struct pde));
	pde_clear(&pde);
	KASSERT((pde_frame_get(&pde) == 0) && (pde_is_present(&pde) == 0));
}

/*----------------------------------------------------------------------------*
 * Set a Frame of a PTE (nondestructive)                                      *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set a Frame of a PTE (nondestructive)
 */
PRIVATE void mmu_pte_frame_set(void)
{
	struct pte pte;
	frame_t frame = (1 << (VADDR_BIT - PAGE_SHIFT)) - 1;

	kmemset(&pte, 1, sizeof(struct pte));
	pte_clear(&pte);
	pte_frame_set(&pte, frame);

#if (TEST_MMU_VERBOSE)
	kprintf("frame = %x, pte->frame = %x",
		frame,
		pte_frame_get(&pte)
	);
#endif

	KASSERT(pte_frame_get(&pte) == frame);
}

/*----------------------------------------------------------------------------*
 * Set a Frame of a PDE (nondestructive)                                      *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set a Frame of a PDE (nondestructive)
 */
PRIVATE void mmu_pde_frame_set(void)
{
	struct pde pde;
	frame_t frame = (1 << (VADDR_BIT - PAGE_SHIFT)) - 1;

	kmemset(&pde, 1, sizeof(struct pde));
	pde_clear(&pde);
	pde_frame_set(&pde, frame);

#if (TEST_MMU_VERBOSE)
	kprintf("frame = %x, pde->frame = %x",
		frame,
		pde_frame_get(&pde)
	);
#endif

	KASSERT(pde_frame_get(&pde) == frame);
}

/*----------------------------------------------------------------------------*
 * Set Present Bit of a PTE (nondestructive)                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Present Bit of a PTE (nondestructive)
 */
PRIVATE void mmu_pte_present_set(void)
{
	struct pte pte;

	kmemset(&pte, 1, sizeof(struct pte));
	pte_clear(&pte);
	pte_present_set(&pte, 1);
	KASSERT(pte_is_present(&pte));
	pte_present_set(&pte, 0);
	KASSERT(!pte_is_present(&pte));
}

/*----------------------------------------------------------------------------*
 * Set Present Bit of a PDE (nondestructive)                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Present Bit of a PDE (nondestructive)
 */
PRIVATE void mmu_pde_present_set(void)
{
	struct pde pde;

	kmemset(&pde, 1, sizeof(struct pde));
	pde_clear(&pde);
	pde_present_set(&pde, 1);
	KASSERT(pde_is_present(&pde));
	pde_present_set(&pde, 0);
	KASSERT(!pde_is_present(&pde));
}

/*----------------------------------------------------------------------------*
 * Set User Bit of a PTE (nondestructive)                                     *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set User Bit of a PTE (nondestructive)
 */
PRIVATE void mmu_pte_user_set(void)
{
	struct pte pte;

	kmemset(&pte, 1, sizeof(struct pte));
	pte_clear(&pte);
	pte_user_set(&pte, 1);
	KASSERT(pte_is_user(&pte));
	pte_user_set(&pte, 0);
	KASSERT(!pte_is_user(&pte));
}

/*----------------------------------------------------------------------------*
 * Set User Bit of a PDE (nondestructive)                                     *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set User Bit of a PDE (nondestructive)
 */
PRIVATE void mmu_pde_user_set(void)
{
	struct pde pde;

	kmemset(&pde, 1, sizeof(struct pde));
	pde_clear(&pde);
	pde_user_set(&pde, 1);
	KASSERT(pde_is_user(&pde));
	pde_user_set(&pde, 0);
	KASSERT(!pde_is_user(&pde));
}

/*----------------------------------------------------------------------------*
 * Set Write Bit of a PTE (nondestructive)                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Write Bit of a PTE (nondestructive)
 */
PRIVATE void mmu_pte_write_set(void)
{
	struct pte pte;

	kmemset(&pte, 1, sizeof(struct pte));
	pte_clear(&pte);
	pte_write_set(&pte, 1);
	KASSERT(pte_is_write(&pte));
	pte_write_set(&pte, 0);
	KASSERT(!pte_is_write(&pte));
}

/*----------------------------------------------------------------------------*
 * Set Write Bit of a PDE (nondestructive)                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Write Bit of a PDE (nondestructive)
 */
PRIVATE void mmu_pde_write_set(void)
{
	struct pde pde;

	kmemset(&pde, 1, sizeof(struct pde));
	pde_clear(&pde);
	pde_write_set(&pde, 1);
	KASSERT(pde_is_write(&pde));
	pde_write_set(&pde, 0);
	KASSERT(!pde_is_write(&pde));
}

/*----------------------------------------------------------------------------*
 * Get a PTE                                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Get a PTE
 */
PRIVATE void mmu_pte_get(void)
{
	KASSERT(pte_get(kernel_pgtab, _KBASE_VIRT) != NULL);
}

/*----------------------------------------------------------------------------*
 * Get a PDE                                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Get a PDE
 */
PRIVATE void mmu_pde_get(void)
{
	KASSERT(pde_get(root_pgdir, _KBASE_VIRT) != NULL);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test mmu_api_tests[] = {
	{ mmu_pte_clear,       "clear pte"       },
	{ mmu_pde_clear,       "clear pde"       },
	{ mmu_pte_frame_set,   "pte frame set"   },
	{ mmu_pde_frame_set,   "pde frame set"   },
	{ mmu_pte_present_set, "pte present set" },
	{ mmu_pde_present_set, "pde present set" },
	{ mmu_pte_user_set,    "pte user set"    },
	{ mmu_pde_user_set,    "pde user set"    },
	{ mmu_pte_write_set,   "pte write set"   },
	{ mmu_pde_write_set,   "pde write set"   },
	{ mmu_pte_get,         "pte get"         },
	{ mmu_pde_get,         "pde get"         },
	{ NULL,                 NULL             },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Clear Invalid PTE                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Clear Invalid PTE
 */
PRIVATE void mmu_pte_clear_inval(void)
{
	KASSERT(pte_clear(NULL) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Clear Invalid PDE                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Fault Injection Test: Clear Invalid PDE
 */
PRIVATE void mmu_pde_clear_inval(void)
{
	KASSERT(pde_clear(NULL) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Frame in an Invalid PTE                                              *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set a Frame in an Invalid PTE
 */
PRIVATE void mmu_pte_frame_set_inval(void)
{
	frame_t frame = (1 << (VADDR_BIT - PAGE_SHIFT)) - 1;

	KASSERT(pte_frame_set(NULL, frame) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Frame in an Invalid PDE                                              *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set a Frame in an Invalid PDE
 */
PRIVATE void mmu_pde_frame_set_inval(void)
{
	frame_t frame = (1 << (VADDR_BIT - PAGE_SHIFT)) - 1;

	KASSERT(pde_frame_set(NULL, frame) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set Invalid Frame in a PTE                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Invalid Frame in a PTE
 */
PRIVATE void mmu_pte_frame_set_inval2(void)
{
	struct pte pte;

	KASSERT(pte_frame_set(&pte, -1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set Invalid Frame in a PDE                                                 *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Invalid Frame in a PDE
 */
PRIVATE void mmu_pde_frame_set_inval2(void)
{
	struct pde pde;

	KASSERT(pde_frame_set(&pde, -1) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Present Bit in an Invalid PTE                                        *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Present Bit in an Invalid PTE
 */
PRIVATE void mmu_pte_present_set_inval(void)
{
	KASSERT(pte_present_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Present Bit in an Invalid PDE                                        *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Present Bit in an Invalid PDE
 */
PRIVATE void mmu_pde_present_set_inval(void)
{
	KASSERT(pde_present_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a User Bit in an Invalid PTE                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set User Bit in an Invalid PTE
 */
PRIVATE void mmu_pte_user_set_inval(void)
{
	KASSERT(pte_user_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a User Bit in an Invalid PDE                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set User Bit in an Invalid PDE
 */
PRIVATE void mmu_pde_user_set_inval(void)
{
	KASSERT(pde_user_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Write Bit in an Invalid PTE                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Write Bit in an Invalid PTE
 */
PRIVATE void mmu_pte_write_set_inval(void)
{
	KASSERT(pte_write_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Set a Write Bit in an Invalid PDE                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Set Write Bit in an Invalid PDE
 */
PRIVATE void mmu_pde_write_set_inval(void)
{
	KASSERT(pde_write_set(NULL, 0) == -EINVAL);
}

/*----------------------------------------------------------------------------*
 * Invalid Get PTE                                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Invalid Get PTE
 */
PRIVATE void mmu_pte_get_inval(void)
{
	KASSERT(pte_get(NULL, _KBASE_VIRT) == NULL);
}

/*----------------------------------------------------------------------------*
 * Invalid Get PDE                                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief API Test: Invalid Get PDE
 */
PRIVATE void mmu_pde_get_inval(void)
{
	KASSERT(pde_get(NULL, _KBASE_VIRT) == NULL);
}

/*----------------------------------------------------------------------------*
 * Test Driver Table                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test mmu_fault_tests[] = {
	{ mmu_pte_clear_inval,       "clear invalid pte"              },
	{ mmu_pde_clear_inval,       "clear invalid pde"              },
	{ mmu_pte_frame_set_inval,   "set frame in invalid pte"       },
	{ mmu_pde_frame_set_inval,   "set frame in invalid pde"       },
	{ mmu_pte_frame_set_inval2,  "set invalid frame in pte"       },
	{ mmu_pde_frame_set_inval2,  "set invalid frame in pde"       },
	{ mmu_pte_present_set_inval, "set present bit in invalid pte" },
	{ mmu_pde_present_set_inval, "set present bit in invalid pde" },
	{ mmu_pte_user_set_inval,    "set user bit in invalid pte"    },
	{ mmu_pde_user_set_inval,    "set user bit in invalid pde"    },
	{ mmu_pte_write_set_inval,   "set write bit in invalid pte"   },
	{ mmu_pde_write_set_inval,   "set write bit in invalid pde"   },
	{ mmu_pte_get_inval,         "get invalid pte"                },
	{ mmu_pde_get_inval,         "get invalid pde"                },
	{ NULL, NULL },
};

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_mmu() function launches testing units on the
 * MMU Interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_mmu(void)
{
	for (int i = 0; mmu_api_tests[i].test_fn != NULL; i++)
	{
		mmu_api_tests[i].test_fn();
		kprintf("[test][api][mmu] %s [passed]", mmu_api_tests[i].name);
	}

	for (int i = 0; mmu_fault_tests[i].test_fn != NULL; i++)
	{
		mmu_fault_tests[i].test_fn();
		kprintf("[test][fault][mmu] %s [passed]", mmu_fault_tests[i].name);
	}
}

