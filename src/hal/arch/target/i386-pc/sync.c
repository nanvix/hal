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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SK1BL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <arch/target/qemu/i386-pc/sync.h>
#include <nanvix/hal/resource.h>
#include <nanvix/klib.h>
#include <errno.h>

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
		struct resource resource; /**< Control flags. */
	} rxs[QEMU_I386_PC_NR_SYNC_RX_MAX];

	/**
	 * @brief Sender Synchronization Points
	 */
	struct tx
	{
		struct resource resource; /**< Control flags. */
	} txs[QEMU_I386_PC_NR_SYNC_TX_MAX];
} synctab;

/**
 * @brief Pools of Synchronization Resource
 */
PRIVATE struct syncpools
{
	const struct resource_pool rx_pool;
	const struct resource_pool tx_pool;
} syncpools = {
	.rx_pool = {synctab.rxs, QEMU_I386_PC_NR_SYNC_RX_MAX, sizeof(struct rx)},
	.tx_pool = {synctab.txs, QEMU_I386_PC_NR_SYNC_TX_MAX, sizeof(struct tx)},
};


/**
 * @brief Assess limits of the receiver resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int qemu_i386pc_sync_rx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, QEMU_I386_PC_NR_SYNC_RX_MAX);
}

/**
 * @brief Assess limits of the sender resource ID.
 *
 * @param syncid Resource ID.
 * @return No zero if ID is valid and zero otherwise.
 */
PRIVATE int qemu_i386pc_sync_tx_is_valid(int syncid)
{
	return WITHIN(syncid, 0, QEMU_I386_PC_NR_SYNC_TX_MAX);
}

/**
 * @brief Allocates and configures the receiving side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * @param type   Type of synchronization point.
 * 
 * @return The tag of underlying resource ID.
 */
PUBLIC int qemu_i386pc_sync_create(const int *nodes, int nnodes, int type)
{
	int resourceid;

	if (nodes == NULL || !WITHIN(nnodes, 2, I386_QEMU_NR_NOC_NODES))
		goto error0;

	if (type != QEMU_I386_PC_SYNC_ALL_TO_ONE && type != QEMU_I386_PC_SYNC_ONE_TO_ALL)
		goto error0;
	
		/* Allocate a synchronization point. */
	if ((resourceid = resource_alloc(&syncpools.rx_pool)) < 0)
		goto error0;

	resource_set_used(&synctab.rxs[resourceid].resource);
	resource_set_async(&synctab.rxs[resourceid].resource);
	resource_set_notbusy(&synctab.rxs[resourceid].resource);

	return resourceid;

error0:
	return (-EAGAIN);
}

/**
 * @brief Allocates and configures the sending side of the synchronization point.
 *
 * @param nodes  IDs of target NoC nodes.
 * @param nnodes Number of target NoC nodes. 
 * @param type   Type of synchronization point.
 * 
 * @return The tag of underlying resource ID.
 */
PUBLIC int qemu_i386pc_sync_open(const int *nodes, int nnodes, int type)
{
	int resourceid;

	if (nodes == NULL || !WITHIN(nnodes, 2, I386_QEMU_NR_NOC_NODES))
		goto error0;
	
	if (type != QEMU_I386_PC_SYNC_ALL_TO_ONE && type != QEMU_I386_PC_SYNC_ONE_TO_ALL)
		goto error0;

	/* Allocate a synchronization point. */
	if ((resourceid = resource_alloc(&syncpools.tx_pool)) < 0)
		goto error0;

	resource_set_used(&synctab.txs[resourceid].resource);
	resource_set_sync(&synctab.txs[resourceid].resource);
	resource_set_notbusy(&synctab.txs[resourceid].resource);

	return resourceid;

error0:
	return (-EAGAIN);
}

/**
 * @brief Free the receiver resources.
 *
 * @param  tag Resource ID.
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int qemu_i386pc_sync_unlink(int syncid)
{
	if (!qemu_i386pc_sync_rx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		goto error0;

	resource_free(&syncpools.rx_pool, syncid);

	return (0);

error0:
	return (-EAGAIN);
}

/**
 * @brief Free the sender resources on a specific DMA channel.
 *
 * @param syncid Resource ID.
 * 
 * @return Zero if free the resource and non zero otherwise.
 */
PUBLIC int qemu_i386pc_sync_close(int syncid)
{
	if (!qemu_i386pc_sync_tx_is_valid(syncid))
		goto error0;

	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		goto error0;

	resource_free(&syncpools.tx_pool, syncid);

	return (0);

error0:
	return (-EAGAIN);
}

/**
 * @brief Wait signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 * 
 * @return Zero if wait signal correctly and non zero otherwise.
 */
PUBLIC int qemu_i386pc_sync_wait(int syncid)
{
	if (!qemu_i386pc_sync_rx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.rxs[syncid].resource))
		goto error0;

	return (0);

error0:
	return (-EAGAIN);
}

/**
 * @brief Send signal on a specific synchronization point.
 *
 * @param syncid Resource ID.
 * 
 * @return Zero if send signal correctly and non zero otherwise.
 */
PUBLIC int qemu_i386pc_sync_signal(int syncid)
{
	if (!qemu_i386pc_sync_tx_is_valid(syncid))
		goto error0;
	
	/* Bad sync. */
	if (!resource_is_used(&synctab.txs[syncid].resource))
		goto error0;

	return (0);

error0:
	return (-EAGAIN);
}
