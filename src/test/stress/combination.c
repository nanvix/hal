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
#include "stress.h"

#if (__TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL)

/**
 * @name Number of setups and communications.
 */
/**@{*/
#define NSETUPS 10
#define NCOMMUNICATIONS 10
/**@}*/

/**
 * @name Possible value returned by aread/awrite of Portal.
 */
 /**@{*/
#define PORTAL_AREAD_CHECKS(_ret)      \
	  ((_ret == -EBUSY)                \
	|| (_ret == -ENOMSG)               \
	|| (_ret == HAL_PORTAL_MAX_SIZE))
#define PORTAL_AWRITE_CHECKS(_ret)     \
	  ((_ret == -EACCES)               \
	|| (_ret == -EBUSY)                \
	|| (_ret == HAL_PORTAL_MAX_SIZE))
/**@}*/

/**
 * @name Possible value returned by aread/awrite of Mailbox.
 */
 /**@{*/
#define MAILBOX_AREAD_CHECKS(_ret)     \
	  ((_ret == -ETIMEDOUT)            \
	|| (_ret == -EBUSY)                \
	|| (_ret == -ENOMSG)               \
	|| (_ret == -EAGAIN)               \
	|| (_ret == HAL_MAILBOX_MSG_SIZE))
#define MAILBOX_AWRITE_CHECKS(_ret)    \
	  ((_ret == -EAGAIN)               \
	|| (_ret == -EBUSY)                \
	|| (_ret == HAL_MAILBOX_MSG_SIZE))
/**@}*/


/**
 * @brief Auxiliar buffer.
 */
static char data[HAL_PORTAL_MAX_SIZE];

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
		KASSERT((mbxid = vsys_mailbox_create(local)) >= 0);
		KASSERT((portalid = vsys_portal_create(local)) >= 0);

		KASSERT(vsys_portal_unlink(portalid) == 0);
		KASSERT(vsys_mailbox_unlink(mbxid) == 0);
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
		KASSERT((mbxid = vsys_mailbox_open(remote)) >= 0);
		KASSERT((portalid = vsys_portal_open(local, remote)) >= 0);

		KASSERT(vsys_portal_close(portalid) == 0);
		KASSERT(vsys_mailbox_close(mbxid) == 0);
	}
}

/**
 * @brief Stress auxiliar: Sender rule
 */
PRIVATE void do_sender(int local, int remote)
{
	int ret;
	int mbxid;
	int portalid;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((mbxid = vsys_mailbox_open(remote)) >= 0);
		KASSERT((portalid = vsys_portal_open(local, remote)) >= 0);

		test_stress_barrier();

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (j % sizeof(char));
				do
				{
					ret = vsys_mailbox_awrite(mbxid, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AWRITE_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(mbxid) == 0);

				data[0] = ((j + 1) % sizeof(char));
				do
				{
					ret = vsys_portal_awrite(portalid, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AWRITE_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(portalid) == 0);
			}

		KASSERT(vsys_portal_close(portalid) == 0);
		KASSERT(vsys_mailbox_close(mbxid) == 0);

		test_stress_barrier();
	}
}

/**
 * @brief Stress auxiliar: Receiver rule
 */
PRIVATE void do_receiver(int local, int remote)
{
	int ret;
	int mbxid;
	int portalid;

	for (unsigned int i = 0; i < NSETUPS; ++i)
	{
		KASSERT((mbxid = vsys_mailbox_create(local)) >= 0);
		KASSERT((portalid = vsys_portal_create(local)) >= 0);

		test_stress_barrier();

			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (-1);
				do
				{
					ret = vsys_mailbox_aread(mbxid, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AREAD_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(mbxid) == 0);

				KASSERT(data[0] == (j % sizeof(char)));

				data[0] = (-1);
				KASSERT(vsys_portal_allow(portalid, remote) == 0);
				do
				{
					ret = vsys_portal_aread(portalid, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AREAD_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(portalid) == 0);

				KASSERT(data[0] == ((j + 1) % sizeof(char)));
			}

		KASSERT(vsys_portal_unlink(portalid) == 0);
		KASSERT(vsys_mailbox_unlink(mbxid) == 0);

		test_stress_barrier();
	}
}

/**
 * @brief Stress Test: Combination Broadcast
 */
PRIVATE void stress_combination_broadcast(void)
{
	if (processor_node_get_num() == NODENUM_MASTER)
		do_sender(NODENUM_MASTER, NODENUM_SLAVE);
	else
		do_receiver(NODENUM_SLAVE, NODENUM_MASTER);
}

/**
 * @brief Stress Test: Combination Gather
 */
PRIVATE void stress_combination_gather(void)
{
	if (processor_node_get_num() == NODENUM_MASTER)
		do_receiver(NODENUM_MASTER, NODENUM_SLAVE);
	else
		do_sender(NODENUM_SLAVE, NODENUM_MASTER);
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
		KASSERT((inbox = vsys_mailbox_create(local)) >= 0);
		KASSERT((outbox = vsys_mailbox_open(remote)) >= 0);
		KASSERT((inportal = vsys_portal_create(local)) >= 0);
		KASSERT((outportal = vsys_portal_open(local, remote)) >= 0);

		test_stress_barrier();

		if (local == NODENUM_MASTER)
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (-1);
				do
				{
					ret = vsys_mailbox_aread(inbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AREAD_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(inbox) == 0);

				KASSERT(data[0] == (j % sizeof(char)));

				data[0] = (-1);
				KASSERT(vsys_portal_allow(inportal, remote) == 0);
				do
				{
					ret = vsys_portal_aread(inportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AREAD_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(inportal) == 0);

				KASSERT(data[0] == ((j + 1) % sizeof(char)));

				data[0] = ((j + 2) % sizeof(char));
				do
				{
					ret = vsys_mailbox_awrite(outbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AWRITE_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(outbox) == 0);

				data[0] = ((j + 3) % sizeof(char));
				do
				{
					ret = vsys_portal_awrite(outportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AWRITE_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(outportal) == 0);
			}
		}
		else
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (j % sizeof(char));
				do
				{
					ret = vsys_mailbox_awrite(outbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AWRITE_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(outbox) == 0);

				data[0] = ((j + 1) % sizeof(char));
				do
				{
					ret = vsys_portal_awrite(outportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AWRITE_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(outportal) == 0);

				data[0] = (-1);
				do
				{
					ret = vsys_mailbox_aread(inbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AREAD_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(inbox) == 0);

				KASSERT(data[0] == ((j + 2) % sizeof(char)));

				data[0] = (-1);
				KASSERT(vsys_portal_allow(inportal, remote) == 0);
				do
				{
					ret = vsys_portal_aread(inportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AREAD_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(inportal) == 0);

				KASSERT(data[0] == ((j + 3) % sizeof(char)));
			}
		}

		KASSERT(vsys_portal_close(outportal) == 0);
		KASSERT(vsys_portal_unlink(inportal) == 0);
		KASSERT(vsys_mailbox_close(outbox) == 0);
		KASSERT(vsys_mailbox_unlink(inbox) == 0);

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
		KASSERT((inbox = vsys_mailbox_create(local)) >= 0);
		KASSERT((outbox = vsys_mailbox_open(remote)) >= 0);
		KASSERT((inportal = vsys_portal_create(local)) >= 0);
		KASSERT((outportal = vsys_portal_open(local, remote)) >= 0);

		test_stress_barrier();

		if (local == NODENUM_MASTER)
		{
			for (int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (-1);
				do
				{
					ret = vsys_mailbox_aread(inbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AREAD_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(inbox) == 0);

				KASSERT(data[0] == (j % sizeof(char)));

				data[0] = ((j + 1) % sizeof(char));
				do
				{
					ret = vsys_portal_awrite(outportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AWRITE_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(outportal) == 0);

				data[0] = ((j + 2) % sizeof(char));
				do
				{
					ret = vsys_mailbox_awrite(outbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AWRITE_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);

				KASSERT(vsys_mailbox_wait(outbox) == 0);

				data[0] = (-1);
				KASSERT(vsys_portal_allow(inportal, remote) == 0);
				do
				{
					ret = vsys_portal_aread(inportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AREAD_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(inportal) == 0);

				KASSERT(data[0] == ((j + 3) % sizeof(char)));
			}
		}
		else
		{
			for (unsigned int j = 0; j < NCOMMUNICATIONS; ++j)
			{
				data[0] = (j % sizeof(char));
				do
				{
					ret = vsys_mailbox_awrite(outbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AWRITE_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(outbox) == 0);

				data[0] = (-1);
				KASSERT(vsys_portal_allow(inportal, remote) == 0);
				do
				{
					ret = vsys_portal_aread(inportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AREAD_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(inportal) == 0);

				KASSERT(data[0] == ((j + 1) % sizeof(char)));

				data[0] = (-1);
				do
				{
					ret = vsys_mailbox_aread(inbox, data, HAL_MAILBOX_MSG_SIZE);
					KASSERT(MAILBOX_AREAD_CHECKS(ret));
				} while (ret != HAL_MAILBOX_MSG_SIZE);
				KASSERT(vsys_mailbox_wait(inbox) == 0);

				KASSERT(data[0] == ((j + 2) % sizeof(char)));

				data[0] = ((j + 3) % sizeof(char));
				do
				{
					ret = vsys_portal_awrite(outportal, data, HAL_PORTAL_MAX_SIZE);
					KASSERT(PORTAL_AWRITE_CHECKS(ret));
				} while (ret != HAL_PORTAL_MAX_SIZE);
				KASSERT(vsys_portal_wait(outportal) == 0);
			}
		}

		KASSERT(vsys_portal_close(outportal) == 0);
		KASSERT(vsys_portal_unlink(inportal) == 0);
		KASSERT(vsys_mailbox_close(outbox) == 0);
		KASSERT(vsys_mailbox_unlink(inbox) == 0);

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
