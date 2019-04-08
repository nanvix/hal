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

#ifndef ARCH_CORE_RV32I_ASM_H_
#define ARCH_CORE_RV32I_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

	/* Must come first. */
	#define __NEED_CONTEXT

	#include <arch/core/rv32i/context.h>
	#include <arch/core/rv32i/mmu.h>

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
	 * @brief Number of preserved registers.
	 */
	#define RV32I_PRESERVED_REGS_NUM 12

	/**
	 * @brief Stack frame size for slow call.
	 */
	#define RV32I_SLOW_CALL_FRAME_SIZE \
		(RV32I_PRESERVED_REGS_NUM*RV32I_WORD_SIZE)

	/**
	* @brief Offsets to Stack Frame
	*/
	/**@{*/
	#define RV32I_FRAME_FP   0*RV32I_WORD_SIZE /**< Frame Pointer Register */
	#define RV32I_FRAME_S1   1*RV32I_WORD_SIZE /**< Saved Register  1      */
	#define RV32I_FRAME_S2   2*RV32I_WORD_SIZE /**< Saved Register  2      */
	#define RV32I_FRAME_S3   3*RV32I_WORD_SIZE /**< Saved Register  3      */
	#define RV32I_FRAME_S4   4*RV32I_WORD_SIZE /**< Saved Register  4      */
	#define RV32I_FRAME_S5   5*RV32I_WORD_SIZE /**< Saved Register  5      */
	#define RV32I_FRAME_S6   6*RV32I_WORD_SIZE /**< Saved Register  6      */
	#define RV32I_FRAME_S7   7*RV32I_WORD_SIZE /**< Saved Register  7      */
	#define RV32I_FRAME_S8   8*RV32I_WORD_SIZE /**< Saved Register  8      */
	#define RV32I_FRAME_S9   9*RV32I_WORD_SIZE /**< Saved Register  9      */
	#define RV32I_FRAME_S10 10*RV32I_WORD_SIZE /**< Saved Register 10      */
	#define RV32I_FRAME_S11 11*RV32I_WORD_SIZE /**< Saved Register 11      */
	/**@}*/

	/*
	 * @brief Saves preserved registers in the stack.
	 */
	.macro rv32i_do_prologue_slow

		/* Allocate stack frame. */
		addi sp, sp, -RV32I_SLOW_CALL_FRAME_SIZE

		/* Save preserved registers. */
		sw fp,  RV32I_FRAME_FP(sp)
		sw s1,  RV32I_FRAME_S1(sp)
		sw s2,  RV32I_FRAME_S2(sp)
		sw s3,  RV32I_FRAME_S3(sp)
		sw s4,  RV32I_FRAME_S4(sp)
		sw s5,  RV32I_FRAME_S5(sp)
		sw s6,  RV32I_FRAME_S6(sp)
		sw s7,  RV32I_FRAME_S7(sp)
		sw s8,  RV32I_FRAME_S8(sp)
		sw s9,  RV32I_FRAME_S9(sp)
		sw s10, RV32I_FRAME_S10(sp)
		sw s11, RV32I_FRAME_S11(sp)

		/* Save stack frame. */
		mv fp, sp

	.endm

	/*
	 * @brief Restores preserved registers from the stack.
	 */
	.macro rv32i_do_epilogue_slow

		/* Restore stack frame. */
		mv sp, fp

		/* Restore preserved registers. */
		lw fp,  RV32I_FRAME_FP(sp)
		lw s1,  RV32I_FRAME_S1(sp)
		lw s2,  RV32I_FRAME_S2(sp)
		lw s3,  RV32I_FRAME_S3(sp)
		lw s4,  RV32I_FRAME_S4(sp)
		lw s5,  RV32I_FRAME_S5(sp)
		lw s6,  RV32I_FRAME_S6(sp)
		lw s7,  RV32I_FRAME_S7(sp)
		lw s8,  RV32I_FRAME_S8(sp)
		lw s9,  RV32I_FRAME_S9(sp)
		lw s10, RV32I_FRAME_S10(sp)
		lw s11, RV32I_FRAME_S11(sp)

		/* Wipe out stack frame. */
		addi sp, sp, RV32I_SLOW_CALL_FRAME_SIZE

	.endm

/*============================================================================*
 * Execution Context                                                          *
 *============================================================================*/

	/*
	 * Saves the current execution context.
	 */
	.macro rv32i_context_save

		/* Allocate context structure. */
		addi sp, sp, -RV32I_CONTEXT_SIZE

		/*
		 * Save all GPRs, but the
		 * Stack Pointer (SP).
		 */
		sw  x1, RV32I_CONTEXT_RA(sp)
		sw  x3, RV32I_CONTEXT_GP(sp)
		sw  x4, RV32I_CONTEXT_TP(sp)
		sw  x5, RV32I_CONTEXT_T0(sp)
		sw  x6, RV32I_CONTEXT_T1(sp)
		sw  x7, RV32I_CONTEXT_T2(sp)
		sw  x8, RV32I_CONTEXT_FP(sp)
		sw  x9, RV32I_CONTEXT_S1(sp)
		sw x10, RV32I_CONTEXT_A0(sp)
		sw x11, RV32I_CONTEXT_A1(sp)
		sw x12, RV32I_CONTEXT_A2(sp)
		sw x13, RV32I_CONTEXT_A3(sp)
		sw x14, RV32I_CONTEXT_A4(sp)
		sw x15, RV32I_CONTEXT_A5(sp)
		sw x16, RV32I_CONTEXT_A6(sp)
		sw x17, RV32I_CONTEXT_A7(sp)
		sw x18, RV32I_CONTEXT_S2(sp)
		sw x19, RV32I_CONTEXT_S3(sp)
		sw x20, RV32I_CONTEXT_S4(sp)
		sw x21, RV32I_CONTEXT_S5(sp)
		sw x22, RV32I_CONTEXT_S6(sp)
		sw x23, RV32I_CONTEXT_S7(sp)
		sw x24, RV32I_CONTEXT_S8(sp)
		sw x25, RV32I_CONTEXT_S9(sp)
		sw x26, RV32I_CONTEXT_S10(sp)
		sw x27, RV32I_CONTEXT_S11(sp)
		sw x28, RV32I_CONTEXT_T3(sp)
		sw x29, RV32I_CONTEXT_T4(sp)
		sw x30, RV32I_CONTEXT_T5(sp)
		sw x31, RV32I_CONTEXT_T6(sp)

		/* Save Program Counter (pc)*/
	#ifndef __RV32I_MACHINE_CODE
		csrr t0, sepc
	#else
		csrr t0, mepc
	#endif
		sw   t0, RV32I_CONTEXT_PC(sp)

		/* Save Stack Pointer (SP). */
		addi t0, sp, RV32I_CONTEXT_SIZE
		sw   t0, RV32I_CONTEXT_SP(sp)
	.endm

	/*
	 * Restores a saved execution context.
	 */
	.macro rv32i_context_restore

		/* Restore Program Counter (pc)*/
		lw   t0, RV32I_CONTEXT_PC(sp)
	#ifndef __RV32I_MACHINE_CODE
		csrw sepc, t0
	#else
		csrw mepc, t0
	#endif

		/*
		 * Save all GPRs, but the
		 * Stack Pointer (SP).
		 */
		lw  x1, RV32I_CONTEXT_RA(sp)
		lw  x3, RV32I_CONTEXT_GP(sp)
		lw  x4, RV32I_CONTEXT_TP(sp)
		lw  x5, RV32I_CONTEXT_T0(sp)
		lw  x6, RV32I_CONTEXT_T1(sp)
		lw  x7, RV32I_CONTEXT_T2(sp)
		lw  x8, RV32I_CONTEXT_FP(sp)
		lw  x9, RV32I_CONTEXT_S1(sp)
		lw x10, RV32I_CONTEXT_A0(sp)
		lw x11, RV32I_CONTEXT_A1(sp)
		lw x12, RV32I_CONTEXT_A2(sp)
		lw x13, RV32I_CONTEXT_A3(sp)
		lw x14, RV32I_CONTEXT_A4(sp)
		lw x15, RV32I_CONTEXT_A5(sp)
		lw x16, RV32I_CONTEXT_A6(sp)
		lw x17, RV32I_CONTEXT_A7(sp)
		lw x18, RV32I_CONTEXT_S2(sp)
		lw x19, RV32I_CONTEXT_S3(sp)
		lw x20, RV32I_CONTEXT_S4(sp)
		lw x21, RV32I_CONTEXT_S5(sp)
		lw x22, RV32I_CONTEXT_S6(sp)
		lw x23, RV32I_CONTEXT_S7(sp)
		lw x24, RV32I_CONTEXT_S8(sp)
		lw x25, RV32I_CONTEXT_S9(sp)
		lw x26, RV32I_CONTEXT_S10(sp)
		lw x27, RV32I_CONTEXT_S11(sp)
		lw x28, RV32I_CONTEXT_T3(sp)
		lw x29, RV32I_CONTEXT_T4(sp)
		lw x30, RV32I_CONTEXT_T5(sp)
		lw x31, RV32I_CONTEXT_T6(sp)

		/* Wipe out context structure. */
		addi sp, sp, RV32I_CONTEXT_SIZE

	.endm

/*============================================================================*
 * Stack Operations                                                           *
 *============================================================================*/

	/*
	 * Pushes a 32-bit register onto the stack.
	 *   - reg Target register.
	 */
	.macro rv32i_pushw reg
		addi sp, sp, -RV32I_WORD_SIZE
		sw   \reg, 0(sp)
	.endm

	/*
	 * Pops a 32-bit register from the stack.
	 *   - reg Target register.
	 */
	.macro rv32i_popw reg
		lw   \reg, 0(sp)
		addi sp, sp, RV32I_WORD_SIZE
	.endm

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro rv32i_clear_gprs

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
	.macro rv32i_core_stack_reset coreid

		mv    t0, \coreid
		slli  t0, t0, RV32I_PAGE_SHIFT
		la    sp, kstacks + RV32I_PAGE_SIZE
		add   sp, sp, t0
		mv    fp, sp

	.endm

#endif /* ARCH_CORE_RV32I_ASM_H_ */
