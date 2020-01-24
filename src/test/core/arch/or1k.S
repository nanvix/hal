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
#define __NEED_OR1K_REGS
#include <arch/core/or1k/asm.h>

.globl _upcall_issue
.globl _upcall_issue_ret
.globl saved_sp

.section .text

/*
 * Issues an upcall.
 */
.align 8
_upcall_issue:
	_do_prologue_slow      // Save all preserved registers since
	                       // they may be used by the HAL.

	OR1K_LOAD_SYMBOL_2_GPR(r4, saved_sp)
	l.sw  0(r4), sp

	/* Switch to fake stack. */
	l.sw 16 + OR1K_CONTEXT_R1(r3),  sp    // Stack pointer
	l.sw 16 + OR1K_CONTEXT_R2(r3),  bp    // Base pointer
	l.mfspr r4, r0, OR1K_SPR_SR           // Status register
	l.sw 16 + OR1K_CONTEXT_ESR(r3), r4
	l.or sp, r0, r3                       // Switch to fake stack

	/* Return from upcall*/
	l.j or1k_upcall_ret
	l.nop

_upcall_issue_ret:

	/* Switch back to real stack. */
	OR1K_LOAD_SYMBOL_2_GPR(r4, saved_sp)
	l.lwz sp, 0(r4)

	_do_epilogue_slow     // Restore all preserved
	                      // registers.

	l.jr r9
	l.nop
