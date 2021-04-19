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

#ifndef ARCH_CORE_ARM64_INT_H_
#define ARCH_CORE_ARM64_INT_H_

	/* Must come first. */
	#define __NEED_CORE_LPIC

/**
 * @addtogroup arm64-core-int Hardware Interrupts
 * @ingroup arm64-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <nanvix/const.h>
	#include <arch/core/arm64/lpic.h>
	#include <arch/cluster/arm64-cluster/gic.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define ARM64_INT_NUM ARM64_IRQ_NUM


	#ifndef _ASM_FILE_
		EXTERN void arm64_do_interrupt(int intnum);
	#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM         ARM64_INT_NUM   /**< @ref ARM64_INT_NUM   */
	#define INTERRUPT_TIMER        27 /**< @ref ARM64_INT_TIMER */
	#define INTERRUPT_IPI          250 /**< @ref ARM64_INT_TIMER */
	#define INTERRUPT_LEVEL_LOW    0  /**< @ref ARM64_IRQLVL_5  */
	#define INTERRUPT_LEVEL_MEDIUM 0  /**< @ref ARM64_IRQLVL_3  */
	#define INTERRUPT_LEVEL_HIGH   0  /**< @ref ARM64_IRQLVL_1  */
	#define INTERRUPT_LEVEL_NONE   0  /**< @ref ARM64_IRQLVL_0  */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __interrupt_handlers_var /**< @ref interrupt_handlers */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __interrupts_disable_fn   /**< @ref interrupts_disable()   */
	#define __interrupts_enable_fn    /**< @ref interrupts_enable()    */
	#define __interrupts_get_level_fn /**< @ref interrupts_get_level() */
	#define __interrupts_set_level_fn /**< @ref interrupts_set_level() */
	#define __interrupt_next_fn       /**< @ref interrupt_next()       */
	#define __interrupt_mask_fn       /**< @ref interrupt_mask()       */
	#define __interrupt_unmask_fn     /**< @ref interrupt_unmask()     */
	#define __interrupt_ack_fn        /**< @ref interrupt_ack()        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see arm64_enable_irq().
	 */
	static inline void interrupts_enable(void)
	{
		arm64_enable_irq();
	}

	/**
	 * @see arm64_disable_irq().
	 */
	static inline void interrupts_disable(void)
	{
		arm64_disable_irq();
	}

	/**
	 * 
	 */
	static inline int interrupts_get_level(void)
	{
		return 0;
	}

	/**
	 * 
	 */
	static inline int interrupts_set_level(int newlevel)
	{
		UNUSED(newlevel);
		return 0;
	}

	/**
	 * @see arm64_gicd_enable_int()
	 */
	static inline int interrupt_mask(int intnum)
	{
		arm64_gicd_disable_int(intnum);
		return 0;
	}

	/**
	 * @see arm64_gicd_disable_int()
	 */
	static inline int interrupt_unmask(int intnum)
	{
		arm64_gicd_enable_int(intnum);
		
		return 0;
	}

	/**
	 * @see arm64_pic_ack()
	 */
	static inline void interrupt_ack(int intnum)
	{
		arm64_gic_eoi(intnum);
	}

	/**
	 * @see arm64_pic_next().
	 */
	static inline int interrupt_next(void)
	{
		return arm64_gic_find_pending_irq();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_INT_H_ */
