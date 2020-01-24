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

/* Must come first. */
#define _ASM_FILE_

#include <grub/mboot.h>
#include <arch/cluster/x86-cluster/cores.h>

/**
 * @brief Multiboot flags.
 */
#define MBOOT_FLAGS (MBOOT_PAGE_ALIGN | MBOOT_MEMORY_INFO)

/* Exported symbols. */
.globl start
.globl kstacks

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

	i486_clear_gprs

	/* Setup stack. */
	movl $0, %eax
	i486_core_stack_reset eax

	/* Core and cluster setup. */
	call x86_cluster_setup

	/* Halt execution. */
	start.halt:
		hlt
		jmp start.halt

/*----------------------------------------------------------------------------*
 * kstacks                                                                    *
 *----------------------------------------------------------------------------*/

/**
 * @brief Kernel stack for all cores.
 */
.align I486_PAGE_SIZE
kstacks:
	.skip I486_PAGE_SIZE * X86_CLUSTER_NUM_CORES
