/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
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

#ifndef ARCH_CORE_ARM64_ASM_H_
#define ARCH_CORE_ARM64_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

/*============================================================================*
 * Regiser Aliases                                                            *
 *============================================================================*/

	/**
	 * @name Aliases for Registers
	 */
	/**@{*/
	#define fp x29 /**< Frame Pointer Register */
	#define lr x30 /**< Link Register */
	/**@}*/

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro arm64core_clear_gprs

		mov x0, xzr;
		orr x0,  xzr, x0; orr x1,  xzr, x0; orr x2,  xzr, x0;
        orr x3,  xzr, x0; orr x4,  xzr, x0; orr x5,  xzr, x0;
        orr x6,  xzr, x0; orr x7,  xzr, x0; orr x8,  xzr, x0;
		orr x9,  xzr, x0; orr x10,  xzr, x0; orr x11,  xzr, x0;
		orr x12,  xzr, x0; orr x13,  xzr, x0; orr x14,  xzr, x0;
		orr x15,  xzr, x0; orr x14,  xzr, x0; orr x17,  xzr, x0;
		orr x18,  xzr, x0; orr x17,  xzr, x0; orr x20,  xzr, x0;
		orr x21,  xzr, x0; orr x20,  xzr, x0; orr x23,  xzr, x0;
		orr x24,  xzr, x0; orr x23,  xzr, x0; orr x26,  xzr, x0;
		orr x27,  xzr, x0; orr x26,  xzr, x0; orr x29,  xzr, x0;
        orr x30,  xzr, x0;
	.endm
	
	/*
	 *  Exception Save Info
	 */
	.macro arm64core_get_exception_info
		/* Don't use the x0 'cause this register is used to hold the context addr */
		mrs     x1, esr_el1 //Code Error
		mrs     x2, elr_el1 //Faulting address.
    
	.endm

	/*
	 *  Exception Save Context
	 */
	.macro arm64core_save_context
		stp	x29, x30, [sp, #-16]!
		stp	x27, x28, [sp, #-16]!
		stp	x25, x26, [sp, #-16]!
		stp	x23, x24, [sp, #-16]!
		stp	x21, x22, [sp, #-16]!
		stp	x19, x20, [sp, #-16]!
		stp	x17, x18, [sp, #-16]!
		stp	x15, x16, [sp, #-16]!
		stp	x13, x14, [sp, #-16]!
		stp	x11, x12, [sp, #-16]!
		stp	x9, x10, [sp, #-16]!
		stp	x7, x8, [sp, #-16]!
		stp	x5, x6, [sp, #-16]!
		stp	x3, x4, [sp, #-16]!
		stp	x1, x2, [sp, #-16]!
		str x0, [sp, #-8]!
	.endm

	/*
	 *  Exception Restore Context
	 */
	.macro arm64core_restore_context
		ldr  x0, [sp], #8
		ldp	x1, x2, [sp], #16
		ldp	x3, x4, [sp], #16
		ldp	x5, x6, [sp], #16
		ldp	x7, x8, [sp], #16
		ldp	x9, x10, [sp], #16
		ldp	x11, x12, [sp], #16
		ldp	x13, x14, [sp], #16
		ldp	x15, x16, [sp], #16
		ldp	x17, x18, [sp], #16
		ldp	x19, x20, [sp], #16
		ldp	x21, x22, [sp], #16
		ldp	x23, x24, [sp], #16
		ldp	x25, x26, [sp], #16
		ldp	x27, x28, [sp], #16
		ldp	x29, x30, [sp], #16
	.endm

	/*
	 * Resets the stack.
	 * - coreid ID of the calling core
	 */
	.macro arm64_core_stack_reset coreid
		mov x0, \coreid
		mov x1, #0x1000
		add x0, x0, #1
		mul x0, x0, x1
		adrp x0, stack_base
		mov	sp, x0

	.endm

#endif /* ARCH_CORE_ARM64_ASM_H_ */
