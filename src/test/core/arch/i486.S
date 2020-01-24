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

/* Must come first. */
#define _ASM_FILE_

#include <arch/core/i486/asm.h>
#include <arch/core/i486/gdt.h>

.globl _upcall_issue
.globl _upcall_issue_ret
.globl saved_sp

.section .text

/*
 * Issue an Upcall
 */
.align 8
_upcall_issue:
	_do_prologue_slow      // Save all preserved registers since
	                       // they may be used by the HAL.

	leal saved_sp, %eax
	movl %esp, (%eax)

	/* Switch to fake stack. */
	movl 8(%ebp), %eax
	addl %esp, (16 + I486_CONTEXT_ESP)(%eax)
	movl $KERNEL_CS, (16 + I486_CONTEXT_CS)(%eax)
	movl $KERNEL_DS, (16 + I486_CONTEXT_SS)(%eax)
	pushfl
	popl %ebx
	movl %ebx,        (16 + I486_CONTEXT_EFLAGS)(%eax)
	movl %eax, %esp

	/* Return from upcall*/
	jmp i486_upcall_ret

_upcall_issue_ret:

	/* Switch back to real stack. */
	leal saved_sp, %eax
	movl (%eax), %esp

	_do_epilogue_slow     // Restore all preserved
	                      // registers.

	ret
