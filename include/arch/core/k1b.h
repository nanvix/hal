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

#ifndef CORE_K1B_H_
#define CORE_K1B_H_

	/**
	 * @addtogroup k1b-core k1b Core
	 * @ingroup cores
	 */

	#ifndef __NEED_CORE_K1B
		#error "k1b core not required"
	#endif

	#include <arch/core/k1b/cache.h>
	#include <arch/core/k1b/timer.h>
	#include <arch/core/k1b/core.h>
	#include <arch/core/k1b/excp.h>
	#include <arch/core/k1b/int.h>
	#include <arch/core/k1b/mmu.h>
	#include <arch/core/k1b/mOS.h>
	#include <arch/core/k1b/perf.h>
	#include <arch/core/k1b/pmio.h>
	#include <arch/core/k1b/lpic.h>
	#include <arch/core/k1b/ivt.h>
	#include <arch/core/k1b/spinlock.h>
	#include <arch/core/k1b/tlb.h>
	#include <arch/core/k1b/trap.h>
	#include <arch/core/k1b/upcall.h>

	#ifdef _ASM_FILE_
		#include <arch/core/k1b/asm.h>
	#endif

/**
 * @cond k1b
 */

	/**
	 * @name Core Features
	 */
	/**@{*/
	#define CORE_HAS_PERF         1 /**< Has Performance Monitors?   */
	#define CORE_HAS_ATOMICS      1 /**< Has Atomic Instructions?    */
	#define CORE_HAS_PMIO         1 /**< Has Programmed I/O?         */
	#define CORE_HAS_TLB_HW       0 /**< Has Hardware-Managed TLB?   */
	#define CORE_HAS_CACHE_HW     0 /**< Has Hardware-Managed Cache? */
	#define CORE_HAS_HUGE_PAGES   0 /**< Are Huge Pages Supported?   */
	#define CORE_IS_LITTLE_ENDIAN 1 /**< Is Little Endian?           */
	/**@}*/

/**@endcond*/

#endif /* CORE_K1B_H_ */

