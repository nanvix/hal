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
#include <arch/cluster/arm64-cluster/cores.h>
#include <arch/cluster/arm64-cluster/psci.h>

.section ".text"

/* Exported symbols. */
.globl _do_start
.globl psci_v0_2_cpu_turn_on

/*----------------------------------------------------------------------------*
 * _do_start()                                                                *
 *----------------------------------------------------------------------------*/

/*
 * Entry point.
 */
_do_start:
    arm64core_clear_gprs

/*
 * Configure the Exception Level.
 * Commonly starts at EL3 and this will change to EL1
 */
_config_exceptions:
    /* Get the value of the current exception level */
    mrs     x0, CurrentEL
    and     x0, x0, #12      // Mask
    cmp     x0, #12          // Exception Level 3?
    bne     _set_vector_table


    mov     x1, #0x5b1       // Set EL0 and EL1 to non-secure state
    msr     scr_el3, x1

    mov     x1, #0x3c9       // Mask interruptions
    msr     SPSR_EL3, x1

    adr     x1, _set_vector_table
    msr     elr_el3, x1
    eret

/*
 * Set the vector table register.
 */
_set_vector_table: 
    ldr     x0, =_exception_vector_table
    msr     vbar_el1, x0
    isb

    /* Get the core ID and branch to the correspondent label*/
    mrs	x0, mpidr_el1
	and	x0, x0, #3
    cbz x0, _do_start.master

_do_start.slave:
    /* Set stack adress */
    arm64_core_stack_reset x0
    bl arm64_cluster_slave_setup
    b halt

_do_start.master:
    /* Set stack adress */
    arm64_core_stack_reset x0
    /* Branch the master core to _main function */
    bl arm64_cluster_master_setup
    b halt

/* Infinite loop */
halt: wfe
      b halt

/*----------------------------------------------------------------------------*
 * psci_v0_2_cpu_turn_on(cpu_id)                                                                *
 *----------------------------------------------------------------------------*/

/* Turn on a cpu core */
psci_v0_2_cpu_turn_on:
    mov x1, x0
    ldr x0, =PSCI_v0_2_64BIT_CPU_ON
    ldr x2, =_do_start
    mov x3, xzr
    hvc #0
    ret
