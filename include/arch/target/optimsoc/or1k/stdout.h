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

#ifndef TARGET_OPTIMSOC_STDOUT_H_
#define TARGET_OPTIMSOC_STDOUT_H_

	/* Target Interface Implementation */
	#include <arch/target/optimsoc/or1k/_or1k.h>

/**
 * @defgroup target-qemu-or1k-pc-stdout Standard Output
 * @ingroup target-qemu-or1k-pc
 *
 * @brief Standard Output System
 */
/**@{*/

	#include <arch/stdout/systrace.h>
	#include <sys/types.h>

	/**
	 * @see systrace_init()
	 */
	static inline void optimsoc_stdout_init(void)
	{
		systrace_init();
	}

	/**
	 * @see systrace_write()
	 */
	static inline void optimsoc_stdout_write(const char *buf, size_t n)
	{
		systrace_write(buf, n);
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
	 * @see qemu_or1kpc_stdout_init().
	 */
	static inline void stdout_init(void)
	{
		optimsoc_stdout_init();
	}

	/**
	 * @see qemu_or1kpc_stdout_write().
	 */
	static inline void stdout_write(const char *buf, size_t n)
	{
		optimsoc_stdout_write(buf, n);
	}

/**@endcond*/

#endif /* TARGET_OPTIMSOC_STDOUT_H_ */
