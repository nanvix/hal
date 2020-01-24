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

#ifndef ARCH_LINUX64_CACHE_H_
#define ARCH_LINUX64_CACHE_H_

/**
 * @addtogroup linux64-core-cache Memory Cache
 * @ingroup linux64-core
 *
 * @brief Memory Cache Interface
 */
/**@{*/

	#include <nanvix/hlib.h>
	#include <stdlib.h>

	/**
	 * @brief recover the cache line size
	 */
	EXTERN size_t linux64_core_dcache_line_size_get(void);

	/**
	 * @brief recover the cache line size log2
	 */
	EXTERN size_t linux64_core_dcache_line_size_log2_get(void);

	/**
	 * @brief recover the cache line size
	 */
	EXTERN size_t linux64_core_dcache_size_get(void);

	/**
	 * @brief Get the cache line size log2
	 */
	EXTERN size_t linux64_core_dcache_size_log2_get(void);

	/**
	 * @brief Invalidates the data cache of the underlying core.
	 */
	EXTERN void linux64_core_dcache_invalidate(void);

	/**
	 * @brief Flushes changes of the data cache into memory
	 */
	EXTERN void linux64_core_dcache_flush(void);

	/**
	 * @brief Wait for ongoing flush operations in the data cache to finish
	 */
	EXTERN void linux64_core_dcache_fence(void);

	/**
	 * @brief Prefetches a line of the data cache
	 */
	EXTERN void linux64_core_dcache_line_prefetch(void);

	/**
	 * @brief Invalidates a line of the data cache
	 */
	EXTERN void linux64_core_dcache_line_invalidate(void);

	/**
	 * @brief Dump information on data cache.
	 */
	EXTERN void linux64_core_dcache_dump_info(void);

	/**
	 * @brief Dump statistics on data cache.
	 */
	EXTERN void linux64_core_dcache_dump_stats(void);

	/**
	 * @brief Enables the data cache
	 */
	EXTERN void linux64_core_dcache_setup(void);

	/**
	 * @brief Invalidates the instruction cache
	 */
	EXTERN void linux64_core_icache_invalidate(void);

	/**
	 * @brief Prefetches a line of the instruction cache
	 */
	EXTERN void linux64_core_icache_line_prefetch(void);

	/**
	 * @brief Invalidates a line of the instruction cache
	 */
	EXTERN void linux64_core_icache_line_invalidate(void);

	/**
	 * @brief Dump statistics on instruction cache.
	 */
	EXTERN void linux64_core_icache_dump_stats(void);

	/**
	 * @brief Enables the instruction cache
	 */
	EXTERN void linux64_core_icache_setup(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_core
 */

	/**
	 * @name Cache Capabilities
	 */
	/**@{*/
	#define CORE_HAS_HW_DCACHE 0 /**< Has Data Cache?        */
	#define CORE_HAS_HW_ICACHE 0 /**< Has Instruction Cache? */
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __dcache_invalidate_fn /**< dcache_invalidate() */
	/**@}*/

	/**
	 * @see Dummy cache line size log2.
	 */
	#define CACHE_LINE_SIZE_LOG2 6

	/**
	 * @see Dummy cache line size (in bytes).
	 */
	#define CACHE_LINE_SIZE (1 << CACHE_LINE_SIZE_LOG2)

	/**
	 * @see Dummy cache size log2.
	 */
	#define CACHE_SIZE_LOG2 15

	/**
	 * @see Dummy cache size (in bytes).
	 */
	#define CACHE_SIZE (1 << CACHE_SIZE_LOG2)

	/**
	 * @see linux_core_dcache_invalidate().
	 */
	static inline void dcache_invalidate(void)
	{
		linux64_core_dcache_invalidate();
	}

	/**
	 * @see linux_core_dcache_flush().
	 */
	static inline void dcache_flush(void)
	{
		linux64_core_dcache_flush();
	}

	/**
	 * @see linux_core_dcache_fence().
	 */
	static inline void dcache_fence(void)
	{
		linux64_core_dcache_fence();
	}

	/**
	 * @see linux_core_dcache_prefetch().
	 */
	static inline void dcache_line_prefetch(void)
	{
		linux64_core_dcache_line_prefetch();
	}

	/**
	 * @see linux_core_dcache_invalidate().
	 */
	static inline void dcache_line_invalidate(void)
	{
		linux64_core_dcache_line_invalidate();
	}

	/**
	 * @see linux_core_dcache_dump_info().
	 */
	static inline void dcache_dump_info(void)
	{
		linux64_core_dcache_dump_info();
	}

	/**
	 * @see linux_core_dcache_dump_stats().
	 */
	static inline void dcache_dump_stats(void)
	{
		linux64_core_dcache_dump_stats();
	}

	/**
	 * @see linux_core_icache_invalidate().
	 */
	static inline void icache_invalidate(void)
	{
		linux64_core_icache_invalidate();
	}

	/**
	 * @see linux_core_icache_prefetch().
	 */
	static inline void icache_line_prefetch(void)
	{
		linux64_core_icache_line_prefetch();
	}

	/**
	 * @see linux_core_icache_invalidate().
	 */
	static inline void icache_line_invalidate(void)
	{
		linux64_core_icache_line_invalidate();
	}

	/**
	 * @see linux_core_icache_dump_stats().
	 */
	static inline void icache_dump_stats(void)
	{
		linux64_core_icache_dump_stats();
	}

/**@endcond*/

#endif /* ARCH_LINUX64_CACHE_H_ */
