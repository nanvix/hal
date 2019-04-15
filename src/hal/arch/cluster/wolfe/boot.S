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
#define __NEED_CLUSTER_WOLFE

#include <arch/cluster/wolfe.h>

/* Exported symbols. */
.global _do_start

/*============================================================================*
 * bootstrap section                                                          *
 *============================================================================*/

.section .text,"ax",@progbits

/*
 * Skip 128 bytes. Some firmwares assume this offset.
 */
.org 0x80

/*----------------------------------------------------------------------------*
 * _do_start()                                                                *
 *----------------------------------------------------------------------------*/

/*
 * Entry point.
 */
.align RI5CY_WORD_SIZE
_do_start:

	/* Clear BSS. */
	la   t0, __BSS_START
	la   t1, __BSS_END
	1: sw   x0 ,0(t0)
	   addi t0, t0, RI5CY_WORD_SIZE
	   bltu t0, t1, 1b

	ri5cy_clear_gprs

	/* Get ID of current core. */
	csrr  s1, mhartid

	/* Reset stack. */
	andi t0, s1, RI5CY_MHARTID_COREID_MASK
	ri5cy_stack_reset wolfe_stacks, WOLFE_STACK_SIZE_LOG2, t0

    /* Halt slave cores. */
	li    t1, (WOLFE_CLUSTERID_MASTER | WOLFE_COREID_MASTER)
	beq   s1, t1, _do_start.master

	/* Slave Core */
	_do_start.slave:
		halt

	/* Master Core */
	_do_start.master:
		call wolfe_master_setup
		halt

/*============================================================================*
 * BSS                                                                        *
 *============================================================================*/

.section .bss,"aw",@nobits

.align RI5CY_WORD_SIZE
wolfe_stacks:
	.skip WOLFE_STACK_SIZE*WOLFE_NUM_CORES
