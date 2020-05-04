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

#ifndef PROCESSOR_BOSTAN_NOC_DMA_H_
#define PROCESSOR_BOSTAN_NOC_DMA_H_

/**
 * @addtogroup processor-bostan-noc
 * @ingroup processor-bostan
 */
/**@*/

/* Processor Interface Implementation */
#include <arch/processor/bostan/_bostan.h>

/* NoC Interface Implementation */
#include <arch/processor/bostan/noc/ctag.h>
#include <arch/processor/bostan/noc/dtag.h>

/* Error Identification */
#include <posix/errno.h>

/*============================================================================*
 * DMA configuration                                                          *
 *============================================================================*/

	/**
	 * The bostan_dma_init() function initializes the control
	 * structures and configures the interrupt handler of the
	 * Control and Data NoC.
	 */
	static inline void bostan_dma_init(void)
	{
		bostan_cnoc_setup();
		bostan_dnoc_setup();
	}

/*============================================================================*
 * Control DMA Interface                                                      *
 *============================================================================*/

	/**
	 * @brief Allocates and configures the control receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control receiver buffer.
	 * @param mode      C-NoC receiver mode.
	 * @param mask      Initial value of the buffer.
	 * @param handler   Interrupt handler to receive the signal (if NULL use events).
	 *
	 * @return Zero if create successfully and non zero otherwise.
	 */
	EXTERN int bostan_dma_control_create(
		int interface,
		int tag,
		int mode,
		uint64_t mask,
		bostan_processor_noc_handler_fn handler
	);

	/**
	 * @brief Re-configure C-NoC control receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control receiver buffer.
	 * @param mode      C-NoC receiver mode.
	 * @param mask      Initial value of the buffer.
	 * @param handler   Interrupt handler to receive the signal (if NULL use events).
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	static inline int bostan_dma_control_config(
		int interface,
		int tag,
		int mode,
		uint64_t mask,
		bostan_processor_noc_handler_fn handler
	)
	{
		return bostan_cnoc_rx_config(
			interface,
			tag,
			mode,
			mask,
			handler
		);
	}

	/**
	 * @brief Allocates the control transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control transfer buffer.
	 *
	 * @return Zero if open successfully and non zero otherwise.
	 */
	static inline int bostan_dma_control_open(int interface, int tag)
	{
		return bostan_cnoc_tx_alloc(interface, tag);
	}

	/**
	 * @brief Releases and cleans the control receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control transfer buffer.
	 *
	 * @return Zero if unlink successfully and non zero otherwise.
	 */
	static inline int bostan_dma_control_unlink(int interface, int tag)
	{
		bostan_cnoc_rx_clear(interface, tag);

		return bostan_cnoc_rx_free(interface, tag);
	}

	/**
	 * @brief Releases the control transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control transfer buffer.
	 *
	 * @return Zero if close successfully and non zero otherwise.
	 */
	static inline int bostan_dma_control_close(int interface, int tag)
	{
		return bostan_cnoc_tx_free(interface, tag);
	}

	/**
	 * @brief Wait an event on the control receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the control transfer buffer.
	 *
	 * @return Zero if wait successfully and non zero otherwise.
	 */
	static inline int bostan_dma_control_wait(int interface, int tag)
	{
		return bostan_cnoc_rx_wait(interface, tag);
	}

	/**
	 * @brief Configure and send a signal on a control transfer buffer.
	 *
	 * @param interface  Number of the DMA channel.
	 * @param tag        Number of the control transfer buffer.
	 * @param remotes    Target Node IDs.
	 * @param nremotes   Amount of targets.
	 * @param remote_tag Number of the target control receiver buffer.
	 * @param mask       Signal value.
	 *
	 * @return Zero if send successfully and non zero otherwise.
	 */
	EXTERN int bostan_dma_control_signal(
		int interface,
		int tag,
		const int * remotes,
		int nremotes,
		const int * remotes_tags,
		uint64_t mask
	);

/*============================================================================*
 * Data DMA Interface                                                         *
 *============================================================================*/

	/**
	 * @brief Allocates the data receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data receiver buffer.
	 *
	 * @return Zero if create successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_create(int interface, int tag)
	{
		return bostan_dnoc_rx_alloc(interface, tag);
	}

	/**
	 * @brief Allocates the data transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data transfer buffer.
	 *
	 * @return Zero if open successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_open(int interface, int tag)
	{
		return bostan_dnoc_uc_alloc(interface, tag, tag);
	}

	/**
	 * @brief Releases and cleans the data receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data transfer buffer.
	 *
	 * @return Zero if unlink successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_unlink(int interface, int tag)
	{
		return bostan_dnoc_rx_free(interface, tag);
	}

	/**
	 * @brief Releases the data transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data transfer buffer.
	 *
	 * @return Zero if close successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_close(int interface, int tag)
	{
		return bostan_dnoc_uc_free(interface, tag, tag);
	}

	/**
	 * @brief Wait an event on the data receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data transfer buffer.
	 *
	 * @return Zero if wait successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_wait_read(int interface, int tag)
	{
		return bostan_dnoc_rx_wait(interface, tag);
	}

	/**
	 * @brief Wait an event on the data sender buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the data transfer buffer.
	 *
	 * @return Zero if wait successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_wait_write(int interface, int tag)
	{
		return bostan_dnoc_uc_wait(interface, tag);
	}

	/**
	 * @brief Configure an async read on local buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number of receiver buffer.
	 * @param buffer    Local data pointer.
	 * @param min_size  Minimal value to generate an event (in bytes).
	 * @param max_size  Size of the receiver buffer (in bytes).
	 * @param offset    Offset in receiver buffer where data shall be written.
	 * @param handler   Interrupt handler to receive the data (if NULL use events).
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	static inline int bostan_dma_data_aread(
		int interface,
		int tag,
		void * buffer,
		uint64_t min_size,
		uint64_t max_size,
		uint64_t offset,
		bostan_processor_noc_handler_fn handler
	)
	{
		return bostan_dnoc_rx_config(
			interface,
			tag,
			buffer,
			min_size,
			max_size,
			offset,
			handler
		);
	}

	/**
	 * @brief Configure and async write to the target node.
	 *
	 * @param interface   Number of the DMA channel.
	 * @param tag         Number of the data transfer buffer.
	 * @param remote      Logic ID of the Target Node.
	 * @param remote_tag  Target receiver buffer.
	 * @param buffer      Local data pointer.
	 * @param size        Amount of bytes to transfer.
	 * @param offset      Offset on target buffer.
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	EXTERN int bostan_dma_data_awrite(
		int interface,
		int tag,
		int remote,
		int remote_tag,
		const void * buffer,
		uint64_t size,
		uint64_t offset
	);

	/**
	 * @brief Configure and write to the target node.
	 *
	 * @param interface   Number of the DMA channel.
	 * @param tag         Number of the data transfer buffer.
	 * @param remote      Logic ID of the Target Node.
	 * @param remote_tag  Target receiver buffer.
	 * @param buffer      Local data pointer.
	 * @param size        Amount of bytes to transfer.
	 * @param offset      Offset on target buffer
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	EXTERN int bostan_dma_data_write(
		int interface,
		int tag,
		int remote,
		int remote_tag,
		const void * buffer,
		uint64_t size,
		uint64_t offset
	);

/**@}*/

#endif /* PROCESSOR_BOSTAN_NOC_DMA_H_ */
