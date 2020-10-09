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
#define __NEED_RESOURCE

#include <nanvix/hal/resource.h>
#include <nanvix/hlib.h>
#include <nanvix/const.h>

/*============================================================================*
 * resource_dumb_alloc()                                                      *
 *============================================================================*/

/**
 * @brief Dumb resource allocator.
 *
 * @param pool Target generic resource pool.
 *
 * @returns Upon successful completion, the ID of a newly allocated
 * resource point is returned. Upon failure, -1 is returned instead.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 * @note This function is reentrant.
 */
PRIVATE int resource_dumb_alloc(const struct resource_pool *pool)
{
	char *base = (char *) pool->resources;
	int n = pool->nresources;
	size_t size = pool->resource_size;

	/* Search for a free synchronization point. */
	for (int i = 0; i < n; i++)
	{
		struct resource *resource;

		resource = (struct resource *)(&base[mult(i, size)]);

		/* Found. */
		if (!resource_is_used(resource))
		{
			*resource = RESOURCE_INITIALIZER;
			resource_set_used(resource);
			return (i);
		}
	}

	return (-1);
}

/*============================================================================*
 * resource_dumb_free()                                                       *
 *============================================================================*/

/**
 * @brief Dumb resource releaser.
 *
 * @param pool Target generic resource pool.
 * @param id   ID of the target resource.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 * @note This function is reentrant.
 */
PRIVATE void resource_dumb_free(const struct resource_pool *pool, int id)
{
	char *base = (char *) pool->resources;
	size_t size = pool->resource_size;
	struct resource *resource;

	resource = (struct resource *)(&base[mult(id, size)]);

	resource_set_unused(resource);
}

/*============================================================================*
 * resource_dumb_enqueue()                                                    *
 *============================================================================*/

/**
 * @brief Dumb resource queuing.
 *
 * @param queue    Target generic resource queue.
 * @param resource Target generic resource.
 *
 * @returns Upon successful completion, the @p resource has been enqueued.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE void resource_dumb_enqueue(struct resource_queue * queue, struct resource * resource)
{
	KASSERT(queue && resource && !resource->next);

	/* Empty queue. */
	if (queue->size == 0)
	{
		KASSERT(queue->head == NULL);
		KASSERT(queue->tail == NULL);

		queue->head = resource;
	}

	/* Update next resource of the tail. */
	else
		queue->tail->next = resource;

	queue->size++;
	queue->tail    = resource;
	resource->next = NULL;
}

/*============================================================================*
 * resource_dumb_dequeue()                                                    *
 *============================================================================*/

/**
 * @brief Dumb resource dequeuing.
 *
 * @param queue    Target generic resource queue.
 *
 * @returns Upon successful completion, the first resource will be returned.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE struct resource * resource_dumb_dequeue(struct resource_queue * queue)
{
	struct resource * resource;

	KASSERT(queue != NULL);

	resource = queue->head;

	/* Queue size cases. */
	switch (queue->size)
	{
		/* Empty. */
		case 0: {
			KASSERT(queue->head == NULL);
			KASSERT(queue->tail == NULL);
		} break;

		/* Last. */
		case 1: {
			KASSERT(queue->head != NULL);
			KASSERT(queue->head == queue->tail);

			queue->size = 0;
			queue->head = NULL;
			queue->tail = NULL;
			resource->next = NULL;
		} break;

		/* More than 2. */
		default: {
			KASSERT(queue->head != NULL);
			KASSERT(queue->tail != NULL);
			KASSERT(queue->head != queue->tail);

			queue->size--;
			queue->head    = resource->next;
			resource->next = NULL;
		} break;
	}

	return (resource);
}

/*============================================================================*
 * Resource Allocator                                                         *
 *============================================================================*/

/**
 * @brief Default resource allocator.
 */
alloc_fn resource_alloc = resource_dumb_alloc;

/**
 * @brief Default resource de-allocator.
 */
free_fn resource_free = resource_dumb_free;

/**
 * @brief Default resource allocator.
 */
enqueue_fn resource_enqueue = resource_dumb_enqueue;

/**
 * @brief Default resource de-allocator.
 */
dequeue_fn resource_dequeue = resource_dumb_dequeue;
