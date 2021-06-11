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
#define __NEED_CLUSTER_MEMMAP

#include <arch/cluster/arm64-cluster/memmap.h>
#include <arch/core/arm64/mmu.h>
#include <arch/core/arm64/gic.h>
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
    mov x1, #0xff
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

/* 
    MMU kernel map	(DRAM + DEVICE regs)
	map 0x4000 0000 				to 	0xffff 0000 0000 0000 			(1020MB)
	map 0xUART_PA_BASE 				to 	0xffff 0000 0000 0000 + 1020MB 	(2MB)
	map 0xQEMU_VIRT_GIC_PA_BASE 	to 	0xffff 0000 0000 0000 + 1022MB 	(2MB)
*/
    // 在内存中创建页表， PGD一项， PUD一项，PMD 512项（每项2MB），各级各占用一页
    mov  x0, #KERNEL_VA_START   // x0  VA
    adrp x1, pg_tbl_start       // x1  PA for PGD base
    add  x2, x1, #PAGE_SIZE     // x2  PA for PUD base
    add  x3, x2, #PAGE_SIZE     // x3  PA for PMD base

    add  x4, x1, #0             // x4  PA for PGD entry
    add  x5, x2, #0             // x5  PA for PUD entry
    //populate first PGD
    orr  x6, x2, #MM_TYPE_PAGE_TABLE
    str  x6, [x4]               // populate PGD entry
    //populate first PUD
    orr  x6, x3, #MM_TYPE_PAGE_TABLE
    str  x6, [x5]               // populate PUD entry
    //populate 512 PMD
    mov  x6, #MEM_BASE          // x6  PA base
    mov  x7, #0                 // offset of VA/PA
    mov  x8, #0                 // index of PMD
1:
    lsl  x9, x8, #3             // offset of PMD
    add  x10, x6, x7            // PA
    mov  x11, #MMU_FLAGS
    orr  x10, x10, x11          // PME
    add  x12, x3, x9
    str  x10, [x12]

    add  x7, x7, #ARM64_PMD_SIZE  // offset of PA
    add  x8, x8, #1
    cmp  x8, #509               // 510>509则跳转，511、512table 给 中断控制器和串口控制器
    b.ls    1b                  // <=

//UART REG，511号entry，对应VA = VA_START + (511-1) * 2 * 1024 * 1024
    lsl  x9, x8, #3
    mov  x10, #UART_PA_BASE
    mov  x11, #MMU_DEVICE_FLAGS
    orr  x10, x10, x11
    add  x12, x3,  x9
    str  x10, [x12]

//GIC REG, 512号entry，对应VA = VA_START + (512-1)*2*1024*1024
    add  x8, x8, #1
    lsl  x9, x8, #3
    mov  x10, #QEMU_VIRT_GIC_PA_BASE
    mov  x11, #MMU_DEVICE_FLAGS
    orr  x10, x10, x11
    add  x12, x3,  x9
    str  x10, [x12]

/*
    MMU DRAM temporary map
    map 0x4000 0000 - 0x4020 0000(DRAM) to 0x4000 0000 - 0x4020 0000(VA)
*/
    mov  x0, #MEM_BASE
    adrp x1, ram_tbl_start      // x1  PA for PGD base
    add  x2, x1, #ARM64_PAGE_SIZE     // x2  PA for PUD base
    add  x3, x2, #ARM64_PAGE_SIZE     // x3  PA for PMD base

    add  x4, x1, #0             // x4  PA for PGD entry
    add  x5, x2, #8             // x5  PA for PUD entry, second entry
    //populate second PGD, 1G-2G
    orr  x6, x2, #MM_TYPE_PAGE_TABLE
    str  x6, [x4]               // populate PGD entry
    //populate first PUD
    orr  x6, x3, #MM_TYPE_PAGE_TABLE
    str  x6, [x5]               // populate PUD entry
    //populate 1 PMD, map 0x4000 0000 - 0x4020 0000(DRAM) to 0x4000 0000 - 0x4020 0000(VA)
    mov  x6, #MEM_BASE          // x6  PA base
    mov  x7, #0                 // offset of VA/PA
    mov  x8, #0                 // index of PMD
    lsl  x9, x8, #3             // offset of PMD
    add  x10, x6, x7            // PA
    mov  x11, #MMU_FLAGS
    orr  x10, x10, x11          // PME
    add  x12, x3, x9
    str  x10, [x12]

    add  x7, x7, #ARM64_PMD_SIZE  // offset of PA
    add  x8, x8, #1


    /* Invalidate TLBs */
    ic      iallu                           // I+BTB cache invalidate
    tlbi    vmalle1                         // invalidate I + D TLBs
    dsb     sy

    adrp    x0,  pg_tbl_start
    msr ttbr1_el1, x0
    adrp    x0,  ram_tbl_start
    msr ttbr0_el1, x0

    ldr x0, =(TCR_VALUE)    
    msr tcr_el1, x0

    ldr x0, =(MAIR_VALUE)
    msr mair_el1, x0

/*
 * Configure the Exception Level.
 */
_config_exceptions:
    /* Get the value of the current exception level */
    mrs     x0, CurrentEL
    and     x0, x0, #12      // Mask
    cmp     x0, #12          // Exception Level 3?
    bne     _set_vector_table


    mov     x1, #0x5b1      // Set EL0 and EL1 to non-secure state
    msr     scr_el3, x1

    mov     x1, #0x3c9       // Mask interruptions
    msr     SPSR_EL3, x1

    adr     x1, _teste
    msr     elr_el3, x1
    eret

_teste:
    mrs     x0, CurrentEL
    and     x0, x0, #12      // Mask
    mov    x1, #(1 << 31)
    msr    hcr_el2, x1

    mov     x2, #0x3c4
    msr     spsr_el2, x2

    adr     x2, _set_vector_table
    msr     elr_el2, x2
    eret

/*
 * Set the vector table register.
 */
_set_vector_table:
    mov     x0, #0x3C5
    msr     spsr_el1, x0
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
    //bl arm64_gic_initialize
    mrs x0, s3_1_c15_c2_1
    orr x0, x0, #(0x1<<6)       //cpuectlr.smpen = 1
    msr s3_1_c15_c2_1, x0
    mrs x0, sctlr_el1
    orr x0, x0, #(0x1 << 2)       // D bit, dcache
    orr x0, x0, #(0x1 << 12)      // I bit, icache
    orr x0, x0, #1              // M bit, mmu
    msr sctlr_el1, x0
    dsb sy
    isb

    ldr x2, =arm64_cluster_master_setup
    br x2
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
