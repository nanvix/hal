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
#define __NEED_OR1K_REGS
#define __NEED_CORE_TYPES

#include <arch/core/or1k/asm.h>
#include <arch/core/or1k/upcall.h>

#include <arch/core/or1k/types.h>

.global or1k_upcall_ret

.section .text

/*===========================================================================*
 * or1k_upcall_ret()                                                          *
 *===========================================================================*/

/*
 * Returns from an upcall.
 */
.align 8
or1k_upcall_ret:

	/* Call target function. */
	l.lwz  r4, 0(sp)
	l.addi r3, sp, 2*OR1K_WORD_SIZE
	l.jalr r4
	l.nop

	/* Wipe out argument. */
	l.lwz  r3, 4(sp)
	l.addi r3, r3, 2*OR1K_WORD_SIZE
	l.add  sp, sp, r3

	/* Call special syscall. */
	l.ori r11, r0, OR1K_NR_upcall_ret
	l.ori r3,  sp, 0
	l.sys 1
