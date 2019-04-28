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

/* Must come first. */
#define _ASM_FILE_

#if (defined(__or1k_cluster__))
	#include <arch/cluster/or1k-cluster/cores.h>
#elif (defined(__optimsoc_cluster__))
	#include <arch/cluster/optimsoc-cluster/cores.h>
#endif

/* Exported symbols. */
.globl core.kstack

.section .data
/*----------------------------------------------------------------------------*
 * core.kstack                                                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief Kernel stack for all cores.
 */
.align OR1K_PAGE_SIZE
core.kstack:
	.fill (OR1K_PAGE_SIZE/OR1K_PTE_SIZE)*OR1K_CLUSTER_NUM_CORES, OR1K_PTE_SIZE, 0
