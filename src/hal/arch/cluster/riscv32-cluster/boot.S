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
#define __RV32GC_MACHINE_CODE

#include <arch/cluster/riscv32-cluster/cores.h>

/* Exported symbols. */
.global _do_start
.global kstacks

/*============================================================================*
 * bootstrap section                                                          *
 *============================================================================*/

.section .text,"ax",@progbits

/*----------------------------------------------------------------------------*
 * _do_start()                                                                *
 *----------------------------------------------------------------------------*/

/*
 * Entry point.
 */
.align RV32GC_WORD_SIZE
_do_start:
	 j  _do_start

/*============================================================================*
 * DATA                                                                       *
 *============================================================================*/

.section .data

mtrap_table:
	.word rv32gc_do_mexcp /**< Instruction Address Misaligned        */
	.word rv32gc_do_mexcp /**< Instruction Access Fault              */
	.word rv32gc_do_mexcp /**< Illegal Instruction                   */
	.word rv32gc_do_mexcp /**< Breakpoint                            */
	.word rv32gc_do_mexcp /**< Load Address Misaligned               */
	.word rv32gc_do_mexcp /**< Load Access Fault                     */
	.word rv32gc_do_mexcp /**< Store/AMO Address Misaligned          */
	.word rv32gc_do_mexcp /**< Store/AMO Access Fault                */
	.word rv32gc_do_mcall /**< Environment Call from User Mode       */
	.word rv32gc_do_mcall /**< Environment Call from Supervisor Mode */
	.word rv32gc_do_mbad  /**< Reserved                              */
	.word rv32gc_do_mcall /**< Environment Call from Machine Mode    */
	.word rv32gc_do_mexcp /**< Instruction Page Fault                */
	.word rv32gc_do_mexcp /**< Load Page Fault                       */
	.word rv32gc_do_mbad  /**< Reserved                              */
	.word rv32gc_do_mexcp /**< Store/AMO Page Fault                  */

/*============================================================================*
 * BSS                                                                        *
 *============================================================================*/

.section .bss,"aw",@nobits

.align RV32GC_WORD_SIZE
kstacks:
	.skip RV32GC_PAGE_SIZE * RISCV32_CLUSTER_NUM_CORES
