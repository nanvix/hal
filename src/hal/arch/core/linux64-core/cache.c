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

#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @name Data Cache Dimensions
 */
/**@{*/
PRIVATE size_t linux64_cache_line_size = 0;
PRIVATE size_t linux64_cache_line_size_log2 = 0;
PRIVATE size_t linux64_cache_size = 0;
PRIVATE size_t linux64_cache_size_log2 = 0;
/**@}*/

/**
 * @name Data Cache Statistics
 */
/**@{*/
PRIVATE unsigned dcache_invalidate_count = 0;      /**< Number of Invalidates      */
PRIVATE unsigned dcache_flush_count = 0;           /**< Number of Flushes          */
PRIVATE unsigned dcache_fence_count = 0;           /**< Number of Fences           */
PRIVATE unsigned dcache_line_invalidate_count = 0; /**< Number of Line Invalidates */
PRIVATE unsigned dcache_line_prefetch_count = 0;   /**< Number of Line Prefetches  */
/**@}*/

/**
 * @name Instruction Cache Statistics
 */
/**@{*/
PRIVATE unsigned icache_invalidate_count = 0;      /**< Number of Invalidates      */
PRIVATE unsigned icache_line_prefetch_count = 0;   /**< Number of Line Invalidates */
PRIVATE unsigned icache_line_invalidate_count = 0; /**< Number of Line Prefetches  */
/**@{*/

/*============================================================================*
 * Data Cache                                                                 *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC size_t linux64_core_dcache_line_size_get(void)
{
	return linux64_cache_line_size;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC size_t linux64_core_dcache_line_size_log2_get(void)
{
	return linux64_cache_line_size_log2;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC size_t linux64_core_dcache_size_get(void)
{
    return linux64_cache_size;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC size_t linux64_core_dcache_size_log2_get(void)
{
    return linux64_cache_size_log2;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_invalidate(void)
{
	dcache_invalidate_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_flush(void)
{
	dcache_flush_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_fence(void)
{
	dcache_fence_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_prefetch_line(void)
{
	dcache_line_prefetch_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_invalidate_line(void)
{
	dcache_line_invalidate_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 * @todo TODO: this should be platform independent.
 */
PUBLIC void linux64_core_dcache_dump_info(void)
{
	kprintf("dcache_line_size = %d B", linux64_core_dcache_line_size_get());
	kprintf("dcache_line_size_log2 = %d B", linux64_core_dcache_line_size_log2_get());
	kprintf("dcache_size = %d B", linux64_core_dcache_size_get());
	kprintf("dcache_size_log2 = %d B", linux64_core_dcache_size_log2_get());
}

/**
 * @todo TODO: provide a detailed description for this function.
 * @todo TODO: this should be platform independent.
 */
PUBLIC void linux64_core_dcache_dump_stats(void)
{
	kprintf("dcache_invalidate = %d", dcache_invalidate_count);
	kprintf("dcache_flush = %d", dcache_flush_count);
	kprintf("dcache_fence = %d", dcache_fence_count);
	kprintf("dcache_line_prefetch = %d", dcache_line_prefetch_count);
	kprintf("dcache_line_invalidate = %d", dcache_line_invalidate_count);
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_dcache_setup(void)
{
	/*
	 * Retrieve dimension of cache line size. Note that we
	 * intentionally truncate the line size to a of power 2, because
	 * that may not be true in some archs.
	 */
	linux64_cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
	linux64_cache_line_size_log2 = log2(linux64_cache_line_size);
	linux64_cache_line_size = pow(2, linux64_cache_line_size_log2);

	/*
	 * Retrieve dimension of cache. Note that we intentionally
	 * truncate the line size to a of power 2, because that may not be
	 * true in some archs.
	 */
	linux64_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);
	linux64_cache_size_log2 = log2(linux64_cache_size);
	linux64_cache_size = pow(2, linux64_cache_size_log2);
}

/*============================================================================*
 * Instruction Cache                                                          *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_icache_invalidate(void)
{
	icache_invalidate_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_icache_line_prefetch(void)
{
	icache_line_prefetch_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void linux64_core_icache_line_invalidate(void)
{
	icache_line_invalidate_count++;
}

/**
 * @todo TODO: provide a detailed description for this function.
 * @todo TODO: implement this function.
 */
PUBLIC void linux64_core_icache_setup(void)
{
}

/**
 * @todo TODO: provide a detailed description for this function.
 * @todo TODO: this should be platform independent.
 */
PUBLIC void linux64_core_icache_dump_stats(void)
{
	kprintf("icache_invalidate = %d", icache_invalidate_count);
	kprintf("icache_line_prefetch = %d", icache_line_prefetch_count);
	kprintf("icache_line_invalidate = %d", icache_line_invalidate_count);
}
