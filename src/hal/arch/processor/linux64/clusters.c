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

/* Must come fist. */
#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

/**
 * @brief Name for virtual processor.
 */
#define UNIX64_CLUSTERS_NAME "nanvix-unix64-clusters"

/**
 * @brief Name for virtual processor lock.
 */
#define UNIX64_CLUSTERS_LOCK_NAME "nanvix-unix64-clusters-lock"

/**
 * Physical ID of master cluster.
 */
PRIVATE int LINUX64_PROCESSOR_CLUSTERID_MASTER;

/**
 * @brief Clusters.
 */
PRIVATE struct
{
	/**
	 * @brief Virtual cluster file descriptor.
	 */
	int shm;

	/**
	 * @brief Lock.
	 */
	sem_t *lock;

	/**
	 * @brief Lookup table for logical cluster numbers.
	 */
	pid_t *pids;

	/**
	 * @brief Types of clusters.
	 */
	const int types[PROCESSOR_CLUSTERS_NUM];

} clusters = {
	.shm = -1,
	.lock = NULL,
	.pids = NULL,

	.types = {
		LINUX64_PROCESSOR_IOCLUSTER,
		LINUX64_PROCESSOR_IOCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
		LINUX64_PROCESSOR_CCLUSTER,
	}
};

/*============================================================================*
 * linux64_processor_clusters_lock()                                          *
 *============================================================================*/

/**
 * @brief Locks the virtual processor.
 */
PRIVATE void linux64_processor_clusters_lock(void)
{
	KASSERT(sem_wait(clusters.lock) != -1);
}

/*============================================================================*
 * linux64_processor_clusters_unlock()                                        *
 *============================================================================*/

/**
 * @brief Unlocks the virtual processor.
 */
PRIVATE void linux64_processor_clusters_unlock(void)
{
	KASSERT(sem_post(clusters.lock) != -1);
}

/*============================================================================*
 * linux64_cluster_get_id()                                                   *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PRIVATE int linux64_cluster_get_id(void)
{
	return (getpid());
}

/*============================================================================*
 * linux64_cluster_get_num()                                                  *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC int linux64_cluster_get_num(void)
{
	int clusterid;

	clusterid = linux64_cluster_get_id();

	linux64_processor_clusters_lock();

		/* Search for cluster ID. */
		for (int i = 0; i < PROCESSOR_CLUSTERS_NUM; i++)
		{
			if (clusters.pids[i] == clusterid)
			{
				linux64_processor_clusters_unlock();
				return (i);
			}
		}

	linux64_processor_clusters_unlock();

	kpanic("[hal][processor] unattached process");
	UNREACHABLE();

	return (-1);
}

/*============================================================================*
 * linux64_processor_cluster_is_compute()                                     *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC int linux64_cluster_is_compute(int clusternum)
{
	KASSERT((clusternum >= 0) && (clusternum < PROCESSOR_CLUSTERS_NUM));

	return (clusters.types[clusternum] == LINUX64_PROCESSOR_CCLUSTER);
}

/*============================================================================*
 * linux64_processor_cluster_is_io()                                          *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC int linux64_cluster_is_io(int clusternum)
{
	KASSERT((clusternum >= 0) && (clusternum < PROCESSOR_CLUSTERS_NUM));

	return (clusters.types[clusternum] == LINUX64_PROCESSOR_IOCLUSTER);
}

/*============================================================================*
 * linux64_processor_clusters_boot()                                          *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC void linux64_processor_clusters_boot(void)
{
	void *p;
	struct stat st;
	int initialize = 0;
	size_t clusters_sz = PROCESSOR_CLUSTERS_NUM*sizeof(pid_t);

	LINUX64_PROCESSOR_CLUSTERID_MASTER = linux64_cluster_get_id();

	KASSERT((clusters.lock =
		sem_open(UNIX64_CLUSTERS_LOCK_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR,
			1)
		) != NULL
	);

	/* Open virtual processor. */
	KASSERT((clusters.shm =
		shm_open(UNIX64_CLUSTERS_NAME,
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR)
		) != -1
	);

	linux64_processor_clusters_lock();

		/* Allocate virtual processor. */
		KASSERT(fstat(clusters.shm, &st) != -1);
		if (st.st_size == 0)
		{
			kprintf("[hal][processor] allocating virtual clusters...");
			initialize = 1;
			KASSERT(ftruncate(clusters.shm, clusters_sz) != -1);
		}

		/* Attach virtual processor. */
		KASSERT((p =
			mmap(NULL,
				clusters_sz,
				PROT_READ | PROT_WRITE,
				MAP_SHARED,
				clusters.shm,
				0)
			) != NULL
		);
		clusters.pids = p;

		if (initialize)
		{
			for (int i = 0; i < PROCESSOR_CLUSTERS_NUM; i++)
				clusters.pids[i] = -1;
		}

		kprintf("[hal][processor] attaching process to virtual cluster...");

		/* Search for an unused virtual cluster. */
		for (int i = 0; i < PROCESSOR_CLUSTERS_NUM; i++)
		{
			if (clusters.pids[i] == -1)
			{
				clusters.pids[i] = getpid();
				linux64_processor_clusters_unlock();
				break;
			}
		}

		LINUX64_PROCESSOR_CLUSTERID_MASTER = clusters.pids[0];

	linux64_processor_clusters_unlock();
}

/*============================================================================*
 * linux64_processor_clusters_shutdown()                                      *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC void linux64_processor_clusters_shutdown(void)
{
	size_t clusters_sz = PROCESSOR_CLUSTERS_NUM*sizeof(pid_t);

	KASSERT(munmap(clusters.pids, clusters_sz) != -1);
	KASSERT(close(clusters.shm) != -1);
	KASSERT(sem_close(clusters.lock) != -1);
}

/*============================================================================*
 * linux64_processor_clusters_setup()                                         *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description for this function.
 */
PUBLIC void linux64_processor_clusters_setup(void)
{
}
