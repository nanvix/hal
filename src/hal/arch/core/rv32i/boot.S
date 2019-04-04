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
#define __NEED_CORE_TYPES

#include <arch/core/rv32i/asm.h>
#include <arch/core/rv32i/types.h>

#define RV32I_HARTS_NUM    4
#define RV32I_STACK_SIZE   1024
#define RV32I_STACK_SHIFT  10

/* Exported symbols. */
.global _do_start
.global kstacks

/*============================================================================*
 * bootstrap section                                                          *
 *============================================================================*/

.section .text,"ax",@progbits

/*----------------------------------------------------------------------------*
 * _do_start()                                                                *
 *----------------------------------------------------------------------------*/

/*
 * Entry point.
 */
.align RV32I_WORD_SIZE
_do_start:

	rv32i_clear_gprs

	/* Setup trap vector. */
    la   t0, rv32i_early_ivt
    csrw mtvec, t0

	/* Setup stack. */
	csrr t0, mhartid
    slli t0, t0, RV32I_STACK_SHIFT
    la   sp, kstacks + RV32I_STACK_SIZE
    add  sp, sp, t0

    /* Halt slave harts. */
	csrr t0, mhartid
    bnez t0, halt

	/* Jump to C code. */
	j rv32i_master_setup

	j halt

/*----------------------------------------------------------------------------*
 * Early Interrupt Vector Table                                               *
 *----------------------------------------------------------------------------*/

/*
 * Early interrupt vector table.
 */
.align RV32I_WORD_SIZE
rv32i_early_ivt:
	j rv32i_early_ivt

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

.align RV32I_WORD_SIZE
kstacks:
	.skip RV32I_STACK_SIZE * RV32I_HARTS_NUM
