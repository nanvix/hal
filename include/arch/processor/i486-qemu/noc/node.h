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

#ifndef PROCESSOR_I486_QEMU_NOC_NODE_H_
#define PROCESSOR_I486_QEMU_NOC_NODE_H_

/**
 * @cond i486-qemu-noc
 */

	/* Cluster API. */
	#include <arch/processor/i486-qemu/_i486-qemu.h>

/*============================================================================*
 * Node Interface                                                             *
 *============================================================================*/

	/**
	 * @brief Gets the ID of the NoC node attached to the underlying core.
	 *
	 * @returns The ID of the NoC node attached to the underlying core is
	 * returned.
	 *
	 * @note This function is blocking.
	 * @note This function is thread-safe.
	 */
	static inline int i486_qemu_node_get_id(void)
	{
		return (0);
	}

/**@endcond*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486-qemu
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __hal_processor_node
	#define __processor_node_get_id   /**< processor_node_get_id()   */
	/**@}*/

	/**
	 * @see i486_qemu_node_get_id()
	 */
	static inline int processor_node_get_id(void)
	{
		return i486_qemu_node_get_id();
	}

/**@endcond*/

#endif /* PROCESSOR_I486_QEMU_NOC_NODE_H_ */
