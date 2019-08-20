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
#define __NEED_CORE_TYPES

#include <arch/core/rv32gc/asm.h>
#include <arch/core/rv32gc/upcall.h>
#include <arch/core/rv32gc/types.h>

.global rv32gc_upcall_ret

.section .text

/*===========================================================================*
 * rv32gc_upcall_ret()                                                       *
 *===========================================================================*/

//
// Returns from an upcal
//
.align RV32GC_WORD_SIZE
rv32gc_upcall_ret:

	// Call target function
	lw    t0, 0(sp)
	addi  a0, sp, 2*RV32GC_WORD_SIZE

	rv32gc_do_fn_call_save
	jalr  t0
	rv32gc_do_fn_call_restore

	// Wipe out argument.
	lw    t0, RV32GC_WORD_SIZE(sp)    // arg
	addi  t0, t0, 2*RV32GC_WORD_SIZE  // argsize + fn
	add   sp, sp, t0

	// Call special kcall
	li  a0, RV32GC_NR_upcall_ret
	mv  a1, sp
	j   _rv32gc_do_kcall
