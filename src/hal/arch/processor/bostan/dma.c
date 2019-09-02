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

/* Must come fist. */
#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>

/*============================================================================*
 * Control DMA Functions                                                      *
 *============================================================================*/

/*============================================================================*
 * bostan_dma_control_create()                                                *
 *============================================================================*/

/**
 * @brief Allocates and configures the control receiver buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the control receiver buffer.
 * @param mask      Initial value of the buffer.
 * @param handler   Interrupt handler to receive the signal (if NULL use events).
 *
 * @return Zero if create successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_control_create(
	int interface,
	int tag,
	uint64_t mask,
	bostan_processor_noc_handler_fn handler
)
{
	if (bostan_cnoc_rx_alloc(interface, tag) != 0)
		return (-EBUSY);

	if (bostan_cnoc_rx_config(interface, tag, BOSTAN_CNOC_BARRIER_MODE, mask, handler) != 0)
	{
		bostan_cnoc_rx_free(interface, tag);
		return (-ECONNABORTED);
	}

	return (0);
}

/*============================================================================*
 * bostan_dma_control_signal()                                                *
 *============================================================================*/

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
PUBLIC int bostan_dma_control_signal(
	int interface,
	int tag,
	const int *remotes,
	int nremotes,
	int remote_tag,
	uint64_t mask
)
{
	int local;
	int localid;
	int remoteid;

	local = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	localid = bostan_processor_noc_node_num_to_id(local);

	for (int i = 0; i < nremotes; i++)
	{
		if ((remoteid = bostan_processor_noc_node_num_to_id(remotes[i])) < 0)
			return (-EINVAL);

		if (bostan_cnoc_tx_config(interface, localid, tag, remoteid, remote_tag))
			return (-EINVAL);

		if (bostan_cnoc_tx_write(interface, tag, mask) != 0)
			return (-EINVAL);
	}

	return (0);
}

/*============================================================================*
 * Data DMA Functions                                                         *
 *============================================================================*/

/*============================================================================*
 * bostan_dma_data_awrite()                                                   *
 *============================================================================*/

/**
 * @brief Configure and async write to the target node.
 *
 * @param interface   Number of the DMA channel.
 * @param tag         Number of the data transfer buffer.
 * @param remote      Target Node ID.
 * @param remote_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 * @param offset      Offset on target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_awrite(
	int interface,
	int tag,
	int remote,
	int remote_tag,
	const void *buffer,
	uint64_t size,
	uint64_t offset
)
{
	int local;
	int localid;
	int remoteid;

	local = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	localid  = bostan_processor_noc_node_num_to_id(local);
	remoteid = bostan_processor_noc_node_num_to_id(remote);

	return bostan_dnoc_uc_config_write(
		interface,
		localid,
		tag,
		tag,
		remoteid,
		remote_tag,
		buffer,
		size,
		offset
	);
}

/*============================================================================*
 * bostan_dma_data_write()                                                    *
 *============================================================================*/

/**
 * @brief Configure and write to the target node.
 *
 * @param interface   Number of the DMA channel.
 * @param tag         Number of the data transfer buffer.
 * @param remote      Target Node ID.
 * @param remote_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 * @param offset      Offset on target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_write(
	int interface,
	int tag,
	int remote,
	int remote_tag,
	const void *buffer,
	uint64_t size,
	uint64_t offset
)
{
	int local;
	int localid;
	int remoteid;

	local = bostan_processor_noc_cluster_to_node_num(cluster_get_num()) + interface;

	localid  = bostan_processor_noc_node_num_to_id(local);
	remoteid = bostan_processor_noc_node_num_to_id(remote);

	if (bostan_dnoc_tx_config(interface, localid, tag, remoteid, remote_tag) != 0)
		return (-EINVAL);

	return bostan_dnoc_tx_write(
		interface,
		tag,
		buffer,
		size,
		offset
	);
}