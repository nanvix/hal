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

#include <arch/processor/bostan/noc/ctag.h>
#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <mOS_vcore_u.h>
#include <mOS_mailbox_u.h>
#include <mppa_noc.h>
#include <mppa_routing.h>

/**
 * @todo Provide a detailed comment for this.
 */
PUBLIC int bostan_cnoc_rx_config(int interface, int tag, int mode, uint64_t mask)
{
	int ret;

	mppa_noc_cnoc_rx_configuration_t config = {0};
	config.mode = mode;
	config.init_value = mask;

	mppa_cnoc_mailbox_notif_t notif = {0};
	notif._.enable = 1;
	notif._.evt_en = 1;
#ifdef __node__
	notif._.pe = (uint16_t) ~0;
#else
	notif._.rm = (uint8_t) 1 << interface;
#endif

	__builtin_k1_wpurge();
	__builtin_k1_fence();
	__builtin_k1_dinval();

	ret = mppa_noc_cnoc_rx_configure(interface, tag, config, &notif);

	return (ret);
}

/**
 * @todo Provide a detailed comment for this.
 */
PUBLIC int bostan_cnoc_tx_config(
	int interface,
	int source_node,
	int source_tag,
	int target_node,
	int target_tag
)
{
	mppa_cnoc_config_t config = {0};
	mppa_cnoc_header_t header = {0};

	if (mppa_routing_get_cnoc_unicast_route(source_node, target_node, &config, &header) != 0)
		return -1;
	header._.tag = target_tag;

	__builtin_k1_wpurge();
	__builtin_k1_fence();
	__builtin_k1_dinval();

	return mppa_noc_cnoc_tx_configure(interface, source_tag, config, header);
}
