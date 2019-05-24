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

#ifndef TARGET_UNIX64_STDOUT_H_
#define TARGET_UNIX64_STDOUT_H_

	/* Target Interface Implementation */
	#include <arch/target/unix64/unix64/_unix64.h>

/**
 * @defgroup target-qemu-unix64-pc-stdout Standard Output
 * @ingroup target-qemu-unix64-pc
 *
 * @brief Standard Output System
 */
/**@{*/

	#include <nanvix/const.h>
	#include <nanvix/klib.h>
	#include <arch/stdout/tty-virt.h>

	/**
	 * @see systrace_init()
	 */
	static inline void unix64_stdout_init(void)
	{
		tty_virt_init();
	}

	/**
	 * @see systrace_write()
	 */
	static inline void unix64_stdout_write(const char *buf, size_t n)
	{
		tty_virt_write(buf,n);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond unix64
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __stdout_init_fn  /**< stdout_init()  */
	#define __stdout_write_fn /**< stdout_write() */
	/**@}*/

	/**
	 * @see unix64_stdout_init().
	 */
	static inline void stdout_init(void)
	{
		unix64_stdout_init();
	}

	/**
	 * @see unix64_stdout_write().
	 */
	static inline void stdout_write(const char *buf, size_t n)
	{
		unix64_stdout_write(buf, n);
	}

/**@endcond*/

#endif /* TARGET_UNIX64_STDOUT_H_ */
