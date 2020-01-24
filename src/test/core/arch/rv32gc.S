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

#include <arch/core/rv32gc/asm.h>

.globl _upcall_issue
.globl _upcall_issue_ret

.section .text

//
// Issues an upcall.
//
.align 4
_upcall_issue:

	// Save return address.
	rv32gc_pushw ra

	rv32gc_do_prologue  // Save all preserved registers since
	                    // they may be used by the HAL.

	// Save pointer to real stack.
	la  t0, saved_sp
	sw  sp, 0(t0)

	// Switch to fake stack.
	sw  sp, 16 + RV32GC_CONTEXT_SP(a0)
	sw  fp, 16 + RV32GC_CONTEXT_FP(a0)
	mv  sp, a0

	// Return from upcall.
	j rv32gc_upcall_ret

_upcall_issue_ret:

	// Switch back to real stack.
	la  t0, saved_sp
	lw  fp, 0(t0)

	rv32gc_do_epilogue   // Restore all preserved registers.

	// Restore return address.
	rv32gc_popw ra

	ret
