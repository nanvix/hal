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

#include <arch/processor/bostan/noc/dtag.h>
#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * @brief TODO: Describe.
 */
PUBLIC void bostan_dnoc_setup(void)
{

}

/*============================================================================*
 * Data Receiver                                                              *
 *============================================================================*/

/**
 * @brief Allocate D-NoC receiver buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof receiver buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_rx_alloc(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	return (0);
}

/**
 * @brief Free D-NoC receiver buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof receiver buffer.
 */
PUBLIC void bostan_dnoc_rx_free(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);
}

/**
 * @brief Wait events on D-NoC receiver buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Number tag.
 *
 * @return Amount of received events.
 */
PUBLIC int bostan_dnoc_rx_wait(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	return (0);
}

/**
 * @brief Configure D-NoC receiver buffer.
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
PUBLIC int bostan_dnoc_rx_config(
	int interface,
	int tag,
	void *buffer,
	size_t min_size,
	size_t max_size,
	size_t offset)
{
	UNUSED(interface);
	UNUSED(tag);
	UNUSED(buffer);
	UNUSED(min_size);
	UNUSED(max_size);
	UNUSED(offset);

	return (0);
}

/*============================================================================*
 * Data Transfer                                                              *
 *============================================================================*/

/**
 * @brief Free D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof transfer buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_tx_alloc(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	return (0);
}

/**
 * @brief Free D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Number of transfer buffer.
 */
PUBLIC void bostan_dnoc_tx_free(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);
}

/**
 * @brief Configure D-NoC transfer buffer.
 *
 * @param interface   Number of the interface.
 * @param source_node Source interface ID.
 * @param source_tag  Number of source buffer.
 * @param target_node Target interface ID.
 * @param target_tag  Number of target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_dnoc_tx_config(
	int interface,
	int source_node,
	int source_tag,
	int target_node,
	int target_tag)
{
	UNUSED(interface);
	UNUSED(source_node);
	UNUSED(source_tag);
	UNUSED(target_node);
	UNUSED(target_tag);

	return (0);
}

/**
 * @brief Writes on D-NoC transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Transfer buffer ID.
 * @param buffer    Local data pointer.
 * @param size      Amount of bytes to transfer.
 * @param offset    Offset on local buffer data.
 */
PUBLIC void bostan_dnoc_tx_write(
	int interface,
	int tag,
	const void *buffer,
	size_t size,
	size_t offset)
{
	UNUSED(interface);
	UNUSED(tag);
	UNUSED(buffer);
	UNUSED(size);
	UNUSED(offset);
}

/*============================================================================*
 * UCore                                                                      *
 *============================================================================*/

/**
 * @brief Free D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param uctag     Number of ucore.
 * @param txtag     Number of transfer buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_uc_alloc(int interface, int uctag, int txtag)
{
	UNUSED(interface);
	UNUSED(uctag);
	UNUSED(txtag);

	return (0);
}

/**
 * @brief Free D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Number of transfer buffer.
 */
PUBLIC void bostan_dnoc_uc_free(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);
}

/**
 * @brief Wait events on D-NoC sender buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Ucore thread ID.
 *
 * @return Amount of received events.
 */
PUBLIC int bostan_dnoc_uc_wait(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);

	// return mppa_noc_wait_clear_event(interface, MPPA_NOC_INTERRUPT_LINE_DNOC_TX, tag);
	return (0);
}

/**
 * @brief Releases D-NoC Ucore thread.
 *
 * @param interface Number of the interface.
 * @param txtag     Number of transfer buffer.
 * @param uctag     Number of ucore thread.
 */
PUBLIC int bostan_dnoc_uc_desconfig(int interface, int txtag, int uctag)
{
	UNUSED(txtag);
	UNUSED(uctag);
	UNUSED(interface);

	return (0);
}

/**
 * @brief Configure and async writes on D-NoC transfer buffer.
 *
 * @param interface   Number of the DMA channel.
 * @param source_node Local Node ID.
 * @param txtag       Transfer buffer ID.
 * @param uctag       Ucore thread ID.
 * @param target_node Target Node ID.
 * @param target_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 */
PUBLIC int bostan_dnoc_uc_config_write(
	int interface,
	int source_node,
	int txtag,
	int uctag,
	int target_node,
	int target_tag,
	const void *buffer,
	size_t size)
{
	UNUSED(interface);
	UNUSED(source_node);
	UNUSED(txtag);
	UNUSED(uctag);
	UNUSED(target_node);
	UNUSED(target_tag);
	UNUSED(buffer);
	UNUSED(size);

	return (0);
}
