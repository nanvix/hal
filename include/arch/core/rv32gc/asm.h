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

#ifndef ARCH_CORE_RV32GC_ASM_H_
#define ARCH_CORE_RV32GC_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

	/* Must come first. */
	#define __NEED_CORE_CONTEXT

	#include <arch/core/rv32gc/ctx.h>
	#include <arch/core/rv32gc/mmu.h>

/*============================================================================*
 * Regiser Aliases                                                            *
 *============================================================================*/

	/**
	 * @name Aliases for Registers
	 */
	/**@{*/
	#define fp s0 /**< Stack Pointer Register */
	/**@}*/

/*============================================================================*
 * Pseudo Instructions                                                        *
 *============================================================================*/

	/*
	 * Halts instruction execution.
	 */
	.macro halt
		1:  wfi
			j 1b
	.endm

/*============================================================================*
 * Procedure Linkage                                                          *
 *============================================================================*/

	/**
	 * @brief Number of scratch registers.
	 */
	#define RV32GC_SCRATCH_REGS_NUM 17

	/**
	 * @brief Caller stack frame size.
	 */
	#define RV32GC_CALLER_FRAME_SIZE \
		(RV32GC_SCRATCH_REGS_NUM*RV32GC_WORD_SIZE)

	/**
	 * @brief Offsets to Caller Stack Frame
	 */
	/**@{*/
	#define RV32GC_CALLER_FRAME_RA   0*RV32GC_WORD_SIZE /**< Scratch Register  0 */
	#define RV32GC_CALLER_FRAME_GP   1*RV32GC_WORD_SIZE /**< Scratch Register  1 */
	#define RV32GC_CALLER_FRAME_TP   2*RV32GC_WORD_SIZE /**< Scratch Register  2 */
	#define RV32GC_CALLER_FRAME_T0   3*RV32GC_WORD_SIZE /**< Scratch Register  3 */
	#define RV32GC_CALLER_FRAME_T1   4*RV32GC_WORD_SIZE /**< Scratch Register  4 */
	#define RV32GC_CALLER_FRAME_T2   5*RV32GC_WORD_SIZE /**< Scratch Register  5 */
	#define RV32GC_CALLER_FRAME_T3   6*RV32GC_WORD_SIZE /**< Scratch Register  6 */
	#define RV32GC_CALLER_FRAME_T4   7*RV32GC_WORD_SIZE /**< Scratch Register  7 */
	#define RV32GC_CALLER_FRAME_T5   8*RV32GC_WORD_SIZE /**< Scratch Register  8 */
	#define RV32GC_CALLER_FRAME_T6   9*RV32GC_WORD_SIZE /**< Scratch Register  9 */
	#define RV32GC_CALLER_FRAME_A1  10*RV32GC_WORD_SIZE /**< Scratch Register 10 */
	#define RV32GC_CALLER_FRAME_A2  11*RV32GC_WORD_SIZE /**< Scratch Register 11 */
	#define RV32GC_CALLER_FRAME_A3  12*RV32GC_WORD_SIZE /**< Scratch Register 12 */
	#define RV32GC_CALLER_FRAME_A4  13*RV32GC_WORD_SIZE /**< Scratch Register 13 */
	#define RV32GC_CALLER_FRAME_A5  14*RV32GC_WORD_SIZE /**< Scratch Register 14 */
	#define RV32GC_CALLER_FRAME_A6  15*RV32GC_WORD_SIZE /**< Scratch Register 15 */
	#define RV32GC_CALLER_FRAME_A7  16*RV32GC_WORD_SIZE /**< Scratch Register 16 */
	/**@}*/

	.macro rv32gc_do_fn_call_save function
		add  sp, sp, -RV32GC_CALLER_FRAME_SIZE

		sw  ra, RV32GC_CALLER_FRAME_RA(sp)
		sw  gp, RV32GC_CALLER_FRAME_GP(sp)
		sw  tp, RV32GC_CALLER_FRAME_TP(sp)
		sw  t0, RV32GC_CALLER_FRAME_T0(sp)
		sw  t1, RV32GC_CALLER_FRAME_T1(sp)
		sw  t2, RV32GC_CALLER_FRAME_T2(sp)
		sw  t3, RV32GC_CALLER_FRAME_T3(sp)
		sw  t4, RV32GC_CALLER_FRAME_T4(sp)
		sw  t5, RV32GC_CALLER_FRAME_T5(sp)
		sw  t6, RV32GC_CALLER_FRAME_T6(sp)
		sw  a1, RV32GC_CALLER_FRAME_A1(sp)
		sw  a2, RV32GC_CALLER_FRAME_A2(sp)
		sw  a3, RV32GC_CALLER_FRAME_A3(sp)
		sw  a4, RV32GC_CALLER_FRAME_A4(sp)
		sw  a5, RV32GC_CALLER_FRAME_A5(sp)
		sw  a6, RV32GC_CALLER_FRAME_A6(sp)
		sw  a7, RV32GC_CALLER_FRAME_A7(sp)
	.endm

	.macro rv32gc_do_fn_call_restore function

		lw  ra, RV32GC_CALLER_FRAME_RA(sp)
		lw  gp, RV32GC_CALLER_FRAME_GP(sp)
		lw  tp, RV32GC_CALLER_FRAME_TP(sp)
		lw  t0, RV32GC_CALLER_FRAME_T0(sp)
		lw  t1, RV32GC_CALLER_FRAME_T1(sp)
		lw  t2, RV32GC_CALLER_FRAME_T2(sp)
		lw  t3, RV32GC_CALLER_FRAME_T3(sp)
		lw  t4, RV32GC_CALLER_FRAME_T4(sp)
		lw  t5, RV32GC_CALLER_FRAME_T5(sp)
		lw  t6, RV32GC_CALLER_FRAME_T6(sp)
		lw  a1, RV32GC_CALLER_FRAME_A1(sp)
		lw  a2, RV32GC_CALLER_FRAME_A2(sp)
		lw  a3, RV32GC_CALLER_FRAME_A3(sp)
		lw  a4, RV32GC_CALLER_FRAME_A4(sp)
		lw  a5, RV32GC_CALLER_FRAME_A5(sp)
		lw  a6, RV32GC_CALLER_FRAME_A6(sp)
		lw  a7, RV32GC_CALLER_FRAME_A7(sp)

		add  sp, sp, RV32GC_CALLER_FRAME_SIZE
	.endm

	.macro rv32gc_do_fn_call function
		rv32gc_do_fn_call_save

		call \function

		rv32gc_do_fn_call_restore
	.endm

	/**
	 * @brief Number of saved registers.
	 */
	#define RV32GC_SAVED_REGS_NUM 12

	/**
	 * @brief Callee stack frame size.
	 */
	#define RV32GC_CALLEE_FRAME_SIZE \
		(RV32GC_SAVED_REGS_NUM*RV32GC_WORD_SIZE)

	/**
	 * @brief Offsets to Callee Stack Frame
	 */
	/**@{*/
	#define RV32GC_CALLE_FRAME_FP   0*RV32GC_WORD_SIZE /**< Saved Register  0 */
	#define RV32GC_CALLE_FRAME_S1   1*RV32GC_WORD_SIZE /**< Saved Register  1 */
	#define RV32GC_CALLE_FRAME_S2   2*RV32GC_WORD_SIZE /**< Saved Register  2 */
	#define RV32GC_CALLE_FRAME_S3   3*RV32GC_WORD_SIZE /**< Saved Register  3 */
	#define RV32GC_CALLE_FRAME_S4   4*RV32GC_WORD_SIZE /**< Saved Register  4 */
	#define RV32GC_CALLE_FRAME_S5   5*RV32GC_WORD_SIZE /**< Saved Register  5 */
	#define RV32GC_CALLE_FRAME_S6   6*RV32GC_WORD_SIZE /**< Saved Register  6 */
	#define RV32GC_CALLE_FRAME_S7   7*RV32GC_WORD_SIZE /**< Saved Register  7 */
	#define RV32GC_CALLE_FRAME_S8   8*RV32GC_WORD_SIZE /**< Saved Register  8 */
	#define RV32GC_CALLE_FRAME_S9   9*RV32GC_WORD_SIZE /**< Saved Register  9 */
	#define RV32GC_CALLE_FRAME_S10 10*RV32GC_WORD_SIZE /**< Saved Register 10 */
	#define RV32GC_CALLE_FRAME_S11 11*RV32GC_WORD_SIZE /**< Saved Register 11 */
	/**@}*/

	/*
	 * @brief Saves saved registers in the stack.
	 */
	.macro rv32gc_do_prologue

		/* Allocate stack frame. */
		addi sp, sp, -RV32GC_CALLEE_FRAME_SIZE

		/* Save saved registers. */
		sw fp,  RV32GC_CALLE_FRAME_FP(sp)
		sw s1,  RV32GC_CALLE_FRAME_S1(sp)
		sw s2,  RV32GC_CALLE_FRAME_S2(sp)
		sw s3,  RV32GC_CALLE_FRAME_S3(sp)
		sw s4,  RV32GC_CALLE_FRAME_S4(sp)
		sw s5,  RV32GC_CALLE_FRAME_S5(sp)
		sw s6,  RV32GC_CALLE_FRAME_S6(sp)
		sw s7,  RV32GC_CALLE_FRAME_S7(sp)
		sw s8,  RV32GC_CALLE_FRAME_S8(sp)
		sw s9,  RV32GC_CALLE_FRAME_S9(sp)
		sw s10, RV32GC_CALLE_FRAME_S10(sp)
		sw s11, RV32GC_CALLE_FRAME_S11(sp)

		/* Save stack frame. */
		mv fp, sp

	.endm

	/*
	 * @brief Restores saved registers from the stack.
	 */
	.macro rv32gc_do_epilogue

		/* Restore stack frame. */
		mv sp, fp

		/* Restore saved registers. */
		lw fp,  RV32GC_CALLE_FRAME_FP(sp)
		lw s1,  RV32GC_CALLE_FRAME_S1(sp)
		lw s2,  RV32GC_CALLE_FRAME_S2(sp)
		lw s3,  RV32GC_CALLE_FRAME_S3(sp)
		lw s4,  RV32GC_CALLE_FRAME_S4(sp)
		lw s5,  RV32GC_CALLE_FRAME_S5(sp)
		lw s6,  RV32GC_CALLE_FRAME_S6(sp)
		lw s7,  RV32GC_CALLE_FRAME_S7(sp)
		lw s8,  RV32GC_CALLE_FRAME_S8(sp)
		lw s9,  RV32GC_CALLE_FRAME_S9(sp)
		lw s10, RV32GC_CALLE_FRAME_S10(sp)
		lw s11, RV32GC_CALLE_FRAME_S11(sp)

		/* Wipe out stack frame. */
		addi sp, sp, RV32GC_CALLEE_FRAME_SIZE

	.endm

/*============================================================================*
 * Execution Context                                                          *
 *============================================================================*/

	/*
	 * Saves the current execution context.
	 */
	.macro rv32gc_context_save

		/* Allocate context structure. */
		addi sp, sp, -RV32GC_CONTEXT_SIZE

		/*
		 * Save all GPRs, but the
		 * Stack Pointer (SP).
		 */
		sw  x1, RV32GC_CONTEXT_RA(sp)
		sw  x3, RV32GC_CONTEXT_GP(sp)
		sw  x4, RV32GC_CONTEXT_TP(sp)
		sw  x5, RV32GC_CONTEXT_T0(sp)
		sw  x6, RV32GC_CONTEXT_T1(sp)
		sw  x7, RV32GC_CONTEXT_T2(sp)
		sw  x8, RV32GC_CONTEXT_FP(sp)
		sw  x9, RV32GC_CONTEXT_S1(sp)
		sw x10, RV32GC_CONTEXT_A0(sp)
		sw x11, RV32GC_CONTEXT_A1(sp)
		sw x12, RV32GC_CONTEXT_A2(sp)
		sw x13, RV32GC_CONTEXT_A3(sp)
		sw x14, RV32GC_CONTEXT_A4(sp)
		sw x15, RV32GC_CONTEXT_A5(sp)
		sw x16, RV32GC_CONTEXT_A6(sp)
		sw x17, RV32GC_CONTEXT_A7(sp)
		sw x18, RV32GC_CONTEXT_S2(sp)
		sw x19, RV32GC_CONTEXT_S3(sp)
		sw x20, RV32GC_CONTEXT_S4(sp)
		sw x21, RV32GC_CONTEXT_S5(sp)
		sw x22, RV32GC_CONTEXT_S6(sp)
		sw x23, RV32GC_CONTEXT_S7(sp)
		sw x24, RV32GC_CONTEXT_S8(sp)
		sw x25, RV32GC_CONTEXT_S9(sp)
		sw x26, RV32GC_CONTEXT_S10(sp)
		sw x27, RV32GC_CONTEXT_S11(sp)
		sw x28, RV32GC_CONTEXT_T3(sp)
		sw x29, RV32GC_CONTEXT_T4(sp)
		sw x30, RV32GC_CONTEXT_T5(sp)
		sw x31, RV32GC_CONTEXT_T6(sp)

		/* Save Program Counter (pc)*/
	#ifndef __RV32GC_MACHINE_CODE
		csrr t0, sepc
	#else
		csrr t0, mepc
	#endif
		sw   t0, RV32GC_CONTEXT_PC(sp)

		/* Save Stack Pointer (SP). */
		addi t0, sp, RV32GC_CONTEXT_SIZE
		sw   t0, RV32GC_CONTEXT_SP(sp)
	.endm

	/*
	 * Restores a saved execution context.
	 */
	.macro rv32gc_context_restore

		/* Restore Program Counter (pc)*/
		lw   t0, RV32GC_CONTEXT_PC(sp)
	#ifndef __RV32GC_MACHINE_CODE
		csrw sepc, t0
	#else
		csrw mepc, t0
	#endif

		/*
		 * Restore all GPRs, but the
		 * Stack Pointer (SP).
		 */
		lw  x1, RV32GC_CONTEXT_RA(sp)
		lw  x3, RV32GC_CONTEXT_GP(sp)
		lw  x4, RV32GC_CONTEXT_TP(sp)
		lw  x5, RV32GC_CONTEXT_T0(sp)
		lw  x6, RV32GC_CONTEXT_T1(sp)
		lw  x7, RV32GC_CONTEXT_T2(sp)
		lw  x8, RV32GC_CONTEXT_FP(sp)
		lw  x9, RV32GC_CONTEXT_S1(sp)
		lw x10, RV32GC_CONTEXT_A0(sp)
		lw x11, RV32GC_CONTEXT_A1(sp)
		lw x12, RV32GC_CONTEXT_A2(sp)
		lw x13, RV32GC_CONTEXT_A3(sp)
		lw x14, RV32GC_CONTEXT_A4(sp)
		lw x15, RV32GC_CONTEXT_A5(sp)
		lw x16, RV32GC_CONTEXT_A6(sp)
		lw x17, RV32GC_CONTEXT_A7(sp)
		lw x18, RV32GC_CONTEXT_S2(sp)
		lw x19, RV32GC_CONTEXT_S3(sp)
		lw x20, RV32GC_CONTEXT_S4(sp)
		lw x21, RV32GC_CONTEXT_S5(sp)
		lw x22, RV32GC_CONTEXT_S6(sp)
		lw x23, RV32GC_CONTEXT_S7(sp)
		lw x24, RV32GC_CONTEXT_S8(sp)
		lw x25, RV32GC_CONTEXT_S9(sp)
		lw x26, RV32GC_CONTEXT_S10(sp)
		lw x27, RV32GC_CONTEXT_S11(sp)
		lw x28, RV32GC_CONTEXT_T3(sp)
		lw x29, RV32GC_CONTEXT_T4(sp)
		lw x30, RV32GC_CONTEXT_T5(sp)
		lw x31, RV32GC_CONTEXT_T6(sp)

		/* Wipe out context structure. */
		addi sp, sp, RV32GC_CONTEXT_SIZE

	.endm

/*============================================================================*
 * Stack Operations                                                           *
 *============================================================================*/

	/*
	 * Pushes a 32-bit register onto the stack.
	 *   - reg Target register.
	 */
	.macro rv32gc_pushw reg
		addi sp, sp, -RV32GC_WORD_SIZE
		sw   \reg, 0(sp)
	.endm

	/*
	 * Pops a 32-bit register from the stack.
	 *   - reg Target register.
	 */
	.macro rv32gc_popw reg
		lw   \reg, 0(sp)
		addi sp, sp, RV32GC_WORD_SIZE
	.endm

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro rv32gc_clear_gprs

		ori x1,  x0, 0; ori x2,  x0, 0; ori x3,  x0, 0; ori x4,  x0, 0;
		ori x5,  x0, 0; ori x6,  x0, 0; ori x7,  x0, 0; ori x8,  x0, 0;
		ori x9,  x0, 0; ori x10, x0, 0; ori x11, x0, 0; ori x12, x0, 0;
		ori x13, x0, 0; ori x14, x0, 0; ori x15, x0, 0; ori x16, x0, 0;
		ori x17, x0, 0; ori x18, x0, 0; ori x19, x0, 0; ori x20, x0, 0;
		ori x21, x0, 0; ori x22, x0, 0; ori x23, x0, 0; ori x24, x0, 0;
		ori x25, x0, 0; ori x26, x0, 0; ori x27, x0, 0; ori x28, x0, 0;
		ori x29, x0, 0; ori x30, x0, 0; ori x31, x0, 0;

	.endm

	/*
	 * Resets the stack.
	 * - coreid ID of the calling core
	 */
	.macro rv32gc_core_stack_reset coreid

		mv    t0, \coreid
		slli  t0, t0, RV32GC_PAGE_SHIFT
		la    sp, kstacks + RV32GC_PAGE_SIZE
		add   sp, sp, t0
		mv    fp, sp

	.endm

#endif /* ARCH_CORE_RV32GC_ASM_H_ */
