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

#include <arch/processor/bostan/clusters.h>
#include <arch/processor/bostan/noc/dma.h>
#include <nanvix/const.h>
#include <errno.h>

/**
 * @brief Allocates and configures the control receiver buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the control receiver buffer.
 * @param mask      Initial value of the buffer.
 *
 * @return Zero if create successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_control_create(int interface, int tag, uint64_t mask, bostan_noc_handler_fn handler)
{
	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_cnoc_rx_alloc(interface, tag) != 0)
		return (-EBUSY);

	if (bostan_cnoc_rx_config(interface, tag, BOSTAN_CNOC_BARRIER_MODE, mask, handler) != 0)
	{
		bostan_cnoc_rx_free(interface, tag);
		return (-ECONNABORTED);
	}

	return (0);
}

/**
 * @brief Re-configure C-NoC control receiver buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the control receiver buffer.
 * @param mask      Initial value of the buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_control_config(int interface, int tag, uint64_t mask, bostan_noc_handler_fn handler)
{
	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_cnoc_rx_config(interface, tag, BOSTAN_CNOC_BARRIER_MODE, mask, handler) != 0)
		return (-ECONNABORTED);

	return (0);
}

/**
 * @brief Allocates the control transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the control transfer buffer.
 *
 * @return Zero if open successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_control_open(int interface, int tag)
{
	if (!bostan_cnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_cnoc_tx_alloc(interface, tag) != 0)
		return (-EBUSY);

	return (0);
}

/**
 * @brief Configure and send a signal on a control transfer buffer.
 *
 * @param interface    Number of the DMA channel.
 * @param tag          Number of the control transfer buffer.
 * @param target_nodes Target Node IDs.
 * @param ntargets     Amount of targets.
 * @param target_tag   Number of the target control receiver buffer.
 * @param mask         Signal value.
 *
 * @return Zero if send successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_control_signal(
	int interface,
	int tag,
	const int *target_nodes,
	int ntargets,
	int target_tag,
	uint64_t mask
)
{
	int source_node;

	if (!bostan_cnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	source_node = bostan_cluster_get_id() + interface;

	for (int i = 0; i < ntargets; i++)
	{
		if (bostan_cnoc_tx_config(interface, source_node, tag, target_nodes[i], target_tag))
			return (-ECONNABORTED);

		bostan_cnoc_tx_write(interface, tag, mask);
	}

	return (0);
}

/**
 * @brief Allocates the data transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the data transfer buffer.
 *
 * @return Zero if open successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_open(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	return (0);
}

/**
 * @brief Releases the data transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the data transfer buffer.
 *
 * @return Zero if close successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_close(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	return (0);
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
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_aread(
	int interface,
	int tag,
	void *buffer,
	size_t min_size,
	size_t max_size,
	size_t offset
)
{
	UNUSED(interface);
	UNUSED(tag);
	UNUSED(buffer);
	UNUSED(min_size);
	UNUSED(max_size);
	UNUSED(offset);

	return (0);
}

/**
 * @brief Configure and async write to the target node.
 *
 * @param interface   Number of the DMA channel.
 * @param tag         Number of the data transfer buffer.
 * @param target_node Target Node ID.
 * @param target_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_awrite(
	int interface,
	int tag,
	int target_node,
	int target_tag,
	const void *buffer,
	size_t size
)
{
	UNUSED(interface);
	UNUSED(tag);
	UNUSED(target_node);
	UNUSED(target_tag);
	UNUSED(buffer);
	UNUSED(size);

	return (0);
}


/**
 * @brief Configure and write to the target node.
 *
 * @param interface   Number of the DMA channel.
 * @param tag         Number of the data transfer buffer.
 * @param target_node Target Node ID.
 * @param target_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dma_data_write(
	int interface,
	int tag,
	int target_node,
	int target_tag,
	const void *buffer,
	size_t size
)
{
	UNUSED(interface);
	UNUSED(tag);
	UNUSED(target_node);
	UNUSED(target_tag);
	UNUSED(buffer);
	UNUSED(size);

	return (0);
}
