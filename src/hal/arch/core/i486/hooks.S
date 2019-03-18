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

#include <arch/core/i486/8259.h>
#include <arch/core/i486/asm.h>
#include <arch/core/i486/context.h>
#include <arch/core/i486/core.h>
#include <arch/core/i486/excp.h>
#include <arch/core/i486/gdt.h>

/* Exported symbols. */
.globl _do_excp0
.globl _do_excp1
.globl _do_excp2
.globl _do_excp3
.globl _do_excp4
.globl _do_excp5
.globl _do_excp6
.globl _do_excp7
.globl _do_excp8
.globl _do_excp9
.globl _do_excp10
.globl _do_excp11
.globl _do_excp12
.globl _do_excp13
.globl _do_excp14
.globl _do_excp15
.globl _do_excp16
.globl _do_excp17
.globl _do_excp18
.globl _do_excp19
.globl _do_excp20
.globl _do_excp30
.globl i486_syscall
.globl _do_hwint0
.globl _do_hwint1
.globl _do_hwint2
.globl _do_hwint3
.globl _do_hwint4
.globl _do_hwint5
.globl _do_hwint6
.globl _do_hwint7
.globl _do_hwint8
.globl _do_hwint9
.globl _do_hwint10
.globl _do_hwint11
.globl _do_hwint12
.globl _do_hwint13
.globl _do_hwint14
.globl _do_hwint15

/*============================================================================*
 * _do_excp()                                                                 *
 *============================================================================*/

/*
 * Offset to exception structure.
 */
#define I486_EXCEPTION_SKIP -I486_CONTEXT_SW_SIZE - I486_EXCEPTION_SIZE + I486_EXCEPTION_ERR

/*
 * Exception without error code.
 */
.macro _do_excp_noerr_code, number
	_do_excp\()\number:
		i486_context_save %eax
		movl $(\number), %ebx
		movl $0, %ecx
		jmp _do_excp
.endm

/*
 * Exception with error code.
 */
.macro _do_excp_err_code, number
	_do_excp\()\number:
		xchg %eax, (%esp)
		xchg %eax, I486_EXCEPTION_SKIP(%esp)
		xchg %eax, (%esp)
		i486_context_save %eax
		movl $(\number), %ebx
		movl $0, %ecx
		jmp _do_excp
.endm

/*
 * Exception with error code.
 */
.macro _do_excp_err2_code, number
	_do_excp\()\number:
		xchg %eax, (%esp)
		xchg %eax, I486_EXCEPTION_SKIP(%esp)
		xchg %eax, (%esp)
		i486_context_save %eax
		movl $(\number), %ebx
		movl %cr2, %ecx
		jmp _do_excp
.endm

/* Exception hooks. */
_do_excp_noerr_code  0 /* Division-by-Zero Error      */
_do_excp_noerr_code  1 /* Debug Exception             */
_do_excp_noerr_code  2 /* Non-Maskable Interrupt      */
_do_excp_noerr_code  3 /* Breakpoint Exception        */
_do_excp_noerr_code  4 /* Overflow Exception          */
_do_excp_noerr_code  5 /* Bounds Check Exception      */
_do_excp_noerr_code  6 /* Invalid Opcode Exception    */
_do_excp_noerr_code  7 /* Coprocessor Not Available   */
_do_excp_err_code    8 /* Double Fault                */
_do_excp_noerr_code  9 /* Coprocessor Segment Overrun */
_do_excp_err_code   10 /* Invalid TSS                 */
_do_excp_err_code   11 /* Segment Not Present         */
_do_excp_err_code   12 /* Stack Segment Fault         */
_do_excp_err_code   13 /* General Protection Fault    */
_do_excp_err2_code  14 /* Page Fault                  */
_do_excp_noerr_code 15 /* Reserved                    */
_do_excp_noerr_code 16 /* Floating Point Exception    */
_do_excp_err_code   17 /* Alignment Check Exception   */
_do_excp_noerr_code 18 /* Machine Check Exception     */
_do_excp_noerr_code 19 /* SMID Unit Exception         */
_do_excp_noerr_code 20 /* Virtual Exception           */
_do_excp_err_code   30 /* Security Exception.         */

/*
 * Low-level exception handler dispatcher.
 */
_do_excp:
	/* Save exception information. */
	movl I486_CONTEXT_EIP(%eax), %edx
	subl $I486_EXCEPTION_SIZE, %esp
	movl %ebx, I486_EXCEPTION_NUM(%esp)
	movl %ecx, I486_EXCEPTION_DATA(%esp)
	movl %edx, I486_EXCEPTION_CODE(%esp)
	movl %esp, %ebx

	/* Call high-level exception dispatcher. */
	pushl %eax /* Execution context. */
	pushl %ebx /* Exception context. */
	call  do_excp
	addl  $(2*I486_WORD_SIZE), %esp

    i486_context_restore
	iret

/*============================================================================*
 * syscall()                                                                  *
 *============================================================================*/

/*
 * System call hook.
 */
i486_syscall:
	cmpl $I486_NR_upcall_ret, %eax
	jnz _do_syscall.continue

		movl %ebx, %esp
		jmp _do_syscall.out

	_do_syscall.continue:

		/* Save system call context. */
		i486_context_save %ebx

		/* Push system call parameters. */
		movl I486_CONTEXT_EAX(%ebx), %eax
		pushl %eax
		movl I486_CONTEXT_EDI(%ebx), %eax
		pushl %eax
		movl I486_CONTEXT_ESI(%ebx), %eax
		pushl %eax
		movl I486_CONTEXT_EDX(%ebx), %eax
		pushl %eax
		movl I486_CONTEXT_ECX(%ebx), %eax
		pushl %eax
		movl I486_CONTEXT_EBX(%ebx), %eax
		pushl %eax

		/* Handle system call. */
		call do_syscall

		/* Wipe out system call parameters. */
		addl $6*I486_WORD_SIZE, %esp

		/* Copy return value to user stack. */
		movl %eax, I486_CONTEXT_EAX(%esp)

	_do_syscall.out:

		/* restore system call context. */
		i486_context_restore

		iret

/*============================================================================*
 * i486_pic_master_ack()                                                      *
 *============================================================================*/

/*
 * Acknowledges an interrupt to the master PIC.
 */
.macro i486_pic_master_ack
	movb $0x20, %al
	outb %al, $PIC_CTRL_MASTER
.endm

/*============================================================================*
 * i486_pic_slave_ack()                                                       *
 *============================================================================*/

/*
 * Acknowledges an interrupt to the slave PIC.
 */
.macro i486_pic_slave_ack
	movb $0x20, %al
	outb %al, $PIC_CTRL_MASTER
	outb %al, $PIC_CTRL_SLAVE
.endm

/*============================================================================*
 * _do_hwint()                                                                *
 *============================================================================*/

/*
 * Low-level hardware interrupt dispatcher.
 */
.macro _do_hwint_master, num
	_do_hwint\()\num:
		i486_context_save %ebx
		i486_pic_master_ack
		pushl %ebx
		pushl $(\num)
		call i486_do_hwint
		addl $(2*I486_WORD_SIZE), %esp
		i486_context_restore
		iret
.endm

/*
 * Low-level hardware interrupt dispatcher.
 */
.macro _do_hwint_slave, num
	_do_hwint\()\num:
		i486_context_save %ebx
		i486_pic_slave_ack
		pushl %ebx
		pushl $(\num)
		call i486_do_hwint
		addl $(2*I486_WORD_SIZE), %esp
		i486_context_restore
		iret
.endm

/* Hardware interrupt hooks. */
_do_hwint_master  0
_do_hwint_master  1
_do_hwint_master  2
_do_hwint_master  3
_do_hwint_master  4
_do_hwint_master  5
_do_hwint_master  6
_do_hwint_master  7
_do_hwint_slave   8
_do_hwint_slave   9
_do_hwint_slave  10
_do_hwint_slave  11
_do_hwint_slave  12
_do_hwint_slave  13
_do_hwint_slave  14
_do_hwint_slave  15
