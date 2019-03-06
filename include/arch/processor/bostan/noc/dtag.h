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

#ifndef PROCESSOR_BOSTAN_NOC_DTAG_H_
#define PROCESSOR_BOSTAN_NOC_DTAG_H_

/**
 * @cond bostan-noc
 */

	/* Processor Interface Implementation */
	#include <arch/processor/bostan/_bostan.h>

	#include <arch/processor/bostan/noc/tag.h>

	/**
	 * @name Type of D-NoC Tag
	 */
	/**@{*/
	#define BOSTAN_DNOC_TAG_RX 2 /**< D-NoC receive tag.  */
	#define BOSTAN_DNOC_TAG_TX 3 /**< D-NoC transfer tag. */
	#define BOSTAN_DNOC_TAG_UC 4 /**< D-NoC ucore tag.    */
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
	 * @name Number of D-NoC buffer
	 */
	/**@{*/
	#define BOSTAN_NR_DNOC_RX BOSTAN_DNOC_RX_MAX - BOSTAN_NR_RESERVED_TAGS /**< Number of receive buffers.  */
	#define BOSTAN_NR_DNOC_TX BOSTAN_DNOC_TX_MAX - BOSTAN_NR_RESERVED_TAGS /**< Number of transfer buffers. */
	#define BOSTAN_NR_DNOC_UC BOSTAN_DNOC_UC_MAX - BOSTAN_NR_RESERVED_TAGS /**< Number of ucore threads.	*/
	/**@}*/

	/**
	 * @name D-NoC tag offset
	 */
	/**@{*/
	#define BOSTAN_DNOC_RX_OFFSET 0 /**< Receive tag offset.  */
	#define BOSTAN_DNOC_TX_OFFSET 0 /**< Transfer tag offset. */
	#define BOSTAN_DNOC_UX_OFFSET 0 /**< UCore thread offset. */
	/**@}*/

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_NOC_DTAG_H_ */

