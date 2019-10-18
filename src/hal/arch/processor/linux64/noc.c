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

#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <posix/sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

/**
 * @brief Name for virtual NoC.
 */
#define UNIX64_NOC_NAME "nanvix-unix64-noc"

/**
 * @brief Name for virtual NoC lock.
 */
#define UNIX64_NOC_LOCK_NAME "nanvix-unix64-noc-lock"

/**
 * @brief NoC node.
 */
struct noc_node
{
	/**
	 * @brief Buffer.
	 */
	struct
	{
		int head; /* First element. */
		int tail; /* Last element.  */
	} buffer;
};

/**
 * @brief NoC.
 */
struct
{
	/**
	 * @brief Virtual NoC file descriptor.
	 */
	int shm;

	sem_t *lock;                               /* Lock          */
	struct noc_node *nodes;                    /* Nodes         */
	int configuration[PROCESSOR_CLUSTERS_NUM]; /* Configuration */
} noc = {
	.shm = -1,
	.lock = NULL,
	.nodes = NULL,

	/* TODO check if sums up number of nodes */
	.configuration = {
		/* IO clusters. */
		1, 1,

		/* Compute clusters. */
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1
	}
};

/**
 * @brief Map of core ids to nodenums.
 */
PRIVATE int linux64_corenums[CORES_NUM];

/*============================================================================*
 * linux64_processor_node_get_id()                                            *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PRIVATE int linux64_processor_node_get_id(void)
{
	return (linux64_cluster_get_num());
}

/*=======================================================================*
 * linux64_processor_noc_setup()                                         *
 *=======================================================================*/

/**
 * @brief Initializes the noc interface.
 */
PUBLIC void linux64_processor_noc_setup(void)
{
	for (int i = 0; i < CORES_NUM; ++i)
		linux64_corenums[i] = linux64_processor_node_get_id();
}

/*============================================================================*
 * linux64_processor_noc_lock()                                               *
 *============================================================================*/

/**
 * @brief Locks the virtual NoC.
 */
PRIVATE void linux64_processor_noc_lock(void)
{
	KASSERT(sem_wait(noc.lock) != -1);
}

/*============================================================================*
 * linux64_processor_noc_unlock()                                             *
 *============================================================================*/

/**
 * @brief Unlocks the virtual NoC.
 */
PRIVATE void linux64_processor_noc_unlock(void)
{
	KASSERT(sem_post(noc.lock) != -1);
}

/*============================================================================*
 * linux64_processor_noc_node_to_cluster_num()                                *
 *============================================================================*/

/**
 * @brief Converts a NoC node number to cluster number.
 *
 * @param nodenum Target node number.
 *
 * @returns The logical cluster number in which the node number @p
 * nodenum is located.
 */
PRIVATE int linux64_processor_noc_node_to_cluster_num(int nodenum)
{
	KASSERT((nodenum >= 0) && (nodenum <= PROCESSOR_NOC_NODES_NUM));

	/* Search for node number. */
	for (int i = 0, j = 0; i < PROCESSOR_CLUSTERS_NUM; i++)
	{
		/* Found. */
		if ((nodenum >= j) && (nodenum < (j + noc.configuration[i])))
			return (i);

		j += noc.configuration[i];
	}

	return (0);
}

/*============================================================================*
 * linux64_processor_node_get_num()                                           *
 *============================================================================*/

/**
 * @brief Gets the logic number of the target NoC node
 * attached with a core.
 *
 * @param coreid Attached core ID.
 *
 * @returns The logic number of the target NoC node attached
 * with the @p coreid.
 */
PUBLIC int linux64_processor_node_get_num(int coreid)
{
	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	return (linux64_corenums[coreid]);
}

/*============================================================================*
 * linux64_processor_node_get_num()                                           *
 *============================================================================*/

/**
 * @brief Exchange the logic number of the target NoC node
 * attached with a core.
 *
 * @param coreid  Attached core ID.
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns Zero if the target NoC node is successfully attached
 * to the requested @p coreid, and non zero otherwise.
 */
PUBLIC int linux64_processor_node_set_num(int coreid, int nodenum)
{
	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	/* Invalid nodenum. */
	if (cluster_get_num() != linux64_processor_noc_node_to_cluster_num(nodenum))
		return (-EINVAL);

	linux64_corenums[coreid] = nodenum;

	return (0);
}

/*============================================================================*
 * linux64_processor_noc_is_ionode()                                          *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC int linux64_processor_noc_is_ionode(int nodenum)
{
	int clusternum;

	clusternum = linux64_processor_noc_node_to_cluster_num(nodenum);

	return (linux64_cluster_is_io(clusternum));
}

/*============================================================================*
 * linux64_processor_noc_is_cnode()                                          *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC int linux64_processor_noc_is_cnode(int nodenum)
{
	int clusternum;

	clusternum = linux64_processor_noc_node_to_cluster_num(nodenum);

	return (linux64_cluster_is_compute(clusternum));
}

/*============================================================================*
 * linux64_processor_noc_boot()                                               *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC void linux64_processor_noc_boot(void)
{
	void *p;
	int nnodes;
	struct stat st;
	int initialize = 0;
	size_t nodes_sz = PROCESSOR_NOC_NODES_NUM*sizeof(struct noc_node);

	/* Check NoC configuration. */
	nnodes = 0;
	for (int i = 0 ; i < PROCESSOR_CLUSTERS_NUM; i++)
		nnodes += noc.configuration[i];
	KASSERT(nnodes == PROCESSOR_NOC_NODES_NUM);

	KASSERT((noc.lock =
		sem_open(UNIX64_NOC_LOCK_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR,
			1)
		) != NULL
	);

	/* Open virtual NoC. */
	KASSERT((noc.shm =
		shm_open(UNIX64_NOC_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR)
		) != -1
	);

	linux64_processor_noc_lock();

		/* Allocate virtual NoC. */
		KASSERT(fstat(noc.shm, &st) != -1);
		if (st.st_size == 0)
		{
			kprintf("[hal][processor] allocating virtual network-on-chip...");
			initialize = 1;
			KASSERT(ftruncate(noc.shm, nodes_sz) != -1);
		}

		KASSERT((p =
			mmap(NULL,
				nodes_sz,
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				noc.shm,
				0)
			) != NULL
		);
		noc.nodes = p;

		/* Initialize nodes. */
		if (initialize)
		{
			for (int i = 0; i < PROCESSOR_NOC_NODES_NUM; i++)
			{
				noc.nodes[i].buffer.head = 0;
				noc.nodes[i].buffer.tail = 0;
			}
		}

	linux64_processor_noc_unlock();
}

/*============================================================================*
 * linux64_processor_noc_shutdown()                                           *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC void linux64_processor_noc_shutdown(void)
{
	size_t nodes_sz = PROCESSOR_NOC_NODES_NUM*sizeof(struct noc_node);

	KASSERT(munmap(noc.nodes, nodes_sz) != -1);
	KASSERT(close(noc.shm) != -1);
	KASSERT(sem_close(noc.lock) != -1);
}
