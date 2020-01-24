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

#ifndef ARCH_CORE_I486_ASM_H_
#define ARCH_CORE_I486_ASM_H_

	#ifndef _ASM_FILE_
		#error "do not include this header in C files"
	#endif

	/* Must come first. */
	#define __NEED_CORE_CONTEXT

	#include <arch/core/i486/ctx.h>

/*============================================================================*
 * Procedure Linkage                                                          *
 *============================================================================*/

	/**
	 * @brief Stack frame size for slow call.
	 */
	#define SLOW_CALL_STACK_FRAME_SIZE 16

	/**
	* @brief Offsets to Stack Frame
	*/
	/**@{*/
	#define STACK_FRAME_EBX 0  /**< ebx */
	#define STACK_FRAME_ESI 4  /**< esi */
	#define STACK_FRAME_EDI 8  /**< edi */
	#define STACK_FRAME_EBP 12 /**< ebp */
	/**@}*/

	/*
	 * @brief Saves preserved registers.
	 */
	.macro _do_prologue_slow

		push  %ebp
		movl  %esp, %ebp
		pushl %edi
		pushl %esi
		pushl %ebx

	.endm

	/*
	 * @brief Restores preserved registers.
	 */
	.macro _do_epilogue_slow

		popl %ebx
		popl %esi
		popl %edi
		popl %ebp

	.endm

/*============================================================================*
 * i486_context_save()                                                        *
 *============================================================================*/

	/*
	 * The i486_context_save() macro saves the content of general purpose
	 * and segment registers in the stack of the caller. A pointer to the
	 * saved execution context is saved into ret.
	 *
	 * Note that the extended instruction pointer register (eip), code
	 * segment register (cs), extended flags register (eflags), extended
	 * stack pointer register (esp) and stack segment register (ss) are
	 * not saved by this macro, because they are automatically saved by
	 * the hardware upon an exception/interrupt.
	 */
	.macro i486_context_save, ret

		subl $I486_CONTEXT_SW_SIZE, %esp

		/* Save general purpose registers. */
		movl %eax, I486_CONTEXT_EAX(%esp)
		movl %ebx, I486_CONTEXT_EBX(%esp)
		movl %ecx, I486_CONTEXT_ECX(%esp)
		movl %edx, I486_CONTEXT_EDX(%esp)
		movl %ebp, I486_CONTEXT_EBP(%esp)
		movl %esi, I486_CONTEXT_ESI(%esp)
		movl %edi, I486_CONTEXT_EDI(%esp)

		/* Save segment registers. */
		movw %ds, I486_CONTEXT_DS(%esp)
		movw %es, I486_CONTEXT_ES(%esp)
		movw %fs, I486_CONTEXT_FS(%esp)
		movw %gs, I486_CONTEXT_GS(%esp)

		movl %esp, \ret

	.endm

/*============================================================================*
 * i486_context_restore()                                                     *
 *============================================================================*/

	/*
	 * The i486_context_restore() macro restores the content of general
	 * purpose and segment registers from the stack of the caller.
	 *
	 * It is up to the caller to switch the current stack to the target one.
	 */
	.macro i486_context_restore

		/* Restore general purpose registers. */
		movl I486_CONTEXT_EAX(%esp), %eax
		movl I486_CONTEXT_EBX(%esp), %ebx
		movl I486_CONTEXT_ECX(%esp), %ecx
		movl I486_CONTEXT_EDX(%esp), %edx
		movl I486_CONTEXT_EBP(%esp), %ebp
		movl I486_CONTEXT_ESI(%esp), %esi
		movl I486_CONTEXT_EDI(%esp), %edi

#ifdef XXX
		/* Restore segment registers. */
		movw I486_CONTEXT_DS(%esp), %ds
		movw I486_CONTEXT_ES(%esp), %es
		movw I486_CONTEXT_FS(%esp), %fs
		movw I486_CONTEXT_GS(%esp), %gs
#endif

		addl $I486_CONTEXT_SW_SIZE, %esp

		/*
		 * Special registers are
		 * restored by a further call to iret.
		 */

	.endm

/*============================================================================*
 * Misc                                                                       *
 *============================================================================*/

	/*
	 * Clear all GPR registers.
	 */
	.macro i486_clear_gprs

		xorl %eax, %eax
		xorl %ebx, %ebx
		xorl %ecx, %ecx
		xorl %edx, %edx
		xorl %esi, %esi
		xorl %edi, %edi
		xorl %ebp, %ebp
		xorl %esp, %esp

	.endm

	/*
	 * Resets the stack.
	 * - coreid ID of the calling core
	 */
	.macro i486_core_stack_reset coreid

		movl $kstacks, %esp
		movl %\coreid, %eax
		addl %eax, 1
		shll $I486_PAGE_SHIFT, %eax
		addl %esp, %eax
		addl $-I486_WORD_SIZE, %esp  /* Stack pointer. */
		movl %esp, %ebp              /* Frame pointer. */

	.endm

#endif /* ARCH_CORE_I486_ASM_H_ */
