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

/*
 * OpenRISC head.S
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

/* Must come first. */
#define _ASM_FILE_
#define __NEED_OR1K_REGS

#include <arch/core/or1k/asm.h>
#include <arch/core/or1k/core.h>
#include <arch/core/or1k/mmu.h>
#include <arch/core/or1k/regs.h>
#include <arch/core/or1k/excp.h>
#include <arch/core/or1k/int.h>

/*===========================================================================*
 * _do_handler() macros                                                      *
 *===========================================================================*/

.macro _do_handler address handler handler_number

.org \address

	/* Backup scratch registers. */
	OR1K_EXCEPTION_STORE_GPR3
	OR1K_EXCEPTION_STORE_GPR4
	OR1K_EXCEPTION_STORE_GPR5
	OR1K_EXCEPTION_STORE_GPR6

	/* Save handler number. */
	l.ori r6, r0, \handler_number

	/* Jump into \handler. */
	l.j \handler
	l.nop

.endm

/*============================================================================*
 * hardcoded hw exceptions                                                    *
 *============================================================================*/


.section .bootstrap,"ax",@progbits

/*
 * In OpenRISC architecture, all the exceptions are hardcoded to a very
 * specific addresses.
 */

_do_handler 0x100, _do_start, OR1K_EXCEPTION_RESET               /* Reset.                  */
_do_handler 0x200, _do_excp,  OR1K_EXCEPTION_BUSERROR            /* Bus error.              */
_do_handler 0x300, _do_excp,  OR1K_EXCEPTION_PAGE_FAULT          /* Data page fault.        */
_do_handler 0x400, _do_excp,  OR1K_EXCEPTION_PAGE_FAULT          /* Instruction page fault. */
_do_handler 0x500, _do_hwint, OR1K_INT_CLOCK                     /* Tick timer.             */
_do_handler 0x600, _do_excp,  OR1K_EXCEPTION_ALIGNMENT           /* Alignment.              */
_do_handler 0x700, _do_excp,  OR1K_EXCEPTION_ILLEGAL_INSTRUCTION /* Illegal Instruction.    */
_do_handler 0x800, _do_hwint, OR1K_INT_EXTERNAL                  /* External Interrupt.     */
_do_handler 0x900, _do_excp,  OR1K_EXCEPTION_DTLB_FAULT          /* DTLB Fault.             */
_do_handler 0xa00, _do_excp,  OR1K_EXCEPTION_ITLB_FAULT          /* ITLB Fault.             */
_do_handler 0xb00, _do_excp,  OR1K_EXCEPTION_RANGE               /* Range.                  */
_do_handler 0xc00, _syscall,  0                                  /* Syscall.                */
_do_handler 0xd00, _do_excp,  OR1K_EXCEPTION_FLOAT_POINT         /* Floating point.         */
_do_handler 0xe00, _do_excp,  OR1K_EXCEPTION_TRAP                /* Trap.                   */

/*===========================================================================*
 * _do_excp()                                                                *
 *===========================================================================*/

/*
 * Exception hook.
 */
_do_excp:
	_do_prologue
	redzone_alloc

	/*
	 * Save execution context
	 * in the current stack.
	 */
	l.addi sp, sp, -OR1K_CONTEXT_SIZE
	or1k_context_save sp

	/*
	 * Save a reference to execution context
	 * into r4 (second parameter). The high-level
	 * exception handler dispatcher will need it.
	 *
	 * r4 should not be used as a scratch register
	 * from this point, until do_excp.
	 */
	l.ori  r4, sp, 0

	/*
	 * Save exception information
	 * in the current stack.
	 */
	l.addi sp, sp, -OR1K_EXCEPTION_SIZE
	l.sw OR1K_EXCEPTION_NUM (sp), r6

	l.mfspr r3, r0, OR1K_SPR_EEAR_BASE
	l.sw OR1K_EXCEPTION_EEAR(sp), r3
	
	l.mfspr r3, r0, OR1K_SPR_EPCR_BASE
	l.sw OR1K_EXCEPTION_EPCR(sp), r3

	/*
	 * Save a reference to exception information
	 * into r3 (first parameter). The high-level
	 * exception handler dispatcher will need it.
	 *
	 * r3 should not be used as a scratch register
	 * from this point, until do_excp.
	 */
	l.ori  r3, sp, 0

	/*
	 * Call exception dispatcher.
	 */
	l.jal do_excp
	l.nop

	/*
	 * Wipe out exception information
	 * from current stack.
	 */
	l.addi sp, sp, OR1K_EXCEPTION_SIZE

	/*
	 * Restore saved execution context.
	 */
	or1k_context_restore sp

	_do_epilogue

	l.rfe
	l.nop

/*===========================================================================*
 * _do_hwint()                                                               *
 *===========================================================================*/

/*
 * Interrupt hook.
 */
_do_hwint:
	_do_prologue
	redzone_alloc

	/*
	 * Save execution context
	 * in the current stack.
	 */
	l.addi sp, sp, -OR1K_CONTEXT_SIZE
	or1k_context_save sp

	/*
	 * Save a reference to execution context
	 * into r4 (second parameter). The high-level
	 * interrupt handler will need it.
	 *
	 * r4 should not be used as a scratch register
	 * from this point, until do_hwint.
	 */
	l.ori r4, sp, 0

	/*
	 * Save a reference to interrupt information
	 * into r3 (first parameter). The high-level
	 * interrupt handler will need it.
	 *
	 * r3 should not be used as a scratch register
	 * from this point, until do_hwint.
	 */
	l.or r3, r0, r6

	/*
	 * Call interrupt handler.
	 */
	l.jal or1k_do_hwint
	l.nop

	/*
	 * Restore saved execution context.
	 */
	or1k_context_restore sp

	_do_epilogue

	l.rfe
	l.nop

/*===========================================================================*
 * _syscall()                                                                *
 *===========================================================================*/

/*
 * System call hook.
 */
_syscall:
	_do_prologue
	redzone_alloc

	/*
	 * Save execution context
	 * in the current stack.
	 */
	l.addi sp, sp, -OR1K_CONTEXT_SIZE
	or1k_context_save sp

	/*
	 * Syscall parameters.
	 */
	l.lwz r3, OR1K_CONTEXT_R13(sp) /* arg6. */
	l.sw -8(sp), r3
	l.lwz r3, OR1K_CONTEXT_R11(sp) /* syscall_nr */
	l.sw -4(sp), r3

	l.lwz r8, OR1K_CONTEXT_R8(sp)  /* arg5. */
	l.lwz r7, OR1K_CONTEXT_R7(sp)  /* arg4. */
	l.lwz r6, OR1K_CONTEXT_R6(sp)  /* arg3. */
	l.lwz r5, OR1K_CONTEXT_R5(sp)  /* arg2. */
	l.lwz r4, OR1K_CONTEXT_R4(sp)  /* arg1. */
	l.lwz r3, OR1K_CONTEXT_R3(sp)  /* arg0. */

	l.addi sp, sp, -8

	/*
	 * Call syscall handler.
	 */
	l.jal do_syscall
	l.nop
	l.addi sp, sp, 8

	/* Copy return value to the user stack. */
	l.sw OR1K_CONTEXT_R11(sp), r11

	/*
	 * Restore saved execution context.
	 */
	or1k_context_restore sp

	_do_epilogue

	l.rfe
	l.nop

/*===========================================================================*
 * _do_start()                                                               *
 *===========================================================================*/

/*
 * The very initial beggining of the OpenRISC processor is here, so it's
 * important to leave the registers in an initial state before the
 * start() is called.
 */

_do_start:
	l.ori r1, r0,  0; l.ori r2,  r0, 0; l.ori r3,  r0, 0; l.ori r4,  r0, 0;
	l.ori r5, r0,  0; l.ori r6,  r0, 0; l.ori r7,  r0, 0; l.ori r8,  r0, 0;
	l.ori r9, r0,  0; l.ori r10, r0, 0; l.ori r11, r0, 0; l.ori r12, r0, 0;
	l.ori r13, r0, 0; l.ori r14, r0, 0; l.ori r15, r0, 0; l.ori r16, r0, 0;
	l.ori r17, r0, 0; l.ori r18, r0, 0; l.ori r19, r0, 0; l.ori r20, r0, 0;
	l.ori r21, r0, 0; l.ori r22, r0, 0; l.ori r23, r0, 0; l.ori r24, r0, 0;
	l.ori r25, r0, 0; l.ori r26, r0, 0; l.ori r27, r0, 0; l.ori r28, r0, 0;
	l.ori r29, r0, 0; l.ori r30, r0, 0; l.ori r31, r0, 0;

	l.jal start
	l.nop
