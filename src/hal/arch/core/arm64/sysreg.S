/**
 * \file
 * \brief Cache control routines for ARMv8.
 */

/*
 * Copyright (c) 2015, Hewlett Packard Enterprise Development LP.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Universitaetstrasse 6, CH-8092 Zurich. Attn: Systems Group.
 */

#define _ASM_FILE_
#define __NEED_CLUSTER_MEMMAP

#include <arch/cluster/arm64-cluster/memmap.h>
#include <arch/core/arm64/mmu.h>

.global arm64_invalidate_d_cache,\
        arm64_invalidate_tlb, \
        arm64_enable_mmu
.section .text

/* Based on algorithm from ARM Architecture Reference Manual */
arm64_invalidate_d_cache:

    sub     sp, sp, #96

    stp     x0, x1, [sp, #16 * 0]
    stp     x2, x3, [sp, #16 * 1]
    stp     x4, x5, [sp, #16 * 2]
    stp     x6, x7, [sp, #16 * 3]
    stp     x8, x9, [sp, #16 * 4]
    stp     x10, x11, [sp, #16 * 5]

    dmb    sy                // ensure ordering with previous memory accesses

    mrs x0, clidr_el1
    and w3, w0, #0x07000000     // get 2 x level of coherency
    lsr w3, w3, #23
    cbz w3, finished
    mov w10, #0                 // w10 = 2 x cache level
    mov w8, #1                     // w8 = constant 0b1

loop1: 
    add w2, w10, w10, lsr #1     // calculate 3 x cache level
    lsr w1, w0, w2                 // extract 3-bit cache type for this level
    and w1, w1, #0x7
    cmp w1, #2
    b.lt skip                     // no data or unified cache at this level
    msr csselr_el1, x10         // select this cache level
    isb                         // sync change of csselr
    mrs x1, ccsidr_el1             // read ccsidr
    and w2, w1, #7                 // w2 = log2(linelen)-4
    add w2, w2, #4                 // w2 = log2(linelen)
    ubfx w4, w1, #3, #10         // w4 = max way number, right aligned
    clz w5, w4                     // w5 = 32-log2(ways), bit position of way in dc operand
    lsl w9, w4, w5                 // w9 = max way number, aligned to position in dc operand
    lsl w16, w8, w5             // w16 = amount to decrement way number per iteration
loop2: 
    ubfx w7, w1, #13, #15         // w7 = max set number, right aligned
    lsl w7, w7, w2                 // w7 = max set number, aligned to position in dc operand
    lsl w17, w8, w2             // w17 = amount to decrement set number per iteration
loop3: 
    orr w11, w10, w9             // w11 = combine way number and cache number ...
    orr w11, w11, w7             // ... and set number for dc operand
    dc csw, x11                 // do data cache clean by set and way
    subs w7, w7, w17             // decrement set number
    b.ge loop3
    subs x9, x9, x16             // decrement way number
    b.ge loop2
skip: 
    add w10, w10, #2         // increment 2 x cache level
    cmp w3, w10
    b.gt loop1
finished:
    mov    x10, #0
    msr    csselr_el1, x10
    dsb    sy
    isb

    ldp x0, x1,  [sp], #16
    ldp x2, x3,  [sp], #16
    ldp x4, x5,  [sp], #16
    ldp x6, x7,  [sp], #16
    ldp x8, x9,  [sp], #16
    ldp x10, x11,  [sp], #16
    ret

arm64_invalidate_tlb:

    sub     sp, sp, #96

    stp     x0, x1, [sp, #16 * 0]
    stp     x2, x3, [sp, #16 * 1]
    stp     x4, x5, [sp, #16 * 2]
    stp     x6, x7, [sp, #16 * 3]
    stp     x8, x9, [sp, #16 * 4]
    stp     x10, x11, [sp, #16 * 5]

    tlbi    vmalle1
    dsb    sy
    isb

    ldp x0, x1,  [sp], #16
    ldp x2, x3,  [sp], #16
    ldp x4, x5,  [sp], #16
    ldp x6, x7,  [sp], #16
    ldp x8, x9,  [sp], #16
    ldp x10, x11,  [sp], #16
    ret

arm64_enable_mmu:
    mov x12, x30 //lr
    mov x1, #0xff
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

    mrs x0, sctlr_el1
    orr x0, x0, #(0x1 << 2)       // D bit, dcache
    orr x0, x0, #(0x1 << 12)      // I bit, icache
    orr x0, x0, #1              // M bit, mmu
    msr sctlr_el1, x0
    dsb sy
    isb

    ret x12

