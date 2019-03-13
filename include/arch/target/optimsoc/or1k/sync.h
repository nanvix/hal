/*
 * MIT License
 *
 * Copyright(c) 2018 Pedro Henrique Penna <pedrohenriquepenna@gmail.com>
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

#ifndef TARGET_OPTIMSOC_SYNC_H_
#define TARGET_OPTIMSOC_SYNC_H_

/*============================================================================*
 *                              Sync Interface                                *
 *============================================================================*/

	/* Processor API. */
	#include <arch/target/optimsoc/or1k/_or1k.h>

	#include <nanvix/const.h>

	/**
	 * @brief Type of synchronization points.
	 */
	/**@{*/
	#define OPTIMSOC_SYNC_ONE_TO_ALL 0 /**< One to all. */
	#define OPTIMSOC_SYNC_ALL_TO_ONE 1 /**< All to one. */
	/**@}*/

	/**
	 * @brief Mode of synchronization points.
	 */
	/**@{*/
	#define OPTIMSOC_SYNC_RX 0 /**< Reciever type. */
	#define OPTIMSOC_SYNC_TX 1 /**< Sender type.   */
	/**@}*/

	/**
	 * @name Maximum number of syncs.
	 */
	/**@{*/
	#define OPTIMSOC_NR_SYNC_RX_MAX (1) /**< Amount of receiver sync points. */
	#define OPTIMSOC_NR_SYNC_TX_MAX (1) /**< Amount of sender sync points.   */
	/**@}*/

	/**
	 * @brief Allocates and configures the receiving side of the synchronization point.
	 *
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes. 
	 * @param type   Type of synchronization point.
	 * 
	 * @return The tag of underlying resource ID.
	 */
	EXTERN int optimsoc_sync_create(const int *nodes, int nnodes, int type);

	/**
	 * @brief Allocates and configures the sending side of the synchronization point.
	 *
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes. 
	 * @param type   Type of synchronization point.
	 * 
	 * @return The tag of underlying resource ID.
	 */
	EXTERN int optimsoc_sync_open(const int *nodes, int nnodes, int type);

	/**
	 * @brief Releases and cleans receiver buffer.
	 *
	 * @param syncid Resource ID.
	 * 
	 * @return Zero if free the resource and non zero otherwise.
	 */
	EXTERN int optimsoc_sync_unlink(int syncid);

	/**
	 * @brief Releases the sender resources on a specific DMA channel.
	 *
	 * @param syncid Resource ID.
	 * 
	 * @return Zero if free the resource and non zero otherwise.
	 */
	EXTERN int optimsoc_sync_close(int syncid);

	/**
	 * @brief Wait signal on a specific synchronization point.
	 *
	 * @param syncid Resource ID.
	 * 
	 * @return Zero if wait signal correctly and non zero otherwise.
	 */
	EXTERN int optimsoc_sync_wait(int syncid);

	/**
	 * @brief Send signal on a specific synchronization point.
	 *
	 * @param syncid Resource ID.
	 * 
	 * @return Zero if send signal correctly and non zero otherwise.
	 */
	EXTERN int optimsoc_sync_signal(int syncid);

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __hal_sync
	#define __hal_sync_create /**< hal_sync_create()    */
	#define __hal_sync_open   /**< hal_sync_open()      */
	#define __hal_sync_unlink /**< hal_sync_unlink()    */
	#define __hal_sync_close  /**< hal_sync_close()     */
	#define __hal_sync_wait   /**< hal_sync_wait()      */
	#define __hal_sync_signal /**< hal_sync_signal()    */
	/**@}*/

	/**
	 * @name Provided Test Interface
	 */
	/**@{*/
	#define __hal_incomplete_sync
	/**@}*/

	/**
	 * @see OPTIMSOC_SYNC_ONE_TO_ALL
	 * @see OPTIMSOC_SYNC_ALL_TO_ONE
	 */
	/**@{*/
	#define HAL_SYNC_ONE_TO_ALL OPTIMSOC_SYNC_ONE_TO_ALL
	#define HAL_SYNC_ALL_TO_ONE OPTIMSOC_SYNC_ALL_TO_ONE
	/**@}*/

	/**
	 * @see OPTIMSOC_NR_SYNC
	 */
	#define HAL_NR_SYNC (OPTIMSOC_NR_SYNC_RX_MAX+OPTIMSOC_NR_SYNC_TX_MAX)

	/**
	 * @see optimsoc_sync_create()
	 */
	static inline int hal_sync_create(const int *nodes, int nnodes, int type)
	{
		return optimsoc_sync_create(nodes, nnodes, type);
	}

	/**
	 * @see optimsoc_sync_open()
	 */
	static inline int hal_sync_open(const int *nodes, int nnodes, int type)
	{
		return optimsoc_sync_open(nodes, nnodes, type);
	}

	/**
	 * @see optimsoc_sync_unlink()
	 */
	static inline int hal_sync_unlink(int syncid)
	{
		return optimsoc_sync_unlink(syncid);
	}

	/**
	 * @see optimsoc_sync_close()
	 */
	static inline int hal_sync_close(int syncid)
	{
		return optimsoc_sync_close(syncid);
	}

	/**
	 * @see optimsoc_sync_wait()
	 */
	static inline int hal_sync_wait(int syncid)
	{
		return optimsoc_sync_wait(syncid);
	}

	/**
	 * @see optimsoc_sync_signal()
	 */
	static inline int hal_sync_signal(int syncid)
	{
		return optimsoc_sync_signal(syncid);
	}

#endif /* ARCH_OPTIMSOC_SYNC_H_ */
