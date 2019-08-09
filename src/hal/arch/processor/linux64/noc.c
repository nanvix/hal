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

#include <arch/processor/linux64/noc.h>
#include <arch/processor/linux64/clusters.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

/**
 * @brief Name for virtual NoC.
 */
#define UNIX64_NOC_NAME "unix64-noc"

/**
 * @brief Name for virtual NoC lock.
 */
#define UNIX64_NOC_LOCK_NAME "unix64-noc-lock"

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
	sem_t *lock;                                       /* Lock          */
	struct noc_node *nodes;                            /* Nodes         */
	int configuration[LINUX64_PROCESSOR_CLUSTERS_NUM]; /* Configuration */
} noc = {
	NULL, /* Lock  */
	NULL, /* Nodes */

	/* Configuration. */
	/* TODO check if sums up number of nodes */
	{
		/* IO clusters. */
		1, 1,

		/* Compute clusters. */
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1
	}
};

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
	KASSERT((nodenum >= 0) && (nodenum <= LINUX64_PROCESSOR_NOC_NODES_NUM));

	/* Search for node number. */
	for (int i = 0, j = 0; i < LINUX64_PROCESSOR_CLUSTERS_NUM; i++)
	{
		/* Found. */
		if ((nodenum >= j) < (nodenum < (j + noc.configuration[i])))
			return (i);

		j += noc.configuration[i];
	}

	return (0);
}

/*============================================================================*
 * linux64_processor_noc_attach()                                             *
 *============================================================================*/

/**
 * @brief Attaches calling process to virtual NoC device.
 */
PRIVATE void linux64_processor_noc_attach(void)
{
	int shm;
	void *p;
	size_t nodes_sz = LINUX64_PROCESSOR_NOC_NODES_NUM*sizeof(struct noc_node);

	kprintf("[noc] attaching process to virtual network-on-chip...");

	/* Open virtual NoC. */
	KASSERT((shm = shm_open(UNIX64_NOC_NAME, O_RDWR, S_IRUSR | S_IWUSR)) != -1);

	linux64_processor_noc_lock();

		/* Attach virtual NoC. */
		if (linux64_cluster_get_id() != LINUX64_PROCESSOR_CLUSTERID_MASTER)
		{
			KASSERT((p =
				mmap(NULL,
					nodes_sz,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					shm,
					0)
				) != NULL
			);
			noc.nodes = p;
		}

	linux64_processor_noc_unlock();
}

/*============================================================================*
 * linux64_processor_node_get_id()                                            *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC int linux64_processor_node_get_id(void)
{
	return (linux64_cluster_get_num());
}

/*============================================================================*
 * linux64_processor_node_get_num()                                            *
 *============================================================================*/

/**
 * @todo TODO: Provnume a detailed description to this function.
 */
PUBLIC int linux64_processor_node_get_num(int nodeid)
{
	KASSERT((nodeid >= 0) && (nodeid <= LINUX64_PROCESSOR_NOC_NODES_NUM));

	return (nodeid);
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
	int shm;
	void *p;
	size_t nodes_sz = LINUX64_PROCESSOR_NOC_NODES_NUM*sizeof(struct noc_node);

	KASSERT((noc.lock =
		sem_open(UNIX64_NOC_LOCK_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR,
			1)
		) != NULL
	);

	kprintf("[noc] creating virtual network-on-chip...");

	/* Open virtual NoC. */
	KASSERT((shm =
		shm_open(UNIX64_NOC_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR)
		) != -1
	);

	/* Allocate virtual NoC. */
	KASSERT(ftruncate(shm, nodes_sz) != -1);

	KASSERT((p =
		mmap(NULL,
			nodes_sz,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			shm,
			0)
		) != NULL
	);
	noc.nodes = p;

	/* Initialize nodes. */
	for (int i = 0; i < LINUX64_PROCESSOR_NOC_NODES_NUM; i++)
	{
		noc.nodes[i].buffer.head = 0;
		noc.nodes[i].buffer.tail = 0;
	}
}

/*============================================================================*
 * linux64_processor_noc_shutdown()                                           *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC void linux64_processor_noc_shutdown(void)
{
	size_t nodes_sz = LINUX64_PROCESSOR_NOC_NODES_NUM*sizeof(struct noc_node);

	KASSERT(munmap(noc.nodes, nodes_sz) != -1);
	KASSERT(shm_unlink(UNIX64_NOC_NAME) != -1);
	KASSERT(sem_unlink(UNIX64_NOC_LOCK_NAME) != -1);
}

/*============================================================================*
 * linux64_processor_noc_setup()                                              *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC void linux64_processor_noc_setup(void)
{
	linux64_processor_noc_attach();
}
