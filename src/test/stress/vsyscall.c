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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>
#include "../test.h"
#include "vsyscall.h"

#if (__TARGET_HAS_SYNC && __TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

/**
 * @brief Guards. 
 */
PRIVATE struct semaphore master;
PRIVATE struct semaphore slave;

/**
 * @brief Syscall board. 
 */
PRIVATE struct sysboard {
	word_t nr_syscall;
	word_t arg0;
	word_t arg1;
	word_t arg2;
	word_t ret;
} sysboard;

/**
 * @brief Launches unit tests on Target AL.
 */
PUBLIC void vsys_loop(void)
{
	int ret;
	bool exit;

	exit = false;

	while (!exit)
	{
		semaphore_down(&master);

		switch (sysboard.nr_syscall)
		{
			case NR_exit:
				exit = true;
				ret  = 0;
				break;

			case NR_sync_create:
				ret = sync_create(
					(const int *)(long) sysboard.arg0,
					(int) sysboard.arg1,
					(int) sysboard.arg2
				);
				break;

			case NR_sync_open:
				ret = sync_open(
					(const int *)(long) sysboard.arg0,
					(int) sysboard.arg1,
					(int) sysboard.arg2
				);
				break;

			case NR_sync_unlink:
				ret = sync_unlink(
					(int) sysboard.arg0
				);
				break;

			case NR_sync_close:
				ret = sync_close(
					(int) sysboard.arg0
				);
				break;

			case NR_sync_signal:
				ret = sync_signal(
					(int) sysboard.arg0
				);
				break;

			case NR_mailbox_create:
				ret = mailbox_create(
					(int) sysboard.arg0
				);
				break;

			case NR_mailbox_open:
				ret = mailbox_open(
					(int) sysboard.arg0
				);
				break;

			case NR_mailbox_unlink:
				ret = mailbox_unlink(
					(int) sysboard.arg0
				);
				break;

			case NR_mailbox_close:
				ret = mailbox_close(
					(int) sysboard.arg0
				);
				break;

			case NR_mailbox_aread:
				ret = mailbox_aread(
					(int) sysboard.arg0,
					(void *)(long) sysboard.arg1,
					(size_t) sysboard.arg2
				);
				break;

			case NR_mailbox_awrite:
				ret = mailbox_awrite(
					(int) sysboard.arg0,
					(const void *)(long) sysboard.arg1,
					(size_t) sysboard.arg2
				);
				break;

			case NR_portal_create:
				ret = portal_create(
					(int) sysboard.arg0
				);
				break;

			case NR_portal_allow:
				ret = portal_allow(
					(int) sysboard.arg0,
					(int) sysboard.arg1
				);
				break;

			case NR_portal_open:
				ret = portal_open(
					(int) sysboard.arg0,
					(int) sysboard.arg1
				);
				break;

			case NR_portal_unlink:
				ret = portal_unlink(
					(int) sysboard.arg0
				);
				break;

			case NR_portal_close:
				ret = portal_close(
					(int) sysboard.arg0
				);
				break;

			case NR_portal_aread:
				ret = portal_aread(
					(int) sysboard.arg0,
					(void *)(long) sysboard.arg1,
					(size_t) sysboard.arg2
				);
				break;

			case NR_portal_awrite:
				ret = portal_awrite(
					(int) sysboard.arg0,
					(const void *)(long) sysboard.arg1,
					(size_t) sysboard.arg2
				);
				break;

			default:
				ret = (-EINVAL);
		}

		sysboard.ret = ret; 

		semaphore_up(&slave);
	}
}

PUBLIC void vsys_setup(void)
{
	semaphore_init(&master, 0);
	semaphore_init(&slave, 0);
}

PUBLIC void vsys_exit(void)
{
	sysboard.nr_syscall = NR_exit;

	semaphore_up(&master);
	semaphore_down(&slave);
}

/*============================================================================*
 * Sync Kernel Calls                                                          *
 *============================================================================*/

PUBLIC int vsys_sync_create(const int * a, int b, int c)
{
	sysboard.nr_syscall = NR_sync_create;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_sync_open(const int * a, int b, int c)
{
	sysboard.nr_syscall = NR_sync_open;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_sync_unlink(int a)
{
	sysboard.nr_syscall = NR_sync_unlink;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_sync_close(int a)
{
	sysboard.nr_syscall = NR_sync_close;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_sync_wait(int a)
{
	return (sync_wait(a));
}

PUBLIC int vsys_sync_signal(int a)
{
	sysboard.nr_syscall = NR_sync_signal;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

/*============================================================================*
 * Mailbox Kernel Calls                                                       *
 *============================================================================*/

PUBLIC int vsys_mailbox_create(int a)
{
	sysboard.nr_syscall = NR_mailbox_create;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_open(int a)
{
	sysboard.nr_syscall = NR_mailbox_open;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_unlink(int a)
{
	sysboard.nr_syscall = NR_mailbox_unlink;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_close(int a)
{
	sysboard.nr_syscall = NR_mailbox_close;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_aread(int a, void * b, size_t c)
{
	sysboard.nr_syscall = NR_mailbox_aread;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_awrite(int a, const void * b, size_t c)
{
	sysboard.nr_syscall = NR_mailbox_awrite;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_mailbox_wait(int a)
{
	return (mailbox_wait(a));
}

/*============================================================================*
 * Portal Kernel Calls                                                        *
 *============================================================================*/

PUBLIC int vsys_portal_create(int a)
{
	sysboard.nr_syscall = NR_portal_create;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_allow(int a, int b)
{
	sysboard.nr_syscall = NR_portal_allow;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_open(int a, int b)
{
	sysboard.nr_syscall = NR_portal_open;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_unlink(int a)
{
	sysboard.nr_syscall = NR_portal_unlink;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_close(int a)
{
	sysboard.nr_syscall = NR_portal_close;
	sysboard.arg0 = (word_t) a;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_aread(int a, void * b, size_t c)
{
	sysboard.nr_syscall = NR_portal_aread;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_awrite(int a, const void * b, size_t c)
{
	sysboard.nr_syscall = NR_portal_awrite;
	sysboard.arg0 = (word_t) a;
	sysboard.arg1 = (word_t) b;
	sysboard.arg2 = (word_t) c;

	semaphore_up(&master);
	semaphore_down(&slave);

	return (sysboard.ret);
}

PUBLIC int vsys_portal_wait(int a)
{
	return (portal_wait(a));
}

#endif /* __TARGET_HAS_SYNC && __TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX */
