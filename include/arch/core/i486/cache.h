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

#ifndef ARCH_I486_CACHE_H_
#define ARCH_I486_CACHE_H_

/**
 * @addtogroup i486-core-cache Cache
 * @ingroup i486-core
 *
 * @brief Memory Cache
 */
/**@{*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __dcache_invalidate_fn
	#define __icache_invalidate_fn
	/**@}*/

	/**
	 * @brief Cache line size (in bytes).
	 *
	 * @bug The cache line size of i486 may change.
	 */
	#define I486_CACHE_LINE_SIZE 64

	/**
	 * @see I486_CACHE_LINE_SIZE
	 */
	#define CACHE_LINE_SIZE I486_CACHE_LINE_SIZE

#ifndef _ASM_FILE_

	/**
	 * @brief Flushes the data cache.
	 *
	 * @note The i486 target features cache coherency.
	 */
	static inline void dcache_invalidate(void)
	{
		asm volatile ("wbinvd" ::: "memory");
		asm volatile ("invd" ::: "memory");
	}

	/**
	 * @brief Flushes the instruction cache.
	 *
	 * @note The i486 target features cache coherency.
	 */
	static inline void icache_invalidate(void)
	{
		asm volatile ("wbinvd" ::: "memory");
		asm volatile ("invd" ::: "memory");
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_I486_CACHE_H_ */
