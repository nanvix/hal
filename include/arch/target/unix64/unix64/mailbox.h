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

#ifndef TARGET_UNIX64_UNIX64_MAILBOX_H_
#define TARGET_UNIX64_UNIX64_MAILBOX_H_

/**
 * @addtogroup target-unix64-mailbox Mailbox
 * @ingroup target-unix64
 *
 * @brief Mailbox.
 */
/**@{*/

	/* Processor API. */
	#include <arch/target/unix64/unix64/_unix64.h>

	#include <posix/sys/types.h>
	#include <nanvix/const.h>

	/**
	 * @name Maximum number of mailboxes points.
	 */
	/**@{*/
	#define UNIX64_MAILBOX_CREATE_MAX  1 /**< Maximum amount of create mailboxes. */
	#define UNIX64_MAILBOX_OPEN_MAX   12 /**< Maximum amount of open mailboxes.   */
	/**@}*/

	/**
	 * @name File descriptor offset.
	 */
	/**@{*/
	#define UNIX64_MAILBOX_CREATE_OFFSET 0 /**< Initial file descriptor id for creates. */
	#define UNIX64_MAILBOX_OPEN_OFFSET   0 /**< Initial file descriptor id for opens.   */
	/**@}*/

	/**
	 * @name Sizes (in bytes) of a mailbox message.
	 */
	/**@{*/
	#define UNIX64_MAILBOX_MSG_SIZE 120 /**< Data size. */
	/**@}*/

#ifdef __NANVIX_HAL

	/**
	 * @brief Shutdowns the mailbox interface.
	 */
	PUBLIC void unix64_mailbox_shutdown(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Creates a mailbox.
	 *
	 * @param nodenum Logic ID of the underlying NoC node.
	 *
	 * @returns Upon successful completion, the ID of the newly created
	 * mailbox is returned. Upon failure, a negative error code is
	 * returned instead.
	 */
	EXTERN int unix64_mailbox_create(int nodenum);

	/**
	 * @brief Opens a mailbox.
	 *
	 * @param nodeid Logic ID of the target NoC node.
	 *
	 * @returns Upon successful completion, the ID of the target
	 * mailbox is returned. Upon failure, a negative error code is
	 * returned instead.
	 */
	EXTERN int unix64_mailbox_open(int nodenum);

	/**
	 * @brief Destroys a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_mailbox_unlink(int mbxid);

	/**
	 * @brief Closes a mailbox.
	 *
	 * @param mbxid ID of the target mailbox.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int unix64_mailbox_close(int mbxid);

	/**
	 * @brief Writes data to a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be read from.
	 * @param size   Number of bytes to write.
	 *
	 * @returns Upon successful completion, 0 is returned and non zero
	 * otherwise.
	 */
	EXTERN ssize_t unix64_mailbox_awrite(int mbxid, const void *buffer, uint64_t size);

	/**
	 * @brief Reads data from a mailbox.
	 *
	 * @param mbxid  ID of the target mailbox.
	 * @param buffer Buffer where the data should be written to.
	 * @param size   Number of bytes to read.
	 *
	 * @returns Upon successful completion, 0 is returned and non zero
	 * otherwise.
	 */
	EXTERN ssize_t unix64_mailbox_aread(int mbxid, void *buffer, uint64_t size);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond unix64_mailbox
 */

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __mailbox_setup_fn  /**< mailbox_setup()  */
	#define __mailbox_create_fn /**< mailbox_create() */
	#define __mailbox_open_fn   /**< mailbox_open()   */
	#define __mailbox_unlink_fn /**< mailbox_unlink() */
	#define __mailbox_close_fn  /**< mailbox_close()  */
	#define __mailbox_awrite_fn /**< mailbox_awrite() */
	#define __mailbox_aread_fn  /**< mailbox_aread()  */
	#define __mailbox_wait_fn   /**< mailbox_wait()   */
	/**@}*/

	/**
	 * @name Provided Constants
	 */
	/**@{*/
	#define HAL_MAILBOX_MSG_SIZE      UNIX64_MAILBOX_MSG_SIZE      /**< UNIX64_MAILBOX_MSG_SIZE      */
	#define HAL_MAILBOX_CREATE_OFFSET UNIX64_MAILBOX_CREATE_OFFSET /**< UNIX64_MAILBOX_CREATE_OFFSET */
	#define HAL_MAILBOX_OPEN_OFFSET   UNIX64_MAILBOX_OPEN_OFFSET   /**< UNIX64_MAILBOX_OPEN_OFFSET   */
	#define HAL_MAILBOX_CREATE_MAX    UNIX64_MAILBOX_CREATE_MAX    /**< UNIX64_MAILBOX_CREATE_MAX    */
	#define HAL_MAILBOX_OPEN_MAX      UNIX64_MAILBOX_OPEN_MAX      /**< UNIX64_MAILBOX_OPEN_MAX      */
	/**@}*/

#ifdef __NANVIX_HAL

	/**
	 * @todo TODO: rely on dummy platform-independent dummy function.
	 */
	static inline void mailbox_setup(void)
	{

	}

#endif /* __NANVIX_HAL */

	/**
	 * @see unix64_mailbox_create()
	 */
	#define __mailbox_create(nodenum) \
		unix64_mailbox_create(nodenum)

	/**
	 * @see unix64_mailbox_open()
	 */
	#define __mailbox_open(nodenum) \
		unix64_mailbox_open(nodenum)

	/**
	 * @see unix64_mailbox_unlink()
	 */
	#define __mailbox_unlink(mbxid) \
		unix64_mailbox_unlink(mbxid)

	/**
	 * @see unix64_mailbox_close()
	 */
	#define __mailbox_close(mbxid) \
		unix64_mailbox_close(mbxid)

	/**
	 * @see unix64_mailbox_awrite()
	 */
	#define __mailbox_awrite(mbxid, buffer, size) \
		unix64_mailbox_awrite(mbxid, buffer, size)

	/**
	 * @see unix64_mailbox_aread()
	 */
	#define __mailbox_aread(mbxid, buffer, size) \
		unix64_mailbox_aread(mbxid, buffer, size)

	/**
	 * @brief Dummy operation.
	 *
	 * @param mbxid Unused.
	 */
	#define __mailbox_wait(mbxid) (0)

/**@}*/

#endif /* TARGET_UNIX64_UNIX64_MAILBOX_H_ */
