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

#ifndef ARCH_CLUSTER_RISCV32_CLUSTER_EVENT_H_
#define ARCH_CLUSTER_RISCV32_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/riscv32-cluster/_riscv32-cluster.h>

/**
 * @addtogroup riscv-cluster-event Events
 * @ingroup riscv-cluster
 *
 * @brief Events Interface
 */
/**@{*/

	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Notifies a local core about an event.
	 *
	 * @param coreid ID of target core.
	 */
	EXTERN int riscv32_cluster_event_notify(int coreid);

	/**
	 * @brief Waits for an event.
	 */
	EXTERN int riscv32_cluster_event_wait(void);

	/**
	 * @brief Drops any pending events in the local core.
	 */
	EXTERN int riscv32_cluster_event_drop(void);

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond riscv32_cluster
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __core_clear_fn     /**< core_clear()     */
	#define __core_notify_fn    /**< core_notify()    */
	#define __core_waitclear_fn /**< core_waitclear() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see riscv32_cluster_event_drop().
	 */
	static inline void core_clear(void)
	{
		riscv32_cluster_event_drop();
	}

	/**
	 * @see riscv32_cluster_event_notify()
	 */
	static inline void core_notify(int coreid)
	{
		riscv32_cluster_event_notify(coreid);
	}

	/**
	 * @see riscv32_cluster_event_wait().
	 */
	static inline void core_waitclear(void)
	{
		riscv32_cluster_event_wait();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_RISCV32_EVENT_H_ */
