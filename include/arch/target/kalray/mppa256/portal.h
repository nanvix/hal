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
 *                            Mailbox Interface                               *
 *============================================================================*/

	/* Processor API. */
	#include <arch/target/kalray/mppa256/_mppa256.h>

	#include <sys/types.h>
	#include <nanvix/const.h>

	/**
	 * @name Maximum number of portal points.
	 */
	/**@{*/
	#define MPPA256_PORTAL_CREATE_MAX (BOSTAN_PORTAL_CREATE_PER_DMA * BOSTAN_NR_INTERFACES) /**< Maximum amount of create portals. */
	#define MPPA256_PORTAL_OPEN_MAX   (BOSTAN_PORTAL_OPEN_PER_DMA * BOSTAN_NR_INTERFACES)   /**< Maximum amount of open portals.   */
	/**@}*/

	/**
	 * @brief Maximum size of transfer data.
	 */
	#define MPPA256_PORTAL_MAX_SIZE (1*MB)

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
	 * @param remote   NoC node ID of target remote.
	 *
	 * @returns Upons successful completion zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int mppa256_portal_allow(int portalid, int remote);

	/**
	 * @brief Opens a portal.
	 *
	 * @param nodeid Logic ID of the target NoC node.
	 *
	 * @returns Upon successful completion, the ID of the target portal
	 * is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int mppa256_portal_open(int local, int remote);

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

/*============================================================================*
 *                              Exported Interface                            *
 *============================================================================*/

	/**
	 * @name Provided Interface
	 */
	/**@{*/
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
	 * @see MPPA256_PORTAL_CREATE_MAX
	 * @see MPPA256_PORTAL_OPEN_MAX
	 * @see MPPA256_PORTAL_MAX_SIZE
	 */
	/**@{*/
	#define PORTAL_CREATE_MAX MPPA256_PORTAL_CREATE_MAX
	#define PORTAL_OPEN_MAX   MPPA256_PORTAL_OPEN_MAX
	#define PORTAL_MAX_SIZE   MPPA256_PORTAL_MAX_SIZE
	/**@}*/

	/**
	 * @see mppa256_portal_create()
	 */
	static inline int portal_create(int nodenum)
	{
		return mppa256_portal_create(nodenum);
	}

	/**
	 * @see mppa256_portal_allow()
	 */
	static inline int portal_allow(int portalid, int remote)
	{
		return mppa256_portal_allow(portalid, remote);
	}

	/**
	 * @see mppa256_portal_open()
	 */
	static inline int portal_open(int local, int remote)
	{
		return mppa256_portal_open(local, remote);
	}

	/**
	 * @see mppa256_portal_unlink()
	 */
	static inline int portal_unlink(int portalid)
	{
		return mppa256_portal_unlink(portalid);
	}

	/**
	 * @see mppa256_portal_close()
	 */
	static inline int portal_close(int portalid)
	{
		return mppa256_portal_close(portalid);
	}

	/**
	 * @see mppa256_portal_write()
	 */
	static inline ssize_t portal_awrite(int portalid, const void * buffer, uint64_t size)
	{
		return mppa256_portal_awrite(portalid, buffer, size);
	}

	/**
	 * @see mppa256_portal_aread()
	 */
	static inline ssize_t portal_aread(int portalid, void * buffer, uint64_t size)
	{
		return mppa256_portal_aread(portalid, buffer, size);
	}

	/**
	 * @see mppa256_portal_wait()
	 */
	static inline int portal_wait(int portalid)
	{
		return mppa256_portal_wait(portalid);
	}

#endif /* TARGET_KALRAY_MPPA256_PORTAL_H_ */
