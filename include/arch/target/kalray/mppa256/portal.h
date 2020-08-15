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

#ifndef TARGET_KALRAY_MPPA256_PORTAL_H_
#define TARGET_KALRAY_MPPA256_PORTAL_H_

/*============================================================================*
 *                            Portal Interface                                *
 *============================================================================*/

	/* Processor API. */
	#include <arch/target/kalray/mppa256/_mppa256.h>

	#include <posix/sys/types.h>
	#include <nanvix/const.h>

	/**
	 * @name Maximum number of portal points.
	 */
	/**@{*/
	#define MPPA256_PORTAL_CREATE_MAX (BOSTAN_PORTAL_CREATE_PER_DMA * BOSTAN_PROCESSOR_NOC_INTERFACES_NUM) /**< Maximum amount of create portals. */
	#define MPPA256_PORTAL_OPEN_MAX   (BOSTAN_PORTAL_OPEN_PER_DMA * BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)   /**< Maximum amount of open portals.   */
	/**@}*/

	/**
	 * @name File descriptor offset.
	 */
	/**@{*/
	#define MPPA256_PORTAL_CREATE_OFFSET 0                         /**< Initial File Descriptor ID for Creates. */
	#define MPPA256_PORTAL_OPEN_OFFSET   MPPA256_PORTAL_CREATE_MAX /**< Initial File Descriptor ID for Opens.   */
	/**@}*/

	/**
	 * @name Sizes (in bytes) of a portal message.
	 */
	/**@{*/
	#define MPPA256_PORTAL_RESERVED_SIZE (4 * 4)                                                   /**< Header size (4 * sizeof(int)). */
	#define MPPA256_PORTAL_DATA_SIZE     (PAGE_SIZE)                                               /**< Maximum data size.             */
	#define MPPA256_PORTAL_MAX_SIZE      (MPPA256_PORTAL_RESERVED_SIZE + MPPA256_PORTAL_DATA_SIZE) /**< Maximum size.                  */
	/**@}*/

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

	/**
	 * @brief Setup portal module.
	 */
	EXTERN void mppa256_portal_setup(void);

	/**
	 * @brief Creates a portal.
	 *
	 * @param local ID of the local NoC node.
	 *
	 * @returns Upon successful completion, the ID of a newly created
	 * portal is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int mppa256_portal_create(int nodenum);

	/**
	 * @brief Enables read operations from a remote.
	 *
	 * @param portalid ID of the target portal.
	 * @param nodenum  NoC node ID of target remote.
	 *
	 * @returns Upons successful completion zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int mppa256_portal_allow(int portalid, int nodenum);

	/**
	 * @brief Opens a portal.
	 *
	 * @param localnum  Logic ID of local node.
	 * @param remotenum Logic ID of target node.
	 *
	 * @returns Upon successful completion, the ID of the target portal
	 * is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int mppa256_portal_open(int localnum, int remotenum);

	/**
	 * @brief Destroys a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int mppa256_portal_unlink(int portalid);

	/**
	 * @brief Closes a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int mppa256_portal_close(int portalid);

	/**
	 * @brief Writes data to a portal.
	 *
	 * @param portalid ID of the target portal.
	 * @param buffer   Buffer where the data should be read from.
	 * @param size     Number of bytes to write.
	 *
	 * @returns Upon successful completion, 0 is returned
	 * and non zero otherwise.
	 */
	EXTERN ssize_t mppa256_portal_awrite(int portalid, const void * buffer, uint64_t size);

	/**
	 * @brief Reads data from a portal.
	 *
	 * @param portalid ID of the target portal.
	 * @param buffer   Buffer where the data should be written to.
	 * @param size     Number of bytes to read.
	 *
	 * @returns Upon successful completion, 0 is returned
	 * and non zero otherwise.
	 */
	EXTERN ssize_t mppa256_portal_aread(int portalid, void * buffer, uint64_t size);

	/**
	 * @brief Waits asynchronous operation.
	 *
	 * @param aiocb Asynchronous operation control.
	 *
	 * @return Zero if wait read correctly and non zero otherwise.
	 */
	EXTERN int mppa256_portal_wait(int portalid);

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __portal_setup_fn  /**< portal_setup()  */
	#define __portal_create_fn /**< portal_create() */
	#define __portal_allow_fn  /**< portal_allow()  */
	#define __portal_open_fn   /**< portal_open()   */
	#define __portal_unlink_fn /**< portal_unlink() */
	#define __portal_close_fn  /**< portal_close()  */
	#define __portal_awrite_fn /**< portal_write()  */
	#define __portal_aread_fn  /**< portal_aread()  */
	#define __portal_wait_fn   /**< portal_wait()   */
	/**@}*/

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define HAL_PORTAL_CREATE_MAX    MPPA256_PORTAL_CREATE_MAX    /**< @see MPPA256_PORTAL_CREATE_MAX    */
	#define HAL_PORTAL_CREATE_OFFSET MPPA256_PORTAL_CREATE_OFFSET /**< @see MPPA256_PORTAL_CREATE_OFFSET */
	#define HAL_PORTAL_OPEN_MAX      MPPA256_PORTAL_OPEN_MAX      /**< @see MPPA256_PORTAL_OPEN_MAX      */
	#define HAL_PORTAL_OPEN_OFFSET   MPPA256_PORTAL_OPEN_OFFSET   /**< @see MPPA256_PORTAL_OPEN_OFFSET   */
	#define HAL_PORTAL_RESERVED_SIZE MPPA256_PORTAL_RESERVED_SIZE /**< @see MPPA256_PORTAL_RESERVED_SIZE */
	#define HAL_PORTAL_DATA_SIZE     MPPA256_PORTAL_DATA_SIZE     /**< @see MPPA256_PORTAL_DATA_SIZE     */
	#define HAL_PORTAL_MAX_SIZE      MPPA256_PORTAL_MAX_SIZE      /**< @see MPPA256_PORTAL_MAX_SIZE      */
	/**@}*/

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

	/**
	 * @see mppa256_portal_setup()
	 */
	#define __portal_setup() \
		mppa256_portal_setup()

	/**
	 * @see mppa256_portal_create()
	 */
	#define __portal_create(nodenum) \
		mppa256_portal_create(nodenum)

	/**
	 * @see mppa256_portal_allow()
	 */
	#define __portal_allow(portalid, remote) \
		mppa256_portal_allow(portalid, remote)

	/**
	 * @see mppa256_portal_open()
	 */
	#define __portal_open(local, remote) \
		mppa256_portal_open(local, remote)

	/**
	 * @see mppa256_portal_unlink()
	 */
	#define __portal_unlink(portalid) \
		mppa256_portal_unlink(portalid)

	/**
	 * @see mppa256_portal_close()
	 */
	#define __portal_close(portalid) \
		mppa256_portal_close(portalid)

	/**
	 * @see mppa256_portal_write()
	 */
	#define __portal_awrite(portalid, buffer, size) \
		mppa256_portal_awrite(portalid, buffer, size)

	/**
	 * @see mppa256_portal_aread()
	 */
	#define __portal_aread(portalid, buffer, size) \
		mppa256_portal_aread(portalid, buffer, size)

	/**
	 * @see mppa256_portal_wait()
	 */
	#define __portal_wait(portalid) \
		mppa256_portal_wait(portalid)

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */

#endif /* TARGET_KALRAY_MPPA256_PORTAL_H_ */
