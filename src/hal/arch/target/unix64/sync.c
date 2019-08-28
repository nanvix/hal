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

/* Must come first. */
#define __NEED_PROCESSOR_LINUX64

#include <arch/processor/linux64.h>
#include <arch/target/unix64/unix64/sync.h>
#include <nanvix/hal/resource.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

/**
 * @brief Length of sync name.
 */
#define UNIX64_SYNC_NAME_LENGTH 128

/**
 * @brief Synchronization point.
 */
struct sync
{
	struct resource resource;                   /**< Underlying resource.            */
	int type;                                   /**< Type.                           */
	mqd_t fd;                                   /**< Underlying file descriptor.     */
	int ncount;                                 /**< Number of remotes in broadcast. */
	char pathname[UNIX64_SYNC_NAME_LENGTH];     /**< Name of underlying mqueue.      */
	int nodes[LINUX64_PROCESSOR_NOC_NODES_NUM]; /**< IDs of attached nodes.          */
};

/**
 * @brief Table of synchronization points.
 */
PRIVATE struct
{
	/**
	 * @brief Receiver Synchronization Points
	 */
	struct sync rxs[UNIX64_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct sync txs[UNIX64_SYNC_OPEN_MAX];
} synctab = {
	.rxs[0 ... UNIX64_SYNC_CREATE_MAX - 1] = {
		.resource = {0},
	},

	.txs[0 ... UNIX64_SYNC_OPEN_MAX - 1] = {
		.resource = {0},
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
	.rx = {synctab.rxs, UNIX64_SYNC_CREATE_MAX, sizeof(struct sync)},
	.tx = {synctab.txs, UNIX64_SYNC_OPEN_MAX,   sizeof(struct sync)},
};

/**
 * @brief Sync module lock.
 */
PRIVATE pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Default message queue attribute.
 */
PRIVATE struct mq_attr mq_attr = {
	.mq_maxmsg = LINUX64_PROCESSOR_NOC_NODES_NUM,
	.mq_msgsize = sizeof(int)
};

/*============================================================================*
 * unix64_sync_lock()                                                           *
 *============================================================================*/

/**
 * @brief Locks Unix sync module.
 */
PRIVATE void unix64_sync_lock(void)
{
	pthread_mutex_lock(&lock);
}

/*============================================================================*
 * unix64_sync_unlock()                                                         *
 *============================================================================*/

/**
 * @brief Unlocks Unix sync module.
 */
PRIVATE void unix64_sync_unlock(void)
{
	pthread_mutex_unlock(&lock);
}

/*============================================================================*
 * unix64_sync_rx_is_valid()                                                  *
 *============================================================================*/

/**
 * @brief Asserts if a receiver synchronization point is valid.
 *
 * @param syncid ID of the target synchronization point.
 *
 * @returns One if the target synchronization point is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int unix64_sync_rx_is_valid(int syncid)
{
	return ((syncid >= 0) && (syncid < UNIX64_SYNC_CREATE_MAX));
}

/*============================================================================*
 * unix64_sync_tx_is_valid()                                                  *
 *============================================================================*/

/**
 * @brief Asserts if a sender synchronization point is valid.
 *
 * @param syncid ID of the target synchronization point.
 *
 * @returns One if the target synchronization point is valid, and false
 * otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PRIVATE int unix64_sync_tx_is_valid(int syncid)
{
	return ((syncid >= 0) && (syncid < UNIX64_SYNC_OPEN_MAX));
}

/*============================================================================*
 * unix64_sync_is_remote()                                                    *
 *============================================================================*/

/**
 * @brief Sync remote point validation.
 *
 * @param nodenum Logic ID of local node.
 * @param nodes   IDs of target NoC nodes.
 * @param nnodes  Number of target NoC nodes.
 *
 * @return Zero if remote point is valid and non zero otherwise.
 */
PRIVATE int unix64_sync_is_remote(int nodenum, const int *nodes, int nnodes)
{
	int found = 0;

	/* Underlying NoC node SHOULD NOT be here. */
	if (nodenum == nodes[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
	{
		if (nodenum == nodes[i])
			found++;
	}

	if (found != 1)
		return (0);

	return (1);
}

/*============================================================================*
 * unix64_sync_is_local()                                                     *
 *============================================================================*/

/**
 * @brief Sync local point validation.
 *
 * @param nodenum Logic ID of local node.
 * @param nodes   IDs of target NoC nodes.
 * @param nnodes  Number of target NoC nodes.
 *
 * @return Non zero if local point is valid and zero otherwise.
 */
PRIVATE int unix64_sync_is_local(int nodenum, const int *nodes, int nnodes)
{
	/* Underlying NoC node SHOULD be here. */
	if (nodenum != nodes[0])
		return (0);

	/* Underlying NoC node SHOULD be here. */
	for (int i = 1; i < nnodes; i++)
	{
		if (nodenum == nodes[i])
			return (0);
	}

	return (1);
}

/*============================================================================*
 * unix64_sync_create()                                                       *
 *============================================================================*/

/**
 * @see unix64_sync_create().
 */
PRIVATE int do_unix64_sync_create(const int *nodes, int nnodes, int type)
{
	int fd;         /* NoC connector.         */
	int nodenum;    /* NoC node number.       */
	int syncid;     /* Synchronization point. */
	char *pathname; /* NoC connector name.    */

	nodenum = linux64_processor_node_get_id();

	unix64_sync_lock();

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.rx)) < 0)
		{
			unix64_sync_unlock();
			return (-EAGAIN);
		}

		pathname = synctab.rxs[syncid].pathname;

		/* Broadcast. */
		if (type == UNIX64_SYNC_ONE_TO_ALL)
		{
			if (!unix64_sync_is_remote(nodenum, nodes, nnodes))
			{
				resource_free(&pool.rx, syncid);
				unix64_sync_unlock();
				return (-ECONNREFUSED);
			}

			/* Build pathname for NoC connector. */
			sprintf(pathname,
				"/sync-%d-broadcast",
				nodes[0]
			);
		}

		else
		{
			if (!unix64_sync_is_local(nodenum, nodes, nnodes))
			{
				resource_free(&pool.rx, syncid);
				unix64_sync_unlock();
				return (-ECONNREFUSED);
			}

			/* Build pathname for NoC connector. */
			sprintf(pathname,
				"/sync-%d-gather",
				nodes[0]
			);
		}

		/* Open NoC connector. */
		if ((fd = mq_open(pathname, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &mq_attr)) == -1)
		{
			resource_free(&pool.rx, syncid);
			unix64_sync_unlock();
			return (-EAGAIN);
		}

		/* Initialize synchronization point. */
		synctab.rxs[syncid].fd = fd;
		synctab.rxs[syncid].ncount = nnodes;
		kmemcpy(synctab.rxs[syncid].nodes, nodes, nnodes*sizeof(int));
		synctab.rxs[syncid].type = type;
		resource_set_rdonly(&synctab.rxs[syncid].resource);
		resource_set_notbusy(&synctab.rxs[syncid].resource);

	unix64_sync_unlock();

	return (syncid);
}

/**
 * @todo TODO: provide a long description for this function.
 *
 * @note This function is blocking.
 * @note This function is thread-safe.
 * @note This function is reentrant.
 */
PUBLIC int unix64_sync_create(const int *nodes, int nnodes, int type)
{
	/*  Invalid nodes list. */
	if (nodes == NULL)
		return (-EINVAL);

	/* Bad nodes list. */
	if (!WITHIN(nnodes, 2, LINUX64_PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Bad sync type. */
	if ((type != UNIX64_SYNC_ONE_TO_ALL) && (type != UNIX64_SYNC_ALL_TO_ONE))
		return (-EINVAL);

	return (do_unix64_sync_create(nodes, nnodes, type));
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
PRIVATE int do_unix64_sync_open(const int *nodes, int nnodes, int type)
{
	int fd;         /* NoC connector.         */
	int nodenum;    /* NoC node number.       */
	int syncid;     /* Synchronization point. */
	char *pathname; /* NoC connector name.    */

	nodenum = linux64_processor_node_get_id();

	unix64_sync_lock();

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.tx)) < 0)
			goto error0;

		pathname = synctab.txs[syncid].pathname;

		/* Broadcast. */
		if (type == UNIX64_SYNC_ONE_TO_ALL)
		{
			if (!unix64_sync_is_local(nodenum, nodes, nnodes))
			{
				resource_free(&pool.tx, syncid);
				unix64_sync_unlock();
				return (-ECONNREFUSED);
			}

			/* Build pathname for NoC connector. */
			sprintf(pathname,
				"/sync-%d-broadcast",
				nodes[0]
			);
		}

		else
		{
			if (!unix64_sync_is_remote(nodenum, nodes, nnodes))
			{
				resource_free(&pool.tx, syncid);
				unix64_sync_unlock();
				return (-ECONNREFUSED);
			}

			/* Build pathname for NoC connector. */
			sprintf(pathname,
				"/sync-%d-gather",
				nodes[0]
			);
		}

		/* Open NoC connector. */
		if ((fd = mq_open(pathname, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, &mq_attr)) == -1)
			goto error1;

		/* Initialize synchronization point. */
		synctab.txs[syncid].fd = fd;
		synctab.txs[syncid].ncount = nnodes;
		kmemcpy(synctab.txs[syncid].nodes, nodes, nnodes*sizeof(int));
		synctab.txs[syncid].type = type;
		resource_set_wronly(&synctab.txs[syncid].resource);
		resource_set_notbusy(&synctab.txs[syncid].resource);

	unix64_sync_unlock();

	return (syncid);

error1:
	resource_free(&pool.tx, syncid);
error0:
	unix64_sync_unlock();
	return (-EAGAIN);
}

/**
 * @see do_unix64_sync_open().
 */
PUBLIC int unix64_sync_open(const int *nodes, int nnodes, int type)
{
	/*  Invalid nodes list. */
	if (nodes == NULL)
		return (-EINVAL);

	/* Bad nodes list. */
	if (!WITHIN(nnodes, 2, LINUX64_PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Bad sync type. */
	if ((type != UNIX64_SYNC_ONE_TO_ALL) && (type != UNIX64_SYNC_ALL_TO_ONE))
		return (-EINVAL);

	return (do_unix64_sync_open(nodes, nnodes, type));
}

/*============================================================================*
 * unix64_sync_wait()                                                         *
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
PRIVATE int do_unix64_sync_wait(int syncid, int *sig)
{
	if (mq_receive(synctab.rxs[syncid].fd, (char *)sig, sizeof(int), NULL) == -1)
		return (-EAGAIN);

	return (0);
}

/**
 * @brief Waits for a broadcast signal.
 *
 * @param syncid ID of the target sync.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE inline int do_unix64_sync_wait_broadcast(int syncid)
{
	int sig;
	int ret = 0;

	if ((ret = do_unix64_sync_wait(syncid, &sig)) == 0)
	{
		if (sig != synctab.rxs[syncid].nodes[0])
			ret = -EAGAIN;
	}

	return (ret);
}


/**
 * @brief Gathers broadcast signals.
 *
 * @param syncid ID of the target sync.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE int do_unix64_sync_wait_gather(int syncid)
{
	int nsignals = synctab.rxs[syncid].ncount - 1;
	int signals[LINUX64_PROCESSOR_NOC_NODES_NUM];

	kmemset(signals, 0, nsignals*sizeof(int));

	/* Wait. */
	do
	{
		int err;
		int sig;

		if ((err = do_unix64_sync_wait(syncid, &sig)) < 0)
			return (err);

		/* Check if node is in the list. */
		err = -EINVAL;
		for (int i = 1; i < synctab.rxs[syncid].ncount; i++)
		{
			/* Not this node. */
			if (synctab.rxs[syncid].nodes[i] != sig)
				continue;

			if (signals[i - 1] == 0)
			{
				signals[i - 1] = 1;
				nsignals--;
				err = 0;
				break;
			}
		}

		KASSERT(err == 0);
	} while (nsignals > 0);

	return (0);
}

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

	/* Invalid sync. */
	if (!unix64_sync_rx_is_valid(syncid))
		goto error0;

again:

	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
			goto error1;

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		/* Set sync as busy. */
		resource_set_busy(&synctab.rxs[syncid].resource);

	/*
	 * Release lock, since we may sleep below.
	 */
	unix64_sync_unlock();

	/* Broadcast. */
	ret = (synctab.rxs[syncid].type == UNIX64_SYNC_ONE_TO_ALL) ?
		do_unix64_sync_wait_broadcast(syncid) :
		do_unix64_sync_wait_gather(syncid);

	unix64_sync_lock();
		resource_set_notbusy(&synctab.rxs[syncid].resource);
	unix64_sync_unlock();

	return ((ret == -1) ? -EAGAIN : 0);

error1:
	unix64_sync_unlock();
error0:
	return (-EAGAIN);
}

/*============================================================================*
 * unix64_sync_signal()                                                       *
 *============================================================================*/

/**
 * @brief Sends a signal.
 *
 * @param syncid ID of the target sync.
 * @param sig    Signal.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE inline int do_unix64_sync_signal(int syncid, int sig)
{
	return (mq_send(synctab.txs[syncid].fd, (char *) &sig, sizeof(int), 1));
}

/**
 * @brief Broadcasts a signal.
 *
 * @param syncid ID of the target sync.
 * @param sig    Signal.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE inline int do_unix64_sync_signal_broadcast(int syncid, int sig)
{
	int ret = 0;

	for (int i = 1; i < synctab.txs[syncid].ncount; i++)
	{
		if ((ret = do_unix64_sync_signal(syncid, sig)) == -1)
			break;
	}

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
	int ret;
	int nodeid;

	/* Invalid sync. */
	if (!unix64_sync_tx_is_valid(syncid))
		goto error0;

again:

	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
			goto error1;

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

	nodeid = linux64_processor_node_get_id();

	/* Broadcast. */
	ret = (synctab.txs[syncid].type == UNIX64_SYNC_ONE_TO_ALL) ?
		do_unix64_sync_signal_broadcast(syncid, nodeid) :
		do_unix64_sync_signal(syncid, nodeid);

	unix64_sync_lock();
		resource_set_notbusy(&synctab.txs[syncid].resource);
	unix64_sync_unlock();

	return ((ret == -1) ? -EAGAIN : 0);

error1:
	unix64_sync_unlock();
error0:
	return (-EAGAIN);
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
	/* Invalid sync. */
	if (!unix64_sync_tx_is_valid(syncid))
		goto error0;

again:

	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
			goto error1;

		/* Bad sync. */
		if (!resource_is_writable(&synctab.txs[syncid].resource))
			goto error1;

		/* Busy sync. */
		if (resource_is_busy(&synctab.txs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		if (mq_close(synctab.txs[syncid].fd) < 0)
			goto error1;
		mq_unlink(synctab.txs[syncid].pathname);

		resource_free(&pool.tx, syncid);

	unix64_sync_unlock();

	return (0);

error1:
	unix64_sync_unlock();
error0:
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
	/* Invalid sync. */
	if (!unix64_sync_rx_is_valid(syncid))
		goto error0;

again:

	unix64_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
			goto error1;

		/* Bad sync. */
		if (resource_is_writable(&synctab.rxs[syncid].resource))
			goto error1;

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			unix64_sync_unlock();
			goto again;
		}

		/* Destroy underlying message queue. */
		if (mq_close(synctab.rxs[syncid].fd) < 0)
			goto error1;
		mq_unlink(synctab.rxs[syncid].pathname);

		resource_free(&pool.rx, syncid);

	unix64_sync_unlock();

	return (0);

error1:
	unix64_sync_unlock();
error0:
	return (-EAGAIN);
}
