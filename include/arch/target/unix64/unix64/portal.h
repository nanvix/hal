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

#ifndef TARGET_UNIX64_UNIX64_PORTAL_H_
#define TARGET_UNIX64_UNIX64_PORTAL_H_

	/* Processor API. */
	#include <arch/target/unix64/unix64/_unix64.h>

/**
 * @addtogroup target-unix64-portal Portal
 * @ingroup target-unix64
 *
 * @brief Portal.
 */
/**@{*/

	#include <nanvix/const.h>
	#include <posix/sys/types.h>

	/**
	 * @name Maximum number of portal points.
	 */
	/**@{*/
	#define UNIX64_PORTAL_CREATE_MAX  1 /**< Maximum amount of input portals.  */
	#define UNIX64_PORTAL_OPEN_MAX   12 /**< Maximum amount of output portals. */
	/**@}*/

	/**
	 * @name File descriptor offset.
	 */
	/**@{*/
	#define UNIX64_PORTAL_CREATE_OFFSET 0 /**< Initial File Descriptor ID for Creates. */
	#define UNIX64_PORTAL_OPEN_OFFSET   0 /**< Initial File Descriptor ID for Opens.   */
	/**@}*/

	/**
	 * @name Sizes (in bytes) of a portal message.
	 */
	/**@{*/
	#define UNIX64_PORTAL_RESERVED_SIZE (3 * 4)                                                 /**< Header size (3 * sizeof(int)). */
	#define UNIX64_PORTAL_DATA_SIZE     (8*KB)                                                  /**< Maximum data size.             */
	#define UNIX64_PORTAL_MAX_SIZE      (UNIX64_PORTAL_RESERVED_SIZE + UNIX64_PORTAL_DATA_SIZE) /**< Maximum size.                  */
	/**@}*/

#ifdef __NANVIX_HAL

	/**
	 * @brief Shutdowns the portal interface.
	 */
	PUBLIC void unix64_portal_shutdown(void);

#endif

	/**
	 * @brief Initializes the portal interface.
	 */
	PUBLIC void unix64_portal_setup(void);

	/**
	 * @brief Creates a portal.
	 *
	 * @param local ID of the local NoC node.
	 *
	 * @returns Upon successful completion, the ID of a newly created
	 * portal is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int unix64_portal_create(int nodenum);

	/**
	 * @brief Enables read operations from a remote.
	 *
	 * @param portalid ID of the target portal.
	 * @param remote   NoC node ID of target remote.
	 *
	 * @returns Upons successful completion zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int unix64_portal_allow(int portalid, int remote);

	/**
	 * @brief Opens a portal.
	 *
	 * @param nodeid Logic ID of the target NoC node.
	 *
	 * @returns Upon successful completion, the ID of the target portal
	 * is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int unix64_portal_open(int local, int remote);

	/**
	 * @brief Reads data from a portal.
	 *
	 * @param portalid  ID of the target portal.
	 * @param buffer Buffer where the data should be written to.
	 * @param size   Number of bytes to read.
	 *
	 * @returns Upon successful completion, 0 is returned
	 * and non zero otherwise.
	 */
	EXTERN ssize_t unix64_portal_read(int portalid, void *buffer, uint64_t size);

	/**
	 * @brief Writes data to a portal.
	 *
	 * @param portalid  ID of the target portal.
	 * @param buffer Buffer where the data should be read from.
	 * @param size   Number of bytes to write.
	 *
	 * @returns Upon successful completion, the number of bytes
	 * successfully written is returned. Upon failure, a negative error
	 * code is returned instead.
	 */
	EXTERN ssize_t unix64_portal_write(int portalid, const void *buffer, uint64_t size);

	/**
	 * @brief Destroys a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int unix64_portal_unlink(int portalid);

	/**
	 * @brief Closes a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_portal_close(int portalid);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond unix64_portal
 */

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __portal_setup_fn  /**< portal_setup()  */
	#define __portal_create_fn /**< portal_create() */
	#define __portal_allow_fn  /**< portal_allow()  */
	#define __portal_open_fn   /**< portal_open()   */
	#define __portal_unlink_fn /**< portal_unlink() */
	#define __portal_close_fn  /**< portal_close()  */
	#define __portal_wait_fn   /**< portal_wait()   */
	#define __portal_awrite_fn /**< portal_write()  */
	#define __portal_aread_fn  /**< portal_aread()  */
	#define __portal_wait_fn   /**< portal_wait()   */
	/**@}*/

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define HAL_PORTAL_CREATE_MAX    UNIX64_PORTAL_CREATE_MAX    /**< @see UNIX64_PORTAL_CREATE_MAX    */
	#define HAL_PORTAL_CREATE_OFFSET UNIX64_PORTAL_CREATE_OFFSET /**< @see UNIX64_PORTAL_CREATE_OFFSET */
	#define HAL_PORTAL_OPEN_MAX      UNIX64_PORTAL_OPEN_MAX      /**< @see UNIX64_PORTAL_OPEN_MAX      */
	#define HAL_PORTAL_OPEN_OFFSET   UNIX64_PORTAL_OPEN_OFFSET   /**< @see UNIX64_PORTAL_OPEN_OFFSET   */
	#define HAL_PORTAL_RESERVED_SIZE UNIX64_PORTAL_RESERVED_SIZE /**< @see UNIX64_PORTAL_RESERVED_SIZE */
	#define HAL_PORTAL_DATA_SIZE     UNIX64_PORTAL_DATA_SIZE     /**< @see UNIX64_PORTAL_DATA_SIZE     */
	#define HAL_PORTAL_MAX_SIZE      UNIX64_PORTAL_MAX_SIZE      /**< @see UNIX64_PORTAL_MAX_SIZE      */
	/**@}*/

	/**
	 * @see unix64_portal_setup()
	 */
	#define __portal_setup() \
		unix64_portal_setup()

	/**
	 * @see unix64_portal_create()
	 */
	#define __portal_create(nodenum) \
		unix64_portal_create(nodenum)

	/**
	 * @see unix64_portal_allow()
	 */
	#define __portal_allow(portalid, remote) \
		unix64_portal_allow(portalid, remote)

	/**
	 * @see unix64_portal_read()
	 */
	#define __portal_aread(portalid, buffer, size) \
		unix64_portal_read(portalid, buffer, size)

	/**
	 * @see unix64_portal_awrite()
	 */
	#define __portal_awrite(portalid, buffer, size) \
		unix64_portal_write(portalid, buffer, size)

	/**
	 * @see unix64_portal_open()
	 */
	#define __portal_open(local, remote) \
		unix64_portal_open(local, remote)

	/**
	 * @see unix64_portal_unlink()
	 */
	#define __portal_unlink(portalid) \
		unix64_portal_unlink(portalid)

	/**
	 * @see unix64_portal_close()
	 */
	#define __portal_close(portalid) \
		unix64_portal_close(portalid)

	/**
	 * @brief Dummy operation.
	 *
	 * @param portalid Unused.
	 */
	#define __portal_wait(portalid) (0)

/**@}*/

#endif /* TARGET_UNIX64_UNIX64_PORTAL_H_ */

