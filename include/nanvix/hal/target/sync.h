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

#ifndef HAL_TARGET_SYNC_H_
#define HAL_TARGET_SYNC_H_

	/* Target Interface Implementation */
	#include <nanvix/hal/target/_target.h>

	#include <nanvix/klib.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

	/* Feature Checking */
	#ifndef TARGET_HAS_SYNC
	#error "does this target feature a synchronization service?"
	#endif

	/* Has Stadard Output Device */
	#if (TARGET_HAS_SYNC)

		/* Constants */
		#ifndef HAL_SYNC_ONE_TO_ALL
		#error "HAL_SYNC_ONE_TO_ALL not defined"
		#endif
		#ifndef HAL_SYNC_ALL_TO_ONE
		#error "HAL_SYNC_ALL_TO_ONE not defined"
		#endif
		#ifndef HAL_SYNC_CREATE_MAX
		#error "HAL_SYNC_CREATE_MAX not defined"
		#endif
		#ifndef HAL_SYNC_OPEN_MAX
		#error "HAL_SYNC_OPEN_MAX not defined"
		#endif

		/* Functions */
		#ifndef __hal_sync_create_fn
		#error "hal_sync_create() not defined?"
		#endif
		#ifndef __hal_sync_open_fn
		#error "hal_sync_open() not defined?"
		#endif
		#ifndef __hal_sync_unlink_fn
		#error "hal_sync_unlink() not defined?"
		#endif
		#ifndef __hal_sync_close_fn
		#error "hal_sync_close_() not defined?"
		#endif
		#ifndef __hal_sync_wait_fn
		#error "hal_sync_wait() not defined?"
		#endif
		#ifndef __hal_sync_signal_fn
		#error "hal_sync_signal() not defined?"
		#endif

	#else

		/* Constants */
		#define HAL_SYNC_ONE_TO_ALL 0
		#define HAL_SYNC_ALL_TO_ONE 1
		#define HAL_SYNC_CREATE_MAX 1
		#define HAL_SYNC_OPEN_MAX   1

	#endif

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @defgroup kernel-hal-target-sync Syncrhonization service
 * @ingroup kernel-hal-target
 *
 * @brief Target Standard Output HAL Interface
 */
/**@{*/

	/**
	 * @brief Allocates and configures the receiving side of the synchronization point.
	 *
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes.
	 * @param type   Type of synchronization point.
	 *
	 * @return The tag of underlying resource ID.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_create(const int *nodes, int nnodes, int type);
#else
	static inline int hal_sync_create(const int *nodes, int nnodes, int type)
	{
		UNUSED(nodes);
		UNUSED(nnodes);
		UNUSED(type);

		return (0);
	}
#endif

	/**
	 * @brief Allocates and configures the sending side of the synchronization point.
	 *
	 * @return The tag of underlying resource ID.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_open(void);
#else
	static inline int hal_sync_open(void)
	{
		return (0);
	}
#endif

	/**
	 * @brief Releases and cleans receiver buffer.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if free the resource and non zero otherwise.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_unlink(int syncid);
#else
	static inline int hal_sync_unlink(int syncid)
	{
		UNUSED(syncid);

		return (0);
	}
#endif

	/**
	 * @brief Releases the sender resources on a specific DMA channel.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if free the resource and non zero otherwise.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_close(int syncid);
#else
	static inline int hal_sync_close(int syncid)
	{
		UNUSED(syncid);

		return (0);
	}
#endif

	/**
	 * @brief Wait signal on a specific synchronization point.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if wait signal correctly and non zero otherwise.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_wait(int syncid);
#else
	static inline int hal_sync_wait(int syncid)
	{
		UNUSED(syncid);

		return (0);
	}
#endif

	/**
	 * @brief Send signal on a specific synchronization point.
	 *
	 * @param syncid Resource ID.
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes.
	 * @param type   Type of synchronization point.
	 *
	 * @return Zero if send signal correctly and non zero otherwise.
	 */
#if (TARGET_HAS_SYNC)
	EXTERN int hal_sync_signal(int syncid, const int *nodes, int nnodes, int type);
#else
	static inline int hal_sync_signal(int syncid, const int *nodes, int nnodes, int type)
	{
		UNUSED(syncid);
		UNUSED(nodes);
		UNUSED(nnodes);
		UNUSED(type);

		return (0);
	}
#endif

/**@}*/

#endif /* HAL_TARGET_SYNC_H_ */
