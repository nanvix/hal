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

#ifndef ARCH_LINUX64_CACHE_H_
#define ARCH_LINUX64_CACHE_H_

/**
 * @addtogroup linux64-core-cache Cache
 * @ingroup linux64-core
 *
 * @brief Memory Cache
 */
/**@{*/

	#include <nanvix/klib.h>
	#include <stdlib.h>
	#include <stdio.h>

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __dcache_invalidate_fn
	/**@}*/

	/**
	 * @brief Signalizes if the core supports data cache
	 */
	#define CORE_HAS_DCACHE 0
	
	/**
	 * @brief Signalizes if the core supports instruction cache
	 */
	#define CORE_HAS_ICACHE 0

	/**
	 * @brief Initialize all the cache variables
	 */
	EXTERN void linux64_core_cache_init(void);

	/**
	 * @brief Cache line size
	 */
	EXTERN unsigned int linux64_cache_line_size;

	/**
	 * @brief recover the cache line size
	 */
	EXTERN unsigned int get_linux64_cache_line_size(void);

	/**
	 * @see linux64_cache_line_size
	 */
	#define CACHE_LINE_SIZE get_linux64_cache_line_size()

	/**
	 * @brief Cache line size log2
	 */
	EXTERN unsigned int linux64_cache_line_size_log2;

	/**
	 * @brief recover the cache line size log2
	 */
	EXTERN unsigned int get_linux64_cache_line_size_log2(void);

	/**
	 * @brief Cache size
	 */
	EXTERN unsigned int linux64_cache_size;

	/**
	 * @brief recover the cache line size
	 */
	EXTERN unsigned int get_linux64_cache_size(void);

	/**
	 * @brief : cache size log2
	 */
	#define CACHE_SIZE get_linux64_cache_size()

	/**
	 * @brief Cache line size log2.
	 */
	#define CACHE_LINE_SIZE_LOG2 get_linux64_cache_line_size_log2()
	
	/**
	 * @brief Cache line size log2
	 */
	EXTERN unsigned int linux64_cache_size_log2;

	/**
	 * @brief Get the cache line size log2
	 */
	EXTERN unsigned int get_linux64_cache_size_log2(void);

	/**
	 * @brief Cache size log2
	 */
	#define CACHE_SIZE_LOG2 get_linux64_cache_size_log2()
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_invalidate_count;

	/**
	 * @note The linux64 core features cache coherency.
	 */
	EXTERN void dcache_invalidate(void);

	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_flush_count;

	/**
	 * @brief Flushes changes of the data cache into memory
	 */
	EXTERN void dcache_flush(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_fence_count;

	/**
	 * @brief Wait for ongoing flush operations in the data cache to finish
	 */
	EXTERN void dcache_fence(void);

	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_prefetch_line_count;

	/**
	 * @brief Prefetches a line of the data cache
	 */
	EXTERN void dcache_prefetch_line(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_invalidate_line_count;

	/**
	 * @brief Invalidates a line of the data cache
	 */
	EXTERN void dcache_invalidate_line(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int dcache_setup_count;

	/**
	 * @brief Enables the data cache
	 */
	EXTERN void dcache_setup(void);

	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int icache_invalidate_count;

	/**
	 * @brief Invalidates the instruction cache
	 */
	EXTERN void icache_invalidate(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int icache_prefetch_line_count;

	/**
	 * @brief Prefetches a line of the instruction cache
	 */
	EXTERN void icache_prefetch_line(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int icache_invalidate_line_count;

	/**
	 * @brief Invalidates a line of the instruction cache
	 */
	EXTERN void icache_invalidate_line(void);
	
	/**
	 * @brief A counter to know how many time the dcache invalidate is called
	 */
	EXTERN unsigned int icache_setup_count;

	/**
	 * @brief Enables the instruction cache
	 */
	EXTERN void icache_setup(void);

	/**
	 * @brief Display all the cache variables
	 */
	EXTERN void linux64_core_cache_print(void);

	/**
	 * @brief Display all the cache counters
	 */
	EXTERN void linux64_core_cache_count_print(void);

/**@}*/

#endif /* ARCH_LINUX64_CACHE_H_ */