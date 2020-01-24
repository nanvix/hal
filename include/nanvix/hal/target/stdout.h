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

#ifndef NANVIX_HAL_TARGET_STDOUT_H_
#define NANVIX_HAL_TARGET_STDOUT_H_

	/* Target Interface Implementation */
	#include <nanvix/hal/target/_target.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_TARGET_AL) || defined(__INTERFACE_CHECK_STDOUT)

	/* Feature Checking */
	#ifndef __TARGET_HAS_STDOUT
	#error "does this target feature a standard output device?"
	#endif

	/* Has Stadard Output Device */
	#if (__TARGET_HAS_STDOUT)

		/* Functions */
		#ifndef __stdout_init_fn
		#error "stdout_init() not defined?"
		#endif
		#ifndef __stdout_write_fn
		#error "stdout_write() not defined?"
		#endif

	#endif

#endif

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @defgroup kernel-hal-target-stdout Standard Output
 * @ingroup kernel-hal-target
 *
 * @brief Target Standard Output HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>
	#include <nanvix/hlib.h>

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the standard output device.
	 */
#if (__TARGET_HAS_STDOUT)
	EXTERN void stdout_init(void);
#else
	static inline void stdout_init(void)
	{
	}
#endif

#endif /* __NANVIX_HAL */

	/**
	 * @brief Writes to the standard output device.
	 *
	 * @param buf Target buffer.
	 * @param n   Number of bytes to write.
	 */
#if (__TARGET_HAS_STDOUT)
	EXTERN void stdout_write(const char *buf, size_t n);
#else
	static inline void stdout_write(const char *buf, size_t n)
	{
		UNUSED(buf);
		UNUSED(n);
	}
#endif

/**@}*/

#endif /* NANVIX_HAL_TARGET_STDOUT_H_ */

