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

#ifndef ARCH_CLUSTER_ARM64_CLUSTER_GIC_H_
#define ARCH_CLUSTER_ARM64_CLUSTER_GIC_H_

#include <posix/stdint.h>

/*
 * GIC on QEMU Virt 
 */
#define QEMU_VIRT_GIC_PA_BASE	    (0x08000000)
#define QEMU_VIRT_GIC_BASE			(0xffff00003fe00000)
#define QEMU_VIRT_GIC_INT_MAX		(64)
#define QEMU_VIRT_GIC_PRIO_MAX		(16)
/* SGI: Interrupt IDs 0-15 */
/* PPI: Interrupt IDs 16-31 */
/* SPI: Interrupt IDs 32-63 */
#define QEMU_VIRT_GIC_INTNO_SGIO	(0)
#define QEMU_VIRT_GIC_INTNO_PPIO	(16)
#define QEMU_VIRT_GIC_INTNO_SPIO	(32)

#define GIC_BASE					(QEMU_VIRT_GIC_BASE)
#define GIC_INT_MAX					(QEMU_VIRT_GIC_INT_MAX)
#define GIC_PRIO_MAX				(QEMU_VIRT_GIC_PRIO_MAX)
#define GIC_INTNO_SGI0				(QEMU_VIRT_GIC_INTNO_SGIO)
#define GIC_INTNO_PPI0				(QEMU_VIRT_GIC_INTNO_PPIO)
#define GIC_INTNO_SPI0				(QEMU_VIRT_GIC_INTNO_SPIO)

#define GIC_PRI_SHIFT				(4)
#define GIC_PRI_MASK				(0x0f)

#define TIMER_IRQ					(27)  /** Timer IRQ  */



#define GIC_GICD_BASE		(GIC_BASE)  /* GICD MMIO base address */
#define GIC_GICC_BASE		(GIC_BASE + 0x10000) /* GICC MMIO base address */

#define GIC_GICD_INT_PER_REG			(32)	/* 32 interrupts per reg */
#define GIC_GICD_IPRIORITY_PER_REG		(4)		/* 4 priority per reg */
#define GIC_GICD_IPRIORITY_SIZE_PER_REG	(8) 	/* priority element size */
#define GIC_GICD_ITARGETSR_CORE0_TARGET_BMAP (0x01010101) /* CPU interface 0 */
#define GIC_GICD_ITARGETSR_PER_REG		(4) 
#define GIC_GICD_ITARGETSR_SIZE_PER_REG	(8) 
#define GIC_GICD_ICFGR_PER_REG			(16) 
#define GIC_GICD_ICFGR_SIZE_PER_REG		(2) 
#define GIC_GICD_ICENABLER_PER_REG		(32)
#define GIC_GICD_ISENABLER_PER_REG		(32)
#define GIC_GICD_ICPENDR_PER_REG		(32)
#define GIC_GICD_ISPENDR_PER_REG		(32)

/* 8.12 The GIC CPU interface register map */
#define GIC_GICC_CTLR     (GIC_GICC_BASE + 0x000) /* CPU Interface Control Register */
#define GIC_GICC_PMR      (GIC_GICC_BASE + 0x004) /* Interrupt Priority Mask Register */
#define GIC_GICC_BPR      (GIC_GICC_BASE + 0x008) /* Binary Point Register */
#define GIC_GICC_IAR      (GIC_GICC_BASE + 0x00C) /* Interrupt Acknowledge Register */
#define GIC_GICC_EOIR     (GIC_GICC_BASE + 0x010) /* End of Interrupt Register */
#define GIC_GICC_RPR      (GIC_GICC_BASE + 0x014) /* Running Priority Register */
#define GIC_GICC_HPIR     (GIC_GICC_BASE + 0x018) /* Highest Pending Interrupt Register */
#define GIC_GICC_ABPR     (GIC_GICC_BASE + 0x01C) /* Aliased Binary Point Register */
#define GIC_GICC_IIDR     (GIC_GICC_BASE + 0x0FC) /* CPU Interface Identification Register */

/* 8.13.7 GICC_CTLR, CPU Interface Control Register */
#define GICC_CTLR_ENABLE			(0x1)	/* Enable GICC */
#define GICC_CTLR_DISABLE			(0x0)	/* Disable GICC */

/* 8.13.14 GICC_PMR, CPU Interface Priority Mask Register */
#define GICC_PMR_PRIO_MIN			(0xff)	/* The lowest level mask */
#define GICC_PMR_PRIO_HIGH			(0x0)	/* The highest level mask */

/* 8.13.6 GICC_BPR, CPU Interface Binary Point Register */
/* In systems that support only one Security state, when GICC_CTLR.CBPR == 0, 
this register determines only Group 0 interrupt preemption. */
#define GICC_BPR_NO_GROUP			(0x0)	/* handle all interrupts */

/* 8.13.11 GICC_IAR, CPU Interface Interrupt Acknowledge Register */
#define GICC_IAR_INTR_IDMASK		(0x3ff)	/* 0-9 bits means Interrupt ID */
#define GICC_IAR_SPURIOUS_INTR		(0x3ff)	/* 1023 means spurious interrupt */

/* 8.8 The GIC Distributor register map */
#define GIC_GICD_CTLR			(GIC_GICD_BASE + 0x000)					/* Distributor Control Register */
#define GIC_GICD_TYPER			(GIC_GICD_BASE + 0x004)					/* Interrupt Controller Type Register */
#define GIC_GICD_IIDR			(GIC_GICD_BASE + 0x008)					/* Distributor Implementer Identification Register */
#define GIC_GICD_IGROUPR(n)		(GIC_GICD_BASE + 0x080 + ( (n) * 4 ) )	/* Interrupt Group Registers */
#define GIC_GICD_ISENABLER(n)	(GIC_GICD_BASE + 0x100 + ( (n) * 4 ) )	/* Interrupt Set-Enable Registers */
#define GIC_GICD_ICENABLER(n)	(GIC_GICD_BASE + 0x180 + ( (n) * 4 ) )	/* Interrupt Clear-Enable Registers */
#define GIC_GICD_ISPENDR(n)		(GIC_GICD_BASE + 0x200 + ( (n) * 4 ) )	/* Interrupt Set-Pending Registers */
#define GIC_GICD_ICPENDR(n)		(GIC_GICD_BASE + 0x280 + ( (n) * 4 ) )	/* Interrupt Clear-Pending Registers */
#define GIC_GICD_ISACTIVER(n)	(GIC_GICD_BASE + 0x300 + ( (n) * 4 ) )	/* Interrupt Set-Active Registers */
#define GIC_GICD_ICACTIVER(n)	(GIC_GICD_BASE + 0x380 + ( (n) * 4 ) )	/* Interrupt Clear-Active Registers */
#define GIC_GICD_IPRIORITYR(n)	(GIC_GICD_BASE + 0x400 + ( (n) * 4 ) )	/* Interrupt Priority Registers */
#define GIC_GICD_ITARGETSR(n)	(GIC_GICD_BASE + 0x800 + ( (n) * 4 ) )	/* Interrupt Processor Targets Registers */
#define GIC_GICD_ICFGR(n)		(GIC_GICD_BASE + 0xc00 + ( (n) * 4 ) )	/* Interrupt Configuration Registers */
#define GIC_GICD_NSCAR(n)		(GIC_GICD_BASE + 0xe00 + ( (n) * 4 ) )	/* Non-secure Access Control Registers */
#define GIC_GICD_SGIR			(GIC_GICD_BASE + 0xf00 )				/* Software Generated Interrupt Register */
#define GIC_GICD_CPENDSGIR(n)	(GIC_GICD_BASE + 0xf10 + ( (n) * 4 ) )	/* SGI Clear-Pending Registers */
#define GIC_GICD_SPENDSGIR(n)	(GIC_GICD_BASE + 0xf20 + ( (n) * 4 ) )	/* SGI Set-Pending Registers */

/* 8.9.4 GICD_CTLR, Distributor Control Register */
#define GIC_GICD_CTLR_ENABLE	(0x1)	/* Enable GICD */
#define GIC_GICD_CTLR_DISABLE	(0x0)	/* Disable GICD */

/* 8.9.7 GICD_ICFGR<n>, Interrupt Configuration Registers */
#define GIC_GICD_ICFGR_LEVEL	(0x0)	/* level-sensitive */
#define GIC_GICD_ICFGR_EDGE		(0x2)	/* edge-triggered */

/* Register access macros for GICC */
#define REG_GIC_GICC_CTLR          ((volatile uint32_t *)(uintptr_t)GIC_GICC_CTLR)
#define REG_GIC_GICC_PMR           ((volatile uint32_t *)(uintptr_t)GIC_GICC_PMR)
#define REG_GIC_GICC_BPR           ((volatile uint32_t *)(uintptr_t)GIC_GICC_BPR)
#define REG_GIC_GICC_IAR           ((volatile uint32_t *)(uintptr_t)GIC_GICC_IAR)
#define REG_GIC_GICC_EOIR          ((volatile uint32_t *)(uintptr_t)GIC_GICC_EOIR)
#define REG_GIC_GICC_RPR           ((volatile uint32_t *)(uintptr_t)GIC_GICC_RPR)
#define REG_GIC_GICC_HPIR          ((volatile uint32_t *)(uintptr_t)GIC_GICC_HPIR)
#define REG_GIC_GICC_ABPR          ((volatile uint32_t *)(uintptr_t)GIC_GICC_ABPR)
#define REG_GIC_GICC_IIDR          ((volatile uint32_t *)(uintptr_t)GIC_GICC_IIDR)

/* Register access macros for GICD */
#define REG_GIC_GICD_CTLR             ((volatile uint32_t *)(uintptr_t)GIC_GICD_CTLR)
#define REG_GIC_GICD_TYPE             ((volatile uint32_t *)(uintptr_t)GIC_GICD_TYPE)
#define REG_GIC_GICD_IIDR             ((volatile uint32_t *)(uintptr_t)GIC_GICD_IIDR)
#define REG_GIC_GICD_IGROUPR(n)       ((volatile uint32_t *)(uintptr_t)GIC_GICD_IGROUPR(n))
#define REG_GIC_GICD_ISENABLER(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_ISENABLER(n))
#define REG_GIC_GICD_ICENABLER(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_ICENABLER(n))
#define REG_GIC_GICD_ISPENDR(n)       ((volatile uint32_t *)(uintptr_t)GIC_GICD_ISPENDR(n))
#define REG_GIC_GICD_ICPENDR(n)       ((volatile uint32_t *)(uintptr_t)GIC_GICD_ICPENDR(n))
#define REG_GIC_GICD_ISACTIVER(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_ISACTIVER(n))
#define REG_GIC_GICD_ICACTIVER(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_ICACTIVER(n))
#define REG_GIC_GICD_IPRIORITYR(n)    ((volatile uint32_t *)(uintptr_t)GIC_GICD_IPRIORITYR(n))
#define REG_GIC_GICD_ITARGETSR(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_ITARGETSR(n))
#define REG_GIC_GICD_ICFGR(n)         ((volatile uint32_t *)(uintptr_t)GIC_GICD_ICFGR(n))
#define REG_GIC_GICD_NSCAR(n)         ((volatile uint32_t *)(uintptr_t)GIC_GICD_NSCAR(n))
#define REG_GIC_GICD_SGIR             ((volatile uint32_t *)(uintptr_t)GIC_GICD_SGIR)
#define REG_GIC_GICD_CPENDSGIR(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_CPENDSGIR(n))
#define REG_GIC_GICD_SPENDSGIR(n)     ((volatile uint32_t *)(uintptr_t)GIC_GICD_SPENDSGIR(n))

#define CNTV_CTL_ENABLE		(1 << 0)	/* Enables the timer */	
#define DAIF_IRQ_BIT		(1<<1)	/* IRQ mask bit */
#define DAIF_IRQ_BIT_DISABLE		(0)	/* IRQ mask bit */

#ifndef _ASM_FILE_

    typedef int32_t irq_no;			/* IRQ no */

    void arm64_gic_initialize(void);
    void arm64_gic_eoi(irq_no irq);
    void arm64_gicd_disable_int(irq_no irq);
    void arm64_gicd_enable_int(irq_no irq);
    void arm64_gicd_clear_pending(irq_no irq);
    void arm64_enable_irq(void);
    void arm64_disable_irq(void);
    void arm64_gic_configure_timer(void);
    irq_no arm64_gic_find_pending_irq(void);

#endif

#endif
