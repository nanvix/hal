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

#ifndef PROCESSOR_BOSTAN_NOC_DTAG_H_
#define PROCESSOR_BOSTAN_NOC_DTAG_H_

/**
 * @addtogroup processor-bostan-noc
 * @ingroup processor-bostan
 */
/**@*/

	/* Processor Interface Implementation */
	#include <arch/processor/bostan/_bostan.h>

	#include <arch/processor/bostan/noc/tag.h>

/*============================================================================*
 * Constants                                                                  *
 *============================================================================*/

	/**
	 * @name Type of D-NoC Tag
	 */
	/**@{*/
	#define BOSTAN_DNOC_RX_TYPE (BOSTAN_PROCESSOR_NOC_RX_TYPE)     /**< D-NoC receive tag.  */
	#define BOSTAN_DNOC_TX_TYPE (BOSTAN_PROCESSOR_NOC_TX_TYPE)     /**< D-NoC transfer tag. */
	#define BOSTAN_DNOC_UC_TYPE (BOSTAN_PROCESSOR_NOC_TX_TYPE + 1) /**< D-NoC ucore tag.    */
	/**@}*/

	/**
	 * @name Amount of the Data NoC Resources
	 */
	/**@{*/
	#define BOSTAN_DNOC_RX_MAX 256 /**< Receiver amount. */
	#define BOSTAN_DNOC_TX_MAX 8   /**< Sender amount.   */
	#define BOSTAN_DNOC_UC_MAX 8   /**< ucore amount.    */
	/**@}*/

	/**
	 * @name Initial D-NoC tag
	 */
	/**@{*/
	#define BOSTAN_DNOC_RX_BASE BOSTAN_PROCESSOR_NOC_RESERVED_RXS_NUM /**< Receive tag offset.  */
	#define BOSTAN_DNOC_TX_BASE BOSTAN_PROCESSOR_NOC_RESERVED_TXS_NUM /**< Transfer tag offset. */
	#define BOSTAN_DNOC_UC_BASE BOSTAN_PROCESSOR_NOC_RESERVED_TXS_NUM /**< UCore thread offset. */
	/**@}*/

	/**
	 * @name Number of D-NoC buffer
	 */
	/**@{*/
	#define BOSTAN_DNOC_RXS_NUM (BOSTAN_DNOC_RX_MAX - BOSTAN_DNOC_RX_BASE) /**< Number of receive buffers.  */
	#define BOSTAN_DNOC_TXS_NUM (BOSTAN_DNOC_TX_MAX - BOSTAN_DNOC_TX_BASE) /**< Number of transfer buffers. */
	#define BOSTAN_DNOC_UCS_NUM (BOSTAN_DNOC_UC_MAX - BOSTAN_DNOC_UC_BASE) /**< Number of ucore threads.	*/
	/**@}*/

	/**
	 * @brief Number of D-NoC transfer per communication service.
	 */
	#define BOSTAN_DNOC_TXS_PER_COMM_SERVICE 4

	/**
	 * @name Identifies D-NoC transfer tag reserved for Communication services.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_DNOC_TX_BASE  0                                                               /**< D-NoC Transfer Tag reserved for Mailbox. */
	#define BOSTAN_PORTAL_DNOC_TX_BASE  (BOSTAN_MAILBOX_DNOC_TX_BASE + BOSTAN_DNOC_TXS_PER_COMM_SERVICE) /**< D-NoC Transfer Tag reserved for Portal.  */
	/**@}*/

	/**
	 * @brief Initializes the control structures and configures the
	 * interrupt handlers.
	 */
	EXTERN void bostan_dnoc_setup(void);

/*============================================================================*
 * D-Noc Receiver Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Allocate D-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof receiver buffer.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_rx_alloc(int interface, int tag);

	/**
	 * @brief Free D-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof receiver buffer.
	 */
	EXTERN int bostan_dnoc_rx_free(int interface, int tag);

	/**
	 * @brief Wait events on D-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number tag.
	 *
	 * @return Amount of received events.
	 */
	EXTERN int bostan_dnoc_rx_wait(int interface, int tag);

	/**
	 * @brief Configure D-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number of receiver buffer.
	 * @param buffer    Local data pointer.
	 * @param min_size  Minimal value to generate an event (in bytes).
	 * @param max_size  Size of the receiver buffer (in bytes).
	 * @param offset    Offset in receiver buffer where data shall be written.
	 * @param handler   Interrupt handler (If NULL then uses events).
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_rx_config(
		int interface,
		int tag,
		void *buffer,
		uint64_t min_size,
		uint64_t max_size,
		uint64_t offset,
		bostan_processor_noc_handler_fn handler
	);

/*============================================================================*
 * D-NoC Transfer Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Allocate D-NoC transfer buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof transfer buffer.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_tx_alloc(int interface, int tag);

	/**
	 * @brief Free D-NoC transfer buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number of transfer buffer.
	 * 
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_tx_free(int interface, int tag);

	/**
	 * @brief Configure D-NoC transfer buffer.
	 *
	 * @param interface  Number of the interface.
	 * @param localid    Source interface ID.
	 * @param local_tag  Number of source buffer.
	 * @param remoteid   Target interface ID.
	 * @param remote_tag Number of target buffer.
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_tx_config(
		int interface,
		int localid,
		int local_tag,
		int remoteid,
		int remote_tag
	);

	/**
	 * @brief Writes on D-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Transfer buffer ID.
	 * @param buffer    Local data pointer.
	 * @param size      Amount of bytes to transfer.
	 * @param offset    Offset on local buffer data.
	 */
	EXTERN int bostan_dnoc_tx_write(
		int interface,
		int tag,
		const void *buffer,
		uint64_t size,
		uint64_t offset
	);

/*============================================================================*
 * D-Noc Ucore Interface                                                      *
 *============================================================================*/

	/**
	 * @brief Allocate D-NoC ucore sender.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof of ucore.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_uc_alloc(int interface, int uctag, int txtag);

	/**
	 * @brief Free D-NoC ucore sender.
	 *
	 * @param interface Number of the interface.
	 * @param uctag     Number of ucore.
	 * @param txtag     Number of transfer buffer.
	 * 
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_dnoc_uc_free(int interface, int uctag, int txtag);

	/**
	 * @brief Wait events on D-NoC ucore sender.
	 *
	 * @param interface Number of the interface.
	 * @param uctag     Number of ucore.
	 *
	 * @return Amount of received events.
	 */
	EXTERN int bostan_dnoc_uc_wait(int interface, int uctag);

	/**
	 * @brief Configure and async writes on D-NoC ucore sender.
	 *
	 * @param interface   Number of the DMA channel.
	 * @param localid Local Node ID.
	 * @param uctag       Ucore thread ID.
	 * @param txtag       Transfer buffer ID.
	 * @param remoteid Target Node ID.
	 * @param remote_tag  Target receiver buffer.
	 * @param buffer      Local data pointer.
	 * @param size        Amount of bytes to transfer.
	 */
	EXTERN int bostan_dnoc_uc_config_write(
		int interface,
		int localid,
		int uctag,
		int txtag,
		int remoteid,
		int remote_tag,
		const void *buffer,
		uint64_t size,
		uint64_t offset
	);

/**@}*/

#endif /* PROCESSOR_BOSTAN_NOC_DTAG_H_ */
