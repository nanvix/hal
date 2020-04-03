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

#if (__TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL)

/**
 * @name Number of setups and communications.
 */
/**@{*/
#define NSETUPS 1
#define NCOMMUNICATIONS 1
/**@}*/

/**
 * @name Auxiliar buffers.
 */
/**@{*/
static char msg_in[HAL_PORTAL_MAX_SIZE];
static char msg_out[HAL_PORTAL_MAX_SIZE];
/**@}*/

/*============================================================================*
 * Stress Tests                                                               *
 *============================================================================*/

/**
 * @brief Stress Test: Combination Create Unlink
 */
PRIVATE void stress_combination_create_unlink(void)
{
	int local;
	int mbxid;
    int portalid;

	local = processor_node_get_num();

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
        KASSERT((mbxid = mailbox_create(local)) >= 0);
        KASSERT((portalid = portal_create(local)) >= 0);

        KASSERT(portal_unlink(portalid) == 0);
		KASSERT(mailbox_unlink(mbxid) == 0);
	}
}

/**
 * @brief Stress Test: Combination Open Close
 */
PRIVATE void stress_combination_open_close(void)
{
	int local;
	int remote;
    int mbxid;
    int portalid;

	local  = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
        KASSERT((mbxid = mailbox_open(remote)) >= 0);
        KASSERT((portalid = portal_open(local, remote)) >= 0);

        KASSERT(portal_close(portalid) == 0);
		KASSERT(mailbox_close(mbxid) == 0);
	}
}

/**
 * @brief Stress Test: Combination Broadcast
 */
PRIVATE void stress_combination_broadcast(void)
{
	int ret;
	int local;
	int remote;
	int mbxid;
	int portalid;

	local  = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	if (local == NODENUM_MASTER)
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_open(remote)) >= 0);
			KASSERT((portalid = portal_open(local, remote)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_out[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(mbxid, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				do
				{
					ret = portal_awrite(portalid, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(portalid) == 0);
			}

			KASSERT(portal_close(portalid) == 0);
			KASSERT(mailbox_close(mbxid) == 0);

			test_stress_barrier();
		}
	}
	else
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_create(local)) >= 0);
			KASSERT((portalid = portal_create(local)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0] = (-1);

				do
				{
					ret = mailbox_aread(mbxid, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_in[0] = (-1);

				KASSERT(portal_allow(portalid, remote) == 0);

				do
				{
					ret = portal_aread(portalid, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(portalid) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));
			}

			KASSERT(portal_unlink(portalid) == 0);
			KASSERT(mailbox_unlink(mbxid) == 0);

			test_stress_barrier();
		}
	}
}

/**
 * @brief Stress Test: Combination Gather
 */
PRIVATE void stress_combination_gather(void)
{
	int ret;
	int local;
	int remote;
	int mbxid;
	int portalid;

	local  = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	if (local == NODENUM_SLAVE)
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_open(remote)) >= 0);
			KASSERT((portalid = portal_open(local, remote)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_out[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(mbxid, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				do
				{
					ret = portal_awrite(portalid, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(portalid) == 0);
			}

			KASSERT(portal_close(portalid) == 0);
			KASSERT(mailbox_close(mbxid) == 0);

			test_stress_barrier();
		}
	}
	else
	{
		for (unsigned int i = 0; i < NSETUPS; ++i)
		{
			KASSERT((mbxid = mailbox_create(local)) >= 0);
			KASSERT((portalid = portal_create(local)) >= 0);

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0] = (-1);

				do
				{
					ret = mailbox_aread(mbxid, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(mbxid) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_in[0] = (-1);

				KASSERT(portal_allow(portalid, remote) == 0);

				do
				{
					ret = portal_aread(portalid, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(portalid) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));
			}

			KASSERT(portal_unlink(portalid) == 0);
			KASSERT(mailbox_unlink(mbxid) == 0);

			test_stress_barrier();
		}
	}
}

/**
 * @brief Stress Test: Combination Ping-Pong
 */
PRIVATE void stress_combination_pingpong(void)
{
	int ret;
	int local;
	int remote;
	int inbox;
	int outbox;
	int inportal;
	int outportal;

	local = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((inbox = mailbox_create(local)) >= 0);
		KASSERT((outbox = mailbox_open(remote)) >= 0);
		KASSERT((inportal = portal_create(local)) >= 0);
		KASSERT((outportal = portal_open(local, remote)) >= 0);

		if (local == NODENUM_SLAVE)
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0] = (-1);

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_in[0] = (-1);

				KASSERT(portal_allow(inportal, remote) == 0);

				do
				{
					ret = portal_aread(inportal, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(inportal) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_out[0] = ((j + 1) % sizeof(char));

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);

				do
				{
					ret = portal_awrite(outportal, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(outportal) == 0);
			}
		}
		else
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_out[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);

				do
				{
					ret = portal_awrite(outportal, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(outportal) == 0);

				msg_in[0] = local;

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == ((j + 1) % sizeof(char)));

				msg_in[0] = local;

				KASSERT(portal_allow(inportal, remote) == 0);

				do
				{
					ret = portal_aread(inportal, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(inportal) == 0);

				KASSERT(msg_in[0] == ((j + 1) % sizeof(char)));
			}
		}

		KASSERT(portal_close(outportal) == 0);
		KASSERT(portal_unlink(inportal) == 0);
		KASSERT(mailbox_close(outbox) == 0);
		KASSERT(mailbox_unlink(inbox) == 0);

		test_stress_barrier();
	}
}

/**
 * @brief Stress Test: Combination Ping-Pong Reverse
 */
PRIVATE void stress_combination_pingpong_reverse(void)
{
	int ret;
	int local;
	int remote;
	int inbox;
	int outbox;
	int inportal;
	int outportal;

	local = processor_node_get_num();
	remote = local == NODENUM_MASTER ? NODENUM_SLAVE : NODENUM_MASTER;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((inbox = mailbox_create(local)) >= 0);
		KASSERT((outbox = mailbox_open(remote)) >= 0);
		KASSERT((inportal = portal_create(local)) >= 0);
		KASSERT((outportal = portal_open(local, remote)) >= 0);

		if (local == NODENUM_SLAVE)
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_in[0] = (-1);

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_out[0] = (j % sizeof(char));

				do
				{
					ret = portal_awrite(outportal, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(outportal) == 0);

				msg_out[0] = ((j + 1) % sizeof(char));

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);

				msg_in[0] = (-1);

				KASSERT(portal_allow(inportal, remote) == 0);

				do
				{
					ret = portal_aread(inportal, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(inportal) == 0);

				KASSERT(msg_in[0] == ((j + 1) % sizeof(char)));
			}
		}
		else
		{
			for (unsigned int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				msg_out[0] = (j % sizeof(char));

				do
				{
					ret = mailbox_awrite(outbox, msg_out, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -EAGAIN || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(outbox) == 0);

				msg_in[0] = (-1);

				KASSERT(portal_allow(inportal, remote) == 0);

				do
				{
					ret = portal_aread(inportal, msg_in, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EBUSY || ret == -ENOMSG || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(inportal) == 0);

				KASSERT(msg_in[0] == (j % sizeof(char)));

				msg_in[0] = (-1);

				do
				{
					ret = mailbox_aread(inbox, msg_in, HAL_MAILBOX_MSG_SIZE);
					KASSERT(ret == -ETIMEDOUT || ret == -EBUSY || ret == HAL_MAILBOX_MSG_SIZE);
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(mailbox_wait(inbox) == 0);

				KASSERT(msg_in[0] == ((j + 1) % sizeof(char)));

				msg_out[0] = ((j + 1) % sizeof(char));

				do
				{
					ret = portal_awrite(outportal, msg_out, HAL_PORTAL_MAX_SIZE);
					KASSERT(ret == -EACCES || ret == -EBUSY || ret == HAL_PORTAL_MAX_SIZE);
				} while (ret != HAL_PORTAL_MAX_SIZE);

				KASSERT(portal_wait(outportal) == 0);
			}
		}

		KASSERT(portal_close(outportal) == 0);
		KASSERT(portal_unlink(inportal) == 0);
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
PRIVATE struct test stress_combination_tests[] = {
	/* Intra-Cluster API Tests */
	{ stress_combination_create_unlink,    "create unlink    " },
	{ stress_combination_open_close,       "open close       " },
	{ stress_combination_broadcast,        "broadcast        " },
	{ stress_combination_gather,           "gather           " },
	{ stress_combination_pingpong,         "ping-pong        " },
	{ stress_combination_pingpong_reverse, "ping-pong reverse" },
	{ NULL,                                 NULL               },
};

/**
 * The test_stress_portal() function launches stress testing units on the
 * portal interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_stress_combination(void)
{
	test_stress_barrier();

	/* API Tests */
	if (processor_node_get_num() == NODENUM_MASTER)
		kprintf(HLINE);
	for (int i = 0; stress_combination_tests[i].test_fn != NULL; i++)
	{
		stress_combination_tests[i].test_fn();

		if (processor_node_get_num() == NODENUM_MASTER)
			kprintf("[test][stress][combination] %s [passed]", stress_combination_tests[i].name);

		test_stress_barrier();
	}
}

#endif /* __TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL */
