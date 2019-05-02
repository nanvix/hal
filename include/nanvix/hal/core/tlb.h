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

#ifndef NANVIX_HAL_CORE_TLB_H_
#define NANVIX_HAL_CORE_TLB_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CORE_AL) || defined(__INTERFACE_CHECK_TLB)

	/*
	 * Required interface for software-managed TLBs.
	 */
	#if (!CORE_HAS_TLB_HW)

		/* Constants. */
		#ifndef TLB_INSTRUCTION
		#error "TLB_INSTRUCTION not defined!"
		#endif
		#ifndef TLB_DATA
		#error "TLB_DATA not defined!"
		#endif

		/* Types and Structures */
		#ifndef __tlbe_st
			#error "struct tlbe not defined?"
		#endif

		/* Functions */
		#ifndef __tlbe_vaddr_get_fn
			#error "tlb_vaddr_get() not defined?"
		#endif
		#ifndef __tlbe_paddr_get_fn
			#error "tlb_paddr_get() not defined?"
		#endif
		#ifndef __tlbe_get_index_fn
			#error "tlbe_get_index() not defined?"
		#endif
		#ifndef __tlbe_is_valid_fn
			#error "tlbe_is_valid() not defined?"
		#endif
		#ifndef __tlbe_write_fn
			#error "tlb_write() not defined?"
		#endif
		#ifndef __tlbe_inval_fn
			#error "tlb_inval() not defined?"
		#endif

	#endif

#endif

/*============================================================================*
 * TLB Interface                                                              *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-tlb TLB
 * @ingroup kernel-hal-core
 *
 * @brief TLB HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Casts something to a virtual address.
	 *
	 * @param x Something.
	 */
	#define VADDR(x) ((vaddr_t) (x))

	/**
	 * @brief Casts something to a physical address.
	 *
	 * @param x Something.
	 */
	#define PADDR(x) ((paddr_t) (x))

#if (CORE_HAS_TLB_HW)

	/**
	 * @name TLB Types
	 */
	/**@{*/
	#define TLB_INSTRUCTION 0 /**< Instruction TLB */
	#define TLB_DATA        1 /**< Data TLB        */
	/**@}*/

#endif

#ifndef __NANVIX_HAL

	/**
	 * @brief TLB entry.
	 */
	struct tlbe;

	/**
	 * @brief Gets the virtual address encoded in a TLB entry.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * @returns The virtual address encoded in the TLB entry pointed
	 * to by @p tlbe.
	 */
	EXTERN vaddr_t tlbe_vaddr_get(const struct tlbe *tlbe);

	/**
	 * @brief Gets the physical address encoded in a TLB entry.
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * @returns The physical address encoded in the TLB entry pointed
	 * to by @p tlbe.
	 */
	EXTERN paddr_t tlbe_paddr_get(const struct tlbe *tlbe);

	/**
	 * @brief Assesses if a TLB entry is valid
	 *
	 * @param tlbe Target TLB entry.
	 *
	 * The tlbe_is_valid() function assess if a TLB entry
	 * has the status bit valid.
	 *
	 * @return Non zero if is the TLB entry is valid, zero otherwise.
	 */
	EXTERN int tlbe_is_valid(const struct tlbe *tlbe);

	/**
	 * @brief Gets the tlbe entry index in TLB.
	 * 
	 * @param vaddr Target virtual address.
	 *
	 * @return Index of target entry in the TLB.
	 */
	EXTERN unsigned tlbe_get_index(vaddr_t vaddr);

	/**
	 * @brief Writes a TLB entry.
	 *
	 * @param tlbe       The updated value of target TLB entry.
	 * @param tlb_type   Target TLB.
	 * @param vaddr      Target virtual address.
	 * @param paddr      Target physical address.
	 * @param vaddr_info Virtual address information flags.
	 * 
	 * @return Zero if successfully writes a TLB entry,
	 * non zero otherwise.
	 */
	EXTERN int tlbe_write(
		struct tlbe *tlbe,
		int tlb_type,
		vaddr_t vaddr,
		paddr_t paddr,
		int vadd_info
	);

	/**
	 * @brief Invalidates a TLB entry.
	 *
	 * @param tlbe  The updated value of target TLB entry.
	 * @param tlb_type Target TLB.
	 * @param vaddr Target virtual address.
	 * 
	 * @return Zero if successfully writes a TLB entry,
	 * non zero otherwise.
	 */
	EXTERN int tlbe_inval(struct tlbe *tlbe, int tlb_type, vaddr_t vaddr);

#endif /* __NANVIX_HAL */

/**@}*/

#endif /* NANVIX_HAL_CORE_TLB_H_ */


