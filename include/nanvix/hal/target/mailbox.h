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

#ifndef NANVIX_HAL_TARGET_MAILBOX_H_
#define NANVIX_HAL_TARGET_MAILBOX_H_

	/* Target Interface Implementation */
	#include <nanvix/hal/target/_target.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_TARGET_AL) || defined(__INTERFACE_CHECK_MAILBOX)

	/* Feature Checking */
	#ifndef __TARGET_HAS_MAILBOX
	#error "does this target feature a mailbox interface?"
	#endif

	/* Has Mailbox Interface */
	#if (__TARGET_HAS_MAILBOX)

		/* Constants */
		#ifndef HAL_MAILBOX_CREATE_OFFSET
		#error "HAL_MAILBOX_CREATE_OFFSET not defined"
		#endif
		#ifndef HAL_MAILBOX_OPEN_OFFSET
		#error "HAL_MAILBOX_OPEN_OFFSET not defined"
		#endif
		#ifndef HAL_MAILBOX_CREATE_MAX
		#error "HAL_MAILBOX_CREATE_MAX not defined"
		#endif
		#ifndef HAL_MAILBOX_OPEN_MAX
		#error "HAL_MAILBOX_OPEN_MAX not defined"
		#endif
		#ifndef HAL_MAILBOX_MSG_SIZE
		#error "HAL_MAILBOX_MSG_SIZE not defined"
		#endif
		#ifndef HAL_MAILBOX_RESERVED_SIZE
		#error "HAL_MAILBOX_RESERVED_SIZE not defined"
		#endif
		#ifndef HAL_MAILBOX_DATA_SIZE
		#error "HAL_MAILBOX_DATA_SIZE not defined"
		#endif
		#ifndef HAL_MAILBOX_IOCTL_SET_ASYNC_BEHAVIOR
		#error "HAL_MAILBOX_IOCTL_SET_ASYNC_BEHAVIOR not defined"
		#endif

		/* Functions */
		#ifndef __mailbox_setup_fn
		#error "mailbox_setup() not defined?"
		#endif
		#ifndef __mailbox_create_fn
		#error "mailbox_create() not defined?"
		#endif
		#ifndef __mailbox_open_fn
		#error "mailbox_open() not defined?"
		#endif
		#ifndef __mailbox_unlink_fn
		#error "mailbox_unlink() not defined?"
		#endif
		#ifndef __mailbox_close_fn
		#error "mailbox_close() not defined?"
		#endif
		#ifndef __mailbox_awrite_fn
		#error "mailbox_awrite() not defined?"
		#endif
		#ifndef __mailbox_aread_fn
		#error "mailbox_aread() not defined?"
		#endif
		#ifndef __mailbox_wait_fn
		#error "mailbox_wait() not defined?"
		#endif
		#ifndef __mailbox_ioctl_fn
		#error "mailbox_ioctl() not defined?"
		#endif

	#endif

#endif

/* Dummy Constants */
#if (!__TARGET_HAS_MAILBOX)

	#define HAL_MAILBOX_CREATE_MAX               1
	#define HAL_MAILBOX_CREATE_OFFSET            0
	#define HAL_MAILBOX_OPEN_MAX                 1
	#define HAL_MAILBOX_OPEN_OFFSET              0
	#define HAL_MAILBOX_MSG_SIZE                 1
	#define HAL_MAILBOX_IOCTL_SET_ASYNC_BEHAVIOR 0

#endif /* !__TARGET_HAS_MAILBOX */

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @defgroup kernel-hal-target-mailbox Mailbox service
 * @ingroup kernel-hal-target
 *
 * @brief Target Standard Output HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>
	#include <nanvix/hlib.h>
	#include <posix/errno.h>

	/**
	 * @brief Creates a mailbox.
	 *
	 * @param local Logic ID of the underlying NoC node.
	 *
	 * @returns Upon successful completion, the ID of the newly created
	 * mailbox is returned. Upon failure, a negative error code is
	 * returned instead.
	 */
	EXTERN int mailbox_create(int local);

	/**
	 * @brief Opens a mailbox.
	 *
	 * @param remote Logic ID of the target NoC node.
	 *
	 * @returns Upon successful completion, the ID of the target mailbox
	 * is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
	EXTERN int mailbox_open(int remote);

	/**
	 * @brief Destroys a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int mailbox_unlink(int mbxid);

	/**
	 * @brief Closes a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int mailbox_close(int mbxid);

	/**
	 * @brief Writes data to a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be read from.
	 * @param size   Number of bytes to write.
	 *
	 * @returns Upon successful completion, 0 is returned
	 * and non zero otherwise.
	 */
	EXTERN ssize_t mailbox_awrite(int mbxid, const void *buffer, uint64_t size);

	/**
	 * @brief Reads data from a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be written to.
	 * @param size   Number of bytes to read.
	 *
	 * @returns Upon successful completion, 0 is returned
	 * and non zero otherwise.
	 */
	EXTERN ssize_t mailbox_aread(int mbxid, void *buffer, uint64_t size);

	/**
	 * @brief Waits asynchronous operation.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @return Zero if wait read correctly and non zero otherwise.
	 */
	EXTERN int mailbox_wait(int mbxid);

	/**
	 * @brief Performs control operations in a mailbox.
	 *
	 * @param mbxid   Target mailbox.
	 * @param request Request.
	 * @param ...     Additional arguments.
	 *
	 * @param Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
	EXTERN int mailbox_ioctl(int mbxid, unsigned request, ...);

	/**
	 * @brief Initializes the mailbox interface.
	 */
	EXTERN void mailbox_setup(void);

/**@}*/

#endif /* NANVIX_HAL_TARGET_MAILBOX_H_ */

