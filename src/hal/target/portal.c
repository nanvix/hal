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

#include <nanvix/hal/target/portal.h>
#include <posix/errno.h>
#include <posix/stddef.h>
#include <posix/stdint.h>

/*============================================================================*
 * portal_rx_is_valid()                                                       *
 *============================================================================*/

#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

/**
 * @brief Asserts whether or not a receiver portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int portal_rx_is_valid(int portalid)
{
	return (
		WITHIN(
			portalid,
			HAL_PORTAL_CREATE_OFFSET,
			HAL_PORTAL_CREATE_OFFSET + HAL_PORTAL_CREATE_MAX
		)
	);
}

#endif

/*============================================================================*
 * portal_tx_is_valid()                                                       *
 *============================================================================*/

#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

/**
 * @brief Asserts whether or not a sender portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int portal_tx_is_valid(int portalid)
{
	return (
		WITHIN(
			portalid,
			HAL_PORTAL_OPEN_OFFSET,
			HAL_PORTAL_OPEN_OFFSET + HAL_PORTAL_OPEN_MAX
		)
	);
}

#endif

/*============================================================================*
 * portal_create()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_create(int nodenum)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Is local valid? */
	if (!node_is_valid(nodenum))
		return (-EINVAL);

	/* Bad local NoC node. */
	if (!node_is_local(nodenum))
		return (-EINVAL);

	return (__portal_create(nodenum));

#else
	UNUSED(nodenum);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_open()                                                              *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_open(int localnum, int remotenum)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid local NoC node. */
	if (!node_is_valid(localnum))
		return (-EINVAL);

	/* Invalid remote NoC node. */
	if (!node_is_valid(remotenum))
		return (-EINVAL);

	/* Invalid NoC node ID. */
	if (localnum == remotenum)
		return (-EINVAL);

	/* Bad local NoC node. */
	if (node_is_local(remotenum))
		return (-EINVAL);

	return (__portal_open(localnum, remotenum));

#else
	UNUSED(localnum);
	UNUSED(remotenum);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_allow()                                                             *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_allow(int portalid, int nodenum)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Is nodenum valid? */
	if (!node_is_valid(nodenum))
		return (-EINVAL);

	/* Invalid portal.*/
	if (!portal_rx_is_valid(portalid))
		return (-EBADF);

	/* Bad local NoC node. */
	if (node_is_local(nodenum))
		return (-EINVAL);

	return (__portal_allow(portalid, nodenum));

#else
	UNUSED(portalid);
	UNUSED(nodenum);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_unlink()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_unlink(int portalid)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid portal. */
	if (!portal_rx_is_valid(portalid))
		return (-EBADF);

	return (__portal_unlink(portalid));

#else
	UNUSED(portalid);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_close()                                                             *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_close(int portalid)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid portal. */
	if (!portal_tx_is_valid(portalid))
		return (-EBADF);

	return (__portal_close(portalid));

#else
	UNUSED(portalid);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_awrite()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC ssize_t portal_awrite(int portalid, const void *buffer, uint64_t size)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid NoC node ID. */
	if (!portal_tx_is_valid(portalid))
		return (-EBADF);

	/* Bad buffer*/
	if (buffer == NULL)
		return (-EINVAL);

	/* Bad size. */
	if (size == 0 || size > HAL_PORTAL_MAX_SIZE)
		return (-EINVAL);

	return (__portal_awrite(portalid, buffer, size));

#else
	UNUSED(portalid);
	UNUSED(buffer);
	UNUSED(size);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_aread()                                                            *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC ssize_t portal_aread(int portalid, void *buffer, uint64_t size)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid NoC node ID. */
	if (!portal_rx_is_valid(portalid))
		return (-EBADF);

	/* Bad buffer*/
	if (buffer == NULL)
		return (-EINVAL);

	/* Bad size. */
	if (size == 0 || size > HAL_PORTAL_MAX_SIZE)
		return (-EINVAL);

	return (__portal_aread(portalid, buffer, size));

#else
	UNUSED(portalid);
	UNUSED(buffer);
	UNUSED(size);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_wait()                                                              *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int portal_wait(int portalid)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

	/* Invalid mailbox. */
	if (!(portal_rx_is_valid(portalid) || portal_tx_is_valid(portalid)))
		return (-EBADF);

	return (__portal_wait(portalid));

#else
	UNUSED(portalid);

	return (-ENOSYS);
#endif
}

/*============================================================================*
 * portal_setup()                                                             *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void portal_setup(void)
{
#if (__TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)
	__portal_setup();
#endif /* __TARGET_HAS_PORTAL */
}
