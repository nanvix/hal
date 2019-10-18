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

#include <arch/cluster/linux64-cluster/cores.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief Alias to the log2 fn in <math.h>
 */
EXTERN double linux64_log2(double x);

/**
 * @brief Alias to the pow fn in <math.h>
 */
EXTERN double linux64_pow(double base, double exp);

/**
 * @brief Cache information.
 */
PRIVATE struct cache_info
{
	/**
	 * @name Data Cache Dimensions
	 */
	/**@{*/
	size_t cache_line_size;
	size_t cache_line_size_log2;
	size_t cache_size;
	size_t cache_size_log2;
	/**@}*/

	/**
	 * @name Data Cache Statistics
	 */
	/**@{*/
	unsigned dcache_invalidate_count;      /**< Number of Invalidates      */
	unsigned dcache_flush_count;           /**< Number of Flushes          */
	unsigned dcache_fence_count;           /**< Number of Fences           */
	unsigned dcache_line_invalidate_count; /**< Number of Line Invalidates */
	unsigned dcache_line_prefetch_count;   /**< Number of Line Prefetches  */
	/**@}*/

	/**
 	* @name Instruction Cache Statistics
 	*/
	/**@{*/
	unsigned icache_invalidate_count;      /**< Number of Invalidates      */
	unsigned icache_line_prefetch_count;   /**< Number of Line Invalidates */
	unsigned icache_line_invalidate_count; /**< Number of Line Prefetches  */
	/**@}*/
} linux64_core_cache_info[LINUX64_CLUSTER_NUM_CORES];

/*============================================================================*
 * Data Cache                                                                 *
 *============================================================================*/

/**
 * @brief Return the cache_line_size value set for the underlying core.
 */
PUBLIC size_t linux64_core_dcache_line_size_get(void)
{
	return linux64_core_cache_info[linux64_core_get_id()].cache_line_size;
}

/**
 * @brief Return the cache_line_size_log2 value set for the underlying core.
 */
PUBLIC size_t linux64_core_dcache_line_size_log2_get(void)
{
	return linux64_core_cache_info[linux64_core_get_id()].cache_line_size_log2;
}

/**
 * @brief Return the cache_size value set for the underlying core.
 */
PUBLIC size_t linux64_core_dcache_size_get(void)
{
    return linux64_core_cache_info[linux64_core_get_id()].cache_size;
}

/**
 * @brief Return the cache_size_log2 value set for the underlying core.
 */
PUBLIC size_t linux64_core_dcache_size_log2_get(void)
{
    return linux64_core_cache_info[linux64_core_get_id()].cache_size_log2;
}

/**
 * @brief Incremement the counter, invalidate don't have sense in linux64.
 */
PUBLIC void linux64_core_dcache_invalidate(void)
{
	linux64_core_cache_info[linux64_core_get_id()].dcache_invalidate_count++;
}

/**
 * @brief Incremement the counter, flush don't have sense in linux64.
 */
PUBLIC void linux64_core_dcache_flush(void)
{
	linux64_core_cache_info[linux64_core_get_id()].dcache_flush_count++;
}

/**
 * @brief Incremement the counter, fence don't have sense in linux64.
 */
PUBLIC void linux64_core_dcache_fence(void)
{
	linux64_core_cache_info[linux64_core_get_id()].dcache_fence_count++;
}

/**
 * @brief Incremement the counter, prefetch line don't have sense in linux64.
 */
PUBLIC void linux64_core_dcache_prefetch_line(void)
{
	linux64_core_cache_info[linux64_core_get_id()].dcache_line_prefetch_count++;
}

/**
 * @brief Incremement the counter, invalidate line don't have sense in linux64.
 */
PUBLIC void linux64_core_dcache_invalidate_line(void)
{
	linux64_core_cache_info[linux64_core_get_id()].dcache_line_invalidate_count++;
}

/**
 * @brief Print all informations about the dcache within the underlyin core.
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
 * @brief Print all stats informations about the dcache within the underlying core.
 * @todo TODO: this should be platform independent.
 */
PUBLIC void linux64_core_dcache_dump_stats(void)
{
	kprintf("dcache_invalidate = %d",linux64_core_cache_info[linux64_core_get_id()].dcache_invalidate_count);
	kprintf("dcache_flush = %d", linux64_core_cache_info[linux64_core_get_id()].dcache_flush_count);
	kprintf("dcache_fence = %d", linux64_core_cache_info[linux64_core_get_id()].dcache_fence_count);
	kprintf("dcache_line_prefetch = %d", linux64_core_cache_info[linux64_core_get_id()].dcache_line_prefetch_count);
	kprintf("dcache_line_invalidate = %d", linux64_core_cache_info[linux64_core_get_id()].dcache_line_invalidate_count);
}

/**
 * @brief Setup the dcache of the underlying core.
 */
PUBLIC void linux64_core_dcache_setup(void)
{
	/*
	 * Retrieve dimension of cache line size. Note that we
	 * intentionally truncate the line size to a of power 2, because
	 * that may not be true in some archs.
	 */
	linux64_core_cache_info[linux64_core_get_id()].cache_line_size =
		sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
	linux64_core_cache_info[linux64_core_get_id()].cache_line_size_log2 =
		linux64_log2(linux64_core_cache_info[linux64_core_get_id()].cache_line_size);
	linux64_core_cache_info[linux64_core_get_id()].cache_line_size =
		linux64_pow(2, linux64_core_cache_info[linux64_core_get_id()].cache_line_size_log2);

	/*
	 * Retrieve dimension of cache. Note that we intentionally
	 * truncate the line size to a of power 2, because that may not be
	 * true in some archs.
	 */
	linux64_core_cache_info[linux64_core_get_id()].cache_size =
		sysconf(_SC_LEVEL1_DCACHE_SIZE);
	linux64_core_cache_info[linux64_core_get_id()].cache_size_log2 =
		linux64_log2(linux64_core_cache_info[linux64_core_get_id()].cache_size);
	linux64_core_cache_info[linux64_core_get_id()].cache_size =
		linux64_pow(2, linux64_core_cache_info[linux64_core_get_id()].cache_size_log2);
}

/*============================================================================*
 * Instruction Cache                                                          *
 *============================================================================*/

/**
 * @brief Incremement the counter, invalidate don't have sense in linux64.
 */
PUBLIC void linux64_core_icache_invalidate(void)
{
	linux64_core_cache_info[linux64_core_get_id()].icache_invalidate_count++;
}

/**
 * @brief Incremement the counter, line prefetch don't have sense in linux64.
 */
PUBLIC void linux64_core_icache_line_prefetch(void)
{
	linux64_core_cache_info[linux64_core_get_id()].icache_line_prefetch_count++;
}

/**
 * @brief Incremement the counter, line invalidate don't have sense in linux64.
 */
PUBLIC void linux64_core_icache_line_invalidate(void)
{
	linux64_core_cache_info[linux64_core_get_id()].icache_line_invalidate_count++;
}

/**
 * @brief Setup the icache of the underlying core.
 * @todo TODO: implement this function.
 */
PUBLIC void linux64_core_icache_setup(void)
{
}

/**
 * @brief Print all stats informations about the icache within the underlying core.
 * @todo TODO: this should be platform independent.
 */
PUBLIC void linux64_core_icache_dump_stats(void)
{
	kprintf("icache_invalidate = %d", linux64_core_cache_info[linux64_core_get_id()].icache_invalidate_count);
	kprintf("icache_line_prefetch = %d", linux64_core_cache_info[linux64_core_get_id()].icache_line_prefetch_count);
	kprintf("icache_line_invalidate = %d", linux64_core_cache_info[linux64_core_get_id()].icache_line_invalidate_count);
}
