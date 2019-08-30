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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>
#include "../test.h"

#ifdef __mppa256__

/**
 * @name Tests arguments
 */
/**@{*/
#define INTERFACE   0        /**< DMA channel used.             */
#define RX_TAG      58       /**< Receiver identificantion tag. */
#define TX_TAG      0        /**< Transfer identificantion tag. */
#define BUFFER_MAX_SIZE 1024 /**< Maximum data exchange size.   */
/**@}*/

/**
 * @name Data exchange sizes
 */
/**@{*/
PRIVATE uint64_t BUFFER_SIZE = 50;
PRIVATE uint64_t data_exchange_sizes[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 120, 30, 40, 50, 60, 70, 80, 90,
	100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
	15, 31, 47, 127, 255, 511, 1023,
	16, 32, 48, 128, 256, 512, 1024,
	17, 33, 49, 129, 257, 513
};
/**@}*/

/**
 * @brief Lock used for wait the completion of Data NoC operations.
 */
PRIVATE spinlock_t test_dnoc_lock = K1B_SPINLOCK_LOCKED;

/**
 * @brief Interrupt handler: Data NoC handler to perform the unlock.
 */
PRIVATE void test_dnoc_dummy_handler(int interface, int tag)
{
	UNUSED(tag);
	UNUSED(interface);

	spinlock_unlock(&test_dnoc_lock);
}

/*============================================================================*
 * Control API Tests                                                          *
 *============================================================================*/

/**
 * @brief API Test: Data Exchange Create Unlink
 */
PRIVATE void test_dnoc_create_unlink(void)
{
	KASSERT(bostan_dma_data_create(INTERFACE, RX_TAG) == 0);
	KASSERT(bostan_dma_data_unlink(INTERFACE, RX_TAG) == 0);

	KASSERT(bostan_dma_data_create(INTERFACE, RX_TAG) == 0);
	KASSERT(bostan_dma_data_unlink(INTERFACE, RX_TAG) == 0);
}

/**
 * @brief API Test: Data Exchange Open Close
 */
PRIVATE void test_dnoc_open_close(void)
{
	KASSERT(bostan_dma_data_open(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_data_close(INTERFACE, TX_TAG) == 0);

	KASSERT(bostan_dma_data_open(INTERFACE, TX_TAG) == 0);
	KASSERT(bostan_dma_data_close(INTERFACE, TX_TAG) == 0);
}

/**
 * @brief API Test: Data Exchange With Events
 */
PRIVATE void test_dnoc_loopback_with_events(void)
{
	int local;
	char rx_buffer[BUFFER_MAX_SIZE];
	char tx_buffer[BUFFER_MAX_SIZE];

	local = processor_node_get_num();

	kmemset(rx_buffer, 0, BUFFER_MAX_SIZE);
	kmemset(tx_buffer, 1, BUFFER_MAX_SIZE);

	for (uint64_t i = 0; i < BUFFER_SIZE; i++)
		KASSERT(rx_buffer[i] != tx_buffer[i]);

	KASSERT(bostan_dma_data_create(INTERFACE, RX_TAG) == 0);
	KASSERT(
		bostan_dma_data_aread(
			INTERFACE,
			RX_TAG,
			rx_buffer,
			BUFFER_SIZE,
			BUFFER_SIZE,
			0,
			NULL
		) == 0
	);

	KASSERT(bostan_dma_data_open(INTERFACE, TX_TAG) == 0);
#if 1
	KASSERT(
		bostan_dma_data_write(
			INTERFACE,
			TX_TAG,
			local,
			RX_TAG,
			tx_buffer,
			BUFFER_SIZE,
			0
		) == 0
	);
#else
	KASSERT(
		bostan_dma_data_awrite(
			INTERFACE,
			TX_TAG,
			local,
			RX_TAG,
			tx_buffer,
			BUFFER_SIZE,
			0
		) == 0
	);

	KASSERT(bostan_dma_data_wait_write(INTERFACE, TX_TAG) == 0);
#endif
	KASSERT(bostan_dma_data_close(INTERFACE, TX_TAG) == 0);

	KASSERT(bostan_dma_data_wait_read(INTERFACE, RX_TAG) == 0);
	KASSERT(bostan_dma_data_unlink(INTERFACE, RX_TAG) == 0);

	for (uint64_t i = 0; i < BUFFER_SIZE; i++)
		KASSERT(rx_buffer[i] == tx_buffer[i]);
}

/**
 * @brief API Test: Data Exchange With Interrupts
 */
PRIVATE void test_dnoc_loopback_with_interrupts(void)
{
	int local;
	char rx_buffer[BUFFER_MAX_SIZE];
	char tx_buffer[BUFFER_MAX_SIZE];

	local = processor_node_get_num();

	kmemset(rx_buffer, 0, BUFFER_MAX_SIZE);
	kmemset(tx_buffer, 1, BUFFER_MAX_SIZE);

	for (uint64_t i = 0; i < BUFFER_SIZE; i++)
		KASSERT(rx_buffer[i] != tx_buffer[i]);

	KASSERT(bostan_dma_data_create(INTERFACE, RX_TAG) == 0);
	KASSERT(
		bostan_dma_data_aread(
			INTERFACE,
			RX_TAG,
			rx_buffer,
			BUFFER_SIZE,
			BUFFER_SIZE,
			0,
			test_dnoc_dummy_handler
		) == 0
	);

	interrupts_enable();

		KASSERT(bostan_dma_data_open(INTERFACE, TX_TAG) == 0);
#if 1
	KASSERT(
		bostan_dma_data_write(
			INTERFACE,
			TX_TAG,
			local,
			RX_TAG,
			tx_buffer,
			BUFFER_SIZE,
			0
		) == 0
	);
#else
	KASSERT(
		bostan_dma_data_awrite(
			INTERFACE,
			TX_TAG,
			local,
			RX_TAG,
			tx_buffer,
			BUFFER_SIZE,
			0
		) == 0
	);

	KASSERT(bostan_dma_data_wait_write(INTERFACE, TX_TAG) == 0);
#endif
		KASSERT(bostan_dma_data_close(INTERFACE, TX_TAG) == 0);

		spinlock_lock(&test_dnoc_lock);

	interrupts_disable();

	KASSERT(bostan_dma_data_unlink(INTERFACE, RX_TAG) == 0);

	for (uint64_t i = 0; i < BUFFER_SIZE; i++)
		KASSERT(rx_buffer[i] == tx_buffer[i]);
}

/**
 * @brief API Test: Data Exchange With Offset
 */
PRIVATE void test_dnoc_loopback_with_offset(void)
{
	int local;
	uint64_t offset;
	char rx_buffer[100];
	char tx_buffer[10];

	offset = 10;
	local = processor_node_get_num();

	kmemset(rx_buffer, 0, 100);
	kmemset(rx_buffer, 1, 10);

	for (uint64_t i = 0; i < 100; i++)
		KASSERT(rx_buffer[i] != tx_buffer[0]);

	KASSERT(bostan_dma_data_create(INTERFACE, RX_TAG) == 0);
	KASSERT(
		bostan_dma_data_aread(
			INTERFACE,
			RX_TAG,
			rx_buffer,
			10,
			100,
			0,
			NULL
		) == 0
	);

	KASSERT(bostan_dma_data_open(INTERFACE, TX_TAG) == 0);
	KASSERT(
		bostan_dma_data_write(
			INTERFACE,
			TX_TAG,
			local,
			RX_TAG,
			tx_buffer,
			10,
			offset
		) == 0
	);
	KASSERT(bostan_dma_data_close(INTERFACE, TX_TAG) == 0);

	KASSERT(bostan_dma_data_wait_read(INTERFACE, RX_TAG) == 0);
	KASSERT(bostan_dma_data_unlink(INTERFACE, RX_TAG) == 0);

	for (uint64_t i = 0; i < offset; i++)
		KASSERT(rx_buffer[i] != tx_buffer[i]);

	for (uint64_t i = 0; i < 10; i++)
		KASSERT(rx_buffer[offset + i] == tx_buffer[i]);
}

/**
 * @brief API Test: Stress Data Exchange
 */
PRIVATE void test_dnoc_stress_loopback(void)
{
	/* API Tests */
	for (uint64_t i = 0; i < sizeof(data_exchange_sizes)/sizeof(uint64_t); i++)
	{
		BUFFER_SIZE = data_exchange_sizes[i];
		test_dnoc_loopback_with_events();
	}

	/* API Tests */
	for (uint64_t i = 0; i < sizeof(data_exchange_sizes)/sizeof(uint64_t); i++)
	{
		BUFFER_SIZE = data_exchange_sizes[i];
		test_dnoc_loopback_with_interrupts();
	}
}

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test dnoc_tests_api[] = {
	{ test_dnoc_create_unlink,            "create unlink                           " },
	{ test_dnoc_open_close,               "open close                              " },
	{ test_dnoc_loopback_with_events,     "loopback a data exchange with events    " },
	{ test_dnoc_loopback_with_interrupts, "loopback a data exchange with interrupts" },
	{ test_dnoc_loopback_with_offset,     "loopback a data exchange with offset    " },
	{ test_dnoc_stress_loopback,          "stress loopback data exchange           " },
	{ NULL,                                NULL                                      },
};

/**
 * The test_dnoc() function launches testing units on the dnoc
 * interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_dnoc(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; dnoc_tests_api[i].test_fn != NULL; i++)
	{
		dnoc_tests_api[i].test_fn();
		kprintf("[test][api][noc][data] %s [passed]", dnoc_tests_api[i].name);
	}
}

#endif
