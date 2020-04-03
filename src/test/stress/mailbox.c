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
#include "barrier.h"

#if (__TARGET_HAS_MAILBOX)

/**
 * @name Number of setups and communications.
 */
/**@{*/
#define NSETUPS 1
#define NCOMMUNICATIONS 1
/**@}*/

/*============================================================================*
 * Stress Tests                                                               *
 *============================================================================*/

/**
 * @brief Stress Test: Mailbox Create Unlink
 */
PRIVATE void stress_mailbox_create_unlink(void)
{
	int mbxid;
	int local;

	local = processor_node_get_num();

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((mbxid = mailbox_create(local)) >= 0);
		KASSERT(mailbox_unlink(mbxid) == 0);
	}
}

/**
 * @brief Stress Test: Mailbox Open Close
 */
PRIVATE void stress_mailbox_open_close(void)
{
	int mbxid;
	int remote;

	remote = processor_node_get_num() == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((mbxid = mailbox_open(remote)) >= 0);
		KASSERT(mailbox_close(mbxid) == 0);
	}
}

/**
 * @brief Stress Test: Mailbox Broadcast
 */
PRIVATE void stress_mailbox_broadcast(void)
{
	int ret;
	int local;
	int remote;
	int mbxid;
	char message[HAL_MAILBOX_MSG_SIZE];

	local = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	if (local == NODENUM_MASTER)
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_open(remote)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				message[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(mbxid, message, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);
			}

			KASSERT(mailbox_close(mbxid) == 0);

			test_stress_barrier();
		}
	}
	else
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_create(local)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				message[0] = -1;

				do
				{
					ret = mailbox_aread(mbxid, message, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				KASSERT(message[0] == (j % sizeof(char)));
			}

			KASSERT(mailbox_unlink(mbxid) == 0);

			test_stress_barrier();
		}
	}
}

/**
 * @brief Stress Test: Mailbox Gather
 */
PRIVATE void stress_mailbox_gather(void)
{
	int ret;
	int local;
	int remote;
	int mbxid;
	char message[HAL_MAILBOX_MSG_SIZE];

	local = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	if (local == NODENUM_SLAVE)
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_open(remote)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				message[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(mbxid, message, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);
			}

			KASSERT(mailbox_close(mbxid) == 0);

			test_stress_barrier();
		}
	}
	else
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_create(local)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				message[0] = -1;

				do
				{
					ret = mailbox_aread(mbxid, message, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				KASSERT(message[0] == (j % sizeof(char)));
			}

			KASSERT(mailbox_unlink(mbxid) == 0);

			test_stress_barrier();
		}
	}
}

/**
 * @brief Stress Test: Mailbox Ping-Pong
 */
PRIVATE void stress_mailbox_pingpong(void)
{
	int ret;
	int local;
	int remote;
	int inbox;
	int outbox;
	char msg_in[HAL_MAILBOX_MSG_SIZE];
	char msg_out[HAL_MAILBOX_MSG_SIZE];

	local = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((inbox = mailbox_create(local)) >= 0);
		KASSERT((outbox = mailbox_open(remote)) >= 0);

		if (local == NODENUM_SLAVE)
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0]  = (-1);
				msg_out[0] = ((j + 1) % sizeof(char));

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);
			}
		}
		else
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0]  = (-1);
				msg_out[0] = (j % sizeof(char));
				ret = -EBUSY;

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == ((j + 1) % sizeof(char)));
			}
		}

		KASSERT(mailbox_close(outbox) == 0);
		KASSERT(mailbox_unlink(inbox) == 0);

		test_stress_barrier();
	}
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test stress_mailbox_tests[] = {
	/* Intra-Cluster API Tests */
	{ stress_mailbox_create_unlink, "create unlink" },
	{ stress_mailbox_open_close,    "open close   " },
	{ stress_mailbox_broadcast,     "broadcast    " },
	{ stress_mailbox_gather,        "gather       " },
	{ stress_mailbox_pingpong,      "ping-pong    " },
	{ NULL,                          NULL           },
};

/**
 * The test_stress_mailbox() function launches stress testing units on the
 * mailbox interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_stress_mailbox(void)
{
	test_stress_barrier();

	/* API Tests */
	if (processor_node_get_num() == NODENUM_MASTER)
		kprintf(HLINE);
	for (int i = 0; stress_mailbox_tests[i].test_fn != NULL; i++)
	{
		stress_mailbox_tests[i].test_fn();

		if (processor_node_get_num() == NODENUM_MASTER)
			kprintf("[test][stress][mailbox] %s [passed]", stress_mailbox_tests[i].name);

		test_stress_barrier();
	}
}

#endif /* __TARGET_HAS_MAILBOX */
