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

#include <arch/core/or1k/core.h>
#include <arch/core/or1k/mmu.h>
#include <arch/cluster/or1k/cores.h>

/* Exported symbols. */
.globl kernel_pgtab
.globl kpool_pgtab
.globl dtlb_table
.globl itlb_table
.globl core.kstack
.globl ompic_pgtab
.globl uart_pgtab

.section .data
/*----------------------------------------------------------------------------*
 * kernel_pgtab                                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief Page table for kernel code and data.
 */
.align OR1K_PAGE_SIZE
kernel_pgtab:
	.fill OR1K_PAGE_SIZE/OR1K_PTE_SIZE, OR1K_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * kpool_pgtab                                                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief Page table for kernel page pool.
 */
.align OR1K_PAGE_SIZE
kpool_pgtab:
	.fill OR1K_PAGE_SIZE/OR1K_PTE_SIZE, OR1K_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * dtlb                                                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief dtlb entries
 */
.align OR1K_PAGE_SIZE
dtlb_table:
	.fill 64, OR1K_WORD_SIZE, 0

/*----------------------------------------------------------------------------*
 * itlb                                                                       *
 *----------------------------------------------------------------------------*/

/**
 * @brief itlb entries
 */
.align OR1K_PAGE_SIZE
itlb_table:
	.fill 64, OR1K_WORD_SIZE, 0

/*----------------------------------------------------------------------------*
 * core.kstack                                                                *
 *----------------------------------------------------------------------------*/

/**
 * @brief Kernel stack for all cores.
 */
.align OR1K_PAGE_SIZE
core.kstack:
	.fill (OR1K_PAGE_SIZE/OR1K_PTE_SIZE)*OR1K_NUM_CORES, OR1K_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * ompic_pgtab                                                                *
 *----------------------------------------------------------------------------*/

/*
 * OMPIC page table.
 */
.align PAGE_SIZE
ompic_pgtab:
	.fill OR1K_PAGE_SIZE/OR1K_PTE_SIZE, OR1K_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * uart_pgtab                                                                 *
 *----------------------------------------------------------------------------*/

/*
 * uart8250 page table.
 */
.align PAGE_SIZE
uart_pgtab:
	.fill OR1K_PAGE_SIZE/OR1K_PTE_SIZE, OR1K_PTE_SIZE, 0
