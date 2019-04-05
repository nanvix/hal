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

#include <grub/mboot.h>
#include <arch/core/i486/core.h>
#include <arch/core/i486/mmu.h>

/**
 * @brief Multiboot flags.
 */
#define MBOOT_FLAGS (MBOOT_PAGE_ALIGN | MBOOT_MEMORY_INFO)

/* Exported symbols. */
.globl start
.globl i486_root_pgdir
.globl i486_kernel_pgtab
.globl i486_kpool_pgtab

/*============================================================================*
 *                              bootstrap section                             *
 *============================================================================*/

.section .bootstrap

/*----------------------------------------------------------------------------*
 * multiboot_header                                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief Multiboot header.
 */
.align 4
mboot_header:
    .long  MBOOT_MAGIC                  /* Magic number.              */
    .long  MBOOT_FLAGS                  /* Flags.                     */
    .long  -(MBOOT_MAGIC + MBOOT_FLAGS) /* Checksum.                  */
    .long  mboot_header                 /* Pointer to this structure. */

/*----------------------------------------------------------------------------*
 * start()                                                                    *
 *----------------------------------------------------------------------------*/

/*
 * Kernel entry point.
 */
start:

	/* Build kernel page tables. */
	start.kernel.init:
		movl $i486_kernel_pgtab, %edi
		movl $0x00000000 + 7, %eax
		start.kernel.init.loop:
			stosl
			addl $I486_PAGE_SIZE, %eax
			cmpl $i486_kernel_pgtab + I486_PAGE_SIZE, %edi
			jl start.kernel.init.loop

	/* Build kernel pool tables. */
	start.kpool.init:
		movl $i486_kpool_pgtab, %edi
		movl $0x01000000 + 7, %eax
		start.kpool.init.loop:
			stosl
			addl $I486_PAGE_SIZE, %eax
			cmpl $i486_kpool_pgtab + I486_PAGE_SIZE, %edi
			jl start.kpool.init.loop

	/*
	 * Build initial page directory.
	 *   - Kernel code + data at 0x00000000
	 *   - Kernel code + data at 0xc0000000
	 *   - Kernel page pool at 0xc1000000
	 */
	start.kpgdir.init:
		movl $i486_kernel_pgtab + 3, i486_root_pgdir + I486_PTE_SIZE*0
		movl $i486_kernel_pgtab + 3, i486_root_pgdir + I486_PTE_SIZE*768
		movl $i486_kpool_pgtab  + 3, i486_root_pgdir + I486_PTE_SIZE*772

	/* Enable paging. */
	start.paging.enable:
		movl $i486_root_pgdir, %eax
		movl %eax, %cr3
		movl %cr0, %eax
		orl $0x80000000, %eax
		movl %eax, %cr0

	/* Setup stack. */
	start.kstack.setup:
		movl $core0.kstack + I486_PAGE_SIZE - I486_WORD_SIZE, %ebp
		movl $core0.kstack + I486_PAGE_SIZE - I486_WORD_SIZE, %esp

	call i486_core_setup

	call kmain

	/* Halt execution. */
	start.halt:
		hlt
		jmp start.halt

/*----------------------------------------------------------------------------*
 * i486_kernel_pgtab                                                          *
 *----------------------------------------------------------------------------*/

/**
 * @brief Page table for kernel code and data.
 */
.align I486_PAGE_SIZE
i486_kernel_pgtab:
	.fill I486_PAGE_SIZE/I486_PTE_SIZE, I486_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * i486_kpool_pgtab                                                           *
 *----------------------------------------------------------------------------*/

/**
 * @brief Page table for kernel page pool.
 */
.align I486_PAGE_SIZE
i486_kpool_pgtab:
	.fill I486_PAGE_SIZE/I486_PTE_SIZE, I486_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * i486_root_pgdir                                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief Page directory of idle process.
 */
.align I486_PAGE_SIZE
i486_root_pgdir:
	.fill I486_PAGE_SIZE/I486_PTE_SIZE, I486_PTE_SIZE, 0

/*----------------------------------------------------------------------------*
 * core0.kstack                                                               *
 *----------------------------------------------------------------------------*/

/**
 * @brief Kernel stack for core 0.
 */
.align I486_PAGE_SIZE
core0.kstack:
	.fill I486_PAGE_SIZE/I486_PTE_SIZE, I486_PTE_SIZE, 0
