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

#ifndef ARCH_CORE_ARM64_CACHE_H_
#define ARCH_CORE_ARM64_CACHE_H_

/**
 * @addtogroup arm64-core-cache Cache
 * @ingroup arm64-core
 *
 * @brief Memory Cache
 */
/**@{*/

#include "arch/core/arm64/mmu.h"

#ifndef _ASM_FILE_

	/**
	 * @brief Invalide the data cache.
	 */
	static inline void arm64_dcache_inval(void)
	{
		__asm__ __volatile__("tlbi alle1is\n\t" : : : "memory");
		dsb(ish);
		isb();
	}

	/**
	 * @brief Flushes the instruction cache.
	 *
	 */
	static inline void arm64_icache_inval(void)
	{

	}

#endif /* !_ASM_FILE_ */

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __dcache_invalidate_fn /**< dcache_invalidate() */
	#define __icache_invalidate_fn /**< icache_invalidate() */
	/**@}*/

	/**
	 * @see RV32GC_CACHE_LINE_SIZE.
	 */
	#define CACHE_LINE_SIZE 1

#ifndef _ASM_FILE_

	/**
	 * @see arm64_dcache_inval().
	 */
	static inline void dcache_invalidate(void)
	{
		arm64_dcache_inval();
	}

	/**
	 * @see arm64_icache_inval().
	 */
	static inline void icache_invalidate(void)
	{
		arm64_icache_inval();
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_ARM64_CACHE_H_ */
