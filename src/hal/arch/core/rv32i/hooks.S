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
	beqz t0, rv32i_do_event.excp

	/* Interrupt. */
	rv32i_do_event.int:

		/*
		 * TODO: forward interrupts and
		 * traps to high-level dispatcher.
		 */

		j rv32i_do_event.ret

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

	rv32i_do_event.ret:

		/* Retore all registers. */
		rv32i_context_restore

		/* Return from event. */
		mret
