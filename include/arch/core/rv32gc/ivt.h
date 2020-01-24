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

#ifndef ARCH_CORE_RV32GC_IVT_H_
#define ARCH_CORE_RV32GC_IVT_H_

	#ifndef __NEED_CORE_IVT
		#error "do not include this file"
	#endif

	/* Must come first. */
	#define __NEED_CORE_TYPES

/**
 * @addtogroup rv32gc-core-ivt IVT
 * @ingroup rv32gc-core
 *
 * @brief Interrupt Vector Table
 */
/**@{*/

	#include <arch/core/rv32gc/types.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Event handler.
	 */
	typedef void (*rv32gc_handler_fn)(void);

#ifdef __NANVIX_HAL

	/**
	 * @brief Set ups the interrupt vector table.
	 *
	 * @param do_event Event handler.
	 *
	 * @note We assume a direct vector table.
	 */
	static inline void rv32gc_mtvec_set(rv32gc_handler_fn do_event)
	{
		asm volatile (
			"csrw mtvec, %0;"
			:
			: "r" (RV32GC_WORD(do_event))
		);
	}

#endif /* __NANVIX_HAL */

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */

#ifndef _ASM_FILE_

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the interrupt vector table.
	 */
	EXTERN void ivt_setup(void *stack);

#endif /* __NANVIX_HAL */

#endif /* !_ASM_FILE_ */

#endif /* ARCH_CORE_RV32GC_IVT_H_ */
