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

/**
 * @defgroup kernel-resource Resource
 * @ingroup kernel
 *
 * @brief Resource abstraction.
 */

#ifndef NANVIX_HAL_RESOURCE_H_
#define NANVIX_HAL_RESOURCE_H_

	#include <nanvix/const.h>

	/**
	 * @brief Resource flags.
	 */
	/**@{*/
	#define RESOURCE_FLAGS_USED  (1 << 0) /**< Used synchronization point? */
	#define RESOURCE_FLAGS_BUSY  (1 << 1) /**< Busy?                       */
	#define RESOURCE_FLAGS_WRITE (1 << 2) /**< Writable?                   */
	#define RESOURCE_FLAGS_READ  (1 << 3) /**< Readable?                   */
	#define RESOURCE_FLAGS_ASYNC (1 << 4) /**< Asynchronous?               */
	/**@}*/

	/**
	 * @brief Resource.
	 */
	struct resource
	{
		int flags; /**< Flags. */
	};

	/**
	 * @brief Resource pool.
	 */
	struct resource_pool
	{
		void *resources;	  /**< Pool of resources.       */
		int nresources;		  /**< Number of resources.     */
		size_t resource_size; /**< Resource size (in byes). */
	};

	/**
	 * @brief Resource allocation interface.
	 */
	/**@{*/
	typedef int (*alloc_fn)(const struct resource_pool *);
	typedef void (*free_fn)(const struct resource_pool *, int);
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
	 * @brief Resource allocator.
	 */
	EXTERN alloc_fn resource_alloc;

	/**
	 * @brief Resource de-allocator.
	 */
	EXTERN free_fn resource_free;

#endif /** NANVIX_HAL_RESOURCE_H_ */
