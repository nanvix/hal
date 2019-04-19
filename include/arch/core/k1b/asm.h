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

#ifndef ARCH_CORE_K1B_ASM_H_
#define ARCH_CORE_K1B_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_CORE_CONTEXT

	#include <arch/core/k1b/types.h>
	#include <arch/core/k1b/ctx.h>

	/**
	 * @name Aliases for Registers
	 */
	/**@{*/
	#define sp r12 /**< Stack Pointer Pointer              */
	#define bp r13 /**< Stack Base Pointer Register        */
	#define pi pcr /**< Processing Identification Register */
	/**@}*/

/*============================================================================*
 * Red Zone                                                                   *
 *============================================================================*/

	/**
	 * @brief Size of red zone (in bytes).
	 */
	#define REDZONE_SIZE 16

	/*
	 * Allocates the red zone in the current stack frame.
	 */
	.macro redzone_alloc
		add $sp = $sp, -REDZONE_SIZE
		;;
	.endm

	/*
	 * Frees the red zone in the current stack frame.
	 */
	.macro redzone_free
		add $sp = $sp, REDZONE_SIZE
		;;
	.endm

/*============================================================================*
 * Procedure Linkage                                                          *
 *============================================================================*/

	/**
	 * @brief Stack frame size for slow call.
	 */
	#define CALL_STACK_FRAME_SIZE 80

	/**
	* @brief Offsets to Stack Frame
	*/
	/**@{*/
	#define STACK_FRAME_RA   0 /**< ra        */
	#define STACK_FRAME_BP   4 /**< bp        */
	#define STACK_FRAME_R10  8 /**< r10       */
	#define STACK_FRAME_R15 12 /**< r15       */
	#define STACK_FRAME_P16 16 /**< r16 + r17 */
	#define STACK_FRAME_P18 24 /**< r18 + r19 */
	#define STACK_FRAME_P20 32 /**< r20 + r21 */
	#define STACK_FRAME_P22 40 /**< r22 + r40 */
	#define STACK_FRAME_P24 48 /**< r24 + r25 */
	#define STACK_FRAME_P26 56 /**< r26 + r27 */
	#define STACK_FRAME_P28 64 /**< r28 + r29 */
	#define STACK_FRAME_P30 72 /**< r30 + r31 */
	/**@}*/

	/*
	 * @brief Saves preserved registers.
	 *
	 * @param s0 Scratch register to use.
	 */
	.macro _do_prologue

		/* Allocate stack frame. */
		add $sp, $sp, -CALL_STACK_FRAME_SIZE
		;;

		/* Save registers preserved registers. */
		sd STACK_FRAME_P30[$sp], $p30
		;;
		sd STACK_FRAME_P28[$sp], $p28
		;;
		sd STACK_FRAME_P26[$sp], $p26
		;;
		sd STACK_FRAME_P24[$sp], $p24
		;;
		sd STACK_FRAME_P22[$sp], $p22
		;;
		sd STACK_FRAME_P20[$sp], $p20
		;;
		sd STACK_FRAME_P18[$sp], $p18
		;;
		sd STACK_FRAME_P16[$sp], $p16
		;;
		sw STACK_FRAME_R15[$sp], $r15
		;;
		sw STACK_FRAME_R10[$sp], $r10
		;;

		/*
		 * Save RA + BP registers.
		 * Note that temporarily we use
		 * BP as a scratch register.
		 */
		sw STACK_FRAME_BP[$sp], $bp
		;;
		get $bp, $ra
		;;
		sw STACK_FRAME_RA[$sp], $bp
		;;

		/* Change stack frame. */
		copy $bp, $sp
		;;

	.endm

	/*
	 * @brief Restores preserved registers.
	 *
	 * @param s0 Scratch register to use.
	 */
	.macro _do_epilogue

		/* Restore stack frame. */
		copy $sp, $bp
		;;

		/*
		 * Restore BP + RA registers.
		 * Note  that temporarily we use
		 * BP as a scratch register.
		 */
		lw  $bp, STACK_FRAME_RA[$sp]
		;;
		set $ra, $bp
		;;
		lw  $bp, STACK_FRAME_BP[$sp]
		;;

		/* Restore preserved registers. */
		lw $r10, STACK_FRAME_R10[$sp]
		;;
		lw $r15, STACK_FRAME_R15[$sp]
		;;
		ld $p16, STACK_FRAME_P16[$sp]
		;;
		ld $p18, STACK_FRAME_P18[$sp]
		;;
		ld $p20, STACK_FRAME_P20[$sp]
		;;
		ld $p22, STACK_FRAME_P22[$sp]
		;;
		ld $p24, STACK_FRAME_P24[$sp]
		;;
		ld $p26, STACK_FRAME_P26[$sp]
		;;
		ld $p28, STACK_FRAME_P28[$sp]
		;;
		ld $p30, STACK_FRAME_P30[$sp]
		;;

		/* Wipe out stack frame. */
		add $sp, $sp, CALL_STACK_FRAME_SIZE
		;;

	.endm

/*============================================================================*
 * k1b_context_save()                                                         *
 *============================================================================*/

	/*
	 * Saves the current execution context in the current stack.
	 */
	.macro k1b_context_save dest

		/* Allocate some memory. */
		add $r12, $r12, -K1B_CONTEXT_SIZE
		;;

		/* Save GPRs. */
		sd K1B_CONTEXT_R0[$r12], $p0   /**< r0  + r1  */
		;;
		sd K1B_CONTEXT_R2[$r12], $p2   /**< r2  + r3  */
		;;
		sd K1B_CONTEXT_R4[$r12], $p4   /**< r4  + r5  */
		;;
		sd K1B_CONTEXT_R6[$r12], $p6   /**< r6  + r7  */
		;;
		sd K1B_CONTEXT_R8[$r12], $p8   /**< r8  + r9  */
		;;
		sd K1B_CONTEXT_R10[$r12], $p10 /**< r10 + r11 */
		;;
		copy $r1 = $r13
		;;
		copy $r0 = $r12
		;;
		add $r0, $r0, K1B_CONTEXT_SIZE
		;;
		sd K1B_CONTEXT_R12[$r12], $p0  /**< r12 + r13 */
		;;
		sd K1B_CONTEXT_R14[$r12], $p14 /**< r14 + r15 */
		;;
		sd K1B_CONTEXT_R16[$r12], $p16 /**< r16 + r17 */
		;;
		sd K1B_CONTEXT_R18[$r12], $p18 /**< r18 + r19 */
		;;
		sd K1B_CONTEXT_R20[$r12], $p20 /**< r20 + r21 */
		;;
		sd K1B_CONTEXT_R22[$r12], $p22 /**< r22 + r23 */
		;;
		sd K1B_CONTEXT_R24[$r12], $p24 /**< r24 + r25 */
		;;
		sd K1B_CONTEXT_R26[$r12], $p26 /**< r26 + r27 */
		;;
		sd K1B_CONTEXT_R28[$r12], $p28 /**< r28 + r29 */
		;;
		sd K1B_CONTEXT_R30[$r12], $p30 /**< r30 + r31 */
		;;
		sd K1B_CONTEXT_R32[$r12], $p32 /**< r32 + r33 */
		;;
		sd K1B_CONTEXT_R34[$r12], $p34 /**< r34 + r35 */
		;;
		sd K1B_CONTEXT_R36[$r12], $p36 /**< r36 + r37 */
		;;
		sd K1B_CONTEXT_R38[$r12], $p38 /**< r38 + r39 */
		;;
		sd K1B_CONTEXT_R40[$r12], $p40 /**< r40 + r41 */
		;;
		sd K1B_CONTEXT_R42[$r12], $p42 /**< r42 + r43 */
		;;
		sd K1B_CONTEXT_R44[$r12], $p44 /**< r44 + r45 */
		;;
		sd K1B_CONTEXT_R46[$r12], $p46 /**< r46 + r47 */
		;;
		sd K1B_CONTEXT_R48[$r12], $p48 /**< r48 + r49 */
		;;
		sd K1B_CONTEXT_R50[$r12], $p50 /**< r50 + r51 */
		;;
		sd K1B_CONTEXT_R52[$r12], $p52 /**< r52 + r53 */
		;;
		sd K1B_CONTEXT_R54[$r12], $p54 /**< r54 + r55 */
		;;
		sd K1B_CONTEXT_R56[$r12], $p56 /**< r56 + r57 */
		;;
		sd K1B_CONTEXT_R58[$r12], $p58 /**< r58 + r59 */
		;;
		sd K1B_CONTEXT_R60[$r12], $p60 /**< r60 + r61 */
		;;
		sd K1B_CONTEXT_R62[$r12], $p62 /**< r62 + r63 */
		;;

		/* Get SFRs*/
		get   $r4 = $ra
		;;
		get   $r5 = $cs
		;;
		get   $r8 = $lc
		;;
		get   $r9 = $ls
		;;
		get   $r10 = $le
		;;

		/*
		 * Save SFRs. Note that $spc,
		 * $ps and $sps are retrieved from mOS.
		 */
		sw K1B_CONTEXT_RA[$r12],  $r4
		;;
		sw K1B_CONTEXT_CS[$r12],  $r5
		;;
		sw K1B_CONTEXT_LC[$r12],  $r8
		;;
		sw K1B_CONTEXT_LS[$r12],  $r9
		;;
		sw K1B_CONTEXT_LE[$r12],  $r10
	.endm

/*============================================================================*
 * k1b_context_restore()                                                      *
 *============================================================================*/

	/*
	 * Restores an execution context from the current stack.
	 */
	.macro k1b_context_restore src

		/*
		 * Restore SFRs. Note that $spc,
		 * $ps and $sps are restored by mOS.
		 */
		lw $r0  = K1B_CONTEXT_RA[$r12]
		;;
		lw $r1  = K1B_CONTEXT_CS[$r12]
		;;
		lw $r2  = K1B_CONTEXT_LC[$r12]
		;;
		lw $r3  = K1B_CONTEXT_LS[$r12]
		;;
		lw $r4 = K1B_CONTEXT_LE[$r12]
		;;

		/* Set SFRs. */
		set   $ra = $r0
		;;
		set   $cs = $r1
		;;
		set   $lc = $r2
		;;
		set   $ls = $r3
		;;
		set   $le = $r4
		;;

		/* Restore GPRs. */
		ld $p0  = K1B_CONTEXT_R0[$r12]  /**< r0  + r1  */
		;;
		ld $p2  = K1B_CONTEXT_R2[$r12]  /**< r2  + r3  */
		;;
		ld $p4  = K1B_CONTEXT_R4[$r12]  /**< r4  + r5  */
		;;
		ld $p6  = K1B_CONTEXT_R6[$r12]  /**< r6  + r7  */
		;;
		ld $p8  = K1B_CONTEXT_R8[$r12]  /**< r8  + r9  */
		;;
		ld $p10 = K1B_CONTEXT_R10[$r12] /**< r10 + r11 */
		;;
		lw $r13 = K1B_CONTEXT_R13[$r12] /**< r13       */
		;;
		ld $p14 = K1B_CONTEXT_R14[$r12] /**< r14 + r15 */
		;;
		ld $p16 = K1B_CONTEXT_R16[$r12] /**< r16 + r17 */
		;;
		ld $p18 = K1B_CONTEXT_R18[$r12] /**< r18 + r19 */
		;;
		ld $p20 = K1B_CONTEXT_R20[$r12] /**< r20 + r21 */
		;;
		ld $p22 = K1B_CONTEXT_R22[$r12] /**< r22 + r23 */
		;;
		ld $p24 = K1B_CONTEXT_R24[$r12] /**< r24 + r25 */
		;;
		ld $p26 = K1B_CONTEXT_R26[$r12] /**< r26 + r27 */
		;;
		ld $p28 = K1B_CONTEXT_R28[$r12] /**< r28 + r29 */
		;;
		ld $p30 = K1B_CONTEXT_R30[$r12] /**< r30 + r31 */
		;;
		ld $p32 = K1B_CONTEXT_R32[$r12] /**< r32 + r33 */
		;;
		ld $p34 = K1B_CONTEXT_R34[$r12] /**< r34 + r35 */
		;;
		ld $p36 = K1B_CONTEXT_R36[$r12] /**< r36 + r37 */
		;;
		ld $p38 = K1B_CONTEXT_R38[$r12] /**< r38 + r39 */
		;;
		ld $p40 = K1B_CONTEXT_R40[$r12] /**< r40 + r41 */
		;;
		ld $p42 = K1B_CONTEXT_R42[$r12] /**< r42 + r43 */
		;;
		ld $p44 = K1B_CONTEXT_R44[$r12] /**< r44 + r45 */
		;;
		ld $p46 = K1B_CONTEXT_R46[$r12] /**< r46 + r47 */
		;;
		ld $p48 = K1B_CONTEXT_R48[$r12] /**< r48 + r49 */
		;;
		ld $p50 = K1B_CONTEXT_R50[$r12] /**< r50 + r51 */
		;;
		ld $p52 = K1B_CONTEXT_R52[$r12] /**< r52 + r53 */
		;;
		ld $p54 = K1B_CONTEXT_R54[$r12] /**< r54 + r55 */
		;;
		ld $p56 = K1B_CONTEXT_R56[$r12] /**< r56 + r57 */
		;;
		ld $p58 = K1B_CONTEXT_R58[$r12] /**< r58 + r59 */
		;;
		ld $p60 = K1B_CONTEXT_R60[$r12] /**< r60 + r61 */
		;;
		ld $p62 = K1B_CONTEXT_R62[$r12] /**< r62 + r63 */
		;;

		/* Wipe out allocated memory. */
		add $r12, $r12, K1B_CONTEXT_SIZE
		;;
	.endm

/*============================================================================*
 * Stack Operations                                                           *
 *============================================================================*/

	/*
	 * Pushes a word on the stack.
	 *
	 * - word Target register.
	 */
	.macro k1b_pushw reg
		add $sp, $sp, -K1B_WORD_SIZE
		;;
		sw  0[$sp], \reg
		;;
	.endm

	/*
	 * Pops a word from the stack.
	 *
	 * - reg Target register.
	 */
	.macro k1b_popw reg
		lw  \reg, 0[$sp]
		;;
		add $sp, $sp, K1B_WORD_SIZE
		;;
	.endm

	/*
	 * Pushes a dword on the stack.
	 *
	 * - reg Target register.
	 */
	.macro k1b_pushd reg
		add $sp, $sp, -K1B_DWORD_SIZE
		;;
		sd  0[$sp], \reg
		;;
	.endm

	/*
	 * Pops a dword from the stack.
	 *
	 * - reg Target register.
	 */
	.macro k1b_popd reg
		ld  \reg, 0[$sp]
		;;
		add $sp, $sp, K1B_DWORD_SIZE
		;;
	.endm

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro k1b_clear_gprs

		and $r0, $r0, 0x00
		and $r1, $r1, 0x00
		;;
		and $r2, $r2, 0x00
		and $r3, $r3, 0x00
		;;
		and $r4, $r4, 0x00
		and $r5, $r5, 0x00
		;;
		and $r6, $r6, 0x00
		and $r7, $r7, 0x00
		;;
		and $r8, $r8, 0x00
		and $r9, $r9, 0x00
		;;
		and $r10, $r10, 0x00
		and $r11, $r11, 0x00
		;;
		and $r12, $r12, 0x00
		and $r13, $r13, 0x00
		;;
		and $r14, $r14, 0x00
		and $r15, $r15, 0x00
		;;
		and $r16, $r16, 0x00
		and $r17, $r17, 0x00
		;;
		and $r18, $r18, 0x00
		and $r19, $r19, 0x00
		;;
		and $r20, $r20, 0x00
		and $r21, $r21, 0x00
		;;
		and $r22, $r22, 0x00
		and $r23, $r23, 0x00
		;;
		and $r24, $r24, 0x00
		and $r25, $r25, 0x00
		;;
		and $r30, $r30, 0x00
		and $r31, $r31, 0x00
		;;
		and $r32, $r32, 0x00
		and $r33, $r33, 0x00
		;;
		and $r34, $r34, 0x00
		and $r35, $r35, 0x00
		;;
		and $r36, $r36, 0x00
		and $r37, $r37, 0x00
		;;
		and $r38, $r38, 0x00
		and $r39, $r39, 0x00
		;;
		and $r40, $r40, 0x00
		and $r41, $r41, 0x00
		;;
		and $r42, $r42, 0x00
		and $r43, $r43, 0x00
		;;
		and $r44, $r44, 0x00
		and $r45, $r45, 0x00
		;;
		and $r46, $r46, 0x00
		and $r47, $r47, 0x00
		;;
		and $r48, $r48, 0x00
		and $r49, $r49, 0x00
		;;
		and $r50, $r50, 0x00
		and $r51, $r51, 0x00
		;;
		and $r52, $r52, 0x00
		and $r53, $r53, 0x00
		;;
		and $r54, $r54, 0x00
		and $r55, $r55, 0x00
		;;
		and $r56, $r56, 0x00
		and $r57, $r57, 0x00
		;;
		and $r58, $r58, 0x00
		and $r59, $r59, 0x00
		;;
		and $r60, $r60, 0x00
		and $r61, $r61, 0x00
		;;
		and $r62, $r62, 0x00
		and $r63, $r63, 0x00
		;;

	.endm

#endif /* ARCH_CORE_K1B_ASM_H_ */
