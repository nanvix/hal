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

#ifndef ARCH_I486_TIMER_H_
#define ARCH_I486_TIMER_H_

/**
 * @addtogroup i486-core-timer Timer
 * @ingroup i486-core
 *
 * @brief Timer Interface
 */
/**@{*/

	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Oscillator frequency (in Hz)
	 */
	#define PIT_FREQUENCY 1193182

	/**
	 * @name Registers
	 */
	/**@{*/
	#define PIT_CTRL 0x43 /**< Control */
	#define PIT_DATA 0x40 /**< Data    */
	/**@}*/

	/**
	 * @brief Initializes the timer device.
	 *
	 * @param freq Target frequency for the timer device.
	 */
	EXTERN void i486_timer_init(unsigned freq);

	/**
	 * @brief Resets the timer device.
	 */
	EXTERN void i486_timer_reset(void);

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* ARCH_I486_TIMER_H_ */
