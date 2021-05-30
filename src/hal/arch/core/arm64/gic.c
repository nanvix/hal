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

#include <arch/core/arm64/gic.h>
#include <nanvix/const.h>
#include <posix/stdint.h>

/**
 * @brief Initialize the Generic Interrupt Controller (CPU Interface).
 */
PRIVATE void arm64_init_gicc(void)
{
	uint32_t pending_irq;

	/* Disable CPU interface */
	*REG_GIC_GICC_CTLR = GICC_CTLR_DISABLE;

	/* Set the priority level as the lowest priority */
	*REG_GIC_GICC_PMR = GICC_PMR_PRIO_MIN;

	/* Handle all of interrupts in a single group */
	*REG_GIC_GICC_BPR = GICC_BPR_NO_GROUP;

	/* Clear all of the active interrupts */
	for(pending_irq = ( *REG_GIC_GICC_IAR & GICC_IAR_INTR_IDMASK ); 
	    ( pending_irq != GICC_IAR_SPURIOUS_INTR );
	    pending_irq = ( *REG_GIC_GICC_IAR & GICC_IAR_INTR_IDMASK ) )
		*REG_GIC_GICC_EOIR = *REG_GIC_GICC_IAR;

	/* Enable CPU interface */
	*REG_GIC_GICC_CTLR = GICC_CTLR_ENABLE;
}

/**
 * @brief Initialize the Generic Interrupt Controller (Distributor).
 */
PRIVATE void arm64_init_gicd(void)
{
	int32_t	i, regs_nr;

	/* Diable distributor */
	*REG_GIC_GICD_CTLR = GIC_GICD_CTLR_DISABLE;

	/* Disable all IRQs */
	regs_nr = (GIC_INT_MAX + GIC_GICD_INT_PER_REG - 1) / GIC_GICD_INT_PER_REG;
	for (i = 0; regs_nr > i; ++i)
		*REG_GIC_GICD_ICENABLER(i) = ~((uint32_t)(0)); 

	/* Clear all pending IRQs */
	regs_nr = (GIC_INT_MAX + GIC_GICD_INT_PER_REG - 1) / GIC_GICD_INT_PER_REG;
	for (i = 0; regs_nr > i; ++i) 
		*REG_GIC_GICD_ICPENDR(i) = ~((uint32_t)(0));

	/* Set all of interrupt priorities as the lowest priority */
	regs_nr = ( GIC_INT_MAX + GIC_GICD_IPRIORITY_PER_REG - 1) / 
		GIC_GICD_IPRIORITY_PER_REG ;
	for (i = 0; regs_nr > i; i++)
		*REG_GIC_GICD_IPRIORITYR(i) = ~((uint32_t)(0));

	/* Set target of all of shared peripherals to processor 0 */
	for (i = GIC_INTNO_SPI0 / GIC_GICD_ITARGETSR_PER_REG;
	     ( (GIC_INT_MAX + (GIC_GICD_ITARGETSR_PER_REG - 1) ) / 
		 GIC_GICD_ITARGETSR_PER_REG ) > i; ++i) 
		*REG_GIC_GICD_ITARGETSR(i) = 
			(uint32_t)GIC_GICD_ITARGETSR_CORE0_TARGET_BMAP;

	/* Set trigger type for all peripheral interrupts level triggered */
	for (i = GIC_INTNO_PPI0 / GIC_GICD_ICFGR_PER_REG;
	     (GIC_INT_MAX + (GIC_GICD_ICFGR_PER_REG - 1)) / GIC_GICD_ICFGR_PER_REG > i; ++i)
		*REG_GIC_GICD_ICFGR(i) = GIC_GICD_ICFGR_LEVEL;

	/* Enable distributor */
	*REG_GIC_GICD_CTLR = GIC_GICD_CTLR_ENABLE;
}

/**
 * @brief Disable a interruption.
 * 
 * @param irq Interruption ID.
 */
PUBLIC void arm64_gicd_disable_int(irq_no irq) {
	*REG_GIC_GICD_ICENABLER( (irq / GIC_GICD_ICENABLER_PER_REG) ) = 
		1U << ( irq % GIC_GICD_ICENABLER_PER_REG );
}

/**
 * @brief Enable a interruption.
 * 
 * @param irq Interruption ID.
 */
PUBLIC void arm64_gicd_enable_int(irq_no irq) {

	*REG_GIC_GICD_ISENABLER( (irq / GIC_GICD_ISENABLER_PER_REG) ) =
		1U << ( irq % GIC_GICD_ISENABLER_PER_REG );

}

/**
 * @brief Clear a pendind interruption.
 * 
 * @param irq Interruption ID.
 */
PUBLIC void arm64_gicd_clear_pending(irq_no irq) {

	*REG_GIC_GICD_ICPENDR( (irq / GIC_GICD_ICPENDR_PER_REG) ) = 
		1U << ( irq % GIC_GICD_ICPENDR_PER_REG );
}

/**
 * @brief Enable IRQ.
 */
PUBLIC void arm64_enable_irq(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_IRQ_BIT)  : "memory");
}

/**
 * @brief Disable IRQ.
 */
PUBLIC void arm64_disable_irq(void)
{
	__asm__ __volatile__("msr DAIFClr, %0\n\t" : : "i" (DAIF_IRQ_BIT_DISABLE)  : "memory");
}

/**
 * @brief Probe a pending interruption.
 * 
 * @param irq Interruption ID.
 */
PRIVATE int arm64_gicd_probe_pending(irq_no irq) {
	int is_pending;

	is_pending = ( *REG_GIC_GICD_ISPENDR( (irq / GIC_GICD_ISPENDR_PER_REG) ) &
	    ( 1U << ( irq % GIC_GICD_ISPENDR_PER_REG ) ) );

	return ( is_pending != 0 );
}

/**
 * @brief Set a interrupt target processor.
 * 
 * @param irq Interruption ID.
 * @param p Processor ID
 * 
 * 0x1 = processor 0
 * 0x2 = processor 1
 * 0x4 = processor 2
 * 0x8 = processor 3
 */
PRIVATE void arm64_gicd_set_target(irq_no irq, uint32_t p){
	uint32_t  shift;
	uint32_t    reg;

	shift = (irq % GIC_GICD_ITARGETSR_PER_REG) * GIC_GICD_ITARGETSR_SIZE_PER_REG;

	reg = *REG_GIC_GICD_ITARGETSR(irq / GIC_GICD_ITARGETSR_PER_REG);
	reg &= ~( ((uint32_t)(0xff)) << shift);
	reg |= (p << shift);
	*REG_GIC_GICD_ITARGETSR(irq / GIC_GICD_ITARGETSR_PER_REG) = reg;
}

/**
 * @brief Set a interrupt priority.
 * 
 * @param irq Interruption ID.
 * @param prio Priority.
 */
PRIVATE void arm64_gicd_set_priority(irq_no irq, uint32_t prio){
	uint32_t  shift;
	uint32_t    reg;

	shift = (irq % GIC_GICD_IPRIORITY_PER_REG) * GIC_GICD_IPRIORITY_SIZE_PER_REG;
	reg = *REG_GIC_GICD_IPRIORITYR(irq / GIC_GICD_IPRIORITY_PER_REG);
	reg &= ~(((uint32_t)(0xff)) << shift);
	reg |= (prio << shift);
	*REG_GIC_GICD_IPRIORITYR(irq / GIC_GICD_IPRIORITY_PER_REG) = reg;
}

/**
 * @brief Configure a interruption.
 * 
 * @param irq Interruption ID.
 * @param config Config
 */
PRIVATE void arm64_gicd_config(irq_no irq, unsigned int config)
{
	uint32_t	shift; 
	uint32_t	  reg;

	shift = (irq % GIC_GICD_ICFGR_PER_REG) * GIC_GICD_ICFGR_SIZE_PER_REG; /* GICD_ICFGR has 16 fields, each field has 2bits. */

	reg = *REG_GIC_GICD_ICFGR( irq / GIC_GICD_ICFGR_PER_REG);

	reg &= ~( ( (uint32_t)(0x03) ) << shift );  /* Clear the field */
	reg |= ( ( (uint32_t)config ) << shift );  /* Set the value to the field correponding to irq */
	*REG_GIC_GICD_ICFGR( irq / GIC_GICD_ICFGR_PER_REG) = reg;
}

/**
 * @brief Set EOI to IRQ line.
 * 
 * @param irq Interruption ID.
 */
PUBLIC void arm64_gic_eoi(irq_no irq) {
	arm64_gicd_clear_pending(irq);
}

/**
 * @brief Get the next pending interruption
 *
 * @param irqp Interruption ID pointer.
 *
 * @return Return the next pending interruption. 0 if there isn't pending interruption.
 */
PUBLIC irq_no arm64_gic_find_pending_irq(void) {
	irq_no i;
	for( i = 0; GIC_INT_MAX > i; ++i) {
		if (arm64_gicd_probe_pending(i)) {
			return i;
		}
	}
	return 0;
}

/**
 * @brief Initialize the Generic Interrupt Controller.
 */
PUBLIC void arm64_gic_initialize(void)
{
	arm64_init_gicd();
	arm64_init_gicc();
	arm64_enable_irq();
}

/**
 * @brief Configure the Timer interrupt.
 */
PUBLIC void arm64_gic_configure_timer(void)
{
	arm64_gicd_config(TIMER_IRQ, GIC_GICD_ICFGR_EDGE);
	arm64_gicd_set_priority(TIMER_IRQ, 0 << GIC_PRI_SHIFT );  /* Set priority */
	arm64_gicd_set_target(TIMER_IRQ, 0x1);  /* processor 0 */
	arm64_gicd_clear_pending(TIMER_IRQ);
	arm64_gicd_enable_int(TIMER_IRQ);
}