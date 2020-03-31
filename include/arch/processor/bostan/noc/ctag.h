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

#ifndef PROCESSOR_BOSTAN_NOC_CTAG_H_
#define PROCESSOR_BOSTAN_NOC_CTAG_H_

/**
 * @addtogroup processor-bostan-noc
 * @ingroup processor-bostan
 */
/**@*/

	/* Cluster API. */
	#include <arch/processor/bostan/_bostan.h>

	#include <arch/processor/bostan/noc/tag.h>

/*============================================================================*
 * Constants                                                                  *
 *============================================================================*/

	/**
	 * @name Type of C-NoC buffers
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_TYPE BOSTAN_PROCESSOR_NOC_RX_TYPE /**< C-NoC receiver type. */
	#define BOSTAN_CNOC_TX_TYPE BOSTAN_PROCESSOR_NOC_TX_TYPE /**< C-NoC transfer type. */
	/**@}*/

    /**
	 * @name Control NoC Modes
	 */
	/**@{*/
    #define BOSTAN_CNOC_MAILBOX_MODE 0 /**< Mailbox mode */
    #define BOSTAN_CNOC_BARRIER_MODE 1 /**< Barrier mode */
    /**@}*/

	/**
	 * @name Amount of the C-NoC Resources
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_MAX 128 /**< Receiver amount. */
    #define BOSTAN_CNOC_TX_MAX 4   /**< Sender amount.   */
	/**@}*/

	/**
	 * @name Initial C-NoC tag.
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_BASE BOSTAN_PROCESSOR_NOC_RESERVED_RXS_NUM /**< Receiver tag offset. */
	#define BOSTAN_CNOC_TX_BASE BOSTAN_PROCESSOR_NOC_RESERVED_TXS_NUM /**< Transfer tag offset. */
	/**@}*/

	/**
	 * @name Number of C-NoC buffer
	 */
	/**@{*/
	#define BOSTAN_CNOC_RXS_NUM (BOSTAN_CNOC_RX_MAX - BOSTAN_CNOC_RX_BASE) /**< Number of receive buffers.  */
	#define BOSTAN_CNOC_TXS_NUM (BOSTAN_CNOC_TX_MAX - BOSTAN_CNOC_TX_BASE) /**< Number of transfer buffers. */
	/**@}*/

	/**
	 * @name Identifies C-NoC transfer tag reserved for Communication services.
	 * 
	 * @details Portal abstraction has 2 TX avaiable {2, 3}.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_CNOC_TX_BASE 0 /**< C-NoC Transfer Tag reserved for Mailbox. */
	#define BOSTAN_SYNC_CNOC_TX_BASE    1 /**< C-NoC Transfer Tag reserved for Sync.    */
	#define BOSTAN_PORTAL_CNOC_TX_BASE  2 /**< C-NoC Transfer Tag reserved for Portal.  */
//! #define BOSTAN_PORTAL_CNOC_TX_BASE  3 /**< C-NoC Transfer Tag reserved for Portal.  */
	/**@}*/

	/**
	 * @brief Initializes the control structures and configures the
	 * interrupt handlers.
	 */
	EXTERN void bostan_cnoc_setup(void);

	/**
	 * @brief Verify lost interrupts.
	 */
	EXTERN void bostan_cnoc_it_verify(void);

/*============================================================================*
 * C-Noc Receiver Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Allocate C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_rx_alloc(int interface, int tag);

	/**
	 * @brief Free C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 * 
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_rx_free(int interface, int tag);

	/**
	 * @brief Clear events of C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 */
	EXTERN void bostan_cnoc_rx_clear(int interface, int tag);

	/**
	 * @brief Wait events on C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 *
	 * @return Zero if wait successfully and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_rx_wait(int interface, int tag);

	/**
	 * @brief Configure C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 * @param mode      C-NoC receiver mode.
	 * @param mask      Initial value of the buffer.
	 * @param handler   Interrupt handler (If NULL then uses events).
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
    EXTERN int bostan_cnoc_rx_config(
		int interface,
		int tag,
		int mode,
		uint64_t mask,
		bostan_processor_noc_handler_fn handler
	);

/*============================================================================*
 * C-Noc Transfer Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Allocate C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_tx_alloc(int interface, int tag);

	/**
	 * @brief Free C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of transfer buffer.
	 * 
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_tx_free(int interface, int tag);

	/**
	 * @brief Write into C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number tag.
	 * @param value     Data to send.
	 * 
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	EXTERN int bostan_cnoc_tx_write(int interface, int tag, uint64_t value);

	/**
	 * @brief Configure C-NoC transfer buffer.
	 *
	 * @param interface  Number of the DMA channel.
	 * @param localid    Source interface ID.
	 * @param local_tag  Number of source buffer.
	 * @param remoteid   Target interface ID.
	 * @param remote_tag Number of target buffer.
	 *
	 * @return Zero if configure successfully and non zero otherwise.
	 */
    EXTERN int bostan_cnoc_tx_config(
		int interface,
		int localid,
		int local_tag,
		int remoteid,
		int remote_tag
	);

/**@}*/

#endif /* PROCESSOR_BOSTAN_NOC_CTAG_H_ */

