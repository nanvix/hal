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

#ifndef ARCH_CORE_RV32I_CACHE_H_
#define ARCH_CORE_RV32I_CACHE_H_

/**
 * @addtogroup rv32i-core-cache Cache
 * @ingroup rv32i-core
 *
 * @brief Memory Cache
 */
/**@{*/

	#include <nanvix/cc.h>

	/**
	 * @brief L1 Cache line shift.
	 */
	#define RV32I_CACHE_LINE_SHIFT 6

	/**
	 * @brief L1 Cache line size (in bytes).
	 */
	#define RV32I_CACHE_LINE_SIZE (1 << RV32I_CACHE_LINE_SHIFT)

#ifndef _ASM_FILE_

	/**
	 * @brief Flushes the data and instruction caches.
	 *
	 * @note This function flushes the whole cache.
	 */
	static inline void rv32i_cache_inval(void)
	{
		asm volatile ("fence.i" ::: "memory");
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __dcache_invalidate_fn /**< dcache_invalidate() */
	#define __icache_invalidate_fn /**< icache_invalidate() */
	/**@}*/

	/**
	 * @see RV32I_CACHE_LINE_SIZE.
	 */
	#define CACHE_LINE_SIZE RV32I_CACHE_LINE_SIZE

#ifndef _ASM_FILE_

	/**
	 * @see rv32i_dcache_inval().
	 */
	static inline void dcache_invalidate(void)
	{
		rv32i_cache_inval();
	}

	/**
	 * @see rv32i_icache_inval().
	 */
	static inline void icache_invalidate(void)
	{
		rv32i_cache_inval();
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32I_CACHE_H_ */
