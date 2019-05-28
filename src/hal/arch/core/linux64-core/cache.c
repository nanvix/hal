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
 * @brief Cache line size in bytes.
 */
PRIVATE unsigned int linux64_cache_line_size = 0;
/**
 * @brief Cache line size log2 in bytes.
 */
PRIVATE unsigned int linux64_cache_line_size_log2 = 0;

/**
 * @brief Cache size in bytes.
 */
PRIVATE unsigned int linux64_cache_size = 0;

/**
 * @brief Cache line size log2 in bytes.
 */
PRIVATE unsigned int linux64_cache_size_log2 = 0;

/**
 * @brief Initialize all the cache variables
 */
PUBLIC void linux64_core_cache_init(void)
{
	linux64_cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
	linux64_cache_line_size_log2 = log2((double) linux64_cache_line_size);
	linux64_cache_line_size = pow(2, linux64_cache_line_size_log2);
	
	linux64_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);
	linux64_cache_size_log2 = log2((double) linux64_cache_size);
	linux64_cache_size = pow(2, linux64_cache_size_log2);
}

/**
 * @brief Get the cache line size
 */
PUBLIC unsigned int get_linux64_cache_line_size(void)
{
	return linux64_cache_line_size;
}

/**
 * @brief Get the cache line size log2
 */
PUBLIC unsigned int get_linux64_cache_line_size_log2(void)
{
	return linux64_cache_line_size_log2;
}

/**
 * @brief Get the cache size
 */
PUBLIC unsigned int get_linux64_cache_size(void)
{
    return linux64_cache_size;
}

/**
 * @brief Get the cache size log2
 */
PUBLIC unsigned int get_linux64_cache_size_log2(void)
{
    return linux64_cache_size_log2;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_invalidate_count = 0;

/**
 * @note The linux64 core features cache coherency.
 */
PUBLIC void dcache_invalidate(void)
{
	dcache_invalidate_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_flush_count = 0;

/**
 * @brief Flushes changes of the data cache into memory
 */
PUBLIC void dcache_flush(void)
{
	dcache_flush_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_fence_count = 0;

/**
 * @brief Wait for ongoing flush operations in the data cache to finish
 */
PUBLIC void dcache_fence(void)
{
	dcache_fence_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_prefetch_line_count = 0;

/**
 * @brief Prefetches a line of the data cache
 */
PUBLIC void dcache_prefetch_line(void)
{
	dcache_prefetch_line_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_invalidate_line_count = 0;

/**
 * @brief Invalidates a line of the data cache
 */
PUBLIC void dcache_invalidate_line(void)
{
	dcache_invalidate_line_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int dcache_setup_count = 0;

/**
 * @brief Enables the data cache
 */
PUBLIC void dcache_setup(void)
{
	dcache_setup_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int icache_invalidate_count = 0;

/**
 * @brief Invalidates the instruction cache
 */
PUBLIC void icache_invalidate(void)
{
	icache_invalidate_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int icache_prefetch_line_count = 0;

/**
 * @brief Prefetches a line of the instruction cache
 */
PUBLIC void icache_prefetch_line(void)
{
	icache_prefetch_line_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int icache_invalidate_line_count = 0;

/**
 * @brief Invalidates a line of the instruction cache
 */
PUBLIC void icache_invalidate_line(void)
{
	icache_invalidate_line_count++;
}

/**
 * @brief A counter to know how many time the dcache invalidate is called
 */
PRIVATE unsigned int icache_setup_count = 0;

/**
 * @brief Enables the instruction cache
 */
PUBLIC void icache_setup(void)
{
	icache_setup_count++;
}

/**
 * @brief Display all the cache variables
 */
PUBLIC void linux64_core_cache_print(void)
{
	kprintf("cache_line_size = %dB", get_linux64_cache_line_size());
	kprintf("cache_line_size_log2 = %dB", get_linux64_cache_line_size_log2());
	kprintf("cache_size = %dB", get_linux64_cache_size());
	kprintf("cache_size_log2 = %dB", get_linux64_cache_size_log2());
}

/**
 * @brief Display all the cache counters
 */
PUBLIC void linux64_core_cache_count_print(void)
{
	kprintf("dcache_invalidate_count = %d", dcache_invalidate_count);
	kprintf("dcache_flush_count = %d", dcache_flush_count);
	kprintf("dcache_fence_count = %d", dcache_fence_count);
	kprintf("dcache_prefetch_line_count = %d", dcache_prefetch_line_count);
	kprintf("dcache_invalidate_line_count = %d", dcache_invalidate_line_count);
	kprintf("dcache_setup_count = %d", dcache_setup_count);
	kprintf("icache_invalidate_count = %d", icache_invalidate_count);
	kprintf("icache_prefetch_line_count = %d", icache_prefetch_line_count);
	kprintf("icache_invalidate_line_count = %d", icache_invalidate_line_count);
	kprintf("icache_setup_count = %d", icache_setup_count);
}