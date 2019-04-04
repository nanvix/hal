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

#include <arch/core/rv32i/asm.h>
#include <arch/core/rv32i/excp.h>
#include <arch/core/rv32i/types.h>
#include <arch/core/rv32i/regs.h>

.global rv32i_do_event

.section .text,"ax",@progbits

/*----------------------------------------------------------------------------*
 * Event Handler                                                              *
 *----------------------------------------------------------------------------*/

/*
 * Low-level dispatcher for interrupts, exceptions and interrupts.
 */
.align RV32I_WORD_SIZE
rv32i_do_event:

	/* Save all registers. */
	rv32i_context_save

	/* Get reference to context structure. */
	mv s1, sp

	csrr s2, mcause

	/* Parse event. */
	li   t0, RV32I_MCAUSE_INT
	and  t0, s2, t0
	beqz t0, rv32i_do_event.sync

	/* Interrupt. */
	rv32i_do_event.async:

		/*
		 * Call high-level
		 * interrupt dispatcher.
		 */
		andi a0, s2, RV32I_MCAUSE_CAUSE
		mv   a1, s1
		call rv32i_do_int

		j rv32i_do_event.ret

	/* Synchronous event. */
	rv32i_do_event.sync:

		li  t0, RV32I_MCAUSE_CAUSE
		and t0, s2, t0
		li  t1, RV32I_MCAUSE_SYNC_ECALL_MMODE
		beq t0, t1, rv32i_do_event.syscall
		li  t1, RV32I_MCAUSE_SYNC_ECALL_SMODE
		beq t0, t1, rv32i_do_event.syscall
		li  t1, RV32I_MCAUSE_SYNC_ECALL_UMODE
		beq t0, t1, rv32i_do_event.syscall

	/* Event. */
	rv32i_do_event.excp:

		/*
		 * Allocate exception
		 * information structure.
		 */
		addi sp, sp, -RV32I_EXCP_SIZE

		/*
		 *  Build exception
		 * information structure.
		 */
		andi t0, s2, RV32I_MCAUSE_CAUSE
		sw   t0, RV32I_EXCP_ID(sp)
		csrr t0, mtval
		sw   t0, RV32I_EXCP_ADDR(sp)
		csrr t0, mepc
		sw   t0, RV32I_EXCP_INSTR(sp)

		/*
		 * Call high-level
		 * exception dispatcher.
		 */
		mv   a0, sp
		mv   a1, s1
		call rv32i_do_excp

		/*
		 * Wipe out exception
		 * information structure.
		 */
		addi sp, sp, RV32I_EXCP_SIZE

		j rv32i_do_event.ret

	/* Kernel call. */
	rv32i_do_event.syscall:

		/*
		 * Get system call parameters.
		 *  Note that the system call
		 * number is the last  paramenter.
		 */
		lw a5, RV32I_CONTEXT_A0(fp) /* System Call Number */
		lw a0, RV32I_CONTEXT_A1(fp) /* Argument 0         */
		lw a1, RV32I_CONTEXT_A2(fp) /* Argument 1         */
		lw a2, RV32I_CONTEXT_A3(fp) /* Argument 2         */
		lw a3, RV32I_CONTEXT_A4(fp) /* Argument 3         */
		lw a4, RV32I_CONTEXT_A5(fp) /* Argument 4         */

		/* Call high-level dispatcher. */
		call do_syscall

		/* Copy return value to the user stack. */
		sw a0, RV32I_CONTEXT_A0(fp)

		/*
		 * Advance PC to next instruction,
		 * otherwise another supervisor
		 * call will be triggered.
		 */
		lw t0, RV32I_CONTEXT_PC(fp)
		add t0, t0, RV32I_WORD_SIZE
		sw t0, RV32I_CONTEXT_PC(fp)

	rv32i_do_event.ret:

		/* Retore all registers. */
		rv32i_context_restore

		/* Return from event. */
		mret
