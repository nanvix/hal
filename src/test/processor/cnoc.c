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

#ifdef __mppa256__

/**
 * @brief Tests arguments
 */
#define NOC_ITERATIONS (10)

/**
 * @name Tests arguments
 */
/**@{*/
#define INTERFACE (0)                           /**< DMA channel used.             */
#define RX_TAG    (120)                         /**< Receiver identificantion tag. */
#define RX_MASK   (0x1)                         /**< Receiver default mask.        */
#define TX_TAG    (BOSTAN_MAILBOX_CNOC_TX_BASE) /**< Transfer identificantion tag. */
#define TX_MASK   (~RX_MASK)                    /**< Transfer default mask.        */
/**@}*/

/**
 * @brief Lock used for wait the completion of Control NoC operations.
 */
PRIVATE spinlock_t test_cnoc_lock = K1B_SPINLOCK_LOCKED;

/**
 * @brief Interrupt handler: Control NoC handler to perform the unlock.
 */
PRIVATE void test_cnoc_dummy_handler(int interface, int tag)
{
	KASSERT(interface == INTERFACE);
	KASSERT(tag       == RX_TAG);

	spinlock_unlock(&test_cnoc_lock);
}

/*============================================================================*
 * Control API Tests                                                          *
 *============================================================================*/

/**
 * @brief API Test: Synchronization Point Create Unlink
 */
PRIVATE void test_cnoc_create_unlink(void)
{
	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	KASSERT(bostan_dma_control_config(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point Open Close
 */
PRIVATE void test_cnoc_open_close(void)
{
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);

	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Events
 */
PRIVATE void test_cnoc_loopback_with_events(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	KASSERT(
		bostan_dma_control_signal(
			INTERFACE,
			TX_TAG,
			&local,
			1,
			RX_TAG,
			TX_MASK
		) == 0
	);

	KASSERT(bostan_dma_control_wait(INTERFACE, RX_TAG) == 0);

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Interrupts
 */
PRIVATE void test_cnoc_loopback_with_interrupts(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, test_cnoc_dummy_handler) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	interrupts_enable();

		KASSERT(
			bostan_dma_control_signal(
				INTERFACE,
				TX_TAG,
				&local,
				1,
				RX_TAG,
				TX_MASK
			) == 0
		);

		spinlock_lock(&test_cnoc_lock);

	interrupts_disable();

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Events
 */
PRIVATE void test_cnoc_mailbox_with_events(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_MAILBOX_MODE, 0ULL, NULL) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	KASSERT(
		bostan_dma_control_signal(
			INTERFACE,
			TX_TAG,
			&local,
			1,
			RX_TAG,
			0xdeadbeef
		) == 0
	);

	KASSERT(bostan_dma_control_wait(INTERFACE, RX_TAG) == 0);

	KASSERT(bostan_dma_control_read(INTERFACE, RX_TAG) == 0xdeadbeef);

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Interrupts
 */
PRIVATE void test_cnoc_mailbox_with_interrupts(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_MAILBOX_MODE, 0ULL, test_cnoc_dummy_handler) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	interrupts_enable();

		KASSERT(
			bostan_dma_control_signal(
				INTERFACE,
				TX_TAG,
				&local,
				1,
				RX_TAG,
				0xdeadbeef
			) == 0
		);

		spinlock_lock(&test_cnoc_lock);

	interrupts_disable();

	KASSERT(bostan_dma_control_read(INTERFACE, RX_TAG) == 0xdeadbeef);

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Events
 */
static void test_cnoc_stress_with_events(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	for (int i = 0; i < NOC_ITERATIONS; ++i)
	{
		KASSERT(
			bostan_dma_control_signal(
				INTERFACE,
				TX_TAG,
				&local,
				1,
				RX_TAG,
				TX_MASK
			) == 0
		);

		KASSERT(bostan_dma_control_wait(INTERFACE, RX_TAG) == 0);

		KASSERT(bostan_dma_control_config(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, NULL) == 0);
	}

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Synchronization Point With Interrupts
 */
static void test_cnoc_stress_with_interrupts(void)
{
	int local;

	local = processor_node_get_num();

	KASSERT(bostan_dma_control_create(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, test_cnoc_dummy_handler) == 0);
	KASSERT(bostan_dma_control_open(INTERFACE, TX_TAG) == 0);

	interrupts_enable();

		for (int i = 0; i < NOC_ITERATIONS; ++i)
		{
			KASSERT(
				bostan_dma_control_signal(
					INTERFACE,
					TX_TAG,
					&local,
					1,
					RX_TAG,
					TX_MASK
				) == 0
			);

			spinlock_lock(&test_cnoc_lock);

			KASSERT(bostan_dma_control_config(INTERFACE, RX_TAG, BOSTAN_CNOC_BARRIER_MODE, RX_MASK, test_cnoc_dummy_handler) == 0);
		}

	interrupts_disable();

	KASSERT(bostan_dma_control_close(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_control_unlink(INTERFACE, RX_TAG) == 0);
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
struct test cnoc_tests_api[] = {
	/* Intra-Cluster API Tests */
	{test_cnoc_create_unlink,            "Create Unlink                    " },
	{test_cnoc_open_close,               "Open Close                       " },
	{test_cnoc_loopback_with_events,     "Loopback a signal with events    " },
	{test_cnoc_loopback_with_interrupts, "Loopback a signal with interrupts" },
	{test_cnoc_mailbox_with_events,      "Mailbox a signal with events     " },
	{test_cnoc_mailbox_with_interrupts,  "Mailbox a signal with interrupts " },
	{test_cnoc_stress_with_events,       "Stress a signal with events      " },
	{test_cnoc_stress_with_interrupts,   "Stress a signal with interrupts  " },
	{NULL,                               NULL                                },
};

/**
 * The test_cnoc() function launches testing units on the cnoc
 * interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_cnoc(void)
{
	/* API Tests */
	CLUSTER_KPRINTF(HLINE);
	for (int i = 0; cnoc_tests_api[i].test_fn != NULL; i++)
	{
		cnoc_tests_api[i].test_fn();
		CLUSTER_KPRINTF("[test][api][noc][control] %s [passed]", cnoc_tests_api[i].name);
	}
}

#endif
