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

#include <nanvix/hal/target/mailbox.h>
#include <posix/errno.h>
#include <posix/stddef.h>
#include <posix/stdint.h>

#if __TARGET_HAS_MAILBOX

/*============================================================================*
 * node_is_valid()                                                            *
 *============================================================================*/

/**
 * @brief Asserts whether or not a sender mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int node_is_valid(int nodenum)
{
	return (WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM));
}

/*============================================================================*
 * mppa256_mailbox_rx_is_valid()                                              *
 *============================================================================*/

/**
 * @brief Asserts whether or not a receiver mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mailbox_rx_is_valid(int mbxid)
{
	return (
		WITHIN(
			mbxid,
			MAILBOX_CREATE_OFFSET,
			(MAILBOX_CREATE_OFFSET + MAILBOX_CREATE_MAX)
		)
	);
}

/*============================================================================*
 * mailbox_tx_is_valid()                                                      *
 *============================================================================*/

/**
 * @brief Asserts whether or not a sender mailbox is valid.
 *
 * @param mbxid ID of the target mailbox.
 *
 * @returns One if the target mailbox is valid, and false otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int mailbox_tx_is_valid(int mbxid)
{
	return (
		WITHIN(
			mbxid,
			MAILBOX_OPEN_OFFSET,
			MAILBOX_OPEN_OFFSET + MAILBOX_OPEN_MAX
		)
	);
}

/*============================================================================*
 * mailbox_create()                                                           *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int mailbox_create(int nodenum)
{
	/* Invalid NoC node. */
	if (!node_is_valid(nodenum))
		return (-EINVAL);

	return (__mailbox_create(nodenum));
}

/*============================================================================*
 * mailbox_open()                                                             *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int mailbox_open(int nodenum)
{
	/* Invalid NoC node. */
	if (!node_is_valid(nodenum))
		return (-EINVAL);

	return (__mailbox_open(nodenum));
}

/*============================================================================*
 * mailbox_unlink()                                                           *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int mailbox_unlink(int mbxid)
{
	/* Invalid mailbox. */
	if (!mailbox_rx_is_valid(mbxid))
		return (-EBADF);


	return (__mailbox_unlink(mbxid));
}

/*============================================================================*
 * mailbox_close()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int mailbox_close(int mbxid)
{
	/* Invalid mailbox. */
	if (!mailbox_tx_is_valid(mbxid))
		return (-EBADF);

	return (__mailbox_close(mbxid));
}

/*============================================================================*
 * mailbox_aread()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC ssize_t mailbox_aread(int mbxid, void *buffer, uint64_t size)
{
	/* Invalid buffer. */
	if (buffer == NULL)
		return (-EINVAL);

	/* Invalid read size. */
	if (size != MAILBOX_MSG_SIZE)
		return (-EINVAL);

	/* Invalid mailbox. */
	if (!mailbox_rx_is_valid(mbxid))
		return (-EBADF);

	return (__mailbox_aread(mbxid, buffer, size));
}

/*============================================================================*
 * mailbox_awrite()                                                           *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC ssize_t mailbox_awrite(int mbxid, const void *buffer, uint64_t size)
{
	/* Invalid buffer. */
	if (buffer == NULL)
		return (-EINVAL);

	/* Invalid write size. */
	if (size != MAILBOX_MSG_SIZE)
		return (-EINVAL);

	/* Invalid mailbox. */
	if (!mailbox_tx_is_valid(mbxid))
		return (-EBADF);

	return (__mailbox_awrite(mbxid, buffer, size));
}

/*============================================================================*
 * mailbox_wait()                                                             *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int mailbox_wait(int mbxid)
{
	/* Invalid mailbox. */
	if (!(mailbox_tx_is_valid(mbxid) || mailbox_tx_is_valid(mbxid)))
		return (-EBADF);

	return (__mailbox_wait(mbxid));
}

#endif /* __TARGET_HAS_MAILBOX */
