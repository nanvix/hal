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

#ifndef DRIVER_PL011_H_
#define DRIVER_PL011_H_


/**
 * @addtogroup dev-pl011 pl011 UART
 * @ingroup dev
 */
/**@{*/

    #include <posix/stddef.h>
    #include <posix/stdint.h>
    #include <posix/stdbool.h>

    /**
	 * @brief UART definitions
	 */
	/**@{*/
    #define UART_ADDR   0x9000000
    #define UART_BAUD      921600
    #define UART_TIMER   20000000

    /**
	 * @brief Register Interface
	 */
	/**@{*/
    #define UART_DR     0x00  /* 0x00 Data Register */
    #define UART_RSRECT 0x01  /* 0x01 Receive status / error clear register */
    #define UART_FR     0x06  /* 0x06 Flag register */
    #define UART_ILPR   0x08  /* 0x08 Low-power counter register */
    #define UART_IBRD   0x09  /* 0x09 Integer baudrate register */
    #define UART_FBRD   0x0A  /* 0x0A Fractional baudrate register */
    #define UART_LCRH   0x0B  /* 0x0B Line control register */
    #define UART_CR     0x0C  /* 0x0C Control register */

    
    /**
	 * PL011 Shift Macros
	 */
	/**@{*/
    #define DR_DATA_MASK    (0xFFu)
    #define FR_BUSY         (1 << 3u)
    #define FR_TXFF         (1 << 5u)
    #define LCRH_FEN        (1 << 4u)
    #define LCRH_PEN        (1 << 1u)
    #define LCRH_EPS        (1 << 2u)
    #define LCRH_STP2       (1 << 3u)
    #define LCRH_SPS        (1 << 7u)
    #define CR_UARTEN       (1 << 0u)
    #define LCRH_WLEN_8BITS (3u << 5u)


    /**
	 * @brief Initializes the pl011 driver.
	 */
    extern void uart_pl011_init();

    /**
	 * @brief Writes a buffer on the pl011 device.
	 *
	 * @param buf Target buffer.
	 * @param n   Number of bytes to write.
	 */
    extern void uart_pl011_write(const char *buf, size_t n);
    
/**@}*/

#endif  /* DRIVER_PL011_H_ */