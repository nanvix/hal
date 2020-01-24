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

#ifndef NANVIX_HAL_LOG_H_
#define NANVIX_HAL_LOG_H_

/**
 * @addtogroup kernel-hal
 */
/**@{*/

	#include <nanvix/const.h>

#ifdef __NANVIX_HAL

	/**
	 * @brief Size of log (in bytes).
	 */
	#define HAL_LOG_SIZE 512


	/**
	 * @brief Setups the HAL log.
	 */
	EXTERN void hal_log_setup(void);

	/**
	 * @brief Writes to HAL log.
	 *
	 * @param buf Target buffer.
	 * @param n   Number of characters to write.
	 */
	EXTERN void hal_log_write(const char *, size_t);

#endif /* __NANVIX_HAL */

/**@}*/

#endif /* NANVIX_HAL_LOG_H_ */
