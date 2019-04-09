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
#define __NEED_CONTEXT
#define __NEED_CORE_REGS
#define __NEED_CORE_TYPES
#define __RV32I_MACHINE_CODE


#include <arch/core/rv32i/asm.h>
#include <arch/core/rv32i/context.h>
#include <arch/core/rv32i/types.h>
#include <arch/core/rv32i/regs.h>

#define RV32I_HARTS_NUM    4
#define RV32I_STACK_SIZE   4096
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
    la   t0, rv32i_do_mtrap
    csrw mtvec, t0

	/* Setup stack. */
	csrr t0, mhartid
    slli t0, t0, RV32I_STACK_SHIFT
    la   sp, kstacks + RV32I_STACK_SIZE
    add  sp, sp, t0

    /* Halt slave harts. */
	csrr t0, mhartid
    bnez t0, halt

	/* Enter supervisor mode. */
	la a0, _do_start.supervisor
	call rv32i_machine_setup

	/* Hopefully, we never get here. */
	j halt

_do_start.supervisor:
	j rv32i_master_setup

	j halt

/*----------------------------------------------------------------------------*
 * Machine Interrupt Vector Table                                             *
 *----------------------------------------------------------------------------*/

/*
 * Low-level dispatcher for machine interrupts and exceptions.
 */
.align RV32I_WORD_SIZE
rv32i_do_mtrap:

	/* Save all registers. */
	rv32i_context_save

	/*
	 * Save pointer to
	 * context structure.
	 */
	csrw mscratch, sp

	/*
	 * Raise privilege level
	 * to machine mode.
	 */
	call rv32i_prv_raise

	/*
	 * Get reference to
	 * context structure.
	 */
	csrr a0, mscratch

	/* Parse trap cause. */
	csrr  t0, mcause
	li    t1, RV32I_MCAUSE_INT
	and   t1, t0, t1
	bnez  t1, rv32i_do_mtrap.mint

	/* Exceptions and machine calls. */
	rv32i_do_mtrap.mexcp:
		li    t1, RV32I_MCAUSE_CAUSE
		and   t1, t0, t1             # t1 = cause
		slli  t1, t1, 2              # t1 = cause*sizeof(uintptr_t)
		la    t2, mtrap_table        # t2 = mtrap_table
		add   t1, t1, t2
		lw    t1, 0(t1)              # t1 = mtrap_table[cause]
		jalr  t1
		j     rv32i_do_mtrap.out

	/* Interrupts. */
	rv32i_do_mtrap.mint:

		/* Parse interrupt number. */
		li    t1, RV32I_MCAUSE_CAUSE
		and   t1, t0, t1             # t1 = cause

		/* Timer interrupt? */
		li    t2, RV32I_MCAUSE_MTI
		bne  t1, t2, rv32i_do_mtrap.out
			li    t1, RV32I_MIE_MTIE
			csrc  mie, t1
			li    t1, RV32I_MIP_STIP
			csrs  mip, t1
			csrs  sip, t1

	/* Return. */
	rv32i_do_mtrap.out:

		/*
		 * Drop privilege level
		 * to previous value.
		 */
		call rv32i_prv_drop

		/* Restore all registers. */
		csrr sp, mscratch
		rv32i_context_restore

		/*
		 * Return from
		 * where we came.
		 */
		mret

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
 * DATA                                                                       *
 *============================================================================*/

.section .data

mtrap_table:
	.word rv32i_do_mexcp /**< Instruction Address Misaligned        */
	.word rv32i_do_mexcp /**< Instruction Access Fault              */
	.word rv32i_do_mexcp /**< Illegal Instruction                   */
	.word rv32i_do_mexcp /**< Breakpoint                            */
	.word rv32i_do_mexcp /**< Load Address Misaligned               */
	.word rv32i_do_mexcp /**< Load Access Fault                     */
	.word rv32i_do_mexcp /**< Store/AMO Address Misaligned          */
	.word rv32i_do_mexcp /**< Store/AMO Access Fault                */
	.word rv32i_do_mcall /**< Environment Call from User Mode       */
	.word rv32i_do_mcall /**< Environment Call from Supervisor Mode */
	.word rv32i_do_mbad  /**< Reserved                              */
	.word rv32i_do_mcall /**< Environment Call from Machine Mode    */
	.word rv32i_do_mexcp /**< Instruction Page Fault                */
	.word rv32i_do_mexcp /**< Load Page Fault                       */
	.word rv32i_do_mbad  /**< Reserved                              */
	.word rv32i_do_mexcp /**< Store/AMO Page Fault                  */

/*============================================================================*
 * BSS                                                                        *
 *============================================================================*/

.section .bss,"aw",@nobits

.align RV32I_WORD_SIZE
kstacks:
	.skip RV32I_STACK_SIZE * RV32I_HARTS_NUM