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

/* TODO: Context save */
.align 7
b exp_handler
eret
/* TODO: Context Restaure */

.endm

/*----------------------------------------------------------------------------*
 * _exception_vector_table()                                                 *
 *----------------------------------------------------------------------------*/

.align 11
_exception_vector_table:

EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste

EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste

EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste

EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste
EXCEPTION_CALL teste

exp_handler:
arm64core_save_context 0
bl teste
arm64core_restore_context