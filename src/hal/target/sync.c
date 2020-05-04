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
 * sync_rx_is_valid()                                                         *
 *============================================================================*/

#if (__TARGET_HAS_SYNC)

/**
 * @brief Asserts whether or not a receiver portal is valid.
 *
 * @param syncid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int sync_rx_is_valid(int syncid)
{
	return (
		WITHIN(
			syncid,
			SYNC_CREATE_OFFSET,
			SYNC_CREATE_OFFSET + SYNC_CREATE_MAX
		)
	);
}

#endif /* __TARGET_HAS_SYNC */

/*============================================================================*
 * sync_tx_is_valid()                                                         *
 *============================================================================*/

#if (__TARGET_HAS_SYNC)

/**
 * @brief Asserts whether or not a sender portal is valid.
 *
 * @param syncid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int sync_tx_is_valid(int syncid)
{
	return (
		WITHIN(
			syncid,
			SYNC_OPEN_OFFSET,
			SYNC_OPEN_OFFSET + SYNC_OPEN_MAX
		)
	);
}

#endif /* __TARGET_HAS_SYNC */

/*============================================================================*
 * sync_nodelist_is_valid()                                                   *
 *============================================================================*/

#if (__TARGET_HAS_SYNC)

/**
 * @brief Node list validation.
 *
 * @param local  Logic ID of local node.
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 *
 * @return Non zero if node list is valid and zero otherwise.
 */
PRIVATE int sync_nodelist_is_valid(const int * nodes, int nnodes, int is_the_one)
{
	int local;       /* Local node.          */
	uint64_t checks; /* Bit-stream of nodes. */

	checks = 0ULL;
	local  = processor_node_get_num();

	/* Is the local the one? */
	if (is_the_one && (nodes[0] != local))
		return (0);

	/* Isn't the local the one? */
	if (!is_the_one && (nodes[0] == local))
		return (0);

	/* Build nodelist. */
	for (int i = 0; i < nnodes; ++i)
	{
		/* Invalid node. */
		if (!WITHIN(nodes[i], 0, PROCESSOR_NOC_NODES_NUM))
			return (0);

		/* Does a node appear twice? */
		if (checks & (1ULL << nodes[i]))
			return (0);

		checks |= (1ULL << nodes[i]);
	}

	/* Is the local node founded? */
	return (checks & (1ULL << local));
}

#endif /* __TARGET_HAS_SYNC */

/*============================================================================*
 * sync_create()                                                              *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int sync_create(const int * nodes, int nnodes, int type)
{
#if (__TARGET_HAS_SYNC)
	int is_the_one;

	/*  Invalid nodes list. */
	if (nodes == NULL)
		return (-EINVAL);

	/* Bad nodes list. */
	if (!WITHIN(nnodes, 2, PROCESSOR_NOC_NODES_NUM + 1))
		return (-EINVAL);

	/* Bad sync type. */
	if ((type != SYNC_ONE_TO_ALL) && (type != SYNC_ALL_TO_ONE))
		return (-EINVAL);

	is_the_one = (type == SYNC_ALL_TO_ONE);

	/* Is nodelist valid? */
	if (!sync_nodelist_is_valid(nodes, nnodes, is_the_one))
		return (-EINVAL);

	return (__sync_create(nodes, nnodes, type));

#else /* __TARGET_HAS_SYNC */
	UNUSED(nodes);
	UNUSED(nnodes);
	UNUSED(type);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_open()                                                                *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int sync_open(const int * nodes, int nnodes, int type)
{
#if (__TARGET_HAS_SYNC)
	int is_the_one;

	/*  Invalid nodes list. */
	if (nodes == NULL)
		return (-EINVAL);

	/* Bad nodes list. */
	if (!WITHIN(nnodes, 2, PROCESSOR_NOC_NODES_NUM + 1))
		return (-EINVAL);

	/* Bad sync type. */
	if ((type != SYNC_ONE_TO_ALL) && (type != SYNC_ALL_TO_ONE))
		return (-EINVAL);

	is_the_one = (type == SYNC_ONE_TO_ALL);

	/* Is nodelist valid? */
	if (!sync_nodelist_is_valid(nodes, nnodes, is_the_one))
		return (-EINVAL);

	return (__sync_open(nodes, nnodes, type));

#else /* __TARGET_HAS_SYNC */
	UNUSED(nodes);
	UNUSED(nnodes);
	UNUSED(type);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_unlink()                                                              *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int sync_unlink(int syncid)
{
#if (__TARGET_HAS_SYNC)

	/* Invalid portal. */
	if (!sync_rx_is_valid(syncid))
		return (-EBADF);

	return (__sync_unlink(syncid));

#else /* __TARGET_HAS_SYNC */
	UNUSED(syncid);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_close()                                                               *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int sync_close(int syncid)
{
#if (__TARGET_HAS_SYNC)

	/* Invalid portal. */
	if (!sync_tx_is_valid(syncid))
		return (-EBADF);

	return (__sync_close(syncid));

#else /* __TARGET_HAS_SYNC */
	UNUSED(syncid);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_signal()                                                              *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC ssize_t sync_signal(int syncid)
{
#if (__TARGET_HAS_SYNC)

	/* Invalid NoC node ID. */
	if (!sync_tx_is_valid(syncid))
		return (-EBADF);

	return (__sync_signal(syncid));

#else /* __TARGET_HAS_SYNC */
	UNUSED(syncid);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_wait()                                                                *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC int sync_wait(int syncid)
{
#if (__TARGET_HAS_SYNC)

	/* Invalid mailbox. */
	if (!sync_rx_is_valid(syncid))
		return (-EBADF);

	return (__sync_wait(syncid));

#else /* __TARGET_HAS_SYNC */
	UNUSED(syncid);

	return (-ENOSYS);
#endif /* __TARGET_HAS_SYNC */
}

/*============================================================================*
 * sync_setup()                                                               *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void sync_setup(void)
{
#if (__TARGET_HAS_SYNC)
	__sync_setup();
#endif /* __TARGET_HAS_SYNC */
}