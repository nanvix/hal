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
#include <arch/stdout/pl011.h>
#include <nanvix/const.h>
#include <posix/stdint.h>

/**
 * @brief Control register.
 */
PRIVATE volatile uint32_t *uart = (uint32_t *)UART_ADDR;

/**
 * @brief Was the device was initialized?
 */
PRIVATE bool initialized = false;

/**
 * Writes into serial port.
 */
PUBLIC void uart_pl011_write(const char *buf, size_t n)
{

	/**
	 * It's important to only try to write if the device
	 * was already initialized.
	 */
	if (!initialized)
		uart_pl011_init();
	
	while (n)
	{
		/* Wait until FIFO is empty. */
		while (uart[UART_FR] & FR_TXFF);
			/* noop */;

		/* Write character to device. */
		uart[UART_DR] = *buf++;

		n--;
	}
}

/**
 * Initializes the UART 16550a device.
 */
void uart_pl011_init(void)
{

	/* Do not re-initialize the device. */
	if (initialized)
		return;

    /* Reset the device */
    uart[UART_CR] &= ~CR_UARTEN;
    while (uart[UART_FR] & FR_BUSY);
    uart[UART_LCRH] &= ~LCRH_FEN;

	/* Calculate and set divisor. */
    uart[UART_IBRD] = (uint16_t) UART_TIMER/(UART_BAUD << 4);
    uart[UART_FBRD] = (uint8_t) ((UART_TIMER/(UART_BAUD << 4) - ((uint16_t) UART_TIMER/(UART_BAUD << 4))) >> 5) + 0.5;

	/*
	 * Set line control register:
	 *  - 8 bits per character
	 *  - 1 stop bit
	 *  - No parity
	 *  - Break disabled
	 *  - Disallow access to divisor latch
	 */
    uint32_t lcrh = 0u;
    lcrh |= LCRH_WLEN_8BITS;
    lcrh &= ~LCRH_PEN;
    lcrh &= ~LCRH_EPS;
    lcrh &= ~LCRH_SPS;
    lcrh &= ~LCRH_STP2;
    lcrh |= LCRH_FEN;
    uart[UART_LCRH] = lcrh;
    uart[UART_CR] |= CR_UARTEN;

	/* Device initialized. */
	initialized = true;
}
