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
#define __NEED_CORE_TYPES
#define __NEED_CORE_CONTEXT

#include <arch/core/i486/asm.h>
#include <arch/core/i486/upcall.h>
#include <arch/core/i486/types.h>
#include <arch/core/i486/trap.h>

.global i486_upcall_ret

.section .text

/*===========================================================================*
 * i486_upcall_ret()                                                         *
 *===========================================================================*/

/*
 * Returns from an upcall.
 */
.align 8
i486_upcall_ret:

	/* Call target function. */
	movl (%esp), %eax
	movl %esp, %ebx
	addl $2*I486_WORD_SIZE, %ebx
	pushl %ebx
	call *%eax
	addl $I486_WORD_SIZE, %esp

	/* Wipe out argument. */
	addl $I486_WORD_SIZE, %esp // fn
	popl %eax                  // argsize
	addl %eax, %esp            // arg

	/* Call special kcall. */
	movl $I486_NR_upcall_ret, %eax
	movl %esp, %ebx
	int $I486_TRAP_GATE

