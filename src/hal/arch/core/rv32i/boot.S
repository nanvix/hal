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

#define RV32I_HARTS_NUM    4
#define RV32I_STACK_SIZE   1024
#define RV32I_STACK_SHIFT  10

/* Exported symbols. */
.global _do_start
.global kstacks

.macro get_hartid dest
	csrr \dest, mhartid
.endm

/*============================================================================*
 * bootstrap section                                                          *
 *============================================================================*/

.section .text,"ax",@progbits

/*----------------------------------------------------------------------------*
 * _do_start()                                                                *
 *----------------------------------------------------------------------------*/

_do_start:

	/* Setup trap vector. */
    la   t0, _do_syscall
    csrw mtvec, t0

	/* Setup stack. */
	get_hartid t0
    slli t0, t0, RV32I_STACK_SHIFT
    la   sp, kstacks + RV32I_STACK_SIZE
    add  sp, sp, t0

    /* Halt slave harts. */
	get_hartid t0
    bnez t0, halt

	/* Jump to C code. */
	j rv32i_master_setup

	j halt

/*----------------------------------------------------------------------------*
 * halt()                                                                     *
 *----------------------------------------------------------------------------*/

/*
 * Halts instruction execution.
 */
.align 4
halt:
    wfi
    j   halt

/*============================================================================*
 * BSS                                                                        *
 *============================================================================*/

.section .bss,"aw",@nobits

.align 4
kstacks:
	.skip RV32I_STACK_SIZE * RV32I_HARTS_NUM
