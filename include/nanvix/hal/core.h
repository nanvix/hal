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

#ifndef NANVIX_HAL_CORE_H_
#define NANVIX_HAL_CORE_H_

	#ifndef __NEED_HAL_CORE
		#error "include <nanvix/hal/cluster.h> instead"
	#endif

	/* Core Interface Implementation */
	#include <nanvix/hal/core/_core.h>

	#include <nanvix/hal/core/cache.h>
	#include <nanvix/hal/core/clock.h>
	#include <nanvix/hal/core/context.h>
	#include <nanvix/hal/core/exception.h>
	#include <nanvix/hal/core/interrupt.h>
	#include <nanvix/hal/core/mmu.h>
	#include <nanvix/hal/core/spinlock.h>
	#include <nanvix/hal/core/tlb.h>
	#include <nanvix/hal/core/trap.h>
	#include <nanvix/hal/core/upcall.h>
	#include <nanvix/const.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Constants */
	#ifndef BYTE_BIT
	#error "BYTE_BIT not defined"
	#endif
	#ifndef HWORD_BIT
	#error "HWORD_BIT not defined"
	#endif
	#ifndef WORD_BIT
	#error "WORD_BIT not defined"
	#endif
	#ifndef DWORD_BIT
	#error "DWORD_BIT not defined"
	#endif
	#ifndef BYTE_SIZE
	#error "BYTE_SIZE not defined"
	#endif
	#ifndef HWORD_SIZE
	#error "HWORD_SIZE not defined"
	#endif
	#ifndef WORD_SIZE
	#error "WORD_SIZE not defined"
	#endif
	#ifndef DWORD_SIZE
	#error "DWORD_SIZE not defined"
	#endif

	/* Types */
	#ifndef __byte_t
	#error "byte_t type not defined?"
	#endif
	#ifndef __hword_t
	#error "hword_t type not defined?"
	#endif
	#ifndef __word_t
	#error "word_t type not defined?"
	#endif
	#ifndef __dword_t
	#error "dword_t type not defined?"
	#endif

	/* Functions */
	#ifndef ___core_reset_fn
	#error "_core_reset() not defined?"
	#endif
	#ifndef __core_clear_fn
	#error "core_clear() not defined?"
	#endif
	#ifndef __core_get_id_fn
	#error "core_get_id() not defined?"
	#endif
	#ifndef __core_notify_fn
	#error "core_notify() not defined?"
	#endif
	#ifndef __core_poweroff_fn
	#error "core_poweroff() not defined?"
	#endif
	#ifndef __core_setup_fn
	#error "core_setup() not defined?"
	#endif
	#ifndef __core_waitclear_fn
	#error "core_waitclear() not defined?"
	#endif

#endif

/*============================================================================*
 * Core Interface                                                             *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core Core
 * @ingroup kernel-hal
 *
 * @brief Core HAL Interface
 */
/**@{*/

	/**
	 * @name States of a Core
	 */
	/**@{*/
	#define CORE_IDLE      0 /**< Idle        */
	#define CORE_SLEEPING  1 /**< Sleeping    */
	#define CORE_RUNNING   2 /**< Running     */
	#define CORE_RESETTING 3 /**< Resetting   */
	#define CORE_OFFLINE   4 /**< Powered Off */
	/**@}*/

	/**
	 * @brief Core information.
	 */
	struct coreinfo
	{
		int initialized;     /**< Initialized?      */
		int state;           /**< State.            */
		int wakeups;         /**< Wakeup signals.   */
		void (*start)(void); /**< Starting routine. */
		spinlock_t lock;     /**< Lock.             */
	};

	/**
	 * @brief Cores table.
	 */
	EXTERN struct coreinfo cores[];

	/**
	 * @brief Resets the state of the underlying core.
	 */
	EXTERN void core_reset(void);

	/**
	 * @brief Clears the current IPIs pending of the underlying core.
	 *
	 * @author Davidson Francis
	 */
	EXTERN void core_clear(void);

	/**
	 * @brief Gets the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 */
	EXTERN int core_get_id(void);

	/**
	 * @brief Suspends execution until a start signal is received.
	 */
	EXTERN void core_idle(void);

	/**
	 * @brief Sends a signal.
	 *
	 * @param coreid ID of the target core.
	 */
	EXTERN void core_notify(int coreid);

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN void core_poweroff(void);

	/**
	 * @brief Reset the underlying core.
	 */
	EXTERN void core_reset(void);

	/**
	 * @brief Resumes instruction execution in the underlying core.
	 */
	EXTERN void core_run(void);

	/**
	 * @brief Shutdowns the underlying core.
	 *
	 * @param status Shutdown status.
	 */
	EXTERN void core_shutdown(int status);

	/**
	 * @brief Suspends instruction execution in the underling core.
	 */
	EXTERN void core_sleep(void);

	/**
	 * @brief Starts a core.
	 *
	 * @param coreid ID of the target core.
	 * @param start  Starting routine to execute.
	 */
	EXTERN void core_start(int coreid, void (*start)(void));

	/**
	 * @brief Wait and clears the current IPIs pending of the underlying core.
	 */
	EXTERN void core_waitclear(void);

	/**
	 * @brief Wakes up a core.
	 *
	 * @param coreid ID of the target core.
	 */
	EXTERN void core_wakeup(int coreid);

/**@}*/

#endif /* NANVIX_HAL_CORE_H_ */

