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

#ifndef TARGET_UNIX64_UNIX64_SYNC_H_
#define TARGET_UNIX64_UNIX64_SYNC_H_

	/* Processor API. */
	#include <arch/target/unix64/unix64/_unix64.h>

/**
 * @addtogroup target-unix64-sync Sync
 * @ingroup target-unix64
 *
 * @brief Synchronization point.
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Type of synchronization points.
	 */
	/**@{*/
	#define UNIX64_SYNC_ONE_TO_ALL 0 /**< One to all. */
	#define UNIX64_SYNC_ALL_TO_ONE 1 /**< All to one. */
	/**@}*/

	/**
	 * @name Maximum number of syncs points.
	 */
	/**@{*/
	#define UNIX64_SYNC_CREATE_MAX  12 /**< Maximum amount of create syncs. */
	#define UNIX64_SYNC_OPEN_MAX    12 /**< Maximum amount of open syncs.   */
	/**@}*/

	/**
	* @name File descriptor offset.
	*/
	/**@{*/
	#define UNIX64_SYNC_CREATE_OFFSET 0                      /**< Initial File Descriptor ID for Creates. */
	#define UNIX64_SYNC_OPEN_OFFSET   UNIX64_SYNC_CREATE_MAX /**< Initial File Descriptor ID for Opens.   */
	/**@}*/

	/**
	 * @brief Total number of sync points.
	 */
	#define UNIX64_SYNC_MAX (UNIX64_SYNC_CREATE_MAX + UNIX64_SYNC_OPEN_MAX)

	/**
	 * @name IO control requests.
	 */
	/**@{*/
	#define UNIX64_SYNC_IOCTL_SET_ASYNC_BEHAVIOR 0 /**< Sets the wait/wakeup functions on a resource. */
	/**@}*/

	/**
	 * @brief Request an I/O operation on a synchronization point.
	 *
	 * @param syncid  Sync resource.
	 * @param request Type of request.
	 * @param args    Arguments of the request.
	 *
	 * @returns Upon successful completion, zero is returned.
	 * Upon failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_sync_ioctl(int syncid, unsigned request, va_list args);

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

#ifdef __NANVIX_HAL

	/**
	 * @brief Shutdowns the sync interface.
	 */
	PUBLIC void unix64_sync_shutdown(void);

#endif

	/**
	 * @todo TODO: rely on dummy platform-independent dummy function.
	 */
	EXTERN void unix64_sync_setup(void);

	/**
	 * @brief Allocates and configures the receiving side of the synchronization point.
	 *
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes.
	 * @param type   Type of synchronization point.
	 *
	 * @returns Upon successful completion, the ID of the newly created
	 * synchronization point is returned. Upon failure, a negative error
	 * code is returned instead.
	 */
	EXTERN int unix64_sync_create(const int *nodes, int nnodes, int type);

	/**
	 * @brief Opens a synchronization point.
	 *
	 * @param nodes  IDs of target NoC nodes.
	 * @param nnodes Number of target NoC nodes.
	 * @param type   Type of synchronization point.
	 *
	 * @returns Upon successful completion, the ID of the target
	 * synchronization point is returned. Upon failure, a negative error
	 * code is returned instead.
	 */
	EXTERN int unix64_sync_open(const int *nodes, int nnodes, int type);

	/**
	 * @brief Destroys a synchronization point.
	 *
	 * @param syncid ID of the target synchronization point.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_sync_unlink(int syncid);

	/**
	 * @brief Closes a synchronization point.
	 *
	 * @param syncid ID of the target synchronization point.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_sync_close(int syncid);

	/**
	 * @brief Waits on a synchronization point.
	 *
	 * @param syncid ID of the target synchronization point.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_sync_wait(int syncid);

	/**
	 * @brief Waits on a synchronization point.
	 *
	 * @param syncid ID of the target synchronization point.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_sync_signal(int syncid);

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond unix64
 */

	/**
	 * @name Provided Functions.
	 */
	/**@{*/
	#define __sync_setup_fn  /**< sync_setup()  */
	#define __sync_create_fn /**< sync_create() */
	#define __sync_open_fn   /**< sync_open()   */
	#define __sync_unlink_fn /**< sync_unlink() */
	#define __sync_close_fn  /**< sync_close()  */
	#define __sync_wait_fn   /**< sync_wait()   */
	#define __sync_signal_fn /**< sync_signal() */
	#define __sync_ioctl_fn  /**< sync_ioctl()  */
	/**@}*/

	/**
	 * @name Provided Constants. 
	 */
	/**@{*/
	#define SYNC_ONE_TO_ALL    UNIX64_SYNC_ONE_TO_ALL    /**< UNIX64_SYNC_ONE_TO_ALL    */
	#define SYNC_ALL_TO_ONE    UNIX64_SYNC_ALL_TO_ONE    /**< UNIX64_SYNC_ALL_TO_ONE    */
	#define SYNC_CREATE_MAX    UNIX64_SYNC_CREATE_MAX    /**< UNIX64_SYNC_CREATE_MAX    */
	#define SYNC_CREATE_OFFSET UNIX64_SYNC_CREATE_OFFSET /**< UNIX64_SYNC_CREATE_OFFSET */
	#define SYNC_OPEN_MAX      UNIX64_SYNC_OPEN_MAX      /**< UNIX64_SYNC_OPEN_MAX      */
	#define SYNC_OPEN_OFFSET   UNIX64_SYNC_OPEN_OFFSET   /**< UNIX64_SYNC_OPEN_OFFSET   */
	/**@}*/

	/**
	 * @name IO control requests.
	 */
	/**@{*/
	#define SYNC_IOCTL_SET_ASYNC_BEHAVIOR UNIX64_SYNC_IOCTL_SET_ASYNC_BEHAVIOR /**< @see UNIX64_SYNC_IOCTL_SET_ASYNC_BEHAVIOR */
	/**@}*/

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

	/**
	 * @see unix64_sync_setup()
	 */
	#define __sync_setup() \
		unix64_sync_setup()

	/**
	 * @see unix64_sync_create()
	 */
	#define __sync_create(nodes, nnodes, type) \
		unix64_sync_create(nodes, nnodes, type)

	/**
	 * @see unix64_sync_open()
	 */
	#define __sync_open(nodes, nnodes, type) \
		unix64_sync_open(nodes, nnodes, type)

	/**
	 * @see unix64_sync_unlink()
	 */
	#define __sync_unlink(syncid) \
		unix64_sync_unlink(syncid)

	/**
	 * @see unix64_sync_close()
	 */
	#define __sync_close(syncid) \
		unix64_sync_close(syncid)

	/**
	 * @see unix64_sync_wait()
	 */
	#define __sync_wait(syncid) \
		unix64_sync_wait(syncid)

	/**
	 * @see unix64_sync_signal()
	 */
	#define __sync_signal(syncid) \
		unix64_sync_signal(syncid)

	/**
	 * @see unix64_sync_ioctl()
	 */
	#define __sync_ioctl(syncid, request, args) \
		unix64_sync_ioctl(syncid, request, args)

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */

/**@}*/

#endif /* ARCH_UNIX64_SYNC_H_ */
