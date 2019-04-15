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

#ifndef ARCH_CLUSTER_WOLFE_STDOUT_H_
#define ARCH_CLUSTER_WOLFE_STDOUT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/wolfe/_wolfe.h>

	#ifndef __NEED_WOLFE_STDOUT
	#error "wolfe uart device not required"
	#endif

/**
 * @addtogroup wolfe-cluster-stdout Stdout
 * @ingroup wolfe-cluster
 *
 * @brief Standard Output Device
 */
/**@{*/

	#include <nanvix/const.h>
	#include <sys/types.h>

	/**
	 * @brief Device version.
	 */
	#define WOLFE_STDOUT_VERSION 1

	/**
	 * @brief Device memory size (in bytes).
	 */
	#define WOLFE_STDOUT_SIZE 0x1000

	/**
	 * @name Device Memory Map
	 */
	/**@{*/
	#define WOLFE_STDOUT_PUTC_OFF 0x0000 /**< Put Character */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Writes to the stdout device.
	 *
	 * @param bug Target buffer.
	 * @param n   Number of bytes to write.
	 */
	EXTERN void wolfe_stdout_write(const char *buf, size_t n);

	/**
	 * @brief Initializes the stdout device.
	 *
	 * @param base Base address of the stdout device.
	 */
	EXTERN void wolfe_stdout_init(paddr_t base);

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* ARCH_CLUSTER_WOLFE_STDOUT_H_ */
