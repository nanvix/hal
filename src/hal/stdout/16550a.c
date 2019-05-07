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

#include <nanvix/hal/hal.h>
#include <arch/stdout/16550a.h>
#include <nanvix/const.h>
#include <stdint.h>

/**
 * @brief lock.
 */
PRIVATE spinlock_t lock = SPINLOCK_UNLOCKED;

/**
 * @brief Control register.
 */
PRIVATE volatile uint8_t *uart = (uint8_t *)UART_ADDR;

/**
 * @brief Was the device was initialized?
 */
PRIVATE bool initialized = false;

/**
 * Writes into serial port.
 */
PUBLIC void uart_16550a_write(const char *buf, size_t n)
{
	size_t counter;
	counter = 0;

	/**
	 * It's important to only try to write if the device
	 * was already initialized.
	 */
	if (!initialized)
		return;

	spinlock_lock(&lock);

	while (n)
	{
		/* Wait until FIFO is empty. */
		while ((uart[UART_LSR] & UART_LSR_RI) == 0)
			/* noop */;

		/* Write character to device. */
		uart[UART_THR] = buf[counter] & 0xff;

		n--;
		counter++;
	}

	spinlock_unlock(&lock);
}

/**
 * Initializes the UART 16550a device.
 */
void uart_16550a_init(void)
{
	uint32_t divisor;

	if (initialized)
		return;

	/* Calculate and set divisor. */
	divisor = UART_TIMER/(UART_BAUD << 4);
    uart[UART_LCR] = UART_LCR_DLAB;
    uart[UART_DLL] = divisor & 0xff;
    uart[UART_DLM] = (divisor >> 8) & 0xff;

	/*
	 * Set line control register:
	 *  - 8 bits per character
	 *  - 1 stop bit
	 *  - No parity
	 *  - Break disabled
	 *  - Disallow access to divisor latch
	 */
    uart[UART_LCR] = UART_LCR_PODD | UART_LCR_8BIT;

	/* Device initialized. */
	initialized = true;
}
