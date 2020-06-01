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

#ifndef NANVIX_HAL_TARGET_PORTAL_H_
#define NANVIX_HAL_TARGET_PORTAL_H_

	/* Target Interface Implementation */
	#include <nanvix/hal/target/_target.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_TARGET_AL) || defined(__INTERFACE_CHECK_PORTAL)

	/* Feature Checking */
	#ifndef __TARGET_HAS_PORTAL
	#error "does this target feature a portal interface?"
	#endif

	/* Has Portal Interface */
	#if (__TARGET_HAS_PORTAL)

		/* Constants */
		#ifndef HAL_PORTAL_CREATE_MAX
		#error "HAL_PORTAL_CREATE_MAX not defined"
		#endif
		#ifndef HAL_PORTAL_CREATE_OFFSET
		#error "HAL_PORTAL_CREATE_OFFSET not defined"
		#endif
		#ifndef HAL_PORTAL_OPEN_MAX
		#error "HAL_PORTAL_OPEN_MAX not defined"
		#endif
		#ifndef HAL_PORTAL_OPEN_OFFSET
		#error "HAL_PORTAL_OPEN_OFFSET not defined"
		#endif
		#ifndef HAL_PORTAL_RESERVED_SIZE
		#error "HAL_PORTAL_RESERVED_SIZE not defined"
		#endif
		#ifndef HAL_PORTAL_DATA_SIZE
		#error "HAL_PORTAL_DATA_SIZE not defined"
		#endif
		#ifndef HAL_PORTAL_MAX_SIZE
		#error "HAL_PORTAL_MAX_SIZE not defined"
		#endif

		/* Functions */
		#ifndef __portal_setup_fn
		#error "portal_setup() not defined?"
		#endif
		#ifndef __portal_create_fn
		#error "portal_create() not defined?"
		#endif
		#ifndef __portal_allow_fn
		#error "portal_allow() not defined?"
		#endif
		#ifndef __portal_open_fn
		#error "portal_open() not defined?"
		#endif
		#ifndef __portal_unlink_fn
		#error "portal_unlink() not defined?"
		#endif
		#ifndef __portal_close_fn
		#error "portal_close() not defined?"
		#endif
		#ifndef __portal_awrite_fn
		#error "portal_write() not defined?"
		#endif
		#ifndef __portal_aread_fn
		#error "portal_aread() not defined?"
		#endif
		#ifndef __portal_wait_fn
		#error "portal_wait() not defined?"
		#endif

	#endif

#endif

/* Dummy Constants */
#if (!__TARGET_HAS_PORTAL)

	#define HAL_PORTAL_CREATE_MAX    1
	#define HAL_PORTAL_CREATE_OFFSET 0
	#define HAL_PORTAL_OPEN_MAX      1
	#define HAL_PORTAL_OPEN_OFFSET   0
	#define HAL_PORTAL_MAX_SIZE      1

#endif /* !__TARGET_HAS_PORTAL */

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @defgroup kernel-hal-target-portal Portal service
 * @ingroup kernel-hal-target
 *
 * @brief Target Standard Output HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>
	#include <nanvix/hlib.h>
	#include <posix/errno.h>

	/**
	 * @brief Creates a portal.
	 *
	 * @param nodenum Logic ID of the underlying NoC node.
	 *
	 * @returns Upon successful completion, the ID of the newly created
	 * portal is returned. Upon failure, a negative error code is
	 * returned instead.
	 */
	EXTERN int portal_create(int nodenum);

	/**
	 * @brief Opens a portal.
	 *
	 * @param localnum  Logic ID of local node.
	 * @param remotenum Logic ID of target node.
	 *
	 * @returns Upons successful completion zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int portal_open(int localnum, int remotenum);

	/**
	 * @brief Allows remote writes in a portal.
	 *
	 * @param portalid ID of the target portal.
	 * @param nodenum  Logic ID of the target NoC node.
	 *
	 * @reutrns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int portal_allow(int portalid, int nodenum);

	/**
	 * @brief Destroys a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int portal_unlink(int portalid);

	/**
	 * @brief Closes a portal.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int portal_close(int portalid);

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
	EXTERN ssize_t portal_awrite(int portalid, const void *buffer, uint64_t size);

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
	EXTERN ssize_t portal_aread(int portalid, void *buffer, uint64_t size);

	/**
	 * @brief Waits asynchronous operation.
	 *
	 * @param portalid ID of the target portal.
	 *
	 * @return Zero if wait read correctly and non zero otherwise.
	 */
	EXTERN int portal_wait(int portalid);

	/**
	 * @brief Initializes the portal interface.
	 */
	EXTERN void portal_setup(void);

/**@}*/

#endif /* NANVIX_HAL_TARGET_PORTAL_H_ */
