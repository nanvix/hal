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

/**
 * @defgroup kernel-resource Resource
 * @ingroup kernel
 *
 * @brief Resource abstraction.
 */

#ifdef __NEED_RESOURCE
#ifndef NANVIX_HAL_RESOURCE_H_
#define NANVIX_HAL_RESOURCE_H_

	#include <nanvix/const.h>

	/**
	 * @brief Resource flags.
	 */
	/**@{*/
	#define RESOURCE_FLAGS_USED   (1 << 0) /**< Used?         */
	#define RESOURCE_FLAGS_BUSY   (1 << 1) /**< Busy?         */
	#define RESOURCE_FLAGS_WRITE  (1 << 2) /**< Writable?     */
	#define RESOURCE_FLAGS_READ   (1 << 3) /**< Readable?     */
	#define RESOURCE_FLAGS_ASYNC  (1 << 4) /**< Asynchronous? */
	#define RESOURCE_FLAGS_SHARED (1 << 5) /**< Shared?       */
	#define RESOURCE_FLAGS_MAPPED (1 << 6) /**< Mapped?       */
	#define RESOURCE_FLAGS_VALID  (1 << 7) /**< Valid?        */
	#define RESOURCE_FLAGS_DIRTY  (1 << 8) /**< Dirty?        */
	/**@}*/

	/**
	 * @brief Static initializer for a resource.
	 */
	/**@{*/
	#define RESOURCE_STATIC_INITIALIZER                          \
		{ 0, NULL }
	#define RESOURCE_POOL_STATIC_INITIALIZER(base, amount, size) \
		{ base, amount, size }
	#define RESOURCE_QUEUE_STATIC_INITIALIZER                    \
		{ NULL, NULL, 0 }
	#define RESOURCE_LIST_STATIC_INITIALIZER                     \
		{ NULL, NULL, 0 }
	/**@}*/

	/**
	 * @brief Initializer for a resource.
	 */
	/**@{*/
	#define RESOURCE_INITIALIZER                          \
		(struct resource) RESOURCE_STATIC_INITIALIZER
	#define RESOURCE_POOL_INITIALIZER(base, amount, size) \
		(struct resource_pool) RESOURCE_POOL_STATIC_INITIALIZER(base, amount, size)
	#define RESOURCE_QUEUE_INITIALIZER                    \
		(struct resource_queue) RESOURCE_QUEUE_STATIC_INITIALIZER
	#define RESOURCE_LIST_INITIALIZER                     \
		(struct resource_list) RESOURCE_LIST_STATIC_INITIALIZER
	/**@}*/


	/**
	 * @brief Resource.
	 */
	struct resource
	{
		int flags;              /**< Flags.                */
		struct resource * next; /**< Linked struct helper. */
	};

	/**
	 * @brief Resource pool.
	 */
	struct resource_pool
	{
		void * resources;     /**< Pool of resources.       */
		int nresources;       /**< Number of resources.     */
		size_t resource_size; /**< Resource size (in byes). */
	};

	/**
	 * @brief Resource queue (First In First Out).
	 */
	struct resource_queue
	{
		struct resource * head; /**< First resource queued.      */
		struct resource * tail; /**< Last resource queued.       */
		size_t size;            /**< Number of resources queued. */
	};

	/**
	 * @brief Resource list (Linked list).
	 */
	struct resource_list
	{
		struct resource * head; /**< First resource queued.      */
		struct resource * tail; /**< Last resource queued.       */
		size_t size;            /**< Number of resources queued. */
	};

	/**
	 * @brief Resource allocation interface.
	 */
	/**@{*/
	typedef int (*alloc_fn)(const struct resource_pool *);
	typedef void (*free_fn)(const struct resource_pool *, int);
	typedef void (*enqueue_fn)(struct resource_queue *, struct resource *);
	typedef struct resource * (*dequeue_fn)(struct resource_queue *);
	typedef void (*push_back_fn)(struct resource_list *, struct resource *);
	typedef int (*pop_fn)(struct resource_list *, struct resource *);
	/**@}*/

	/**
	 * @brief Sets a resource as used.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_used(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_USED;
	}

	/**
	 * @brief Sets a resource as not used.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_unused(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_USED;
	}

	/**
	 * @brief Sets a resource as busy.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_busy(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_BUSY;
	}

	/**
	 * @brief Sets a resource as not busy.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_notbusy(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_BUSY;
	}

	/**
	 * @brief Sets a resource as write-only.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_wronly(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_WRITE;
		rsrc->flags &= ~RESOURCE_FLAGS_READ;
	}

	/**
	 * @brief Sets a resource as read-only.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_rdonly(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_READ;
		rsrc->flags &= ~RESOURCE_FLAGS_WRITE;
	}

	/**
	 * @brief Sets a resource as readable and writable.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_rdwr(struct resource *rsrc)
	{
		rsrc->flags |= (RESOURCE_FLAGS_READ | RESOURCE_FLAGS_WRITE);
	}

	/**
	 * @brief Sets a resource as asynchronous.
	 *
	 * @param rsrc Target resource.
	 *
	 * @note A resource cannot be synchronous and asynchronous at the same time.
	 */
	static inline void resource_set_async(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_ASYNC;
	}

	/**
	 * @brief Sets a resource as synchronous.
	 *
	 * @param rsrc Target resource.
	 *
	 * @note A resource cannot be synchronous and asynchronous at the same time.
	 */
	static inline void resource_set_sync(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_ASYNC;
	}

	/**
	 * @brief Sets a resource as private.
	 *
	 * @param rsrc Target resource.
	 *
	 * @note A resource cannot be private and shared at the same time.
	 */
	static inline void resource_set_private(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_SHARED;
	}

	/**
	 * @brief Sets a resource as shared.
	 *
	 * @param rsrc Target resource.
	 *
	 * @note A resource cannot be private and shared at the same time.
	 */
	static inline void resource_set_shared(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_SHARED;
	}

	/**
	 * @brief Sets a resource as mapped.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_mapped(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_MAPPED;
	}

	/**
	 * @brief Sets a resource as unmapped.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_unmapped(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_MAPPED;
	}

	/**
	 * @brief Sets a resource as invalid.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_invalid(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_VALID;
	}

	/**
	 * @brief Sets a resource as valid.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_valid(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_VALID;
	}

	/**
	 * @brief Sets a resource as clean.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_clean(struct resource *rsrc)
	{
		rsrc->flags &= ~RESOURCE_FLAGS_DIRTY;
	}

	/**
	 * @brief Sets a resource as dirty.
	 *
	 * @param rsrc Target resource.
	 */
	static inline void resource_set_dirty(struct resource *rsrc)
	{
		rsrc->flags |= RESOURCE_FLAGS_DIRTY;
	}

	/**
	 * @brief Asserts whether or not a resource is in use.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is in used, and zero otherwise.
	 */
	static inline int resource_is_used(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_USED);
	}

	/**
	 * @brief Asserts whether or not a resource is busy.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is busy, and zero otherwise.
	 */
	static inline int resource_is_busy(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_BUSY);
	}

	/**
	 * @brief Asserts whether or not a resource is readable.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is readable and zero otherwise.
	 */
	static inline int resource_is_readable(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_READ);
	}

	/**
	 * @brief Asserts whether or not a resource is read-only.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is read-inly and zero otherwise.
	 */
	static inline int resource_is_rdonly(const struct resource *rsrc)
	{
		return ((rsrc->flags & RESOURCE_FLAGS_READ) == RESOURCE_FLAGS_READ);
	}

	/**
	 * @brief Asserts whether or not a resource is writable.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is writable and zero otherwise.
	 */
	static inline int resource_is_writable(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_WRITE);
	}

	/**
	 * @brief Asserts whether or not a resource is write-only.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is write-only and zero otherwise.
	 */
	static inline int resource_is_wronly(const struct resource *rsrc)
	{
		return ((rsrc->flags & RESOURCE_FLAGS_WRITE) == RESOURCE_FLAGS_WRITE);
	}

	/**
	 * @brief Asserts whether or not a resource is asynchronous.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is asynchronous and zero otherwise.
	 *
	 * @note A resource cannot be synchronous and asynchronous at the same time.
	 */
	static inline int resource_is_async(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_ASYNC);
	}

	/**
	 * @brief Asserts whether or not a resource is synchronous.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is synchronous and zero otherwise.
	 *
	 * @note A resource cannot be synchronous and asynchronous at the same time.
	 */
	static inline int resource_is_sync(const struct resource *rsrc)
	{
		return (!(rsrc->flags & RESOURCE_FLAGS_ASYNC));
	}

	/**
	 * @brief Asserts whether or not a resource is private.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is private and zero otherwise.
	 *
	 * @note A resource cannot be private and shared at the same time.
	 */
	static inline int resource_is_private(const struct resource *rsrc)
	{
		return (!(rsrc->flags & RESOURCE_FLAGS_SHARED));
	}

	/**
	 * @brief Asserts whether or not a resource is shared.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is shared and zero otherwise.
	 *
	 * @note A resource cannot be shared and private at the same time.
	 */
	static inline int resource_is_shared(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_SHARED);
	}

	/**
	 * @brief Asserts whether or not a resource is mapped.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is mapped and zero otherwise.
	 *
	 * @note A resource cannot be mapped and unmapped at the same time.
	 */
	static inline int resource_is_mapped(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_MAPPED);
	}

	/**
	 * @brief Asserts whether or not a resource is valid.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is valid and zero otherwise.
	 *
	 * @note A resource cannot be valid and invalid at the same time.
	 */
	static inline int resource_is_valid(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_VALID);
	}

	/**
	 * @brief Asserts whether or not a resource is dirty.
	 *
	 * @param rsrc Target resource.
	 *
	 * @returns One if the target resource is dirty and zero otherwise.
	 *
	 * @note A resource cannot be dirty and indirty at the same time.
	 */
	static inline int resource_is_dirty(const struct resource *rsrc)
	{
		return (rsrc->flags & RESOURCE_FLAGS_DIRTY);
	}

	/**
	 * @brief Resource allocator.
	 */
	EXTERN alloc_fn resource_alloc;

	/**
	 * @brief Resource de-allocator.
	 */
	EXTERN free_fn resource_free;

	/**
	 * @brief Resource queuing.
	 */
	EXTERN enqueue_fn resource_enqueue;

	/**
	 * @brief Resource dequeuing.
	 */
	EXTERN dequeue_fn resource_dequeue;

	/**
	 * @brief Resource push back on a list.
	 */
	EXTERN push_back_fn resource_push_back;

	/**
	 * @brief Resource pop from on a list.
	 */
	EXTERN pop_fn resource_pop;

#endif /** NANVIX_HAL_RESOURCE_H_ */
#endif /* __NEED_RESOURCE */
