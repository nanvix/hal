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

#ifndef PROCESSOR_BOSTAN_NOC_TAG_H_
#define PROCESSOR_BOSTAN_NOC_TAG_H_

/**
 * @addtogroup processor-bostan-noc
 * @ingroup processor-bostan
 */
/**@{*/

	/**
	 * @name Type of operation of the NoC Tag
	 */
	/**@{*/
	#define BOSTAN_NOC_RX_TYPE 0 /**< Receiver tag. */
	#define BOSTAN_NOC_TX_TYPE 1 /**< Transfer tag. */
	/**@}*/

	/**
	 * @name Number of reserved tags.
	 */
	/**@{*/
	#define BOSTAN_NR_RESERVED_RX_TAGS 2 /**< Reserved receiver tag. */
	#define BOSTAN_NR_RESERVED_TX_TAGS 0 /**< Reserved transfer tag. */
	/**@}*/

	/**
	 * @brief Number of interface.
	 */
	#ifdef __node__
		#define BOSTAN_NR_INTERFACES 1
	#elif defined(__ioddr__)
		#define BOSTAN_NR_INTERFACES 4
	#endif


	/** micro interface callback type for asynchronous events */
	typedef void (*bostan_noc_handler_fn)(int interface, int tag);


/**@}*/

#endif /* PROCESSOR_BOSTAN_NOC_TAG_H_ */
