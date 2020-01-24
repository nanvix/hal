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

#ifndef TARGET_UNIX64_H_
#define TARGET_UNIX64_H_

	#ifndef __NEED_TARGET_UNIX64
		#error "bad target configuration?"
	#endif

	/* Target Interface Implementation */
	#include <arch/target/unix64/unix64/_unix64.h>

/**
 * @defgroup target-unix64
 * @ingroup targets
 *
 * @brief Unix64 Platform
 */
/**@{*/

	#include <nanvix/const.h>
	#include <arch/target/unix64/unix64/sync.h>
	#include <arch/target/unix64/unix64/mailbox.h>
	#include <arch/target/unix64/unix64/portal.h>
	#include <arch/target/unix64/unix64/stdout.h>

	/**
	 * @brief Frequency (in MHz).
	 *
	 * @note This is CLOCKS_PER_SEC but hard coded.
	 * @todo TODO: remove hard coded value.
	 */
	#define UNIX64_FREQUENCY 1000

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the underlying target.
	 */
	EXTERN void unix64_setup(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Powers off the underlying target.
	 */
	EXTERN NORETURN void unix64_poweroff(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond unix64
 */

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define __TARGET_HAS_STDOUT  1 /**< Standard Output feature */
	#define __TARGET_HAS_SYNC    1 /**< Synchronization feature */
	#define __TARGET_HAS_MAILBOX 1 /**< Mailbox feature         */
	#define __TARGET_HAS_PORTAL  1 /**< Portal feature          */
	/**@}*/

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define TARGET_FREQUENCY UNIX64_FREQUENCY
	/**@}*/

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __target_poweroff_fn /**< unix64_poweroff() */
	/**@}*/

	/**
	 * @see unix64_poweroff().
	 */
	static inline NORETURN void target_poweroff(void)
	{
		unix64_poweroff();
	}

/**@endcond*/

#endif /* TARGET_UNIX64_H_ */
