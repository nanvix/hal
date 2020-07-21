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

/* Must come fist. */
#define __NEED_HAL_TARGET
#define __NEED_RESOURCE

#include <nanvix/hal/target.h>
#include <nanvix/hal/resource.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

#if !__NANVIX_IKC_USES_ONLY_MAILBOX

/*============================================================================*
 * Definitions                                                                *
 *============================================================================*/

/**
 * @name Gets syncid.
 */
/**@{*/
#define RESOURCEID_RX(interface, tag) ((interface * BOSTAN_SYNC_CREATE_PER_DMA) + (tag - BOSTAN_SYNC_RX_OFF)) /**< synctab.rxs' index. */
#define RESOURCEID_TX(interface)      (interface * BOSTAN_SYNC_OPEN_PER_DMA)                                  /**< synctab.txs' index. */
/**@}*/

/**
 * @name Gets underlying resource IDs.
 */
/**@{*/
#define UNDERLYING_RX_TAG(syncid)       ((syncid % BOSTAN_SYNC_CREATE_PER_DMA) + BOSTAN_SYNC_RX_OFF) /**< Receiver tag ID.         */
#define UNDERLYING_RX_INTERFACE(syncid) (syncid / BOSTAN_SYNC_CREATE_PER_DMA)                        /**< Receiver DMA channel ID. */
#define UNDERLYING_TX_TAG               (BOSTAN_SYNC_CNOC_TX_BASE)                                   /**< Transfer tag ID.         */
#define UNDERLYING_TX_INTERFACE(syncid) (syncid)                                                     /**< Transfer DMA channel ID. */
/**@}*/

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

struct hash
{
	uint8_t type;       /**< Sync type.                   (1 bit used)   */
	uint8_t source;     /**< Source node of the hash.     (5 bits used)  */
	uint8_t master;     /**< Master node of the sync.     (5 bits used)  */
	uint8_t unused;     /**< Unused.                                     */ 
	uint32_t nodeslist; /**< Bit-field of the nodes list. (24 bits used) */
} ALIGN(8);

#define HASH_INITIALIZER ((struct hash){0x0, 0xF, 0xF, 0x0, 0x0})
#define HASH_ACCESS_VALUE(_hash) (*((uint64_t *) (_hash)))

/**
 * @brief Synchronization Points Table
 */
PRIVATE int sync_ack[PROCESSOR_NOC_NODES_NUM] = {
	[0 ... (PROCESSOR_NOC_NODES_NUM - 1)] = 1
};

/*----------------------------------------------------------------------------*
 * Semaphore                                                                  *
 *----------------------------------------------------------------------------*/

/**
 * @brief Semaphore
 */
struct semaphore
{
	spinlock_t lock; /**< Semaphore lock.    */
	int count;       /**< Semaphore counter. */
};

#define SEMAPHORE_INITIALIZER ((struct semaphore){SPINLOCK_UNLOCKED, 0})

/**
 * @brief Initializes the semaphore with the given value.
 *
 * @param s Semaphore to be initialized.
 * @param c Semaphore counter.
 */
PRIVATE void semaphore_init(struct semaphore * s, int c)
{
	spinlock_init(&s->lock);
	s->count = c;
}

/**
 * @brief Tries to decrement the semaphore counter:
 * if greater than zero, decrements and returns,
 * otherwise, does a busy-waiting until the counter
 * becomes greater than zero.
 *
 * @param s Semaphore to decremented.
 */
PRIVATE void semaphore_down(struct semaphore * s)
{
	while (true)
	{
		spinlock_lock(&s->lock);
			if (s->count > 0)
				break;
		spinlock_unlock(&s->lock);
	}
	s->count--;
	spinlock_unlock(&s->lock);
}

/**
 * @brief Increments the semaphore value.
 *
 * @param s Semaphore to be incremented.
 */
PRIVATE void semaphore_up(struct semaphore * s)
{
	spinlock_lock(&s->lock);
		s->count++;
	spinlock_unlock(&s->lock);
}

/**
 * @brief Synchronization Points Table
 */
PRIVATE struct sync
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

		struct hash hash;                       /**< Hash value.                   */
		struct hash barrier;                    /**< Barrier value.                */
		int nreceived[PROCESSOR_NOC_NODES_NUM]; /**< Number of signals received.   */

		struct semaphore sem;                   /**< Wait condition.               */
	} ALIGN(sizeof(dword_t)) rxs[MPPA256_SYNC_CREATE_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		/*
		 * XXX: Don't Touch! This Must Come First!
		 */
		struct resource resource;             /**< Generic resource information.                 */

		int nnodes;                           /**< Number of nodes.                              */
		int nodes[PROCESSOR_NOC_NODES_NUM];   /**< Targets Logic IDs.                            */
		char sended[PROCESSOR_NOC_NODES_NUM]; /**< Indicates when a target has already notified. */
		struct hash hash;                     /**< Hash value.                                   */
	} ALIGN(sizeof(dword_t)) txs[MPPA256_SYNC_OPEN_MAX];
} synctab;
	
/**
 * @brief Pools of Synchronization Resource
 */
PRIVATE struct pool
{
	const struct resource_pool rx;
	const struct resource_pool tx;
} ALIGN(sizeof(dword_t)) pool = {
	.rx = {synctab.rxs, MPPA256_SYNC_CREATE_MAX, sizeof(struct rx)},
	.tx = {synctab.txs, MPPA256_SYNC_OPEN_MAX,   sizeof(struct tx)},
};

/**
 * @brief Global lock
 */
PRIVATE spinlock_t synctab_lock = K1B_SPINLOCK_UNLOCKED;

/*============================================================================*
 * mppa256_sync_lock()                                                        *
 *============================================================================*/

PRIVATE void mppa256_sync_lock(void)
{
	spinlock_lock(&synctab_lock);
}

/*============================================================================*
 * mppa256_sync_unlock()                                                      *
 *============================================================================*/


PRIVATE void mppa256_sync_unlock(void)
{
	spinlock_unlock(&synctab_lock);
}

/*============================================================================*
 * mppa256_sync_build_nodeslist()                                              *
 *============================================================================*/

PRIVATE uint32_t mppa256_sync_build_nodeslist(const int * nodes, int nnodes)
{
	uint32_t nodeslist = 0U;

	for (int j = 0; j < nnodes; j++)
		nodeslist |= (1U << nodes[j]);

	return (nodeslist);
}

/*============================================================================*
 * do_mppa256_sync_search_rx()                                                 *
 *============================================================================*/

PRIVATE int do_mppa256_sync_search_rx(struct hash * hash)
{
	for (unsigned i = 0; i < MPPA256_SYNC_CREATE_MAX; ++i)
	{
		if (!resource_is_used(&synctab.rxs[i].resource))
			continue;

		KASSERT(resource_is_readable(&synctab.rxs[i].resource));

		if (synctab.rxs[i].hash.type != hash->type)
			continue;

		if (synctab.rxs[i].hash.nodeslist != hash->nodeslist)
			continue;

		return (i);
	}

	return (-EINVAL);
}

/*============================================================================*
 * do_mppa256_sync_search_tx()                                                 *
 *============================================================================*/

PRIVATE int do_mppa256_sync_search_tx(struct hash * hash)
{
	for (unsigned i = 0; i < MPPA256_SYNC_OPEN_MAX; ++i)
	{
		if (!resource_is_used(&synctab.txs[i].resource))
			continue;

		KASSERT(resource_is_writable(&synctab.txs[i].resource));

		if (synctab.txs[i].hash.type != hash->type)
			continue;

		if (synctab.txs[i].hash.nodeslist != hash->nodeslist)
			continue;

		return (i);
	}

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_sync_create()                                                      *
 *============================================================================*/

/**
 * @brief Allocates and configures the receiving side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 * @param type   Type of synchronization point.
 *
 * @return The tag of underlying resource ID.
 */
PUBLIC int mppa256_sync_create(const int * nodes, int nnodes, int type)
{
	int syncid;       /* Synchronization point. */
	struct hash hash; /* Synch point Hash.      */

	mppa256_sync_lock();

		hash           = HASH_INITIALIZER;
		hash.source    = processor_node_get_num();
		hash.type      = type;
		hash.master    = nodes[0];
		hash.nodeslist = mppa256_sync_build_nodeslist(nodes, nnodes);

		/* Searchs existing syncid. */
		if (do_mppa256_sync_search_rx(&hash) >= 0)
			goto error;

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.rx)) < 0)
			goto error;

		/* Initialize synchronization point. */
		synctab.rxs[syncid].hash      = hash;
		synctab.rxs[syncid].barrier   = HASH_INITIALIZER;

		for (unsigned i = 0; i < PROCESSOR_NOC_NODES_NUM; ++i)
			synctab.rxs[syncid].nreceived[i] = 0;

		semaphore_init(&synctab.rxs[syncid].sem, 0);

		resource_set_rdonly(&synctab.rxs[syncid].resource);
		resource_set_notbusy(&synctab.rxs[syncid].resource);

	mppa256_sync_unlock();

	return (syncid + MPPA256_SYNC_CREATE_OFFSET);

error:
	mppa256_sync_unlock();
	return (-EAGAIN);
}

/*============================================================================*
 * mppa256_sync_open()                                                        *
 *============================================================================*/

/**
 * @brief Allocates and configures the sending side of the synchronization point.
 *
 * @param noes   IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes.
 * @param type   Type of synchronization point.
 *
 * @return The tag of underlying resource ID.
 */
PUBLIC int mppa256_sync_open(const int * nodes, int nnodes, int type)
{
	int syncid;       /* Synchronization point. */
	struct hash hash; /* Synch point hash.      */

	mppa256_sync_lock();

		hash           = HASH_INITIALIZER;
		hash.source    = processor_node_get_num();
		hash.type      = type;
		hash.master    = nodes[0];
		hash.nodeslist = mppa256_sync_build_nodeslist(nodes, nnodes);

		/* Searchs existing syncid. */
		if (do_mppa256_sync_search_tx(&hash) >= 0)
			goto error;

		/* Allocate a synchronization point. */
		if ((syncid = resource_alloc(&pool.tx)) < 0)
			goto error;

		/* Initialize synchronization point. */
		synctab.txs[syncid].hash   = hash;
		synctab.txs[syncid].nnodes = nnodes;

		for (int i = 0; i < nnodes; i++)
		{
			synctab.txs[syncid].nodes[i]  = nodes[i];
			synctab.txs[syncid].sended[i] = 0;
		}

		resource_set_wronly(&synctab.txs[syncid].resource);
		resource_set_notbusy(&synctab.txs[syncid].resource);

	mppa256_sync_unlock();

	return (syncid + MPPA256_SYNC_OPEN_OFFSET);

error:
	mppa256_sync_unlock();
	return (-EAGAIN);
}

/*============================================================================*
 * mppa256_sync_unlink()                                                      *
 *============================================================================*/

/**
 * @brief Releases and cleans receiver buffer.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int mppa256_sync_unlink(int syncid)
{
	syncid -= MPPA256_SYNC_CREATE_OFFSET;

again:
	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
			goto error;

		/* Bad sync. */
		if (resource_is_writable(&synctab.rxs[syncid].resource))
			goto error;

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			mppa256_sync_unlock();
			goto again;
		}

		synctab.rxs[syncid].hash    = HASH_INITIALIZER;
		synctab.rxs[syncid].barrier = HASH_INITIALIZER;

		resource_free(&pool.rx, syncid);

	mppa256_sync_unlock();

	return (0);

error:
	mppa256_sync_unlock();
	return (-EBADF);
}

/*============================================================================*
 * mppa256_sync_close()                                                       *
 *============================================================================*/

/**
 * @brief Free the sender resources on a specific DMA channel.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int mppa256_sync_close(int syncid)
{
	syncid -= MPPA256_SYNC_OPEN_OFFSET;

again:
	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
			goto error;

		/* Bad sync. */
		if (!resource_is_writable(&synctab.txs[syncid].resource))
			goto error;

		/* Busy sync. */
		if (resource_is_busy(&synctab.txs[syncid].resource))
		{
			mppa256_sync_unlock();
			goto again;
		}

		/* Initialize synchronization point. */
		synctab.txs[syncid].hash   = HASH_INITIALIZER;
		synctab.txs[syncid].nnodes = 0;

		resource_free(&pool.tx, syncid);

	mppa256_sync_unlock();

	return (0);

error:
	mppa256_sync_unlock();
	return (-EBADF);
}

/*============================================================================*
 * mppa256_sync_wait()                                                        *
 *============================================================================*/

/**
 * @brief Wait signal on a specific synchronization point.
 *
 * @param syncid ID of the synchronization point.
 *
 * @return Zero if wait signal correctly and non zero otherwise.
 *
 * @todo How make the slave wait on lock without the need to
 * access the master's private structures? (microkernel approach)
 */
PUBLIC int mppa256_sync_wait(int syncid)
{
	syncid -= MPPA256_SYNC_CREATE_OFFSET;

again:
	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.rxs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

		/* Busy sync. */
		if (resource_is_busy(&synctab.rxs[syncid].resource))
		{
			mppa256_sync_unlock();
			goto again;
		}

		/* Set sync as busy. */
		resource_set_busy(&synctab.rxs[syncid].resource);

	/*
	 * Release lock, since we may sleep below.
	 */
	mppa256_sync_unlock();

	/* Waits for the handler release the lock. */
	semaphore_down(&synctab.rxs[syncid].sem);

	mppa256_sync_lock();
		resource_set_notbusy(&synctab.rxs[syncid].resource);
	mppa256_sync_unlock();

	return (0);
}

/*============================================================================*
 * do_mppa256_sync_signal()                                                   *
 *============================================================================*/

/**
 * @brief Broadcasts a signal.
 *
 * @param i     Initial node ID.
 * @param n     Number of nodes.
 * @param nodes Node IDs.
 * @param hash  Message.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PRIVATE inline int do_mppa256_sync_signal(int i, int n, const int * nodes, char * sended, const struct hash * hash)
{
	int ret;      /* Return value.     */
	int local;    /* Local ID.         */
	int emitted;  /* Signals emited.   */
	int expected; /* Expected signals. */

	ret      = 0;
	emitted  = 0;
	expected = (i == 0) ? (1) : (n - 1);
	local    = processor_node_get_num();

	/* Only sends when has all acks. */
	for (int j = i; j < n; j++)
	{
		/* Already sended. */
		if (sended[j]) 
			continue;

		/* Not sended and not allowed. */
		if (!sync_ack[nodes[j]])
			return (-EAGAIN);
	}

	/* Sends signals. */
	for (; i < n; i++)
	{
		/* Already sended. */
		if (sended[i])
		{
			emitted++;
			continue;
		}

		/* Sends signal. */
		ret = bostan_dma_control_signal(
			0,
			UNDERLYING_TX_TAG,
			&nodes[i],
			1,
			bostan_processor_node_sync_tag(local, 0),
			HASH_ACCESS_VALUE(hash)
		);

		if (ret != 0)
			break;

		emitted++;

		/* Sets current node busy. */
		sync_ack[nodes[i]] = 0;

		/* Sets current target notified. */
		sended[i] = 1;
	}

	return (emitted == expected) ? (0) : (-EAGAIN);
}

/*============================================================================*
 * mppa256_sync_signal()                                                      *
 *============================================================================*/

/**
 * @brief Send signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 *
 * @return Zero if send signal correctly and non zero otherwise.
 */
PUBLIC int mppa256_sync_signal(int syncid)
{
	int ret; /* Return value. */

	syncid -= MPPA256_SYNC_OPEN_OFFSET;

again:
	mppa256_sync_lock();

		/* Bad sync. */
		if (!resource_is_used(&synctab.txs[syncid].resource))
		{
			mppa256_sync_unlock();
			return (-EBADF);
		}

		/* Busy sync. */
		if (resource_is_busy(&synctab.txs[syncid].resource))
		{
			mppa256_sync_unlock();
			goto again;
		}

		/* Set sync as busy. */
		resource_set_busy(&synctab.txs[syncid].resource);

	/*
	 * Release lock, since we may sleep below.
	 */
	mppa256_sync_unlock();

	bostan_noc_it_mask();

		/* Broadcast. */
		if (synctab.txs[syncid].hash.type == MPPA256_SYNC_ONE_TO_ALL)
		{
			ret = do_mppa256_sync_signal(
				1,
				synctab.txs[syncid].nnodes,
				synctab.txs[syncid].nodes,
				synctab.txs[syncid].sended,
				&synctab.txs[syncid].hash
			);
		}

		/* Gather. */
		else
		{
			ret = do_mppa256_sync_signal(
				0,
				1,
				synctab.txs[syncid].nodes,
				synctab.txs[syncid].sended,
				&synctab.txs[syncid].hash
			);
		}

	bostan_noc_it_unmask();

	/* Losing interrupts? */
	bostan_noc_it_verify();

	mppa256_sync_lock();
		resource_set_notbusy(&synctab.txs[syncid].resource);

		/* Is it completed? */
		if (ret == 0)
		{
			for (int i = 0; i < synctab.txs[syncid].nnodes; i++)
				synctab.txs[syncid].sended[i] = 0;
		}
	mppa256_sync_unlock();

	return (ret);
}

/*============================================================================*
 * mppa256_sync_hash_handler()                                                  *
 *============================================================================*/

PRIVATE inline int mppa256_sync_send_ack(int interface, int tag, int target)
{
	int local; /* Local ID. */

	UNUSED(tag);

	local = processor_node_get_num();

	/* Sends ack signal. */
	return (
		bostan_dma_control_signal(
			interface,
			UNDERLYING_TX_TAG,
			&target,
			1,
			bostan_processor_node_sync_tag(local, 1),
			~(0ULL)
		)
	);
}

/*============================================================================*
 * mppa256_sync_barrier_is_complete()                                          *
 *============================================================================*/

PRIVATE int mppa256_sync_barrier_is_complete(struct rx * rx)
{
	uint32_t received;
	uint32_t expected;

	received = rx->barrier.nodeslist;

	/* Does master notifies it? */
	if (rx->hash.type == MPPA256_SYNC_ONE_TO_ALL)
		expected = (rx->hash.nodeslist & (1U << rx->hash.master));

	/* Does slaves notifies it? */
	else
		expected = (rx->hash.nodeslist & ~(1U << rx->hash.master));

	return (received == expected);
}

/*============================================================================*
 * mppa256_sync_barrier_reset()                                               *
 *============================================================================*/

PRIVATE void mppa256_sync_barrier_reset(struct rx * rx)
{
	for (unsigned i = 0; i < PROCESSOR_NOC_NODES_NUM; ++i)
	{
		if (rx->barrier.nodeslist & (1U << i))
		{
			/**
			 * Consume a signals and reset barrier if there are no
			 * signals from that node.
			 **/
			rx->nreceived[i]--; 

			if (rx->nreceived[i] == 0)
				rx->barrier.nodeslist &= ~(1U << i);

			KASSERT(rx->nreceived[i] >= 0);
		}
	}
}

/*============================================================================*
 * do_mppa256_sync_hash_panic()                                            *
 *============================================================================*/

PRIVATE inline void do_mppa256_sync_hash_panic(char * message, struct hash * hash)
{
	int type      = hash->type;
	int source    = hash->source;
	int master    = hash->master;
	int unused    = hash->unused;
	int nodeslist = hash->nodeslist;

	kpanic("[sync] %s (type:%d, source:%d, master:%d, unused:%d, nodeslist:%d)",
		message,
		type,
		source,
		master,
		unused,
		nodeslist
	);
}

/*============================================================================*
 * mppa256_sync_hash_handler()                                                *
 *============================================================================*/

/**
 * @brief Sync Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_sync_hash_handler(int interface, int tag)
{
	int ret;          /* Return valud. */
	int syncid;       /* Sync ID.      */
	struct hash hash; /* Signal value. */

	HASH_ACCESS_VALUE(&hash) = bostan_dma_control_read(interface, tag);

	if (!node_is_valid(hash.source))
		do_mppa256_sync_hash_panic("Invalid source.", &hash);

	if ((syncid = do_mppa256_sync_search_rx(&hash)) < 0)
		do_mppa256_sync_hash_panic("Sync point not found.", &hash);

	synctab.rxs[syncid].barrier.nodeslist |= (1U << hash.source);
	synctab.rxs[syncid].nreceived[hash.source]++;

	if (mppa256_sync_barrier_is_complete(&synctab.rxs[syncid]))
	{
		mppa256_sync_barrier_reset(&synctab.rxs[syncid]);
		semaphore_up(&synctab.rxs[syncid].sem);
	}

	/* Reconfigure sync point. */
	ret = bostan_dma_control_config(
		interface,
		tag,
		BOSTAN_CNOC_MAILBOX_MODE,
		0ULL,
		mppa256_sync_hash_handler
	);

	if (ret != 0)
		kpanic("[hal][sync][handler][hash] Reconfiguration of the sync failed!");

	if (mppa256_sync_send_ack(interface, tag, hash.source) < 0)
		kpanic("[hal][sync][handler][hash] Ack response failed (deadlock)!");
}

/*============================================================================*
 * mppa256_sync_ack_handler()                                                 *
 *============================================================================*/

/**
 * @brief Sync Receiver Handler.
 *
 * @param interface Number of the receiver interface.
 * @param tag       Number of the underlying recevier resource.
 */
PRIVATE void mppa256_sync_ack_handler(int interface, int tag)
{
	int ret;    /* Return value. */
	int source; /* Source ID.    */

	source = (tag - bostan_processor_node_sync_tag(0, 1));

	/* Reconfigure sync point. */
	ret = bostan_dma_control_config(
		interface,
		tag,
		BOSTAN_CNOC_BARRIER_MODE,
		0ULL,
		mppa256_sync_ack_handler
	);

	if (ret != 0)
		kpanic("[hal][sync][handler][ack] Reconfiguration of the sync failed!");

	sync_ack[source] = 1;

	dcache_invalidate();
}

/*============================================================================*
 * mppa256_sync_setup()                                                       *
 *============================================================================*/

/**
 * @see mppa256_sync_setup().
 */
PUBLIC void mppa256_sync_setup(void)
{
	unsigned local;

	kprintf("[hal][sync] Sync Initialization.");

	local = processor_node_get_num();

	for (unsigned i = 0; i < MPPA256_SYNC_CREATE_MAX; ++i)
	{
		synctab.rxs[i].resource  = RESOURCE_INITIALIZER;
		synctab.rxs[i].hash      = HASH_INITIALIZER;
		synctab.rxs[i].barrier   = HASH_INITIALIZER;
		semaphore_init(&synctab.rxs[i].sem, 0);

		for (unsigned j = 0; j < PROCESSOR_NOC_NODES_NUM; ++j)
			synctab.rxs[i].nreceived[j] = 0;
	}

	for (unsigned i = 0; i < MPPA256_SYNC_OPEN_MAX; ++i)
	{
		synctab.txs[i].resource  = RESOURCE_INITIALIZER;
		synctab.txs[i].nnodes    = 0;
		synctab.txs[i].hash      = HASH_INITIALIZER;

		for (unsigned j = 0; j < PROCESSOR_NOC_NODES_NUM; ++j)
			synctab.txs[i].sended[j] = 0;
	}

	KASSERT(bostan_dma_control_open(0, UNDERLYING_TX_TAG) >= 0);

	for (unsigned node = 0; node < PROCESSOR_NOC_NODES_NUM; ++node)
	{
		if (local == node)
			continue;

		/* Creates buffer to receive the sync hash. */
		KASSERT(
			bostan_dma_control_create(
				0,
				bostan_processor_node_sync_tag(node, 0),
				BOSTAN_CNOC_MAILBOX_MODE,
				0ULL,
				mppa256_sync_hash_handler
			) == 0
		);

		/* Creates buffer to receive the ack from a signal. */
		KASSERT(
			bostan_dma_control_create(
				0,
				bostan_processor_node_sync_tag(node, 1),
				BOSTAN_CNOC_BARRIER_MODE,
				0ULL,
				mppa256_sync_ack_handler
			) == 0
		);

		/* First signal is allowed. */
		sync_ack[node] = 1;
	}
}

/*============================================================================*
 * mppa256_sync_shutdown()                                                     *
 *============================================================================*/

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC void mppa256_sync_shutdown(void)
{
	unsigned int local;

	local = processor_node_get_num();

	for (unsigned int node = 0; node < PROCESSOR_NOC_NODES_NUM; ++node)
	{
		if (local == node)
			continue;

		/* Unlink buffer to receive the sync hash. */
		KASSERT(
			bostan_dma_control_unlink(
				0, bostan_processor_node_sync_tag(node, 0)
			) == 0
		);

		/* Unlink buffer to receive the ack from a signal. */
		KASSERT(
			bostan_dma_control_unlink(
				0, bostan_processor_node_sync_tag(node, 1)
			) == 0
		);
	}

	KASSERT(bostan_dma_control_close(0, UNDERLYING_TX_TAG) >= 0);
}

/*============================================================================*
 * IGNORE IT FOR THIS MOMENT                                                  *
 *============================================================================*/

#ifdef MPPA256_USING_MULTIPLE_DMA_INTERFACES

/*============================================================================*
 * mppa256_sync_select_rx_interface()                                         *
 *============================================================================*/

/**
 * @brief Associate a underliyng DMA RX channel.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of the synchronization.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_rx_interface(const int *nodenums, int nnodes, int type)
{
	int i;          /* Iterator value.         */
	int tag;        /* Underlying control tag. */
	int syncid;     /* Sync ID.                */
	int interface;  /* Underlying interface.   */
	int clusternum; /* Local node number.      */

	clusternum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	/* Searchs on local IDs? */
	if (type == MPPA256_SYNC_ALL_TO_ONE)
	{
		i = 0;
		nnodes = 1;
	}

	/* Searchs on Remote IDs? */
	else
		i = 1;

	for (; i < nnodes; i++)
	{
		if (WITHIN(nodenums[i], clusternum, clusternum + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM))
		{
			tag       = bostan_processor_node_sync_tag(nodenums[i], 0);
			interface = (nodenums[i] - clusternum);
			syncid    = RESOURCEID_RX(interface, tag);

			if (!resource_is_used(&synctab.rxs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

/*============================================================================*
 * mppa256_sync_select_tx_interface()                                         *
 *============================================================================*/

/**
 * @brief Associate a underliyng DMA TX channel.
 *
 * @param nodenums IDs of target NoC nodes.
 * @param nnodes   Number of target NoC nodes.
 * @param type     Type of the synchronization.
 *
 * @return Interface ID [0..(K1BIO_CORES_NUM-1)]
 */
PRIVATE int mppa256_sync_select_tx_interface(const int *nodenums, int nnodes, int type)
{
	int i;          /* Iterator value.         */
	int syncid;     /* Sync ID.                */
	int interface;  /* Underlying interface.   */
	int clusternum; /* Local node number.      */

	clusternum = bostan_processor_noc_cluster_to_node_num(cluster_get_num());

	/* Searchs on local IDs? */
	if (type == MPPA256_SYNC_ONE_TO_ALL)
	{
		i = 0;
		nnodes = 1;
	}

	/* Searchs on Remote IDs? */
	else
		i = 1;

	for (; i < nnodes; i++)
	{
		if (WITHIN(nodenums[i], clusternum, clusternum + BOSTAN_PROCESSOR_NOC_INTERFACES_NUM))
		{
			interface = (nodenums[i] - clusternum);
			syncid    = RESOURCEID_TX(interface);

			if (!resource_is_used(&synctab.txs[syncid].resource))
				return (interface);
		}
	}

	return (-EINVAL);
}

#endif /* MPPA256_USING_MULTIPLE_DMA_INTERFACES */

#endif /* !__NANVIX_IKC_USES_ONLY_MAILBOX */
