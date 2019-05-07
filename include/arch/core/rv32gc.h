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

#ifndef ARCH_CORE_RV32GC_H_
#define ARCH_CORE_RV32GC_H_

	/**
	 * @addtogroup rv32gc-core RV32GC Core
	 * @ingroup cores
	 */

	#ifndef __NEED_CORE_RV32GC
		#error "rv32gc core not required"
	#endif

	#include <arch/core/rv32gc/cache.h>
	#include <arch/core/rv32gc/timer.h>
	#include <arch/core/rv32gc/core.h>
	#include <arch/core/rv32gc/excp.h>
	#include <arch/core/rv32gc/int.h>
	#include <arch/core/rv32gc/mmu.h>
	#include <arch/core/rv32gc/spinlock.h>
	#include <arch/core/rv32gc/tlb.h>
	#include <arch/core/rv32gc/trap.h>
	#include <arch/core/rv32gc/upcall.h>

	#ifdef __NEED_CORE_MACHINE
		#include <arch/core/rv32gc/machine.h>
	#endif

	#ifdef _ASM_FILE_
		#include <arch/core/rv32gc/asm.h>
	#endif

/**
 * @cond rv32gc
 */

	/**
	 * @name Core Features
	 */
	/**@{*/
	#define CORE_HAS_PERF         0 /**< Has Performance Monitors?   */
	#define CORE_HAS_ATOMICS      1 /**< Has Atomic Instructions?    */
	#define CORE_HAS_PMIO         0 /**< Has Programmed I/O?         */
	#define CORE_HAS_TLB_HW       1 /**< Has Hardware-Managed TLB?   */
	#define CORE_HAS_CACHE_HW     0 /**< Has Hardware-Managed Cache? */
	#define CORE_HAS_HUGE_PAGES   1 /**< Are Huge Pages Supported?   */
	#define CORE_IS_LITTLE_ENDIAN 1 /**< Is Little Endian?           */
	/**@}*/

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_H_ */

