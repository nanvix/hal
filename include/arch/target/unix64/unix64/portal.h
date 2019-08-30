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
	#include <sys/types.h>

	/**
	 * @name Maximum number of portal points.
	 */
	/**@{*/
	#define UNIX64_PORTAL_CREATE_MAX  1 /**< Maximum amount of input portals.  */
	#define UNIX64_PORTAL_OPEN_MAX   18 /**< Maximum amount of output portals. */
	/**@}*/

	/**
	 * @brief Total number of portals.
	 */
	#define UNIX64_PORTAL_MAX (UNIX64_PORTAL_CREATE_MAX + UNIX64_PORTAL_OPEN_MAX)

	/**
	 * @brief Maximum size of transfer data.
	 */
	#define UNIX64_PORTAL_MAX_SIZE (1*MB)

#ifdef __NANVIX_HAL

	/**
	 * @brief Initializes the portal interface.
	 */
	PUBLIC void unix64_portal_setup(void);

	/**
	 * @brief Shutdowns the portal interface.
	 */
	PUBLIC void unix64_portal_shutdown(void);

#endif

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

	/**
	 * @brief Waits asynchronous operation.
	 *
	 * @param aiocb Asynchronous operation control.
	 *
	 * @return Zero if wait read correctly and non zero otherwise.
	 */
	EXTERN int unix64_portal_wait(int portalid);

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
	#define __portal_create_fn /**< portal_create() */
	#define __portal_allow_fn  /**< portal_allow()  */
	#define __portal_open_fn   /**< portal_open()   */
	#define __portal_unlink_fn /**< portal_unlink() */
	#define __portal_close_fn  /**< portal_close()  */
	#define __portal_wait_fn   /**< portal_wait()   */
	/**@}*/

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define PORTAL_CREATE_MAX UNIX64_PORTAL_CREATE_MAX /**< UNIX64_PORTAL_CREATE_MAX */
	#define PORTAL_OPEN_MAX   UNIX64_PORTAL_OPEN_MAX   /**< UNIX64_PORTAL_OPEN_MAX   */
	#define PORTAL_MAX_SIZE   UNIX64_PORTAL_MAX_SIZE   /**< UNIX64_PORTAL_MAX_SIZE   */
	/**@}*/

	/**
	 * @see unix64_portal_create()
	 */
	static inline int portal_create(int nodenum)
	{
		return unix64_portal_create(nodenum);
	}

	/**
	 * @see unix64_portal_allow()
	 */
	static inline int portal_allow(int portalid, int remote)
	{
		return unix64_portal_allow(portalid, remote);
	}

	/**
	 * @see unix64_portal_create()
	 */
	static inline ssize_t portal_aread(int portalid, void *buffer, uint64_t size)
	{
		return (unix64_portal_read(portalid, buffer, size));
	}

	/**
	 * @see unix64_portal_allow()
	 */
	static inline ssize_t portal_awrite(int portalid, const void *buffer, uint64_t size)
	{
		return (unix64_portal_write(portalid, buffer, size));
	}


	/**
	 * @see unix64_portal_open()
	 */
	static inline int portal_open(int local, int remote)
	{
		return unix64_portal_open(local, remote);
	}

	/**
	 * @see unix64_portal_unlink()
	 */
	static inline int portal_unlink(int portalid)
	{
		return unix64_portal_unlink(portalid);
	}

	/**
	 * @see unix64_portal_close()
	 */
	static inline int portal_close(int portalid)
	{
		return unix64_portal_close(portalid);
	}

	/**
	 * @see unix64_portal_wait()
	 */
	static inline int portal_wait(int portalid)
	{
		return unix64_portal_wait(portalid);
	}

/**@}*/

#endif /* TARGET_UNIX64_UNIX64_PORTAL_H_ */

