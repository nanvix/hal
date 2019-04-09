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

#ifndef ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_
#define ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/k1b-cluster/_k1b-cluster.h>

/**
 * @addtogroup k1b-cluster-event Events
 * @ingroup k1b-cluster
 *
 * @brief Events Interface
 */
/**@{*/

	#include <mOS_vcore_u.h>
	#include <nanvix/const.h>

	/**
	 * @brief Event line used for signals.
	 */
	#define K1B_EVENT_LINE 0

#ifndef _ASM_FILE_

	/**
	 * @brief Clears IPIs in the underlying core.
	 */
	static inline void k1b_core_clear(void)
	{
		mOS_pe_event_clear(K1B_EVENT_LINE);
	}

	/**
	 * @brief Waits and clears IPIs in the underlying core.
	 */
	static inline void k1b_core_waitclear(void)
	{
		mOS_pe_event_waitclear(K1B_EVENT_LINE);
	}

	/**
	 * @brief Sends a signal.
	 *
	 * The k1b_core_notify() function sends a signal to the core whose ID
	 * equals to @p coreid.
	 *
	 * @param coreid ID of the target core.
	 *
	 * @bug No sanity check is performed in @p coreid.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline void k1b_core_notify(int coreid)
	{
		mOS_pe_notify(
			1 << coreid,    /* Target cores.                            */
			K1B_EVENT_LINE, /* Event line.                              */
			1,              /* Notify an event? (I/O clusters only)     */
			0               /* Notify an interrupt? (I/O clusters only) */
		);
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b_cluster
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
	 * @see k1b_core_clear().
	 */
	static inline void core_clear(void)
	{
		k1b_core_clear();
	}

	/**
	 * @see k1b_core_notify().
	 */
	static inline void core_notify(int coreid)
	{
		k1b_core_notify(coreid);
	}

	/**
	 * @see k1b_core_waitclear().
	 */
	static inline void core_waitclear(void)
	{
		k1b_core_waitclear();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CLUSTER_K1B_CLUSTER_EVENT_H_ */

