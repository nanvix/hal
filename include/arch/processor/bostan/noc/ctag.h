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

#ifndef PROCESSOR_BOSTAN_NOC_CTAG_H_
#define PROCESSOR_BOSTAN_NOC_CTAG_H_

/**
 * @cond bostan-noc
 */

	/* Cluster API. */
	#include <arch/processor/bostan/_bostan.h>

	#include <arch/processor/bostan/noc/tag.h>
	#include <mOS_vcore_u.h>
	#include <mOS_mailbox_u.h>
	#include <mppa_noc.h>
	#include <mppa_routing.h>

/*============================================================================*
 * Constants                                                                  *
 *============================================================================*/

	/**
	 * @name Type of C-NoC buffers
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_TYPE BOSTAN_NOC_RX_TYPE /**< C-NoC receiver type. */
	#define BOSTAN_CNOC_TX_TYPE BOSTAN_NOC_TX_TYPE /**< C-NoC transfer type. */
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
    #define BOSTAN_CNOC_TX_MAX 1   /**< Sender amount.   */
	/**@}*/

	/**
	 * @name Initial C-NoC tag.
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_BASE BOSTAN_NR_RESERVED_TAGS /**< Receiver tag offset. */
	#define BOSTAN_CNOC_TX_BASE BOSTAN_NR_RESERVED_TAGS /**< Transfer tag offset. */
	/**@}*/

	/**
	 * @name Number of C-NoC buffer
	 */
	/**@{*/
	#define BOSTAN_NR_CNOC_RX (BOSTAN_CNOC_RX_MAX - BOSTAN_CNOC_RX_BASE) /**< Number of receive buffers.  */
	#define BOSTAN_NR_CNOC_TX (BOSTAN_CNOC_TX_MAX - BOSTAN_CNOC_TX_BASE) /**< Number of transfer buffers. */
	/**@}*/

/*============================================================================*
 * C-Noc Receiver Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Assess reciver tag limits.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the receiver buffer.
	 *
	 * @return Zero if tag is valid non zero otherwise.
	 */
	static inline int bostan_cnoc_rx_is_valid(int interface, int tag)
	{
		return WITHIN(interface, 0, BOSTAN_NR_INTERFACES)
		    && WITHIN(tag, BOSTAN_CNOC_RX_BASE, BOSTAN_NR_CNOC_RX);
	}

	/**
	 * @brief Allocate C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 *
	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	static inline int bostan_cnoc_rx_alloc(int interface, int tag)
	{
		return mppa_noc_cnoc_rx_alloc(interface, tag);
	}

	/**
	 * @brief Free C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 */
	static inline void bostan_cnoc_rx_free(int interface, int tag)
	{
		mppa_noc_cnoc_rx_free(interface, tag);
	}

	/**
	 * @brief Clear events of C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 */
	static inline void bostan_cnoc_rx_clear(int interface, int tag)
	{
		mppa_noc_cnoc_clear_rx_event(interface, tag);
	}

	/**
	 * @brief Wait events on C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 *
	 * @return Zero if wait sucefully and non zero otherwise.
	 */
	static inline int bostan_cnoc_rx_wait(int interface, int tag)
	{
		return mppa_noc_wait_clear_event(interface, MPPA_NOC_INTERRUPT_LINE_CNOC_RX, tag);
	}

	/**
	 * @brief Configure C-NoC receiver buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of receiver buffer.
	 * @param mode      C-NoC receiver mode.
	 * @param mask      Initial value of the buffer.
	 *
	 * @return Zero if configure sucefully and non zero otherwise.
	 */
    EXTERN int bostan_cnoc_rx_config(int interface, int tag, int mode, uint64_t mask);

/*============================================================================*
 * C-Noc Transfer Interface                                                   *
 *============================================================================*/

	/**
	 * @brief Assess reciver tag limits.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of the transfer buffer.
	 *
	 * @return Zero if tag is valid non zero otherwise.
	 */
	static inline int bostan_cnoc_tx_is_valid(int interface, int tag)
	{
		return WITHIN(interface, 0, BOSTAN_NR_INTERFACES)
		    && WITHIN(tag, BOSTAN_CNOC_TX_BASE, BOSTAN_NR_CNOC_TX);
	}

	/**
	 * @brief Free C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of transfer buffer.

	 * @return Zero if allocate correctly and non zero otherwise.
	 */
	static inline int bostan_cnoc_tx_alloc(int interface, int tag)
	{
		return mppa_noc_cnoc_tx_alloc(interface, tag);
	}

	/**
	 * @brief Free C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number of transfer buffer.
	 */
	static inline void bostan_cnoc_tx_free(int interface, int tag)
	{
		mppa_noc_cnoc_tx_free(interface, tag);
	}

	/**
	 * @brief Write into C-NoC transfer buffer.
	 *
	 * @param interface Number of the DMA channel.
	 * @param tag       Number tag.
	 * @param value     Data to send.
	 */
	static inline void bostan_cnoc_tx_write(int interface, int tag, uint64_t value)
	{
		mppa_noc_cnoc_tx_push_eot(interface, tag, value);
	}

	/**
	 * @brief Configure C-NoC transfer buffer.
	 *
	 * @param interface   Number of the DMA channel.
	 * @param source_node Source interface ID.
	 * @param source_tag  Number of source buffer.
	 * @param target_node Target interface ID.
	 * @param target_tag  Number of target buffer.
	 *
	 * @return Zero if configure sucefully and non zero otherwise.
	 */
    EXTERN int bostan_cnoc_tx_config(
		int interface,
		int source_node,
		int source_tag,
		int target_node,
		int target_tag
	);

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_NOC_CTAG_H_ */

