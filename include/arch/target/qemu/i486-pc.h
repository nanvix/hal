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

#ifndef TARGET_QEMU_I486_PC_H_
#define TARGET_QEMU_I486_PC_H_

	#ifndef __NEED_TARGET_QEMU_I486_PC
		#error "bad target configuration?"
	#endif

	/* Target Interface Implementation */
	#include <arch/target/qemu/i486-pc/_i486-pc.h>

/**
 * @defgroup target-qemu-i486-pc QEMU i486
 * @ingroup targets
 *
 * @brief QEMU i486 Platform
 */
/**@{*/

	#include <arch/target/qemu/i486-pc/stdout.h>

	/**
	 * @brief Frequency (in MHz).
	 */
	#define QEMU_X86_FREQUENCY 50

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define TARGET_FREQUENCY QEMU_X86_FREQUENCY
	/**@}*/

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define TARGET_HAS_STDOUT  1 /**< Standard Output feature */
	#define TARGET_HAS_SYNC    0 /**< Synchronization feature */
	#define TARGET_HAS_MAILBOX 0 /**< Mailbox feature         */
	/**@}*/

/**@endcond*/

#endif /* TARGET_QEMU_I486_PC_H_ */
