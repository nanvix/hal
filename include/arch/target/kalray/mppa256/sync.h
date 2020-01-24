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

#ifndef TARGET_KALRAY_MPPA256_SYNC_H_
#define TARGET_KALRAY_MPPA256_SYNC_H_

/*============================================================================*
 *                              Sync Interface                                *
 *============================================================================*/

	/* Processor API. */
	#include <arch/target/kalray/mppa256/_mppa256.h>

	#include <nanvix/const.h>

	/**
	 * @brief Type of synchronization points.
	 */
	/**@{*/
	#define MPPA256_SYNC_ONE_TO_ALL 0 /**< One to all. */
	#define MPPA256_SYNC_ALL_TO_ONE 1 /**< All to one. */
	/**@}*/

	/**
	 * @brief Mode of synchronization points.
	 */
	/**@{*/
	#define MPPA256_SYNC_RX BOSTAN_PROCESSOR_NOC_RX_TYPE /**< Reciever type. */
	#define MPPA256_SYNC_TX BOSTAN_PROCESSOR_NOC_TX_TYPE /**< Sender type.   */
	/**@}*/

	/**
	 * @name Maximum number of syncs points.
	 */
	/**@{*/
	#define MPPA256_SYNC_CREATE_MAX (BOSTAN_SYNC_CREATE_PER_DMA * BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) /**< Maximum amount of create syncs. */
	#define MPPA256_SYNC_OPEN_MAX   (BOSTAN_SYNC_OPEN_PER_DMA * BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)   /**< Maximum amount of open syncs.   */
	/**@}*/

	/**
	 * @brief Allocates and configures the receiving side of the synchronization point.
	 *
	 * @param nodenums Logic IDs of target NoC nodes.
	 * @param nnodes   Number of target NoC nodes.
	 * @param type     Type of synchronization point.
	 *
	 * @return The File descriptor ID.
	 */
	EXTERN int mppa256_sync_create(const int *nodenums, int nnodes, int type);

	/**
	 * @brief Allocates and configures the sending side of the synchronization point.
	 *
	 * @param nodenums Logic IDs of target NoC nodes.
	 * @param nnodes   Number of target NoC nodes.
	 * @param type     Type of synchronization point.
	 *
	 * @return The File descriptor ID.
	 */
	EXTERN int mppa256_sync_open(const int *nodenums, int nnodes, int type);

	/**
	 * @brief Releases and cleans receiver buffer.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if free the resource and non zero otherwise.
	 */
	EXTERN int mppa256_sync_unlink(int syncid);

	/**
	 * @brief Releases the sender resources on a specific DMA channel.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if free the resource and non zero otherwise.
	 */
	EXTERN int mppa256_sync_close(int syncid);

	/**
	 * @brief Wait signal on a specific synchronization point.
	 *
	 * @param syncid ID of the Target Sync.
	 *
	 * @return Zero if wait signal correctly and non zero otherwise.
	 */
	EXTERN int mppa256_sync_wait(int syncid);

	/**
	 * @brief Send signal on a specific synchronization point.
	 *
	 * @param syncid Resource ID.
	 *
	 * @return Zero if send signal correctly and non zero otherwise.
	 */
	EXTERN int mppa256_sync_signal(int syncid);

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __sync_setup_fn  /**< sync_setup()  */
	#define __sync_create_fn /**< sync_create() */
	#define __sync_open_fn   /**< sync_open()   */
	#define __sync_unlink_fn /**< sync_unlink() */
	#define __sync_close_fn  /**< sync_close()  */
	#define __sync_wait_fn   /**< sync_wait()   */
	#define __sync_signal_fn /**< sync_signal() */
	/**@}*/

	/**
	 * @see MPPA256_SYNC_ONE_TO_ALL
	 * @see MPPA256_SYNC_ALL_TO_ONE
	 */
	/**@{*/
	#define SYNC_ONE_TO_ALL MPPA256_SYNC_ONE_TO_ALL
	#define SYNC_ALL_TO_ONE MPPA256_SYNC_ALL_TO_ONE
	/**@}*/

	/**
	 * @see BOSTAN_SYNC_CREATE_MAX
	 * @see BOSTAN_SYNC_OPEN_MAX
	 */
	/**@{*/
	#define SYNC_CREATE_MAX MPPA256_SYNC_CREATE_MAX
	#define SYNC_OPEN_MAX   MPPA256_SYNC_OPEN_MAX
	/**@}*/

	/**
	 * @todo TODO: rely on dummy platform-independent dummy function.
	 *
	 * @todo Maybe this function can be used to initialize the intercluster
	 * fence resources.
	 */
	static inline void sync_setup(void)
	{

	}

	/**
	 * @see mppa256_sync_create()
	 */
	static inline int sync_create(const int *nodenums, int nnodes, int type)
	{
		return mppa256_sync_create(nodenums, nnodes, type);
	}

	/**
	 * @see mppa256_sync_open()
	 */
	static inline int sync_open(const int *nodenums, int nnodes, int type)
	{
		return mppa256_sync_open(nodenums, nnodes, type);
	}

	/**
	 * @see mppa256_sync_unlink()
	 */
	static inline int sync_unlink(int syncid)
	{
		return mppa256_sync_unlink(syncid);
	}

	/**
	 * @see mppa256_sync_close()
	 */
	static inline int sync_close(int syncid)
	{
		return mppa256_sync_close(syncid);
	}

	/**
	 * @see mppa256_sync_wait()
	 */
	static inline int sync_wait(int syncid)
	{
		return mppa256_sync_wait(syncid);
	}

	/**
	 * @see mppa256_sync_signal()
	 */
	static inline int sync_signal(int syncid)
	{
		return mppa256_sync_signal(syncid);
	}

#endif /* TARGET_KALRAY_MPPA256_SYNC_H_ */
