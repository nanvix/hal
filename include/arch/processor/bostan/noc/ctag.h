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

	/**
	 * @name Type of C-NoC Tag
	 */
	/**@{*/
	#define BOSTAN_CNOC_TAG_RX 0 /**< C-NoC receiver tag. */
	#define BOSTAN_CNOC_TAG_TX 1 /**< C-NoC transfer tag. */
	/**@}*/

	/**
	 * @name Amount of the Control NoC Resources
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_MAX 128 /**< Receiver amount. */
    #define BOSTAN_CNOC_TX_MAX 1   /**< Sender amount.   */
	/**@}*/

	/**
	 * @name Number of C-NoC buffer
	 */
	/**@{*/
	#define BOSTAN_NR_CNOC_RX BOSTAN_CNOC_RX_MAX - BOSTAN_NR_RESERVED_TAGS /**< Number of receive buffers.  */
	#define BOSTAN_NR_CNOC_TX BOSTAN_CNOC_TX_MAX                           /**< Number of transfer buffers. */
	/**@}*/

	/**
	 * @name C-NoC tag offset
	 */
	/**@{*/
	#define BOSTAN_CNOC_RX_OFFSET 0 /**< Receiver tag offset. */
	#define BOSTAN_CNOC_TX_OFFSET 0 /**< Transfer tag offset. */
	/**@}*/

    /**
	 * @name Control NoC Modes
	 */
	/**@{*/
    #define BOSTAN_CNOC_MAILBOX_MODE 0 /**< Mailbox mode */
    #define BOSTAN_CNOC_BARRIER_MODE 1 /**< Barrier mode */
    /**@}*/

	/**
	 * @brief Allocate C-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof receiver buffer.
	 */
	static inline int bostan_cnoc_rx_alloc(int interface, int tag)
	{
		return mppa_noc_cnoc_rx_alloc(interface, tag);
	}

	/**
	 * @brief Free C-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof receiver buffer.
	 */
	static inline void bostan_cnoc_rx_free(int interface, int tag)
	{
		mppa_noc_cnoc_rx_free(interface, tag);	
	}

	/**
	 * @brief Clear events of C-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof receiver buffer.
	 */
	static inline void bostan_cnoc_rx_clear(int interface, int tag)
	{
		mppa_noc_cnoc_clear_rx_event(interface, tag);
	}

	/**
	 * @brief Free C-NoC transfer buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Numberof transfer buffer.
	 */
	static inline int bostan_cnoc_tx_alloc(int interface, int tag)
	{
		return mppa_noc_cnoc_tx_alloc(interface, tag);
	}

	/**
	 * @brief Free C-NoC transfer buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number of transfer buffer.
	 */
	static inline void bostan_cnoc_tx_free(int interface, int tag)
	{
		mppa_noc_cnoc_tx_free(interface, tag);
	}

	/**
	 * @brief Write into C-NoC transfer buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number tag.
	 * @param value     Data to send.
	 */
	static inline void bostan_cnoc_tx_write(int interface, int tag, uint64_t value)
	{
		mppa_noc_cnoc_tx_push_eot(interface, tag, value);
	}

	/**
	 * @brief Configure C-NoC receiver buffer.
	 *
	 * @param interface Number of the interface.
	 * @param tag       Number of receiver buffer.
	 * @param mode      Number of target buffer.
	 * @param mask      Initial value of the buffer.
	 */
    EXTERN int bostan_cnoc_rx_config(int interface, int tag, int mode, uint64_t mask);

	/**
	 * @brief Configure C-NoC transfer buffer.
	 *
	 * @param interface   Number of the interface.
	 * @param source_tag       Numberof source buffer.
	 * @param target_tag       Numberof target buffer.
	 * @param target_node Target interface ID.
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

