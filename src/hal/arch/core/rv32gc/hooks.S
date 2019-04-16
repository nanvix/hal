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

#include <arch/core/rv32gc/asm.h>
#include <arch/core/rv32gc/excp.h>
#include <arch/core/rv32gc/types.h>
#include <arch/core/rv32gc/regs.h>

.global rv32gc_do_strap

.section .text,"ax",@progbits

/*----------------------------------------------------------------------------*
 * Event Handler                                                              *
 *----------------------------------------------------------------------------*/

/*
 * Low-level dispatcher for interrupts, exceptions and interrupts.
 */
.align RV32GC_WORD_SIZE
rv32gc_do_strap:

	/* Save all registers. */
	rv32gc_context_save

	/*
	 * Save pointer to
	 * context structure.
	 */
	csrw sscratch, sp

	/*
	 * Get reference to
	 * context structure.
	 */
	csrr s1, sscratch

	/* Parse trap. */
	csrr  s2, scause
	li    t0, RV32GC_SCAUSE_INT
	and   t0, s2, t0
	bnez  t0, rv32gc_do_strap.sint
	li    t0, RV32GC_SCAUSE_CAUSE
	and   t0, s2, t0
	li    t1, RV32GC_SCAUSE_ECALL_UMODE
	beq   t0, t1, rv32gc_do_strap.scall

	/* Exceptions. */
	rv32gc_do_strap.excp:

		/*
		 * Allocate exception
		 * information structure.
		 */
		addi sp, sp, -RV32GC_EXCP_SIZE

		/*
		 * Build exception
		 * information structure.
		 */
		andi  t0, s2, RV32GC_SCAUSE_CAUSE
		sw    t0, RV32GC_EXCP_NR(sp)
		csrr  t0, stval
		sw    t0, RV32GC_EXCP_ADDR(sp)
		csrr  t0, sepc
		sw    t0, RV32GC_EXCP_INSTR(sp)

		/*
		 * Call high-level
		 * exception dispatcher.
		 */
		mv   a0, sp           # struct exception *excp
		mv   a1, s1           # struct context *ctx
		call do_exception

		/*
		 * Wipe out exception
		 * information structure.
		 */
		addi sp, sp, RV32GC_EXCP_SIZE

		j rv32gc_do_strap.ret

	/* Kernel call. */
	rv32gc_do_strap.scall:


#ifdef RISCV_HAS_UMODE

		/*
		 * Get system call parameters.
		 * Note that the system call
		 * number is the last  paramenter.
		 */
		lw a5, RV32GC_CONTEXT_A0(fp) /* System Call Number */
		lw a0, RV32GC_CONTEXT_A1(fp) /* Argument 0         */
		lw a1, RV32GC_CONTEXT_A2(fp) /* Argument 1         */
		lw a2, RV32GC_CONTEXT_A3(fp) /* Argument 2         */
		lw a3, RV32GC_CONTEXT_A4(fp) /* Argument 3         */
		lw a4, RV32GC_CONTEXT_A5(fp) /* Argument 4         */

		/* Call high-level dispatcher. */
		call do_syscall

		/* Copy return value to the user stack. */
		sw a0, RV32GC_CONTEXT_A0(fp)

		/*
		 * Advance PC to next instruction,
		 * otherwise another supervisor
		 * call will be triggered.
		 */
		lw t0, RV32GC_CONTEXT_PC(fp)
		add t0, t0, RV32GC_WORD_SIZE
		sw t0, RV32GC_CONTEXT_PC(fp)

		j rv32gc_do_strap.ret

#endif

	/* Interrupt. */
	rv32gc_do_strap.sint:

		/*
		 * Call high-level
		 * interrupt dispatcher.
		 */
		andi a0, s2, RV32GC_SCAUSE_CAUSE  # int intnum
		mv   a1, s1                      # struct context *ctx
		call do_interrupt

	rv32gc_do_strap.ret:

		/* Retore all registers. */
		csrr sp, sscratch
		rv32gc_context_restore

		/* Return from event. */
		sret
