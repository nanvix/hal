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

#ifndef NANVIX_HAL_TARGET_MAILBOX_H_
#define NANVIX_HAL_TARGET_MAILBOX_H_

	/* Target Interface Implementation */
	#include <nanvix/hal/target/_target.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_TARGET_AL) || defined(__INTERFACE_CHECK_MAILBOX)

	/* Feature Checking */
	#ifndef TARGET_HAS_MAILBOX
	#error "does this target feature a mailbox service?"
	#endif

	/* Has Stadard Output Device */
	#if (TARGET_HAS_MAILBOX)

		/* Constants */
		#ifndef MAILBOX_CREATE_MAX
		#error "MAILBOX_CREATE_MAX not defined"
		#endif
		#ifndef MAILBOX_OPEN_MAX
		#error "MAILBOX_OPEN_MAX not defined"
		#endif

		/* Functions */
		#ifndef __mailbox_create_fn
		#error "sync_create() not defined?"
		#endif
		#ifndef __mailbox_open_fn
		#error "sync_open() not defined?"
		#endif
		#ifndef __mailbox_unlink_fn
		#error "sync_unlink() not defined?"
		#endif
		#ifndef __mailbox_close_fn
		#error "sync_close_() not defined?"
		#endif
		#ifndef __mailbox_write_fn
		#error "sync_wait() not defined?"
		#endif
		#ifndef __mailbox_read_fn
		#error "sync_signal() not defined?"
		#endif

	#else

		/* Constants */
		#define MAILBOX_CREATE_MAX 1
		#define MAILBOX_OPEN_MAX   1
		#define MAILBOX_MSG_SIZE   1

	#endif

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

	#include <nanvix/klib.h>

	/**
	 * @brief Creates a mailbox.
	 *
	 * @param nodenum Logic ID of the underlying NoC node.
	 *
	 * @returns Upon successful completion, the ID of the newly created
	 * mailbox is returned. Upon failure, a negative error code is
	 * returned instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN int mailbox_create(int nodenum);
#else
	EXTERN int mailbox_create(int nodenum)
	{
		UNUSED(nodenum);

		return (0);
	}
#endif

	/**
	 * @brief Opens a mailbox.
	 *
	 * @param nodeid Logic ID of the target NoC node.
	 *
	 * @returns Upon successful completion, the ID of the target mailbox
	 * is returned. Upon failure, a negative error code is returned
	 * instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN int mailbox_open(int nodenum);
#else
	static inline int mailbox_open(int nodenum)
	{
		UNUSED(nodenum);

		return (0);
	}
#endif

	/**
	 * @brief Destroys a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon failure,
	 * a negative error code is returned instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN int mailbox_unlink(int mbxid);
#else
	static inline int mailbox_unlink(int mbxid)
	{
		UNUSED(mbxid);

		return (0);
	}
#endif

	/**
	 * @brief Closes a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN int mailbox_close(int mbxid);
#else
	static inline int mailbox_close(int mbxid)
	{
		UNUSED(mbxid);

		return (0);
	}
#endif

	/**
	 * @brief Writes data to a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be read from.
	 * @param size   Number of bytes to write.
	 *
	 * @returns Upon successful completion, the number of bytes
	 * successfully written is returned. Upon failure, a negative error
	 * code is returned instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN ssize_t mailbox_write(int mbxid, const void * buffer, size_t size);
#else
	static inline ssize_t mailbox_write(int mbxid, const void * buffer, size_t size)
	{
		UNUSED(mbxid);
		UNUSED(buffer);
		UNUSED(size);

		return (0);
	}
#endif

	/**
	 * @brief Reads data from a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be written to.
	 * @param size   Number of bytes to read.
	 *
	 * @returns Upon successful completion, the number of bytes
	 * successfully read is returned. Upon failure, a negative error code
	 * is returned instead.
	 */
#if (TARGET_HAS_MAILBOX)
	EXTERN ssize_t mailbox_read(int mbxid, void * buffer, size_t size);
#else
	static inline ssize_t mailbox_read(int mbxid, void * buffer, size_t size)
	{
		UNUSED(mbxid);
		UNUSED(buffer);
		UNUSED(size);

		return (0);
	}
#endif

/**@}*/

#endif /* NANVIX_HAL_TARGET_MAILBOX_H_ */
