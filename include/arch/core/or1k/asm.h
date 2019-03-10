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

	#include <arch/core/or1k/regs.h>
	#include <arch/core/or1k/context.h>

	/**
	 * @name Aliases for Registers
	 */
	/**@{*/
	#define sp r1 /**< Stack Pointer Pointer        */
	#define bp r2 /**< Stack Base Pointer Register  */
	/**@}*/

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
 * Prologue                                                                   *
 *============================================================================*/

	.macro _do_prologue

		/* Save stack and base pointer. */
		OR1K_EXCEPTION_STORE_SP
		OR1K_EXCEPTION_STORE_GPR2

	.endm

/*============================================================================*
 * Epilogue                                                                   *
 *============================================================================*/

	.macro _do_epilogue

	.endm

/*============================================================================*
 * or1k_context_save()                                                        *
 *============================================================================*/

	/*
	 * Saves the current execution context.
	 */
	.macro or1k_context_save dest

		/* Save GPRs, except SP, BP and scratch registers r3 ... r6. */
		l.sw OR1K_CONTEXT_R0(\dest),   r0
		l.sw OR1K_CONTEXT_R7(\dest),   r7
		l.sw OR1K_CONTEXT_R8(\dest),   r8
		l.sw OR1K_CONTEXT_R9(\dest),   r9
		l.sw OR1K_CONTEXT_R10(\dest), r10
		l.sw OR1K_CONTEXT_R11(\dest), r11
		l.sw OR1K_CONTEXT_R12(\dest), r12
		l.sw OR1K_CONTEXT_R13(\dest), r13
		l.sw OR1K_CONTEXT_R14(\dest), r14
		l.sw OR1K_CONTEXT_R15(\dest), r15
		l.sw OR1K_CONTEXT_R16(\dest), r16
		l.sw OR1K_CONTEXT_R17(\dest), r17
		l.sw OR1K_CONTEXT_R18(\dest), r18
		l.sw OR1K_CONTEXT_R19(\dest), r19
		l.sw OR1K_CONTEXT_R20(\dest), r20
		l.sw OR1K_CONTEXT_R21(\dest), r21
		l.sw OR1K_CONTEXT_R22(\dest), r22
		l.sw OR1K_CONTEXT_R23(\dest), r23
		l.sw OR1K_CONTEXT_R24(\dest), r24
		l.sw OR1K_CONTEXT_R25(\dest), r25
		l.sw OR1K_CONTEXT_R26(\dest), r26
		l.sw OR1K_CONTEXT_R27(\dest), r27
		l.sw OR1K_CONTEXT_R28(\dest), r28
		l.sw OR1K_CONTEXT_R29(\dest), r29
		l.sw OR1K_CONTEXT_R30(\dest), r30
		l.sw OR1K_CONTEXT_R31(\dest), r31

		/* Stack Pointer, r1. */
		OR1K_EXCEPTION_LOAD_SP(r3)
		l.sw OR1K_CONTEXT_R1(\dest),   r3

		/* Frame pointer. */
		OR1K_EXCEPTION_LOAD_GPR2(r3)
		l.sw OR1K_CONTEXT_R2(\dest),   r3

		/* EPCR. */
		l.mfspr r3, r0, OR1K_SPR_EPCR_BASE
		l.sw OR1K_CONTEXT_EPCR(\dest), r3

		/* EEAR. */
		l.mfspr r3, r0, OR1K_SPR_EEAR_BASE
		l.sw OR1K_CONTEXT_EEAR(\dest), r3

		/* ESR. */
		l.mfspr r3, r0, OR1K_SPR_ESR_BASE
		l.sw OR1K_CONTEXT_ESR(\dest),  r3

		/* Scratch registers: r3 ... r6. */
		OR1K_EXCEPTION_LOAD_GPR3(r3)
		l.sw OR1K_CONTEXT_R3(\dest), r3

		OR1K_EXCEPTION_LOAD_GPR4(r3)
		l.sw OR1K_CONTEXT_R4(\dest), r3

		OR1K_EXCEPTION_LOAD_GPR5(r3)
		l.sw OR1K_CONTEXT_R5(\dest), r3

		OR1K_EXCEPTION_LOAD_GPR6(r3)
		l.sw OR1K_CONTEXT_R6(\dest), r3

	.endm

/*============================================================================*
 * or1k_context_restore()                                                     *
 *============================================================================*/

	/*
	 * Restores an execution context.
	 */
	.macro or1k_context_restore src

		/* Load GPRs. */
		l.lwz bp,  OR1K_CONTEXT_R2(\src)
		l.lwz r4,  OR1K_CONTEXT_R4(\src)
		l.lwz r5,  OR1K_CONTEXT_R5(\src)
		l.lwz r6,  OR1K_CONTEXT_R6(\src)
		l.lwz r7,  OR1K_CONTEXT_R7(\src)
		l.lwz r8,  OR1K_CONTEXT_R8(\src)
		l.lwz r9,  OR1K_CONTEXT_R9(\src)
		l.lwz r10, OR1K_CONTEXT_R10(\src)
		l.lwz r11, OR1K_CONTEXT_R11(\src)
		l.lwz r12, OR1K_CONTEXT_R12(\src)
		l.lwz r13, OR1K_CONTEXT_R13(\src)
		l.lwz r14, OR1K_CONTEXT_R14(\src)
		l.lwz r15, OR1K_CONTEXT_R15(\src)
		l.lwz r16, OR1K_CONTEXT_R16(\src)
		l.lwz r17, OR1K_CONTEXT_R17(\src)
		l.lwz r18, OR1K_CONTEXT_R18(\src)
		l.lwz r19, OR1K_CONTEXT_R19(\src)
		l.lwz r20, OR1K_CONTEXT_R20(\src)
		l.lwz r21, OR1K_CONTEXT_R21(\src)
		l.lwz r22, OR1K_CONTEXT_R22(\src)
		l.lwz r23, OR1K_CONTEXT_R23(\src)
		l.lwz r24, OR1K_CONTEXT_R24(\src)
		l.lwz r25, OR1K_CONTEXT_R25(\src)
		l.lwz r26, OR1K_CONTEXT_R26(\src)
		l.lwz r27, OR1K_CONTEXT_R27(\src)
		l.lwz r28, OR1K_CONTEXT_R28(\src)
		l.lwz r29, OR1K_CONTEXT_R29(\src)
		l.lwz r30, OR1K_CONTEXT_R30(\src)
		l.lwz r31, OR1K_CONTEXT_R31(\src)

		/* Special Purpose Registers: EPCR, EEAR and ESR. */
		l.lwz   r3, OR1K_CONTEXT_EPCR(\src)
		l.mtspr r0, r3, OR1K_SPR_EPCR_BASE

		l.lwz   r3, OR1K_CONTEXT_EEAR(\src)
		l.mtspr r0, r3, OR1K_SPR_EEAR_BASE

		l.lwz   r3, OR1K_CONTEXT_ESR(\src)
		l.mtspr r0, r3, OR1K_SPR_ESR_BASE

		/* Last scratch register, r3. */
		l.lwz r3, OR1K_CONTEXT_R3(\src)

		/* Stack Pointer, r1. */
		l.lwz sp, OR1K_CONTEXT_R1(\src)

	.endm

#endif /* ARCH_CORE_OR1K_ASM_H_ */
