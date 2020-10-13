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

/* Must come first. */
#define __NEED_HAL_PROCESSOR
#define __NEED_RESOURCE

#include <arch/target/unix64/unix64/sync.h>
#include <nanvix/hal/processor.h>
#include <nanvix/hal/resource.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <fcntl.h>
#include <posix/errno.h>
#include <stdio.h>

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

/**
 * @brief Length of sync name.
 */
#define UNIX64_SYNC_NAME_LENGTH 128

/**
 * @brief Base name for a sync
 */
#define UNIX64_SYNC_BASENAME "nanvix-sync"

struct hash
{
	uint64_t source    :  5;
	uint64_t type      :  1;
	uint64_t master    :  5;
	uint64_t nodeslist : 20;
	uint64_t unused    : 33;
};

#define HASH_INITIALIZER ((struct hash){-1, 0, -1, 0, 0})

/**
 * @brief Synchronization point.
 */
PRIVATE struct queue
{
	mqd_t fd;                               /**< Underlying file descriptor. */
	char pathname[UNIX64_SYNC_NAME_LENGTH]; /**< Name of underlying mqueue.  */
} mqueues[PROCESSOR_NOC_NODES_NUM];

/**
 * @brief Table of synchronization points.
 */
PRIVATE struct
{
	/**
	 * @brief Receiver Synchronization Points
	 */
	struct rx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;               /**< Generic resource information. */

		int nbarriers;                          /**< Number of barriers completed. */
		struct hash hash;                       /**< Local sync hash.              */
		struct hash barrier;                    /**< Barrier control.              */
		int nreceived[PROCESSOR_NOC_NODES_NUM]; /**< Number of signals received.   */
	} rxs[UNIX64_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;               /**< Generic resource information.        */

		int nnodes;                             /**< Number of remotes in broadcast.      */
		int nodes[PROCESSOR_NOC_NODES_NUM];     /**< IDs of attached nodes.               */
		int sent[PROCESSOR_NOC_NODES_NUM];      /**< Signals when a signal has been sent. */
		struct hash hash;                       /**< Local sync hash.                     */
	} txs[UNIX64_SYNC_OPEN_MAX];
} synctab = {
	.rxs[0 ... (UNIX64_SYNC_CREATE_MAX - 1)] = {
		.resource  = RESOURCE_STATIC_INITIALIZER,
		.nbarriers = 0,
		.hash      = HASH_INITIALIZER,
		.barrier   = HASH_INITIALIZER,
		.nreceived = {0, },
	},

	.txs[0 ... (UNIX64_SYNC_OPEN_MAX - 1)] = {
		.resource = RESOURCE_STATIC_INITIALIZER,
		.nnodes   = 0,
		.nodes    = {0, },
		.sent     = {0, },
		.hash     = HASH_INITIALIZER,
	},
};

/**
 * @brief Pools of Synchronization Resource
 */
PRIVATE struct
{
	const struct resource_pool rx;
	const struct resource_pool tx;
} pool = {
	.rx = {synctab.rxs, UNIX64_SYNC_CREATE_MAX, sizeof(struct rx)},
	.tx = {synctab.txs, UNIX64_SYNC_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Sync module lock.
 */
PRIVATE pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Default message queue attribute.
 */
PRIVATE struct mq_attr mq_attr = {
	.mq_maxmsg = PROCESSOR_NOC_NODES_NUM,
	.mq_msgsize = sizeof(struct hash)
};

/*============================================================================*
 * unix64_sync_lock()                                                         *
 *============================================================================*/

/**
 * @brief Locks Unix sync module.
 */
PRIVATE void unix64_sync_lock(void)
{
	pthread_mutex_lock(&lock);
}

/*============================================================================*
 * unix64_sync_unlock()                                                       *
 *============================================================================*/

/**
 * @brief Unlocks Unix sync module.
 */
PRIVATE void unix64_sync_unlock(void)
{
	pthread_mutex_unlock(&lock);
}

/*============================================================================*
 * unix64_sync_build_nodeslist()                                              *
 *============================================================================*/

PRIVATE uint64_t unix64_sync_build_nodeslist(const int *nodes, int nnodes)
{
	uint64_t nodeslist = 0ULL;

	for (int j = 0; j < nnodes; j++)
		nodeslist |= (1ULL << nodes[j]);

	return (nodeslist);
}

/*============================================================================*
 * do_unix64_sync_search_rx()                                                 *
 *============================================================================*/

PRIVATE int do_unix64_sync_search_rx(struct hash * hash)
{
	for (unsigned i = 0; i < UNIX64_SYNC_CREATE_MAX; ++i)
	{
		if (!resource_is_used(&synctab.rxs[i].resource))
			continue;

		if (synctab.rxs[i].hash.type != hash->type)
			continue;

		if (synctab.rxs[i].hash.nodeslist != hash->nodeslist)
			continue;

		return (i);
	}

	return (-EINVAL);
}

/*============================================================================*
 * do_unix64_sync_search_tx()                                                 *
 *============================================================================*/

PRIVATE int do_unix64_sync_search_tx(struct hash * hash)
{
	for (unsigned i = 0; i < UNIX64_SYNC_OPEN_MAX; ++i)
	{
		if (!resource_is_used(&synctab.txs[i].resource))
			continue;

		if (synctab.txs[i].hash.type != hash->type)
			continue;

		if (synctab.txs[i].hash.nodeslist != hash->nodeslist)
			continue;

		return (i);
	}

	return (-EINVAL);
}

/*============================================================================*
 * unix64_sync_create()                                                       *
 *============================================================================*/

/**
 * @todo TODO: provide a long description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_create(const int * nodes, int nnodes, int type)
{
	int syncid;       /* Synchronization point. */
	struct hash hash; /* Synch point Hash.      */

	unix64_sync_lock();

		hash.source    = processor_node_get_num();
		hash.type      = type;
		hash.master    = nodes[0];
		hash.nodeslist = unix64_sync_build_nodeslist(nodes, nnodes);

		/* Searchs existing syncid. */
		if (do_unix64_sync_search_rx(&hash) >= 0)
			goto error;

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.rx)) < 0)
			goto error;

		/* Initialize synchronization point. */
		synctab.rxs[syncid].hash      = hash;
		synctab.rxs[syncid].barrier   = HASH_INITIALIZER;
		synctab.rxs[syncid].nbarriers = 0;
		kmemset(synctab.rxs[syncid].nreceived, 0, PROCESSOR_NOC_NODES_NUM * sizeof(int));

		resource_set_rdonly(&synctab.rxs[syncid].resource);
		resource_set_notbusy(&synctab.rxs[syncid].resource);

	unix64_sync_unlock();

	return (syncid + UNIX64_SYNC_CREATE_OFFSET);

error:
	unix64_sync_unlock();
	return (-EAGAIN);
}

/*============================================================================*
 * unix64_sync_open()                                                         *
 *============================================================================*/

/**
 * @todo TODO: provide a long description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 *
 * @todo Check for Invalid Remote
 */
PUBLIC int unix64_sync_open(const int * nodes, int nnodes, int type)
{
	int syncid;       /* Synchronization point. */
	struct hash hash; /* Synch point hash.      */

	unix64_sync_lock();

		hash.source    = processor_node_get_num();
		hash.type      = type;
		hash.master    = nodes[0];
		hash.nodeslist = unix64_sync_build_nodeslist(nodes, nnodes);

		/* Searchs existing syncid. */
		if (do_unix64_sync_search_tx(&hash) >= 0)
			goto error;

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.tx)) < 0)
			goto error;

		/* Initialize synchronization point. */
		synctab.txs[syncid].hash   = hash;
		synctab.txs[syncid].nnodes = nnodes;
		kmemcpy(synctab.txs[syncid].nodes, nodes, nnodes * sizeof(int));
		kmemset(synctab.txs[syncid].sent, 0, nnodes * sizeof(int));

		resource_set_wronly(&synctab.txs[syncid].resource);
		resource_set_notbusy(&synctab.txs[syncid].resource);

	unix64_sync_unlock();

	return (syncid + UNIX64_SYNC_OPEN_OFFSET);

error:
	unix64_sync_unlock();
	return (-EAGAIN);
}

/*============================================================================*
 * unix64_sync_unlink()                                                       *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_unlink(int syncid)
{
	syncid -= UNIX64_SYNC_CREATE_OFFSET;

again:
	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
			goto error;

		/* Bad sync. */
		if (resource_is_writable(&synctab.rxs[syncid].resource))
			goto error;

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		synctab.rxs[syncid].hash    = HASH_INITIALIZER;
		synctab.rxs[syncid].barrier = HASH_INITIALIZER;

		resource_free(&pool.rx, syncid);

	unix64_sync_unlock();

	return (0);

error:
	unix64_sync_unlock();
	return (-EBADF);
}

/*============================================================================*
 * unix64_sync_close()                                                        *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_close(int syncid)
{
	syncid -= UNIX64_SYNC_OPEN_OFFSET;

again:
	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
			goto error;

		/* Bad sync. */
		if (!resource_is_writable(&synctab.txs[syncid].resource))
			goto error;

		/* Busy sync. */
		if (resource_is_busy(&synctab.txs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		/* Initialize synchronization point. */
		synctab.txs[syncid].hash   = HASH_INITIALIZER;
		synctab.txs[syncid].nnodes = 0;

		resource_free(&pool.tx, syncid);

	unix64_sync_unlock();

	return (0);

error:
	unix64_sync_unlock();
	return (-EBADF);
}

/*============================================================================*
 * do_unix64_sync_ignore_signal()                                               *
 *============================================================================*/

PRIVATE void do_unix64_sync_ignore_signal(char * message, struct hash * hash)
{
	int source    = hash->source;
	int type      = hash->type;
	int master    = hash->master;
	int nodeslist = hash->nodeslist;

	kprintf("[sync][unix64] Dropping signal: %s | hash = (source:%d, type:%d, master:%d, nodeslist:%d)",
		message,
		source,
		type,
		master,
		nodeslist
	);
}

/*============================================================================*
 * unix64_sync_wait()                                                         *
 *============================================================================*/

/**
 * @brief Sync module lock.
 */
PRIVATE pthread_mutex_t lock_wait = PTHREAD_MUTEX_INITIALIZER;

/*============================================================================*
 * unix64_sync_barrier_is_complete()                                          *
 *============================================================================*/

PRIVATE int unix64_sync_barrier_is_complete(struct rx * rx)
{
	int received;
	int expected;

	received = rx->barrier.nodeslist;

	/* Does master notifies it? */
	if (rx->hash.type == UNIX64_SYNC_ONE_TO_ALL)
		expected = (rx->hash.nodeslist & (1 << rx->hash.master));
	
	/* Does slaves notifies it? */
	else
		expected = (rx->hash.nodeslist & ~(1 << rx->hash.master));
	
	return (received == expected);
}

/*============================================================================*
 * unix64_sync_barrier_reset()                                                *
 *============================================================================*/

PRIVATE void unix64_sync_barrier_reset(struct rx * rx)
{
	for (unsigned i = 0; i < PROCESSOR_NOC_NODES_NUM; ++i)
	{
		if (rx->barrier.nodeslist & (1 << i))
		{
			/**
			 * Consume a signals and reset barrier if there are no
			 * signals from that node.
			 **/
			if ((--rx->nreceived[i]) == 0)
				rx->barrier.nodeslist &= ~(1 << i);
		}
	}
}

/*============================================================================*
 * unix64_sync_barrier_consume()                                              *
 *============================================================================*/

PRIVATE int unix64_sync_barrier_consume(struct rx * rx)
{
	int consumed; /* Indicates if the barrier is consumed. */

	unix64_sync_lock();

		consumed = rx->nbarriers;

		/* Is the barrier complete? */
		if (consumed)
			rx->nbarriers--;

	unix64_sync_unlock();

	return (consumed);
}

/*============================================================================*
 * do_unix64_sync_wait()                                                      *
 *============================================================================*/

/**
 * @brief Waits for a signal.
 *
 * @param syncid ID of the target sync.
 * @param sig    Place where the received signal should be stored.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE int do_unix64_sync_wait(struct rx * rx)
{
	int syncid;       /* Synchronization point. */
	struct hash hash; /* Hash buffer.           */

	/* Is the previous wait released me? */
	if (unix64_sync_barrier_consume(rx))
		return (0);

	spinlock_lock(&lock_wait);

		/* Is other core released me? */
		if (unix64_sync_barrier_consume(rx))
		{
			spinlock_unlock(&lock_wait);
			return (0);
		}

		/* Reads a signal. */ 
		if (mq_receive(mqueues[rx->hash.source].fd, (char *) &hash, sizeof(struct hash), NULL) == -1)
		{
			spinlock_unlock(&lock_wait);
			return (-EAGAIN);
		}

		unix64_sync_lock();

			if (!node_is_valid(hash.source))
			{
				do_unix64_sync_ignore_signal("Invalid source.", &hash);
				goto release;
			}

			if ((syncid = do_unix64_sync_search_rx(&hash)) < 0)
			{
				do_unix64_sync_ignore_signal("Sync point not found.", &hash);
				goto release;
			}

			synctab.rxs[syncid].barrier.nodeslist |= (1 << hash.source);
			synctab.rxs[syncid].nreceived[hash.source]++;

			if (unix64_sync_barrier_is_complete(&synctab.rxs[syncid]))
			{
				unix64_sync_barrier_reset(&synctab.rxs[syncid]);
				synctab.rxs[syncid].nbarriers++;
			}

release:
		unix64_sync_unlock();
	spinlock_unlock(&lock_wait);

	/* Do again. */
	return (1);
}

/*============================================================================*
 * unix64_sync_wait()                                                         *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_wait(int syncid)
{
	int ret;

	ret     = (0);
	syncid -= UNIX64_SYNC_CREATE_OFFSET;

again:
	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
		{
			unix64_sync_unlock();
			return (-EBADF);
		}

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		if (synctab.rxs[syncid].nbarriers)
		{
			synctab.rxs[syncid].nbarriers--;
			goto exit;
		}

		/* Set sync as busy. */
		resource_set_busy(&synctab.rxs[syncid].resource);

	/*
	 * Release lock, since we may sleep below.
	 */
	unix64_sync_unlock();

	while ((ret = do_unix64_sync_wait(&synctab.rxs[syncid])) > 0);

	unix64_sync_lock();
		resource_set_notbusy(&synctab.rxs[syncid].resource);
exit:
	unix64_sync_unlock();

	return ((ret != 0) ? (-EAGAIN) : (0));
}

/*============================================================================*
 * unix64_sync_signal()                                                       *
 *============================================================================*/

/**
 * @brief Broadcasts a signal.
 *
 * @param i      Initial node ID.
 * @param nnodes Number of nodes.
 * @param nodes  Node IDs.
 * @param hash   Message.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE inline int do_unix64_sync_signal(
	int i,
	int nnodes,
	const int * nodes,
	int * sent,
	const struct hash * hash
)
{
	int ret;        /* Return value.    */
	int ntries = 5; /* Number of tries. */

	for (; i < nnodes; ++i)
	{
		if (sent[i])
			continue;

		do
		{
			struct timespec tm;

			if (ntries-- == 0)
			{
				ret = (-ETIMEDOUT);
				goto error;
			}

			clock_gettime(CLOCK_REALTIME, &tm);
			tm.tv_sec += 1;

			if (mq_timedsend(mqueues[nodes[i]].fd, (char *) hash, sizeof(struct hash), 1, &tm) == -1)
			{
				if (errno == EAGAIN)
					continue;

				ret = (-EAGAIN);
				goto error;
			}

			sent[i] = 1;
			break;

		} while (1);
	}

	ret = 0;

error:
	return (ret);
}

/**
 * @todo TODO: provide a detailed description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_signal(int syncid)
{
	int ret; /* Return value. */

	syncid -= UNIX64_SYNC_OPEN_OFFSET;

again:
	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
		{
			unix64_sync_unlock();
			return (-EBADF);
		}

		/* Busy sync. */
		if (resource_is_busy(&synctab.txs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		/* Set sync as busy. */
		resource_set_busy(&synctab.txs[syncid].resource);

	/*
	 * Release lock, since we may sleep below.
	 */
	unix64_sync_unlock();

	/* Broadcast. */
	if (synctab.txs[syncid].hash.type == UNIX64_SYNC_ONE_TO_ALL)
	{
		ret = do_unix64_sync_signal(
			1,
			synctab.txs[syncid].nnodes,
			synctab.txs[syncid].nodes,
			synctab.txs[syncid].sent,
			&synctab.txs[syncid].hash
		);

		if (ret == 0)
		{
			for (int i = 1; i < synctab.txs[syncid].nnodes; ++i)
				synctab.txs[syncid].sent[i] = 0;
		}
	}

	/* Gather. */
	else
	{
		ret = do_unix64_sync_signal(
			0,
			1,
			synctab.txs[syncid].nodes,
			synctab.txs[syncid].sent,
			&synctab.txs[syncid].hash
		);

		if (ret == 0)
			synctab.txs[syncid].sent[0] = 0;
	}

	unix64_sync_lock();
		resource_set_notbusy(&synctab.txs[syncid].resource);
	unix64_sync_unlock();

	return ((ret != 0) ? (-EAGAIN) : (0));
}

/*============================================================================*
 * unix64_sync_ioctl()                                                        *
 *============================================================================*/

/**
 * @brief Request an I/O operation on a sync.
 *
 * @param syncid  Sync resource.
 * @param request Type of request.
 * @param args    Arguments of the request.
 *
 * @returns Upon successful completion, zero is returned.
 * Upon failure, a negative error code is returned instead.
 */
PUBLIC int unix64_sync_ioctl(int syncid, unsigned request, va_list args)
{
	int ret = (-EINVAL); /* Return value. */

	UNUSED(syncid);
	UNUSED(args);

	unix64_sync_lock();

		switch (request)
		{
			case UNIX64_SYNC_IOCTL_SET_ASYNC_BEHAVIOR:
			{
				/**
				 * The Unix64 does not have asynchronous operations, so the
				 * definition of lock functions does not have any sense.
				 */
				ret = (0);
			} break;

			default:
				break;
		}

	/*
	 * Release lock, since we may sleep below.
	 */
	unix64_sync_unlock();

	return (ret);
}

/*============================================================================*
 * unix64_sync_setup()                                                        *
 *============================================================================*/

/**
 * @todo TODO: rely on dummy platform-independent dummy function.
 */
PUBLIC void unix64_sync_setup(void)
{
	int local;

	local = processor_node_get_num();

	/* Build pathname for NoC connector. */
	sprintf(mqueues[local].pathname, "/%s-%d", UNIX64_SYNC_BASENAME, local);

	/* Open NoC connector. */
	KASSERT(
		(mqueues[local].fd = 
			mq_open(mqueues[local].pathname,
				(O_RDONLY | O_CREAT),
				(S_IRUSR | S_IWUSR),
				&mq_attr
			)
		) != -1
	);

	for (int i = 0; i < PROCESSOR_NOC_NODES_NUM; ++i)
	{
		if (i == local)
			continue;

		sprintf(mqueues[i].pathname, "/%s-%d", UNIX64_SYNC_BASENAME, i);

		/* Open NoC connector. */
		KASSERT(
			(mqueues[i].fd = 
				mq_open(mqueues[i].pathname,
					(O_WRONLY | O_CREAT | O_NONBLOCK),
					(S_IRUSR | S_IWUSR),
					&mq_attr
				)
			) != -1
		);
	}

}

/*============================================================================*
 * unix64_sync_shutdown()                                                     *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void unix64_sync_shutdown(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(mq_close(mqueues[local].fd) == 0);
	KASSERT(mq_unlink(mqueues[local].pathname) == 0);

	for (int i = 0; i < PROCESSOR_NOC_NODES_NUM; ++i)
	{
		if (i == local)
			continue;

		KASSERT(mq_close(mqueues[i].fd) == 0);
	}
}

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */

