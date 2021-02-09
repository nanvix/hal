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

#include <nanvix/hal/core/spinlock.h>
#include <nanvix/hal/resource.h>
#include <nanvix/hlib.h>
#include <nanvix/const.h>
#include <posix/errno.h>

PRIVATE spinlock_t _lock = SPINLOCK_UNLOCKED;

/*============================================================================*
 * Pool functions                                                             *
 *============================================================================*/

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
	KASSERT(pool != NULL);

	char *base = (char *) pool->resources;
	int n = pool->nresources;
	size_t size = pool->resource_size;

	spinlock_lock(&_lock);

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
				spinlock_unlock(&_lock);
				return (i);
			}
		}

	spinlock_unlock(&_lock);

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
	KASSERT(pool != NULL && id >= 0);

	char *base = (char *) pool->resources;
	size_t size = pool->resource_size;
	struct resource *resource;

	resource = (struct resource *)(&base[mult(id, size)]);

	spinlock_lock(&_lock);

		resource_set_unused(resource);

	spinlock_unlock(&_lock);
}

/*============================================================================*
 * Arrangement functions                                                      *
 *============================================================================*/

/*============================================================================*
 * Insert Functions                                                           *
 *============================================================================*/

/*============================================================================*
 * resource_dumb_push_front()                                                 *
 *============================================================================*/

/**
 * @brief Dumb resource push front.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target generic resource.
 *
 * @returns Upon successful completion, the @p r has been put on the
 * first position.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_push_front(struct resource_arrangement * arr, struct resource * r)
{
	/* Valid pointers. */
	KASSERT(arr && r && !r->next);

	/* Empty? */
	if (arr->size == 0)
		arr->tail = r;

	/* Update next resource of the tail. */
	else
		r->next = arr->head;

	arr->head = r;
	arr->size++;

	return (0);
}

/*============================================================================*
 * resource_dumb_push_back()                                                  *
 *============================================================================*/

/**
 * @brief Dumb resource push back.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target generic resource.
 *
 * @returns Upon successful completion, the @p r has been put on the
 * last position.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_push_back(struct resource_arrangement * arr, struct resource * r)
{
	/* Valid pointers. */
	KASSERT(arr && r && !r->next);

	/* Empty? */
	if (arr->size == 0)
		arr->head = r;

	/* Update next resource of the tail. */
	else
		arr->tail->next = r;

	arr->tail = r;
	arr->size++;

	return (arr->size - 1);
}

/*============================================================================*
 * resource_dumb_insert()                                                     *
 *============================================================================*/

/**
 * @brief Dumb resource push front.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target generic resource.
 *
 * @returns Upon successful completion, the @p r has been put on the
 * front position.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_insert(struct resource_arrangement * arr, struct resource * r, int pos)
{
	int i;
	struct resource * previous;

	/* Valid pointers. */
	KASSERT(arr && r && !r->next);

	/* Valid position. */
	if (!WITHIN(pos, 0, (int) (arr->size + 1)))
		return (-1);

	/* Front? */
	if (pos == 0)
		return (resource_dumb_push_front(arr, r));

	/* Back? */
	if (pos == (int) arr->size)
		return (resource_dumb_push_back(arr, r));

	/* Middle. */
	i        = 1;
	previous = arr->head;

	/* Find position. */
	while (i != pos)
	{
		previous = previous->next;
		i++;
	}

	/* Insert resource. */
	r->next        = previous->next;
	previous->next = r;

	/* Update size. */
	arr->size++;

	return (pos);
}

/*============================================================================*
 * resource_dumb_insert_ordered()                                             *
 *============================================================================*/

/**
 * @brief Dumb resource push front.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target generic resource.
 * @param cmp Compare function.
 *
 * @returns Upon successful completion, the @p r has been put on a ordered
 * position.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_insert_ordered(
	struct resource_arrangement * arr,
	struct resource * r,
	compare_fn cmp
)
{
	int pos;
	struct resource * previous;

	/* Valid pointers. */
	KASSERT(arr && r && !r->next && cmp);

	/* Head > r ? Insert front. */
	if (arr->size == 0 || cmp(arr->head, r) > 0)
		return (resource_dumb_push_front(arr, r));

	/* Search until reach the tail. */
	pos      = 1;
	previous = arr->head;

	/**
	 * Search until reach the tail (next == NULL) or the cmp function
	 * sinalizes that the next is greater than r.
	 */
	while (previous->next && cmp(previous->next, r) <= 0)
	{
		previous = previous->next;
		pos++;
	}

	/* Insert on tail? */
	if (previous == arr->tail)
		return (resource_dumb_push_back(arr, r));

	/* Insert between two nodes. */
	r->next        = previous->next;
	previous->next = r;

	/* Update size. */
	arr->size++;

	return (pos);
}

/*============================================================================*
 * Remove Functions                                                           *
 *============================================================================*/

/*============================================================================*
 * __resource_remove()                                                        *
 *============================================================================*/

/**
 * @brief Remove a resource from an arrangement.
 *
 * @param arr      Target generic resource arrangement.
 * @param previous Previous resource.
 * @param curr     Target resource.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE void __resource_remove(struct resource_arrangement * arr, struct resource * previous, struct resource * curr)
{
	KASSERT(arr && curr);

	/* Remove head? */
	if (curr == arr->head)
		arr->head = curr->next;
	else
		previous->next = curr->next;

	/* Remove tail? */
	if (curr == arr->tail)
		arr->tail = previous;

	arr->size--;
	curr->next = NULL;

	/* Empty? */
	if (arr->size == 0)
	{
		arr->head = NULL;
		arr->tail = NULL;
	}
}

/*============================================================================*
 * resource_dumb_remove_pos()                                                 *
 *============================================================================*/

/**
 * @brief Generic function that remove by reference or by position. The
 * complementary pointer is seted with the information found.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target resource pointer if different of NULL.
 * @param pos Target position if different greater than 0.
 *
 * @returns Zero if remove with success, non-zero otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_remove_pos(struct resource_arrangement * arr, struct resource ** r, int * pos)
{
	int i;
	struct resource * curr;
	struct resource * previous;

	KASSERT(arr && r && pos);

	/* One of them must be valid. */
	if (*r == NULL && !WITHIN(*pos, 0, (int) arr->size))
		return (-1);

	i        = 0;
	curr     = arr->head;
	previous = NULL;

	/* Search by resource? */
	if (*r)
	{
		while (curr != NULL && curr != *r)
		{
			previous = curr;
			curr     = curr->next;
			i++;
		}
	}

	/* Search by id. */
	else
	{
		while (curr != NULL && i != *pos)
		{
			previous = curr;
			curr     = curr->next;
			i++;
		}
	}

	/* Not Found? */
	if (!curr)
		return (-1);

	*pos = i;
	*r   = curr;

	__resource_remove(arr, previous, curr);

	return (0);
}

/*============================================================================*
 * resource_dumb_pop_front()                                                  *
 *============================================================================*/

/**
 * @brief Dumb resource pop front.
 *
 * @param arr Target generic resource arrangement.
 *
 * @returns Upon successful completion, the first resource will be returned.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE struct resource * resource_dumb_pop_front(struct resource_arrangement * arr)
{
	KASSERT(arr != NULL);

	int pos = 0;
	struct resource * r = NULL;

	if (resource_dumb_remove_pos(arr, &r, &pos) != 0)
		return (NULL);

	return (r);
}

/*============================================================================*
 * resource_dumb_pop_back()                                                   *
 *============================================================================*/

/**
 * @brief Dumb resource pop back.
 *
 * @param arr Target generic resource arrangement.
 *
 * @returns Upon successful completion, the last resource will be returned.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE struct resource * resource_dumb_pop_back(struct resource_arrangement * arr)
{
	KASSERT(arr != NULL);

	int pos = (arr->size - 1);
	struct resource * r = NULL;

	if (resource_dumb_remove_pos(arr, &r, &pos) != 0)
		return (NULL);

	return (r);
}

/*============================================================================*
 * resource_dumb_remove_spec()                                                *
 *============================================================================*/

/**
 * @brief Dumb resource poping.
 *
 * @param arr Target generic resource arrangement.
 * @param pos Target position.
 *
 * @returns Upon successful completion, the @p resource has been removed.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE struct resource * resource_dumb_remove_spec(struct resource_arrangement * arr, int pos)
{
	KASSERT(arr != NULL);

	struct resource * r = NULL;

	if (resource_dumb_remove_pos(arr, &r, &pos) != 0)
		return (NULL);

	return (r);
}

/*============================================================================*
 * resource_dumb_pop()                                                        *
 *============================================================================*/

/**
 * @brief Dumb resource poping.
 *
 * @param arr Target generic resource arrangement.
 * @param r   Target generic resource.
 *
 * @returns Upon successful completion, the @p resource has been removed.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_pop(struct resource_arrangement * arr, struct resource * r)
{
	KASSERT(arr != NULL);

	int pos = -1;

	if (resource_dumb_remove_pos(arr, &r, &pos) != 0)
		return (-EAGAIN);

	return (pos);
}

/*============================================================================*
 * resource_dumb_remove_verify()                                              *
 *============================================================================*/

/**
 * @brief Remove the first occurrence of a verification.
 *
 * @param arr    Target generic resource arrangement.
 * @param verify Verification function.
 *
 * @returns Valid pointer if found some resource that satisfy the verification,
 * NULL otherwise.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE struct resource * resource_dumb_remove_verify(struct resource_arrangement * arr, verify_fn verify)
{
	struct resource * curr;
	struct resource * previous;

	KASSERT(arr && verify);

	curr     = arr->head;
	previous = NULL;

	/* While still has resources and has not been found. */
	while (curr != NULL && !verify(curr))
	{
		previous = curr;
		curr     = curr->next;
	}

	/* Found? */
	if (curr)
		__resource_remove(arr, previous, curr);

	/* Return. */
	return (curr);
}

/*============================================================================*
 * Search Functions                                                           *
 *============================================================================*/

/*============================================================================*
 * resource_dumb_search()                                                     *
 *============================================================================*/

/**
 * @brief Dumb resource search.
 *
 * @param arr      Target generic resource arrangement.
 * @param resource Target generic resource.
 *
 * @returns Upon successful completion, the @p resource has been removed.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_search(struct resource_arrangement * arr, struct resource * r)
{
	int i;
	struct resource * curr;

	if (arr == NULL || r == NULL)
		return (-EINVAL);

	i    = 0;
	curr = arr->head;

	while (curr != NULL && curr != r)
	{
		curr = curr->next;
		i++;
	}

	return (curr ? i : -EAGAIN);
}

/*============================================================================*
 * resource_dumb_search()                                                     *
 *============================================================================*/

/**
 * @brief Dumb resource search.
 *
 * @param arr      Target generic resource arrangement.
 * @param resource Target generic resource.
 *
 * @returns Upon successful completion, the @p resource has been removed.
 *
 * @note This function is non-blocking.
 * @note This function is @b NOT thread safe.
 */
PRIVATE int resource_dumb_search_verify(struct resource_arrangement * arr, verify_fn verify)
{
	int i;
	struct resource * curr;

	if (arr == NULL || verify == NULL)
		return (-EINVAL);

	i    = 0;
	curr = arr->head;

	while (curr != NULL && !verify(curr))
	{
		curr = curr->next;
		i++;
	}

	return (curr ? i : -EAGAIN);
}

/*============================================================================*
 * Exported Functions                                                         *
 *============================================================================*/

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

/*============================================================================*
 * Insert Functions                                                           *
 *============================================================================*/

/**
 * @brief Enqueue a resource from an arrangement.
 */
put_fn resource_enqueue = resource_dumb_push_back;

/**
 * @brief Dequeue a resource from an arrangement.
 */
pop_fn resource_dequeue = resource_dumb_pop_front;

/**
 * @brief Put a resource in the first position of an arrangement.
 */
put_fn resource_push_front = resource_dumb_push_front;

/**
 * @brief Put a resource in the last position of an arrangement.
 */
put_fn resource_push_back = resource_dumb_push_back;

/**
 * @brief Insert a resource in a specific position of an arrangement.
 */
insert_fn resource_insert = resource_dumb_insert;

/**
 * @brief Insert a resource in a specific position of an arrangement by
 * a comparation function.
 */
insert_ordered_fn resource_insert_ordered = resource_dumb_insert_ordered;

/*============================================================================*
 * Remove Functions                                                           *
 *============================================================================*/

/**
 * @brief Pop the especific resource of an arrangement.
 */
remove_fn resource_pop = resource_dumb_pop;

/**
 * @brief Pop the first resource of an arrangement.
 */
pop_fn resource_pop_front = resource_dumb_pop_front;

/**
 * @brief Pop the last resource of an arrangement.
 */
pop_fn resource_pop_back = resource_dumb_pop_back;

/**
 * @brief Remove a specific resource by it position.
 */
remove_spec_fn resource_remove = resource_dumb_remove_spec;

/**
 * @brief Remove a resource by verification.
 */
remove_verify_fn resource_remove_verify = resource_dumb_remove_verify;

/*============================================================================*
 * Search Functions                                                           *
 *============================================================================*/

/**
 * @brief Search for an specific resource on an arrangement.
 */
search_fn resource_search = resource_dumb_search;

/**
 * @brief Search using a verification function on an arrangement.
 */
search_verify_fn resource_search_verify = resource_dumb_search_verify;
