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

#ifndef ARCH_STDOUT_16550A_H_
#define ARCH_STDOUT_16550A_H_

/**
 * @addtogroup dev-16550a 16550a UART
 * @ingroup dev
 */
/**@{*/

	#include <posix/stddef.h>
	#include <posix/stdint.h>

	/**
	 * @brief UART definitions
	 */
	/**@{*/
	#define UART_ADDR  0x10000000
	#define UART_TIMER    1843200
	#define UART_BAUD      115200
	/**@}*/

	/**
	 * @brief Register Interface
	 */
	/**@{*/
	#define UART_RBR 0x00  /**< Receiver Buffer (R)              */
	#define UART_THR 0x00  /**< Transmitter Holding Register (W) */
	#define UART_IER 0x01  /**< Interrupt Enable Register (RW)   */
	#define UART_DLL 0x00  /**< Divisor LSB (LCR_DLAB) (RW)      */
	#define UART_DLM 0x01  /**< Divisor MSB (LCR_DLAB) (RW)      */
	#define UART_FCR 0x02  /**< FIFO Control Register (W)        */
	#define UART_LCR 0x03  /**< Line Control Register (RW)       */
	#define UART_MCR 0x04  /**< Modem Control Register (W)       */
	#define UART_LSR 0x05  /**< Line Status Register (R)         */
	#define UART_MSR 0x06  /**< Modem Status Register (R)        */
	#define UART_SCR 0x07  /**< Scratch Register (RW)            */
	/**@}*/

	/**
	 * Divisor Register (Accessed when DLAB bit in LCR is set)
	 */
	/**@{*/
	#define UART_LCR_DLAB 0x80 /**< Divisor Latch Bit */
	#define UART_LCR_8BIT 0x03 /**< 8-bit             */
	#define UART_LCR_PODD 0x08 /**< Parity Odd        */
	/**@}*/

	/**
	 * Line Status Register
	 */
	/**@{*/
	#define UART_LSR_DA 0x01 /**< Data Available         */
	#define UART_LSR_OE 0x02 /**< Overrun Error          */
	#define UART_LSR_PE 0x04 /**< Parity Error           */
	#define UART_LSR_FE 0x08 /**< Framing Error          */
	#define UART_LSR_BI 0x10 /**< Break Indicator        */
	#define UART_LSR_RE 0x20 /**< THR is Empty           */
	#define UART_LSR_RI 0x40 /**< Idle Line              */
	#define UART_LSR_EF 0x80 /**< Erroneous Data in FIFO */
	/**@}*/

	/**
	 * @brief Initializes the 16550a driver.
	 */
	extern void uart_16550a_init(void);

	/**
	 * @brief Writes a buffer on the 16550a device.
	 *
	 * @param buf Target buffer.
	 * @param n   Number of bytes to write.
	 */
	extern void uart_16550a_write(const char *buf, size_t n);

/**@}*/

#endif /* ARCH_STDOUT_16550A_H_ */
