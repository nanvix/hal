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

#ifndef TARGET_QEMU_RISCV32_STDOUT_H_
#define TARGET_QEMU_RISCV32_STDOUT_H_

	/* Target Interface Implementation */
	#include <arch/target/qemu/riscv32/_riscv32.h>

/**
 * @defgroup target-qemu-riscv32-stdout Standard Output
 * @ingroup target-qemu-riscv32
 *
 * @brief Standard Output System
 */
/**@{*/

	#include <arch/stdout/16550a.h>
	#include <posix/sys/types.h>

	/**
	 * @see uart8250_init()
	 */
	static inline void qemu_riscv32_stdout_init(void)
	{
		uart_16550a_init();
	}

	/**
	 * @see uart8250_write()
	 */
	static inline void qemu_riscv32_stdout_write(const char *buf, size_t n)
	{
		uart_16550a_write(buf, n);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __stdout_init_fn  /**< stdout_init()  */
	#define __stdout_write_fn /**< stdout_write() */
	/**@}*/

	/**
	 * @see qemu_riscv32_stdout_init().
	 */
	static inline void stdout_init(void)
	{
		qemu_riscv32_stdout_init();
	}

	/**
	 * @see qemu_riscv32_stdout_write().
	 */
	static inline void stdout_write(const char *buf, size_t n)
	{
		qemu_riscv32_stdout_write(buf, n);
	}

/**@endcond*/

#endif /* TARGET_QEMU_RISCV32_STDOUT_H_ */
