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

#ifndef ARCH_CORE_RI5CY_H_
#define ARCH_CORE_RI5CY_H_

	/**
	 * @addtogroup ri5cy-core RI5CY Core
	 * @ingroup cores
	 */

	#ifndef __NEED_CORE_RI5CY
		#error "r5scy core not required"
	#endif

	#ifdef _ASM_FILE_
	#include <arch/core/ri5cy/asm.h>
	#endif

	#include <arch/core/ri5cy/core.h>

/**
 * @cond ri5cy
 */

	/**
	 * @name Core Features
	 */
	/**@{*/
	#define CORE_HAS_ATOMICS      1 /**< Has Atomic Instructions?    */
	#define CORE_HAS_PMIO         0 /**< Has Programmed I/O?         */
	#define CORE_HAS_TLB_HW       1 /**< Has Hardware-Managed TLB?   */
	#define CORE_HAS_CACHE_HW     0 /**< Has Hardware-Managed Cache? */
	#define CORE_IS_LITTLE_ENDIAN 1 /**< Is Little Endian?           */
	/**@}*/

/**@endcond*/

#endif /* ARCH_CORE_RI5CY_H_ */

