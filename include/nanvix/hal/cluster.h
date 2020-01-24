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

#ifndef NANVIX_HAL_CLUSTER_H_
#define NANVIX_HAL_CLUSTER_H_

	#ifndef __NEED_HAL_CLUSTER
		#error "include <nanvix/hal/processor.h> instead"
	#endif

	/* Cluster Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Feature Checking */
	#ifndef CLUSTER_IS_MULTICORE
	#error "is this a multicore cluster?"
	#endif
	#ifndef CLUSTER_IS_IO
	#error "is this an i/o cluster?"
	#endif
	#ifndef CLUSTER_IS_COMPUTE
	#error "is this a compute cluster?"
	#endif
	#if (CLUSTER_IS_IO == CLUSTER_IS_COMPUTE)
	#error "ambiguous cluster type!"
	#endif
	#ifndef CLUSTER_HAS_EVENTS
	#error "are events natively supported by this cluster?"
	#endif

	/* Constants */
	#ifndef CORES_NUM
	#error "CORES_NUM not defined"
	#endif
	#ifndef COREID_MASTER
	#error "COREID_MASTER not defined"
	#endif

	/* Functions */
	#ifndef __cluster_get_num_cores
	#error "cluster_get_num_cores() not defined?"
	#endif

#endif

/*============================================================================*
 * Cluster Interface                                                          *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-cluster Cluster
 * @ingroup kernel-hal
 *
 * @brief Cluster HAL Interface
 */
/**@{*/

	#include <nanvix/hal/cluster/timer.h>
	#include <nanvix/hal/cluster/event.h>
	#include <nanvix/hal/cluster/memory.h>
#ifndef __unix64__
	#include <nanvix/hal/cluster/mmio.h>
#endif
	#include <nanvix/const.h>

	/**
	 * @name Types of Cluster
	 */
	/**@{*/
	#define CLUSTER_IO      1 /**< I/O Cluster     */
	#define CLUSTER_COMPUTE 2 /**< Compute Cluster */
	/**@}*/

#ifdef __NANVIX_HAL

	/**
	 * @name States of a Core
	 */
	/**@{*/
	#define CORE_IDLE      0 /**< Idle        */
	#define CORE_SLEEPING  1 /**< Sleeping    */
	#define CORE_RUNNING   2 /**< Running     */
	#define CORE_RESETTING 3 /**< Resetting   */
	#define CORE_OFFLINE   4 /**< Powered Off */
	/**@}*/

	/**
	 * @brief Core information.
	 */
	struct coreinfo
	{
		volatile bool initialized; /**< Initialized?      */
		volatile int state;        /**< State.            */
		volatile int wakeups;      /**< Wakeup signals.   */
		void (*start)(void);       /**< Starting routine. */
		spinlock_t lock;           /**< Lock.             */
	};

	/**
	 * @brief Cores table.
	 */
	EXTERN struct coreinfo cores[CORES_NUM];

#endif /* __NANVIX_HAL */

	/**
	 * @brief Gets the number of cores.
	 *
	 * @returns The number of cores in the underlying cluster
	 */
	EXTERN int cluster_get_num_cores(void);

	/**
	 * @brief Asserts whether the cluster is an I/O Cluster.
	 *
	 * @returns Non-zero if the underlying cluster is an I/O cluster
	 * and zero otherwise.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_is_io(void)
	{
		return (CLUSTER_IS_IO);
	}

	/**
	 * @brief Asserts whether the cluster is an Compute Cluster.
	 *
	 * @returns Non-zero if the underlying cluster is an Compute
	 * cluster and zero otherwise.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_is_compute(void)
	{
		return (CLUSTER_IS_COMPUTE);
	}

	/**
	 * @brief Gets the type of cluster.
	 *
	 * @returns CLUSTER_IO if the underlying cluster is an I/O cluster
	 * and CLUSTER_COMPUTE if it is a compute cluster.
	 *
	 * @author Pedro Henrique Penna
	 */
	static int cluster_get_type(void)
	{
		return ((CLUSTER_IS_IO) ? CLUSTER_IO : CLUSTER_COMPUTE);
	}

#ifdef __NANVIX_HAL

	/**
	* @brief Releases the startup fence.
	*/
	EXTERN void cluster_fence_release(void);

	/**
	* @brief Waits on the startup fence.
	*/
	EXTERN void cluster_fence_wait(void);

	/**
	 * @brief Suspends execution until a start signal is received.
	 */
	EXTERN void core_idle(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Suspends instruction execution in the underling core.
	 */
	EXTERN void core_sleep(void);

	/**
	 * @brief Wakes up a core.
	 *
	 * @param coreid ID of the target core.
	 *
	 * @return Returns 0 if the wakeup was successful and a negative
	 * number otherwise.
	 */
	EXTERN int core_wakeup(int coreid);

	/**
	 * @brief Starts a core.
	 *
	 * @param coreid ID of the target core.
	 * @param start  Starting routine to execute.
	 *
	 * @return Returns 0 if the core start was successful and, otherwise,
	 * non-zero value.
	 */
	EXTERN int core_start(int coreid, void (*start)(void));

	/**
	 * @brief Resumes instruction execution in the underlying core.
	 */
	EXTERN void core_run(void);

	/**
	 * @brief Reset the underlying core.
	 */
	EXTERN int core_reset(void);

	/**
	 * @brief Shutdowns the underlying core.
	 */
	EXTERN NORETURN void core_shutdown(void);

	/**
	 * @brief Powers off the underlying cluster.
	 */
	#ifdef __cluster_poweroff_fn
	EXTERN NORETURN void cluster_poweroff(void);
	#else
	static inline NORETURN void cluster_poweroff(void)
	{
		core_shutdown();
	}
	#endif

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_H_ */
