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

#ifndef ARCH_CORE_OR1K_ASM_H_
#define ARCH_CORE_OR1K_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

	/* Must come first. */
	#define __NEED_OR1K_REGS
	#define __NEED_CORE_CONTEXT

	#include <arch/core/or1k/regs.h>
	#include <arch/core/or1k/ctx.h>

/*============================================================================*
 * Register Aliases                                                           *
 *============================================================================*/

	/**
	 * @name Aliases for Registers
	 */
	/**@{*/
	#define sp r1 /**< Stack Pointer Pointer        */
	#define bp r2 /**< Stack Base Pointer Register  */
	/**@}*/

/*============================================================================*
 * Pseudo Instructions                                                        *
 *============================================================================*/

	/*
	 * Unconditional jump with delay slot.
	 */
	.macro jump N
		l.j \N
		l.nop
	.endm

	/*
	 * Halts instruction execution.
	 */
	.macro halt
		1:
			l.j 1b
			l.nop
	.endm

/*============================================================================*
 * Red Zone                                                                   *
 *============================================================================*/

	/**
	 * @brief Size of red zone (in bytes).
	 */
	#define REDZONE_SIZE 128

	/*
	 * Allocates the red zone in the current stack frame.
	 */
	.macro redzone_alloc
		l.addi sp, sp, -REDZONE_SIZE
	.endm

	/*
	 * Frees the red zone in the current stack frame.
	 */
	.macro redzone_free
		l.addi sp, sp, REDZONE_SIZE
	.endm

/*============================================================================*
 * Procedure Linkage                                                          *
 *============================================================================*/

	/**
	 * @brief Stack frame size for slow call.
	 */
	#define SLOW_CALL_STACK_FRAME_SIZE 48

	/**
	* @brief Offsets to Stack Frame
	*/
	/**@{*/
	#define STACK_FRAME_BP   0 /**< Base Pointer.  */
	#define STACK_FRAME_R9   4 /**< Link Register. */
	#define STACK_FRAME_R10  8 /**< r10.           */
	#define STACK_FRAME_R14 12 /**< r14.           */
	#define STACK_FRAME_R16 16 /**< r16.           */
	#define STACK_FRAME_R18 20 /**< r18.           */
	#define STACK_FRAME_R20 24 /**< r20.           */
	#define STACK_FRAME_R22 28 /**< r22.           */
	#define STACK_FRAME_R24 32 /**< r24.           */
	#define STACK_FRAME_R26 36 /**< r26.           */
	#define STACK_FRAME_R28 40 /**< r28.           */
	#define STACK_FRAME_R30 44 /**< r30.           */
	/**@}*/

	/*
	 * @brief Saves preserved registers.
	 */
	.macro _do_prologue

		/* Save stack and base pointer. */
		OR1K_EXCEPTION_STORE_SP
		OR1K_EXCEPTION_STORE_GPR2

	.endm

	.macro _do_epilogue

	.endm

	/*
	 * @brief Saves preserved registers in the stack.
	 */
	.macro _do_prologue_slow
		/*
		 * In order to avoid 'or1k_skip_prologue' issues with GDB
		 * the following nop's are needed.
		 */
		l.nop
		l.nop

		/* Allocate stack frame. */
		l.addi sp, sp, -SLOW_CALL_STACK_FRAME_SIZE

		/* Save preserved registers. */
		l.sw STACK_FRAME_BP (sp), bp
		l.sw STACK_FRAME_R9 (sp), r9
		l.sw STACK_FRAME_R10(sp), r10
		l.sw STACK_FRAME_R14(sp), r14
		l.sw STACK_FRAME_R16(sp), r16
		l.sw STACK_FRAME_R18(sp), r18
		l.sw STACK_FRAME_R20(sp), r20
		l.sw STACK_FRAME_R22(sp), r22
		l.sw STACK_FRAME_R24(sp), r24
		l.sw STACK_FRAME_R26(sp), r26
		l.sw STACK_FRAME_R28(sp), r28
		l.sw STACK_FRAME_R30(sp), r30

		/* Change stack frame. */
		l.ori bp, sp, 0
	.endm

	/*
	 * @brief Restores preserved registers from the stack.
	 */
	.macro _do_epilogue_slow

		/* Restore stack frame. */
		l.ori sp, bp, 0

		/* Restore preserved registers. */
		l.lwz bp,  STACK_FRAME_BP (sp)
		l.lwz r9,  STACK_FRAME_R9 (sp)
		l.lwz r10, STACK_FRAME_R10(sp)
		l.lwz r14, STACK_FRAME_R14(sp)
		l.lwz r16, STACK_FRAME_R16(sp)
		l.lwz r18, STACK_FRAME_R18(sp)
		l.lwz r20, STACK_FRAME_R20(sp)
		l.lwz r22, STACK_FRAME_R22(sp)
		l.lwz r24, STACK_FRAME_R24(sp)
		l.lwz r26, STACK_FRAME_R26(sp)
		l.lwz r28, STACK_FRAME_R28(sp)
		l.lwz r30, STACK_FRAME_R30(sp)

		/* Wipe out stack frame. */
		l.addi sp, sp, SLOW_CALL_STACK_FRAME_SIZE

	.endm


/*============================================================================*
 * or1k_context_save()                                                        *
 *============================================================================*/

	/*
	 * Saves the current execution context.
	 */
	.macro or1k_context_save

		/* Save GPRs, except SP, BP and scratch registers r3 ... r6. */
		l.sw OR1K_CONTEXT_R0(sp),   r0
		l.sw OR1K_CONTEXT_R7(sp),   r7
		l.sw OR1K_CONTEXT_R8(sp),   r8
		l.sw OR1K_CONTEXT_R9(sp),   r9
		l.sw OR1K_CONTEXT_R10(sp), r10
		l.sw OR1K_CONTEXT_R11(sp), r11
		l.sw OR1K_CONTEXT_R12(sp), r12
		l.sw OR1K_CONTEXT_R13(sp), r13
		l.sw OR1K_CONTEXT_R14(sp), r14
		l.sw OR1K_CONTEXT_R15(sp), r15
		l.sw OR1K_CONTEXT_R16(sp), r16
		l.sw OR1K_CONTEXT_R17(sp), r17
		l.sw OR1K_CONTEXT_R18(sp), r18
		l.sw OR1K_CONTEXT_R19(sp), r19
		l.sw OR1K_CONTEXT_R20(sp), r20
		l.sw OR1K_CONTEXT_R21(sp), r21
		l.sw OR1K_CONTEXT_R22(sp), r22
		l.sw OR1K_CONTEXT_R23(sp), r23
		l.sw OR1K_CONTEXT_R24(sp), r24
		l.sw OR1K_CONTEXT_R25(sp), r25
		l.sw OR1K_CONTEXT_R26(sp), r26
		l.sw OR1K_CONTEXT_R27(sp), r27
		l.sw OR1K_CONTEXT_R28(sp), r28
		l.sw OR1K_CONTEXT_R29(sp), r29
		l.sw OR1K_CONTEXT_R30(sp), r30
		l.sw OR1K_CONTEXT_R31(sp), r31

		/* Stack Pointer, r1. */
		OR1K_EXCEPTION_LOAD_SP(r3)
		l.sw OR1K_CONTEXT_R1(sp),   r3

		/* Frame pointer. */
		OR1K_EXCEPTION_LOAD_GPR2(r3)
		l.sw OR1K_CONTEXT_R2(sp),   r3

		/* EPCR. */
		l.mfspr r3, r0, OR1K_SPR_EPCR_BASE
		l.sw OR1K_CONTEXT_EPCR(sp), r3

		/* EEAR. */
		l.mfspr r3, r0, OR1K_SPR_EEAR_BASE
		l.sw OR1K_CONTEXT_EEAR(sp), r3

		/* ESR. */
		l.mfspr r3, r0, OR1K_SPR_ESR_BASE
		l.sw OR1K_CONTEXT_ESR(sp),  r3

		/* Scratch registers: r3 ... r6. */
		OR1K_EXCEPTION_LOAD_GPR3(r3)
		l.sw OR1K_CONTEXT_R3(sp), r3

		OR1K_EXCEPTION_LOAD_GPR4(r3)
		l.sw OR1K_CONTEXT_R4(sp), r3

		OR1K_EXCEPTION_LOAD_GPR5(r3)
		l.sw OR1K_CONTEXT_R5(sp), r3

		OR1K_EXCEPTION_LOAD_GPR6(r3)
		l.sw OR1K_CONTEXT_R6(sp), r3

	.endm

/*============================================================================*
 * or1k_context_restore()                                                     *
 *============================================================================*/

	/*
	 * Restores an execution context.
	 */
	.macro or1k_context_restore

		/* Load GPRs. */
		l.lwz bp,  OR1K_CONTEXT_R2(sp)
		l.lwz r4,  OR1K_CONTEXT_R4(sp)
		l.lwz r5,  OR1K_CONTEXT_R5(sp)
		l.lwz r6,  OR1K_CONTEXT_R6(sp)
		l.lwz r7,  OR1K_CONTEXT_R7(sp)
		l.lwz r8,  OR1K_CONTEXT_R8(sp)
		l.lwz r9,  OR1K_CONTEXT_R9(sp)
		l.lwz r10, OR1K_CONTEXT_R10(sp)
		l.lwz r11, OR1K_CONTEXT_R11(sp)
		l.lwz r12, OR1K_CONTEXT_R12(sp)
		l.lwz r13, OR1K_CONTEXT_R13(sp)
		l.lwz r14, OR1K_CONTEXT_R14(sp)
		l.lwz r15, OR1K_CONTEXT_R15(sp)
		l.lwz r16, OR1K_CONTEXT_R16(sp)
		l.lwz r17, OR1K_CONTEXT_R17(sp)
		l.lwz r18, OR1K_CONTEXT_R18(sp)
		l.lwz r19, OR1K_CONTEXT_R19(sp)
		l.lwz r20, OR1K_CONTEXT_R20(sp)
		l.lwz r21, OR1K_CONTEXT_R21(sp)
		l.lwz r22, OR1K_CONTEXT_R22(sp)
		l.lwz r23, OR1K_CONTEXT_R23(sp)
		l.lwz r24, OR1K_CONTEXT_R24(sp)
		l.lwz r25, OR1K_CONTEXT_R25(sp)
		l.lwz r26, OR1K_CONTEXT_R26(sp)
		l.lwz r27, OR1K_CONTEXT_R27(sp)
		l.lwz r28, OR1K_CONTEXT_R28(sp)
		l.lwz r29, OR1K_CONTEXT_R29(sp)
		l.lwz r30, OR1K_CONTEXT_R30(sp)
		l.lwz r31, OR1K_CONTEXT_R31(sp)

		/* Special Purpose Registers: EPCR, EEAR and ESR. */
		l.lwz   r3, OR1K_CONTEXT_EPCR(sp)
		l.mtspr r0, r3, OR1K_SPR_EPCR_BASE

		l.lwz   r3, OR1K_CONTEXT_EEAR(sp)
		l.mtspr r0, r3, OR1K_SPR_EEAR_BASE

		l.lwz   r3, OR1K_CONTEXT_ESR(sp)
		l.mtspr r0, r3, OR1K_SPR_ESR_BASE

		/* Last scratch register, r3. */
		l.lwz r3, OR1K_CONTEXT_R3(sp)

		/* Stack Pointer, r1. */
		l.lwz sp, OR1K_CONTEXT_R1(sp)

	.endm

/*============================================================================*
 * Stack Operations                                                           *
 *============================================================================*/

	/*
	 * Pushes a 32-bit register onto the stack.
	 *   - reg Target register.
	 */
	.macro or1k_pushw reg
		l.addi sp, sp, -OR1K_WORD_SIZE
		l.sw 0(sp), \reg
	.endm

	/*
	 * Pops a 32-bit register from the stack.
	 *   - reg Target register.
	 */
	.macro or1k_popw reg
		l.lwz \reg, 0(sp)
		l.addi sp, sp, OR1K_WORD_SIZE
	.endm

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro or1k_clear_gprs

		l.ori r1, r0,  0; l.ori r2,  r0, 0; l.ori r3,  r0, 0; l.ori r4,  r0, 0;
		l.ori r5, r0,  0; l.ori r6,  r0, 0; l.ori r7,  r0, 0; l.ori r8,  r0, 0;
		l.ori r9, r0,  0; l.ori r10, r0, 0; l.ori r11, r0, 0; l.ori r12, r0, 0;
		l.ori r13, r0, 0; l.ori r14, r0, 0; l.ori r15, r0, 0; l.ori r16, r0, 0;
		l.ori r17, r0, 0; l.ori r18, r0, 0; l.ori r19, r0, 0; l.ori r20, r0, 0;
		l.ori r21, r0, 0; l.ori r22, r0, 0; l.ori r23, r0, 0; l.ori r24, r0, 0;
		l.ori r25, r0, 0; l.ori r26, r0, 0; l.ori r27, r0, 0; l.ori r28, r0, 0;
		l.ori r29, r0, 0; l.ori r30, r0, 0; l.ori r31, r0, 0;

	.endm

	/*
	 * Resets the stack.
	 * - coreid ID of the calling core
	 */
	.macro or1k_core_stack_reset coreid

		OR1K_LOAD_SYMBOL_2_GPR(r1, core.kstack)
		l.or   r3, r0, \coreid
		l.addi r3, r3, 1
		l.slli r3, r3, OR1K_PAGE_SHIFT
		l.add  r1, r1, r3
		l.addi r1, r1, -OR1K_WORD_SIZE /* Stack pointer. */
		l.or   r2, r1, r0              /* Frame pointer. */

	.endm

#endif /* ARCH_CORE_OR1K_ASM_H_ */
