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

#include <arch/core/arm64-core/asm.h>

.section ".exceptions"

/* Exported symbols. */
.globl _exception_vector_table

/*----------------------------------------------------------------------------*
 * Exception Call Macro                                                       *
 *----------------------------------------------------------------------------*/

.macro EXCEPTION_CALL exception

b \exception
.balign 0x80

.endm

/*----------------------------------------------------------------------------*
 * Save Context Macro                                                       *
 *----------------------------------------------------------------------------*/

.macro HANDLE_EXCEPTION function
arm64core_save_context
bl \function
arm64core_restore_context
eret
.endm

/*----------------------------------------------------------------------------*
 * _exception_vector_table()                                                 *
 *----------------------------------------------------------------------------*/

.balign 0x800
_exception_vector_table:

EXCEPTION_CALL synchronous_curr_sp0
EXCEPTION_CALL irq_el2_sp0
EXCEPTION_CALL fiq_curr_sp0
EXCEPTION_CALL serror_curr_sp0

EXCEPTION_CALL synchronous_curr_spcurr
EXCEPTION_CALL irq_curr_spcurr
EXCEPTION_CALL fiq_curr_spcurr
EXCEPTION_CALL serror_curr_spcurr

EXCEPTION_CALL synchronous_lower_64
EXCEPTION_CALL irq_lower_64
EXCEPTION_CALL fiq_lower_64
EXCEPTION_CALL serror_lower_64

EXCEPTION_CALL synchronous_lower_32
EXCEPTION_CALL irq_lower_32
EXCEPTION_CALL fiq_lower_32
EXCEPTION_CALL serror_lower_32

synchronous_curr_sp0:
	HANDLE_EXCEPTION exception_handler
irq_el2_sp0:
	HANDLE_EXCEPTION exception_handler
fiq_curr_sp0:
	HANDLE_EXCEPTION exception_handler
serror_curr_sp0:
	HANDLE_EXCEPTION exception_handler
synchronous_curr_spcurr:
	HANDLE_EXCEPTION exception_handler_return
irq_curr_spcurr:
	HANDLE_EXCEPTION exception_handler_return
fiq_curr_spcurr:
	HANDLE_EXCEPTION exception_handler
serror_curr_spcurr:
	HANDLE_EXCEPTION exception_handler
synchronous_lower_64:
	HANDLE_EXCEPTION exception_handler_return
irq_lower_64:
	HANDLE_EXCEPTION exception_handler
fiq_lower_64:
	HANDLE_EXCEPTION exception_handler
serror_lower_64:
	HANDLE_EXCEPTION exception_handler
synchronous_lower_32:
	HANDLE_EXCEPTION exception_handler
irq_lower_32:
	HANDLE_EXCEPTION exception_handler
fiq_lower_32:
	HANDLE_EXCEPTION exception_handler
serror_lower_32:
	HANDLE_EXCEPTION exception_handler