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
#include <mOS_vcore_u.h>
#include <mOS_mailbox_u.h>
#include <mppa_noc.h>
#include <mppa_routing.h>
#include <errno.h>

/**
 * @brief Configure D-NoC receiver buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Number of receiver buffer.
 * @param mode      Number of target buffer.
 * @param mask      Initial value of the buffer.
 *
 * @return Zero if configure sucefully and non zero otherwise.
 */
PUBLIC int bostan_dnoc_rx_config(
    int interface,
    int tag,
    void *buffer,
    size_t size,
    size_t offset)
{
    mppa_noc_dnoc_rx_configuration_t config = {
        .buffer_base = (uintptr_t)buffer,
        .buffer_size = size,
        .current_offset = offset,
        .item_reload = 0,
        .item_counter = 0,
        .event_counter = 0,
        .reload_mode = 0,
        .activation = MPPA_NOC_ACTIVATED,
        .counter_id = 0
	};

    k1b_dcache_inval();

    return mppa_noc_dnoc_rx_configure(interface, tag, config);
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
 * @return Zero if configure sucefully and non zero otherwise.
 */
PUBLIC int bostan_dnoc_tx_config(
    int interface,
    int source_node,
    int source_tag,
    int target_tag,
    int target_node)
{
    mppa_dnoc_channel_config_t config = {0};
    mppa_dnoc_header_t header = {0};

    MPPA_NOC_DNOC_TX_CONFIG_INITIALIZER_DEFAULT(config, 0);
    header._.tag = target_tag;
    header._.valid = 1;

    k1b_dcache_inval();

    if (mppa_routing_get_dnoc_unicast_route(source_node, target_node, &config, &header) != 0)
        return (-EINVAL);

    return mppa_noc_dnoc_tx_configure(interface, source_tag, header, config);
}

/**
 * @brief Sync Writes on D-NoC transfer buffer.
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
    mppa_noc_dnoc_tx_flush(interface, tag);

    mppa_dnoc_push_offset_t offset_config = {0};
    offset_config._.offset = offset;
    offset_config._.protocol = 0x1; //! absolute offset
    offset_config._.valid = 1;

    k1b_dcache_inval();

    mppa_noc_dnoc_tx_set_push_offset(interface, tag, offset_config);
    mppa_noc_dnoc_tx_send_data(interface, tag, size, buffer);
    mppa_noc_dnoc_tx_flush_eot(interface, tag);
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
    mppa_dnoc_channel_config_t tx_config = {0};
    mppa_dnoc_header_t tx_header = {0};

    MPPA_NOC_DNOC_TX_CONFIG_INITIALIZER_DEFAULT(tx_config, 0);
    tx_config._.payload_max = 32; // for better performances
    tx_header._.tag = target_tag;
    tx_header._.valid = 1;

    if (mppa_routing_get_dnoc_unicast_route(source_node, target_node, &tx_config, &tx_header) != 0)
        return (-EINVAL);

    if (mppa_noc_dnoc_tx_configure(interface, txtag, tx_header, tx_config) != 0)
        return (-EINVAL);

    mppa_noc_dnoc_uc_configuration_t uc_config; //! = {0} works?
    memset(&uc_config, 0, sizeof(uc_config));

    uc_config.program_start = (uintptr_t)mppa_noc_linear_firmware_eot_event;
    uc_config.buffer_base = (uintptr_t)buffer;
    uc_config.buffer_size = size;
    uc_config.channel_ids._.channel_0 = txtag;
    uc_config.channel_ids._.channel_1 = txtag;
    uc_config.channel_ids._.channel_2 = txtag;
    uc_config.channel_ids._.channel_3 = txtag;
    uc_config.channel_ids._.channel_4 = txtag;
    uc_config.channel_ids._.channel_5 = txtag;
    uc_config.channel_ids._.channel_6 = txtag;
    uc_config.channel_ids._.channel_7 = txtag;

    k1b_dcache_inval();

    mppa_noc_dnoc_uc_set_linear_params(&uc_config, size, 0, 0);

#ifdef __node__
    if (mppa_noc_dnoc_uc_configure(interface, uctag, uc_config, tx_header, tx_config) != 0)
        return (-EINVAL);

    if (mppa_noc_dnoc_uc_link(interface, uctag, txtag, uc_config) != 0)
        return (-EINVAL);
#else
    if (mppa_noc_dnoc_uc_configure(interface, uctag, uc_config) != 0)
        return (-EINVAL);
#endif

    mppa_noc_uc_program_run_t program_run;
    memset(&program_run, 0, sizeof(mppa_noc_uc_program_run_t));
    program_run.semaphore = 1;
    program_run.activation = 1;

    mppa_noc_dnoc_uc_set_program_run(interface, uctag, program_run);

    return (0);
}
