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

#ifndef ARCH_CORE_RV32I_H_
#define ARCH_CORE_RV32I_H_

	/**
	 * @addtogroup rv32i-core RV32I Core
	 * @ingroup cores
	 */

	#ifndef __NEED_CORE_RV32I
		#error "rv32i core not required"
	#endif

	#include <arch/core/rv32i/cache.h>
	#include <arch/core/rv32i/clock.h>
	#include <arch/core/rv32i/core.h>
	#include <arch/core/rv32i/excp.h>
	#include <arch/core/rv32i/int.h>
	#include <arch/core/rv32i/mmu.h>
	#include <arch/core/rv32i/spinlock.h>

/**
 * @cond rv32i
 */

	/* Feature Declaration */
	#define CORE_SUPPORTS_PMIO    0
	#define CORE_IS_LITTLE_ENDIAN 1

/**@endcond*/

#endif /* ARCH_CORE_RV32I_H_ */

