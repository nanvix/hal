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

#ifndef ARCH_CORE_LINUX64_INT_H_
#define ARCH_CORE_LINUX64_INT_H_

/**
 * @addtogroup linux64-core-int Hardware Interrupts
 * @ingroup linux64-core
 *
 * @brief Hardware and Software Interrupts
 */
/**@{*/

	#include <nanvix/hlib.h>
	#include <signal.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define LINUX64_INT_NUM                2
	#define LINUX64_INT_MAX_NUM (SIGALRM + 1)
	#define LINUX64_INT_IPI              265 /* Dummy IPI */

	/**
	 * @brief Enable all the interrupts.
	 */
	EXTERN void linux64_interrupts_disable(void);

	/**
	 * @brief Disable all the interrupts.
	 */
	EXTERN void linux64_interrupts_enable(void);

	/**
	 * @brief Unmask a interrupt.
	 *
	 * @param The number of the interrupt to mask.
	 *
	 * @return 0 if succesful, negative value otherwise.
	 */
	EXTERN int linux64_interrupt_mask(int intnum);

	/**
	 * @brief Unmask a interrupt.
	 *
	 * @param The number of the interrupt to unmask.
	 *
	 * @return 0 if succesful, negative value otherwise.
	 */
	EXTERN int linux64_interrupt_unmask(int intnum);

	/**
 	* @brief Give the next interrupt called while blocked.
 	*
 	* @return The number of the next interrupt OR a negative value if there is none.
 	*/
	EXTERN int linux64_interrupt_next(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define INTERRUPTS_NUM  LINUX64_INT_MAX_NUM /**< @ref LINUX64_INT_NUM */
	#define INTERRUPT_IPI	LINUX64_INT_IPI     /**< @ref LINUX64_INT_IPI */
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
	#define __interrupts_disable_fn /**< @ref interrupts_disable() */
	#define __interrupts_enable_fn  /**< @ref interrupts_enable()  */
	#define __interrupt_next_fn     /**< @ref interrupt_next()     */
	#define __interrupt_mask_fn     /**< @ref interrupt_mask()     */
	#define __interrupt_unmask_fn   /**< @ref interrupt_unmask()   */
	#define __interrupt_ack_fn      /**< @ref interrupt_ack()      */
	/**@}*/

	/**
	 * @see linux64_core_int_disable().
	 */
	static inline void interrupts_disable(void)
	{
		linux64_interrupts_disable();
	}

	/**
	 * @see linux64_interrupts_enable().
	 */
	static inline void interrupts_enable(void)
	{
		linux64_interrupts_enable();
	}

	/**
	 * @see linux64_interrupt_mask().
	 */
	static inline int interrupt_mask(int intnum)
	{
		return (linux64_interrupt_mask(intnum));
	}

	/**
	 * @see linux64_interrupt_unmask().
	 */
	static inline int interrupt_unmask(int intnum)
	{
		return (linux64_interrupt_unmask(intnum));
	}

	/**
	 * @brief Dummy function, No Interrupt controller in linux64
	 */
	static inline void interrupt_ack(int intnum)
	{
		UNUSED(intnum);
	}

	/**
	 * @see linux64_interrupt_next().
	 */
	static inline int interrupt_next(void)
	{
		return (linux64_interrupt_next());
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_INT_H_ */
