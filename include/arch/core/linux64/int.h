/*
 * MIT License
 *
 * Copyright(c) 2011-2019 The Maintainers of Nanvix
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

	#include <nanvix/klib.h>

	/**
	 * @brief Number of interrupts.
	 */
	#define LINUX64_INT_NUM 0

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
	#define INTERRUPTS_NUM  LINUX64_INT_NUM /**< @ref LINUX64_INT_NUM   */
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
		// TODO: implement.
	}

	/**
	 * @see linux64_core_pic_mask()
	 */
	static inline int interrupt_mask(int intnum)
	{
		// TODO: implement.
		UNUSED(intnum);

		return (0);
	}

	/**
	 * @see linux64_core_pic_unmask()
	 */
	static inline int interrupt_unmask(int intnum)
	{
		// TODO: implement.
		UNUSED(intnum);

		return (0);
	}

	/**
	 * @see linux64_core_pic_ack()
	 */
	static inline void interrupt_ack(int intnum)
	{
		// TODO: implement.
		UNUSED(intnum);
	}

	/**
	 * @see linux64_core_pic_next().
	 */
	static inline int interrupt_next(void)
	{
		// TODO: implement.

		return (0);
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_INT_H_ */
