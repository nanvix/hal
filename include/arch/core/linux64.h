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

#ifndef CORE_LINUX64_H_
#define CORE_LINUX64_H_

	/**
	 * @addtogroup linux64-core Linux64 Core
	 * @ingroup cores
	 */

	#ifndef __NEED_CORE_LINUX64
		#error "linux64 core not required"
	#endif


	#include <arch/core/linux64/core.h>
	#include <arch/core/linux64/cache.h>
	#include <arch/core/linux64/excp.h>
	#include <arch/core/linux64/int.h>
	#include <arch/core/linux64/spinlock.h>
	#include <arch/core/linux64/mmu.h>
	#include <arch/core/linux64/tlb.h>
	#include <arch/core/linux64/trap.h>

/**
 * @cond linux64
 */

	/**
	 * @name Core Features
	 */
	/**@{*/
	#define CORE_HAS_PERF         0 /**< Has Performance Monitors?   */
	#define CORE_HAS_ATOMICS      1 /**< Has Atomic Instructions?    */
	#define CORE_HAS_PMIO         0 /**< Has Programmed I/O?         */
	#define CORE_HAS_TLB_HW       0 /**< Has Hardware-Managed TLB?   */
	#define CORE_HAS_CACHE_HW     1 /**< Has Hardware-Managed Cache? */
	#define CORE_HAS_HUGE_PAGES   0 /**< Are Huge Pages Supported?   */
	#define CORE_IS_LITTLE_ENDIAN 0 /**< Is Little Endian?           */
	/**@}*/

/**@endcond*/

#endif /* CORE_LINUX64_H_ */

