/*
 * MIT License
 *
 * Copyright (c) 2011-2018 Pedro Henrique Penna <pedrohenriquepenna@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* Must come fist. */
#define __NEED_HAL_PROCESSOR

#include <arch/target/unix64/unix64/portal.h>
#include <nanvix/hal/processor.h>
#include <nanvix/hal/resource.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <posix/errno.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Length of portal name.
 */
#define UNIX64_PORTAL_NAME_LENGTH 128

/**
 * @brief Base name for a portal
 */
#define UNIX64_PORTAL_BASENAME "portal"

/**
 * @brief Portal buffer.
 */
struct portal_buffer
{
	volatile int busy;                 /**< Busy?  */
	volatile int ready;                /**< Ready? */
	char data[UNIX64_PORTAL_MAX_SIZE]; /**< Data   */
};

/**
 * @brief Portals
 */
struct portal
{
	struct resource resource;                               /**< Generic resource information.  */
	int remote;                                             /**< Remote NoC node ID.            */
	int local;                                              /**< Local NoC node ID.             */
	sem_t *lock;                                            /**< Portal lock.                   */
	char portalname[UNIX64_PORTAL_NAME_LENGTH];             /**< Name of shared memory region.  */
	char lockname[UNIX64_PORTAL_NAME_LENGTH];               /**< Name of shared memory region.  */
	struct portal_buffer *buffers[PROCESSOR_NOC_NODES_NUM]; /**< Portal buffers.                */
};

/**
 * @brief Table of portals.
 */
PRIVATE struct
{
	/**
	 * @brief Input portals.
	 */
	struct portal rxs[UNIX64_PORTAL_CREATE_MAX];

	/**
	 * @brief Output portals.
	 */
	struct portal txs[UNIX64_PORTAL_OPEN_MAX];
} portaltab = {
	.rxs[0 ... UNIX64_PORTAL_CREATE_MAX - 1] = {
		.resource = {0},
	},

	.txs[0 ... UNIX64_PORTAL_OPEN_MAX - 1] = {
		.resource = {0},
	},
};

/**
 * @brief Sync module lock.
 */
PRIVATE pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Resource pool for portals
 */
PRIVATE struct
{
	const struct resource_pool rx;
	const struct resource_pool tx;
} pool = {
	.rx = {portaltab.rxs, UNIX64_PORTAL_CREATE_MAX, sizeof(struct portal)},
	.tx = {portaltab.txs, UNIX64_PORTAL_OPEN_MAX,   sizeof(struct portal)},
};

/*============================================================================*
 * unix64_portals_lock()                                                       *
 *============================================================================*/

/**
 * @brief Locks Unix portal module.
 */
PRIVATE void unix64_portals_lock(void)
{
	pthread_mutex_lock(&lock);
}

/*============================================================================*
 * unix64_portals_unlock()                                                     *
 *============================================================================*/

/**
 * @brief Unlocks Unix portal module.
 */
PRIVATE void unix64_portals_unlock(void)
{
	pthread_mutex_unlock(&lock);
}

/*============================================================================*
 * unix64_portal_rx_is_valid()                                                *
 *============================================================================*/

/**
 * @brief Asserts if an input portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int unix64_portal_rx_is_valid(int portalid)
{
	return ((portalid >= 0) && (portalid < UNIX64_PORTAL_CREATE_MAX));
}

/*============================================================================*
 * unix64_portal_tx_is_valid()                                                *
 *============================================================================*/

/**
 * @brief Asserts if an input portal is valid.
 *
 * @param portalid ID of the target portal.
 *
 * @returns One if the target portal is valid, and false otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int unix64_portal_tx_is_valid(int portalid)
{
	return ((portalid >= 0) && (portalid < UNIX64_PORTAL_OPEN_MAX));
}

/*============================================================================*
 * unix64_portal_buffer_open()                                                *
 *============================================================================*/

/**
 * @brief Opens a portal buffer.
 *
 * @param portal Target portal.
 * @param local  Target local NoC node.
 * @param remote Target remote NoC node.
 */
PRIVATE void unix64_portal_buffer_open(struct portal *portal, int local, int remote)
{
	int shm;
	struct stat st;
	int initialize = 0;

	/* Build portal name. */
	sprintf(portal->portalname,
		"%s-%d-%d",
		UNIX64_PORTAL_BASENAME,
		local,
		remote
	);

	/* Create portal buffers. */
	KASSERT((shm =
		shm_open(portal->portalname,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR)
		) != -1
	);

	/* Allocate portal buffer. */
	KASSERT(fstat(shm, &st) != -1);
	if (st.st_size == 0)
	{
		initialize = 1;
		KASSERT(
			ftruncate(shm,
				sizeof(struct portal_buffer)
			) != -1
		);
	}

	/* Attach portal buffer. */
	KASSERT((portal->buffers[remote] =
		mmap(NULL,
			PROCESSOR_NOC_NODES_NUM*sizeof(struct portal_buffer),
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			shm,
			0)
		) != NULL
	);

	if (initialize)
	{
		portal->buffers[remote]->busy = 0;
		portal->buffers[remote]->ready = 0;
	}
}

/**
 * @see unix64_portal_buffer_open()
 */
PRIVATE void unix64_portal_buffer_rx_open(struct portal *portal, int local, int remote)
{
	unix64_portal_buffer_open(portal, local, remote);
}

/**
 * @see unix64_portal_buffer_open()
 */
PRIVATE void unix64_portal_buffer_tx_open(struct portal *portal, int local, int remote)
{
	unix64_portal_buffer_open(portal, remote, local);
}

/*============================================================================*
 * unix64_portal_buffer_close()                                               *
 *============================================================================*/

/**
 * @brief Closes a portal buffer.
 *
 * @param portal   Target portal.
 * @param bufferid Target buffer.
 */
PRIVATE void unix64_portal_buffer_close(struct portal *portal, int bufferid)
{
	/* Detach portal buffer. */
	KASSERT(munmap(portal->buffers[bufferid],
			sizeof(struct portal_buffer)
		) != -1
	);

	/* Destroy portal buffers. */
	shm_unlink(portal->portalname);
}

/*============================================================================*
 * unix64_portal_lock_open()                                                  *
 *============================================================================*/

/**
 * @brief Initializes the lock of a portal.
 *
 * @param portal Target portal.
 */
PRIVATE void unix64_portal_lock_open(struct portal *portal, int local)
{
	/* Build portal name. */
	sprintf(portal->lockname,
		"%s-%d",
		UNIX64_PORTAL_BASENAME,
		local
	);

	/* Create and initialize portal buffer lock. */
	KASSERT((portal->lock =
		sem_open(portal->lockname,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR,
			1)
		) != NULL
	);
}

/**
 * @see unix64_portal_lock_open()
 */
PRIVATE void unix64_portal_lock_rx_open(struct portal *portal, int local)
{
	unix64_portal_lock_open(portal, local);
}

/**
 * @see unix64_portal_lock_open()
 */
PRIVATE void unix64_portal_lock_tx_open(struct portal *portal, int remote)
{
	unix64_portal_lock_open(portal, remote);
}

/*============================================================================*
 * unix64_portal_lock_close()                                                 *
 *============================================================================*/

/**
 * @brief Closes the lock of a portal.
 *
 * @param portal Target portal.
 */
PRIVATE void unix64_portal_lock_close(struct portal *portal)
{
	KASSERT(sem_close(portal->lock) != -1);
	sem_unlink(portal->lockname);
}

/*============================================================================*
 * unix64_portal_lock()                                                       *
 *============================================================================*/

/**
 * @brief Locks a portal.
 *
 * @param portal Target portal.
 */
PRIVATE inline void unix64_portal_lock(struct portal *portal)
{
	KASSERT(sem_wait(portal->lock) != -1);
}

/*============================================================================*
 * unix64_portal_unlock()                                                     *
 *============================================================================*/

/**
 * @brief Unlocks a portal.
 *
 * @param portal Target portal.
 */
PRIVATE inline void unix64_portal_unlock(struct portal *portal)
{
	KASSERT(sem_post(portal->lock) != -1);
}

/*============================================================================*
 * unix64_portal_create()                                                     *
 *============================================================================*/

/**
 * @brief Asserts if an input portal exists.
 *
 * @param local Target local NoC node.
 *
 * @returns One if an input portal exists for the target local NoC
 * node and zero otherwise.
 */
PRIVATE int unix64_portal_rx_exists(int local)
{
	for (int i = 0; i < UNIX64_PORTAL_MAX; i++)
	{
		/* Skip invalid portals. */
		if (!resource_is_used(&portaltab.rxs[i].resource))
			continue;

		/* Exists. */
		if (portaltab.rxs[i].local == local)
			return (1);
	}

	return (0);
}

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int do_unix64_portal_create(int local)
{
	int error;    /* Error code.   */
	int portalid; /* ID of portal. */

	unix64_portals_lock();

		/* Exists. */
		if (unix64_portal_rx_exists(local))
		{
			error = -EEXIST;
			goto error0;
		}

		/* Allocate portal. */
		if ((portalid = resource_alloc(&pool.rx)) < 0)
		{
			error = -EAGAIN;
			goto error0;
		}

		/* Open portal lock. */
		unix64_portal_lock_rx_open(&portaltab.rxs[portalid], local);

		/* Initialize portal. */
		portaltab.rxs[portalid].local = local;
		portaltab.rxs[portalid].remote = -1;
		resource_set_rdonly(&portaltab.rxs[portalid].resource);
		resource_set_notbusy(&portaltab.rxs[portalid].resource);

		unix64_portals_unlock();

	return (portalid);

error0:
	unix64_portals_unlock();
	return (error);
}

/**
 * @see do_unix64_portal_create().
 */
PUBLIC int unix64_portal_create(int local)
{
	/* Invalid local NoC node. */
	if ((local < 0) || (local >= PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Bad local NoC node. */
	if (local != processor_node_get_num(core_get_id()))
		return (-EINVAL);

	return (do_unix64_portal_create(local));
}

/*============================================================================*
 * unix64_portal_allow()                                                      *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int do_unix64_portal_allow(int portalid, int remote)
{
again:

	unix64_portals_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.rxs[portalid].resource))
		{
			unix64_portals_unlock();
			return (-EBADF);
		}

		/* Busy portal. */
		if (resource_is_busy(&portaltab.rxs[portalid].resource))
		{
			unix64_portals_unlock();
			goto again;
		}

		/* Read operation is ongoing. */
		if (portaltab.rxs[portalid].remote != -1)
		{
			unix64_portals_unlock();
			return (-EBUSY);
		}

		/*
		 * Set portal as busy, because we
		 * release the global lock below.
		 */
		resource_set_busy(&portaltab.rxs[portalid].resource);

	unix64_portals_unlock();

	unix64_portal_lock(&portaltab.rxs[portalid]);

		if (portaltab.rxs[portalid].buffers[remote] == NULL)
		{
			unix64_portal_buffer_rx_open(
				&portaltab.rxs[portalid],
				portaltab.rxs[portalid].local,
				remote
			);
		}

		/* Device is busy. */
		if (portaltab.rxs[portalid].buffers[remote]->busy)
			goto error0;

		/* Device is ready. */
		if (portaltab.rxs[portalid].buffers[remote]->ready)
			goto error0;

		portaltab.rxs[portalid].remote = remote;
		portaltab.rxs[portalid].buffers[remote]->ready = 1;
		resource_set_notbusy(&portaltab.rxs[portalid].resource);

	unix64_portal_unlock(&portaltab.rxs[portalid]);

	return (0);

error0:
	unix64_portal_unlock(&portaltab.rxs[portalid]);
	return (-EBUSY);
}

/**
 * @see do_unix64_portal_allow().
 */
PUBLIC int unix64_portal_allow(int portalid, int remote)
{
	/* Invalid portal.*/
	if (!unix64_portal_rx_is_valid(portalid))
		return (-EBADF);

	/* Invalid remote NoC node. */
	if ((remote < 0) || (remote >= PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Bad remote. */
	if (remote == processor_node_get_num(core_get_id()))
		return (-EINVAL);

	return (do_unix64_portal_allow(portalid, remote));
}

/*============================================================================*
 * unix64_portal_open()                                                       *
 *============================================================================*/

/**
 * @brief Asserts if an output portal exists.
 *
 * @param local   Target local NoC node.
 * @param remote Target remote NoC node.
 *
 * @returns One if an output portal exists between the target local
 * and remote NoC nodes and zero otherwise.
 */
PRIVATE int unix64_portal_tx_exists(int local, int remote)
{
	for (int i = 0; i < UNIX64_PORTAL_MAX; i++)
	{
		/* Skip invalid portals. */
		if (!resource_is_used(&portaltab.txs[i].resource))
			continue;

		/* Skip invalid remotes. */
		if (portaltab.txs[i].remote != remote)
			continue;

		/* Exists. */
		if (portaltab.txs[i].local == local)
			return (1);
	}

	return (0);
}

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int do_unix64_portal_open(int local, int remote)
{
	int error;    /* Error.         */
	int portalid; /* ID of  portal. */

	unix64_portals_lock();

		/* Exists. */
		if (unix64_portal_tx_exists(local, remote))
		{
			error = -EEXIST;
			goto error0;
		}

		/* Allocate portal. */
		if ((portalid = resource_alloc(&pool.tx)) < 0)
		{
			error = -EAGAIN;
			goto error0;
		}

		/* Open portal lock and buffer. */
		unix64_portal_lock_tx_open(&portaltab.txs[portalid], remote);
		unix64_portal_buffer_tx_open(&portaltab.txs[portalid], local, remote);

		/* Initialize portal. */
		portaltab.txs[portalid].local = local;
		portaltab.txs[portalid].remote = remote;
		resource_set_wronly(&portaltab.txs[portalid].resource);
		resource_set_notbusy(&portaltab.txs[portalid].resource);

	unix64_portals_unlock();

	return (portalid);

error0:
	unix64_portals_unlock();
	return (error);
}

/**
 * @see do_unix64_portal_open().
 */
PUBLIC int unix64_portal_open(int local, int remote)
{
	/* Invalid local NoC node. */
	if ((local < 0) || (local >= PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Invalid remote NoC node. */
	if ((remote < 0) || (remote >= PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Bad local. */
	if (local != processor_node_get_num(core_get_id()))
		return (-EINVAL);

	/* Bad remote. */
	if (remote == local)
		return (-EINVAL);

	return (do_unix64_portal_open(local, remote));
}

/*============================================================================*
 * unix64_portal_wait()                                                       *
 *============================================================================*/

/**
 * @todo TODO: implement this function.
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_portal_wait(int portalid)
{
	UNUSED(portalid);

	return (0);
}

/*============================================================================*
 * unix64_portal_read()                                                       *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE ssize_t do_unix64_portal_read(int portalid, void *buf, size_t n)
{
	int nread;
	int remote;

again:

	unix64_portals_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.rxs[portalid].resource))
			goto error0;

		/* Busy portal. */
		if (resource_is_busy(&portaltab.rxs[portalid].resource))
		{
			unix64_portals_unlock();
			goto again;
		}

		/* No read operation is ongoing. */
		if (portaltab.rxs[portalid].remote == -1)
			goto error0;

		/*
		 * Set portal as busy, because we
		 * release the global lock below.
		 */
		resource_set_busy(&portaltab.rxs[portalid].resource);

	unix64_portals_unlock();

	unix64_portal_lock(&portaltab.rxs[portalid]);

		remote = portaltab.rxs[portalid].remote;

		/* No data is available. */
		if (!portaltab.rxs[portalid].buffers[remote]->busy)
		{
			resource_set_notbusy(&portaltab.rxs[portalid].resource);
			unix64_portal_unlock(&portaltab.rxs[portalid]);
			goto again;
		}

		kmemcpy(buf, portaltab.rxs[portalid].buffers[remote]->data, nread = n);

		portaltab.rxs[portalid].remote = -1;
		portaltab.rxs[portalid].buffers[remote]->busy = 0;
		portaltab.rxs[portalid].buffers[remote]->ready = 0;

		resource_set_notbusy(&portaltab.rxs[portalid].resource);

	unix64_portal_unlock(&portaltab.rxs[portalid]);

	return (nread);

error0:
	unix64_portals_unlock();
	return (-EBADF);
}

/**
 * @see do_unix64_portal_read();
 */
PUBLIC ssize_t unix64_portal_read(int portalid, void *buf, size_t n)
{
	/* Invalid portal ID.*/
	if (!unix64_portal_rx_is_valid(portalid))
		return (-EBADF);

	/* Invalid buffer. */
	if (buf == NULL)
		return (-EINVAL);

	/* Invalid read size. */
	if ((n < 1) || (n > UNIX64_PORTAL_MAX_SIZE))
		return (-EINVAL);

	return (do_unix64_portal_read(portalid, buf, n));
}

/*============================================================================*
 * unix64_portal_write()                                                      *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE ssize_t do_unix64_portal_write(int portalid, const void *buf, size_t n)
{
	int nwrite;
	int local;

again:

	unix64_portals_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.txs[portalid].resource))
			goto error0;

		/* Busy portal. */
		if (resource_is_busy(&portaltab.txs[portalid].resource))
		{
			unix64_portals_unlock();
			goto again;
		}

		/*
		 * Set portal as busy, because we
		 * release the global lock below.
		 */
		resource_set_busy(&portaltab.txs[portalid].resource);

	unix64_portals_unlock();

	unix64_portal_lock(&portaltab.txs[portalid]);

		local = portaltab.txs[portalid].local;

		/* Remote is not ready. */
		if (!portaltab.txs[portalid].buffers[local]->ready)
		{
			resource_set_notbusy(&portaltab.txs[portalid].resource);
			unix64_portal_unlock(&portaltab.txs[portalid]);
			goto again;
		}

		kmemcpy(portaltab.txs[portalid].buffers[local]->data, buf, nwrite = n);

		portaltab.txs[portalid].buffers[local]->busy = 1;

		resource_set_notbusy(&portaltab.txs[portalid].resource);

	unix64_portal_unlock(&portaltab.txs[portalid]);

	return (nwrite);

error0:
	unix64_portals_unlock();
	return (-EBADF);
}

/**
 * @see unix64_do_portal_write().
 */
PUBLIC ssize_t unix64_portal_write(int portalid, const void *buf, size_t n)
{
	/* Invalid portal ID.*/
	if (!unix64_portal_tx_is_valid(portalid))
		return (-EBADF);

	/* Invalid buffer. */
	if (buf == NULL)
		return (-EINVAL);

	/* Invalid write size. */
	if ((n < 1) || (n > UNIX64_PORTAL_MAX_SIZE))
		return (-EINVAL);

	return (do_unix64_portal_write(portalid, buf, n));
}

/*============================================================================*
 * unix64_portal_unlink()                                                     *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_portal_unlink(int portalid)
{
	/* Invalid portal. */
	if (!unix64_portal_rx_is_valid(portalid))
		return (-EBADF);

again:

	unix64_portals_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.rxs[portalid].resource))
		{
			unix64_portals_unlock();
			return (-EBADF);
		}

		/* Busy portal. */
		if (resource_is_busy(&portaltab.rxs[portalid].resource))
		{
			unix64_portals_unlock();
			goto again;
		}

		for (int i = 0; i < PROCESSOR_NOC_NODES_NUM; i++)
		{
			if (portaltab.rxs[portalid].buffers[i] != NULL)
			{
				portaltab.rxs[portalid].buffers[i]->busy = 0;
				portaltab.rxs[portalid].buffers[i]->ready = 0;
			}
		}

		resource_free(&pool.rx, portalid);

	unix64_portals_unlock();

	return (0);
}

/*============================================================================*
 * unix64_portal_close()                                                      *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_portal_close(int portalid)
{
	/* Invalid portal. */
	if (!unix64_portal_tx_is_valid(portalid))
		return (-EBADF);

again:

	unix64_portals_lock();

		/* Bad portal. */
		if (!resource_is_used(&portaltab.txs[portalid].resource))
		{
			unix64_portals_unlock();
			return (-EBADF);
		}

		/* Busy portal. */
		if (resource_is_busy(&portaltab.txs[portalid].resource))
		{
			unix64_portals_unlock();
			goto again;
		}

		resource_free(&pool.tx, portalid);

	unix64_portals_unlock();

	return (0);
}

/*============================================================================*
 * unix64_portal_setup()                                                      *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void unix64_portal_setup(void)
{
	kprintf("[hal][target] initializing portals...");

	for (int i = 0; i < UNIX64_PORTAL_CREATE_MAX; i++)
	{
		for (int j = 0; j < PROCESSOR_NOC_NODES_NUM; j++)
			portaltab.rxs[i].buffers[j] = NULL;
	}
}

/*============================================================================*
 * unix64_portal_shutdown()                                                   *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void unix64_portal_shutdown(void)
{
	/* Input portals. */
	for (int i = 0; i < UNIX64_PORTAL_CREATE_MAX; i++)
	{
		for (int j = 0; j < PROCESSOR_NOC_NODES_NUM; j++)
		{
			munmap(portaltab.rxs[i].buffers[j],
				sizeof(struct portal_buffer)
			);
		}
		sem_close(portaltab.rxs[i].lock);
	}

	/* Output portals. */
	for (int i = 0; i < UNIX64_PORTAL_OPEN_MAX; i++)
	{
		for (int j = 0; j < PROCESSOR_NOC_NODES_NUM; j++)
		{
			munmap(portaltab.txs[i].buffers[j],
				sizeof(struct portal_buffer)
			);
		}
		sem_close(portaltab.txs[i].lock);
	}

	/* Unlink portas. */
	for (int i = 0; i < PROCESSOR_NOC_NODES_NUM; i++)
	{
		char pathname[UNIX64_PORTAL_NAME_LENGTH];

		for (int j = 0; j < PROCESSOR_NOC_NODES_NUM; j++)
		{
			sprintf(pathname, "%s-%d-%d", UNIX64_PORTAL_BASENAME, i, j);
			shm_unlink(pathname);
		}

		sprintf(pathname,"%s-%d", UNIX64_PORTAL_BASENAME, i);
		sem_unlink(pathname);
	}
}
