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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/*============================================================================*
 * Definitions                                                                *
 *============================================================================*/

/**
 * @name Resource flags.
 */
/**@{*/
#define BOSTAN_CNOC_TAG_USED  0x1ULL /**< Used tag?          */
#define BOSTAN_CNOC_IT_ENABLE 0x1ULL /**< Enabled interrupt? */
/**@}*/

/**
 * @name Notification modes.
 */
/**@{*/
#define BOSTAN_CNOC_EVENTS     BSP_EV_LINE /**< Event mode.     */
#define BOSTAN_CNOC_INTERRUPTS BSP_IT_LINE /**< Interrupt mode. */
/**@}*/

/**
 * @name Auxiliar macros.
 */
/**@{*/
#define FIELD(tag)  (tag / (sizeof(uint64_t) * 8)) /**< Bit field that includes the tag.              */
#define OFFSET(tag) (tag % (sizeof(uint64_t) * 8)) /**< Displacement of the tag within the bit field. */
/**@}*/

/**
 * @brief Amount of bit fields for CNoC RX resources.
 */
#define BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT (BOSTAN_CNOC_RX_MAX / (sizeof(uint64_t) * 8))

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

/**
 * @name Reciever resources.
 */
/**@{*/
PRIVATE uint64_t bostan_cnoc_rx_tags[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT]               ALIGN(sizeof(dword_t)); /**< Receiver tags.                  */
PRIVATE uint64_t bostan_cnoc_barrier_modes[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT]         ALIGN(sizeof(dword_t)); /**< Receiver tags modes.            */
PRIVATE uint64_t bostan_cnoc_rx_its[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT]                ALIGN(sizeof(dword_t)); /**< Receiver tags interrupt status. */
PRIVATE bostan_processor_noc_handler_fn bostan_cnoc_rx_handlers[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_CNOC_RX_MAX] ALIGN(sizeof(dword_t)); /**< Receiver tags handlers.         */
/**@}*/

/**
 * @name Transfer resources.
 */
/**@{*/
PRIVATE uint8_t bostan_cnoc_tx_tags[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM] ALIGN(sizeof(dword_t)); /**< Transfer tags. */
/**@}*/

/*============================================================================*
 * Resources manipulation functions                                           *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Receiver functions                                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief Assess reciver tag limits.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the receiver tag.
 *
 * @return Zero if tag is valid non zero otherwise.
 */
PRIVATE inline int bostan_cnoc_rx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)
		&& WITHIN(tag, BOSTAN_CNOC_RX_BASE, BOSTAN_CNOC_RXS_NUM);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_rx_set_used(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] |= (BOSTAN_CNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_rx_set_unused(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] &= ~(BOSTAN_CNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets interrupt enable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_rx_enable_it(int interface, int tag)
{
	bostan_cnoc_rx_its[interface][FIELD(tag)] |= (BOSTAN_CNOC_IT_ENABLE << OFFSET(tag));
}

/**
 * @brief Sets interrupt disable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_rx_disable_it(int interface, int tag)
{
	bostan_cnoc_rx_its[interface][FIELD(tag)] &= ~(BOSTAN_CNOC_IT_ENABLE << OFFSET(tag));
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline int bostan_cnoc_rx_is_used(const int interface, int tag)
{
	return ((bostan_cnoc_rx_tags[interface][FIELD(tag)] >> OFFSET(tag)) & BOSTAN_CNOC_TAG_USED);
}

/**
 * @brief Asserts whether or not a interrupt is enabled for a given resource.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target interrupt is enabled, and zero otherwise.
 */
PRIVATE inline int bostan_cnoc_rx_it_is_enable(const int interface, int tag)
{
	return ((bostan_cnoc_rx_its[interface][FIELD(tag)] >> OFFSET(tag)) & BOSTAN_CNOC_IT_ENABLE);
}

/*----------------------------------------------------------------------------*
 * Transfer functions                                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief Assess reciver tag limits.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of the transfer buffer.
 *
 * @return Zero if tag is valid non zero otherwise.
 */
PRIVATE inline int bostan_cnoc_tx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)
		&& WITHIN(tag, BOSTAN_CNOC_TX_BASE, BOSTAN_CNOC_TXS_NUM);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_tx_set_used(int interface, int tag)
{
	bostan_cnoc_tx_tags[interface] |= (BOSTAN_CNOC_TAG_USED << tag);
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_cnoc_tx_set_unused(int interface, int tag)
{
	bostan_cnoc_tx_tags[interface] &= ~(BOSTAN_CNOC_TAG_USED << tag);
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline int bostan_cnoc_tx_is_used(const int interface, int tag)
{
	return ((bostan_cnoc_tx_tags[interface] >> tag) & BOSTAN_CNOC_TAG_USED);
}

/*============================================================================*
 * bostan_cnoc_rx_update_notify()                                             *
 *============================================================================*/

/**
 * @brief Update underlying notification mode.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 * @param events    0 to use events, 1 to use interrupts.
 * @param core_mask Defines which cores will be notified.
 */
PRIVATE inline void bostan_cnoc_rx_update_notify(
	int interface,
	int tag,
	int events,
	int cores_mask
)
{
	mOS_mb_cfg_line(interface, tag, events, cores_mask);
}

/*============================================================================*
 * bostan_cnoc_clear_interface_flags()                                        *
 *============================================================================*/

/**
 * @brief Cleans the interrupts flags of the current core.
 *
 * @param interface Underlying DMA interface.
 */
PRIVATE inline void bostan_cnoc_clear_interface_flags(int interface)
{
	int cpuid;
	mppa_cnoc_it_status_t core_mask;

	cpuid = core_get_id();

#ifdef __node__
	core_mask._.pe = (1ULL << cpuid);
#else
	core_mask._.rm = (1ULL << cpuid);
#endif

	/* Interrupts Flags. */
	mppa_cnoc[interface]->message_rx.it_status_clear.dword       = core_mask.dword;
	mppa_cnoc[interface]->message_rx.it_error_status_clear.dword = core_mask.dword;

	/* Events Flags. */
	__k1_cnoc_event_clear1_barrier(interface);
}

/**
 * @brief Underlying handler for CNoC interrupts.
 *
 * @details When a CNoC interrupt is triggered, we do not know what is the
 * interface that generated it. So, for each interface, we need to search
 * the RX tags fields to find the correct tag to call its handler.
 *
 * @param ev_src Interrupt ID
 */
PRIVATE void bostan_cnoc_it_handler(int ev_src)
{
	int tag;
	int offset;
	int possible_lost_it;
	uint64_t tags_status;
	volatile dword_t * field_status;
	bostan_processor_noc_handler_fn handler;

	UNUSED(ev_src);

	do
	{
		possible_lost_it = 0;

		for (unsigned int interface = 0; interface < BOSTAN_PROCESSOR_NOC_INTERFACES_NUM; interface++)
		{
			for (unsigned int field = 0; field < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; field++)
			{
				/**
				* Masks the CNOC interrupt to get the triggered tags and clear them
				* without intermediary interrupts.
				*/
				tags_status  = 0ULL;
				field_status = &mppa_cnoc[interface]->message_rx.status[field].dword;

				/* Gets the set of tags that generated an interrupt. */
				while ((tags_status = (*field_status)) != 0ULL)
				{
					possible_lost_it = 1;

					/* Cleans RX Tags Flags for incoming interrupts. */
					mppa_cnoc[interface]->message_rx.status_clear[field].dword = tags_status;

					while (tags_status)
					{
						/**
						* Find the next bit enabled that represents the
						* triggered tag.
						*/
						offset       = __builtin_k1_ctzdl(tags_status);
						tags_status &= ~(0x1ULL << offset);

						tag = field * (sizeof(uint64_t) * 8) + offset;

						handler = (bostan_processor_noc_handler_fn) __k1_umem_read32(
							(void *) &bostan_cnoc_rx_handlers[interface][tag]
						);

						handler(interface, tag);
					}
				}
			}

			/* Cleans CNoC Interrupt Flags on current interface. */
			bostan_cnoc_clear_interface_flags(interface);
		}
	} while (possible_lost_it);
}

/*============================================================================*
 * bostan_cnoc_it_verify()                                                    *
 *============================================================================*/

/**
 * @brief Verify lost interrupts.
 */
PUBLIC void bostan_cnoc_it_verify(void)
{
	bostan_cnoc_it_handler(-1);
}

/*============================================================================*
 * bostan_cnoc_setup()                                                        *
 *============================================================================*/

/**
 * @brief Dummy CNoC handler.
 */
void bostan_cnoc_dummy_noc_handler(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);
}

/**
 * @brief Initializes the control structures and configures the interrupt
 * handlers.
 */
PUBLIC void bostan_cnoc_setup(void)
{
	__k1_cnoc_initialize_rights_allopen();

	/* Registers the underlying handler of the CNoC interrupts. */
	interrupt_register(K1B_INT_CNOC, bostan_cnoc_it_handler);

	for (int interface = 0; interface < BOSTAN_PROCESSOR_NOC_INTERFACES_NUM; interface++)
	{
		mOS_async_mailbox_get_cfg_perm(interface, BOSTAN_CNOC_EVENTS);
		if (mOS_async_mailbox_rda_event_waitclear(BOSTAN_CNOC_EVENTS) != 0)
			kpanic("[hal][processor][bostan] Core cannot configures the RX CNoC!");

		/* All tags are unused. */
		for (unsigned int field = 0; field < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; field++)
		{
			bostan_cnoc_rx_tags[interface][field]       = 0ULL;
			bostan_cnoc_rx_its[interface][field]        = 0ULL;
			bostan_cnoc_barrier_modes[interface][field] = 0ULL;

			/* Sets default mode (mailbox mode). */
			mppa_cnoc[interface]->message_rx.barrier_mode[field].dword =
				bostan_cnoc_barrier_modes[interface][field];

			/* Cleans status and error flags of the current field (64 tags). */
			mppa_cnoc[interface]->message_rx.status_clear[field].dword = (~0ULL);
			mppa_cnoc[interface]->message_rx.error_clear[field].dword  = (~0ULL);
		}

		/* Cleans underlying flags. */
		bostan_cnoc_clear_interface_flags(interface);

		/* Sets default notification method and default message value. */
		for (unsigned int tag = 0; tag < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; tag++)
		{
			bostan_cnoc_rx_handlers[interface][tag] = &bostan_cnoc_dummy_noc_handler;
			bostan_cnoc_rx_update_notify(interface, tag, BOSTAN_CNOC_INTERRUPTS, 0);

			mppa_cnoc[interface]->message_ram[tag].dword = (0ULL);
		}
	}

	/* Flush dcache to sram. */
	k1b_dcache_flush();
	k1b_dcache_fence();
}

/*============================================================================*
 * bostan_cnoc_rx_alloc()                                                     *
 *============================================================================*/

/**
 * @brief Allocate C-NoC receiver tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver tag.
 *
 * @return Zero if has been allocated correctly and non zero otherwise.
 */
PUBLIC int bostan_cnoc_rx_alloc(int interface, int tag)
{
	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_cnoc_rx_is_used(interface, tag))
		return (-EBUSY);

	bostan_cnoc_rx_set_used(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_cnoc_rx_free()                                                      *
 *============================================================================*/

/**
 * @brief Free C-NoC receiver tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver tag.
 *
 * @return Zero if releases correctly and non zero otherwise.
 */
PUBLIC int bostan_cnoc_rx_free(int interface, int tag)
{
	int field;
	int offset;

	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	mOS_async_mailbox_get_cfg_perm(interface, BOSTAN_CNOC_EVENTS);

		field  = FIELD(tag);
		offset = OFFSET(tag);

	if (mOS_async_mailbox_rda_event_waitclear(BOSTAN_CNOC_EVENTS) != 0)
		kpanic("[hal][processor][bostan] Core cannot configures the RX CNoC!");

	/* Cleans interrupt flags. */
	bostan_cnoc_clear_interface_flags(interface);

	/* Cleans RX Tags Flags for incoming interrupts. */
	mppa_cnoc[interface]->message_rx.status_clear[field].dword = (1ULL << offset);

	/* Sets initial value. */
	mppa_cnoc[interface]->message_ram[tag].dword = (0ULL);

	/* Releases tag. */
	bostan_cnoc_rx_set_unused(interface, tag);

	if (bostan_cnoc_rx_it_is_enable(interface, tag))
	{
		bostan_cnoc_rx_handlers[interface][tag] = &bostan_cnoc_dummy_noc_handler;
		bostan_cnoc_rx_disable_it(interface, tag);
	}

	/**
	 * Configures the tag to generate an event but the event does not
	 * notify anyone.
	 */
	bostan_cnoc_rx_update_notify(interface, tag, BOSTAN_CNOC_EVENTS, 0);

	return (0);
}

/*============================================================================*
 * bostan_cnoc_rx_clear()                                                     *
 *============================================================================*/

/**
 * @brief Clear events of C-NoC receiver tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver tag.
 */
PUBLIC void bostan_cnoc_rx_clear(int interface, int tag)
{
	mOS_mb_event_clear(interface, tag);
}

/*============================================================================*
 * bostan_cnoc_rx_wait()                                                      *
 *============================================================================*/

/**
 * @brief Wait events on C-NoC receiver tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver tag.
 *
 * @return Zero if wait successfully and non zero otherwise.
 */
PUBLIC int bostan_cnoc_rx_wait(int interface, int tag)
{
	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	/* Is interrupt enable? */
	if (bostan_cnoc_rx_it_is_enable(interface, tag))
		return (-EINVAL);

	/* Wait on the underlying line events (mOS implementation). */
	mOS_mb_event_waitclear(BOSTAN_CNOC_EVENTS, interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_cnoc_rx_config()                                                    *
 *============================================================================*/

/**
 * @brief Configure C-NoC receiver tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver tag.
 * @param mode      C-NoC receiver mode.
 * @param mask      Initial value of the buffer.
 * @param handler   Interrupt handler (If NULL then uses events).
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_cnoc_rx_config(
	int interface,
	int tag,
	int mode,
	uint64_t mask,
	bostan_processor_noc_handler_fn handler
)
{
	int field;
	int offset;

	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	mOS_async_mailbox_get_cfg_perm(interface, BOSTAN_CNOC_EVENTS);

		field  = FIELD(tag);
		offset = OFFSET(tag);

		/* Updates barrier modes of the tags field. */
		if (mode == BOSTAN_CNOC_BARRIER_MODE)
			bostan_cnoc_barrier_modes[interface][field] |= (0x1ULL << offset);
		else
			bostan_cnoc_barrier_modes[interface][field] &= ~(0x1ULL << offset);

	if (mOS_async_mailbox_rda_event_waitclear(BOSTAN_CNOC_EVENTS) != 0)
		kpanic("[hal][processor][bostan] Core cannot configures the RX CNoC!");

	/* Sets inital value. */
	mppa_cnoc[interface]->message_ram[tag].dword = mask;

	/* Sets mode. */
	mppa_cnoc[interface]->message_rx.barrier_mode[field].dword =
		bostan_cnoc_barrier_modes[interface][field];

	/* Cleans RX Tags Flags for incoming interrupts. */
	mppa_cnoc[interface]->message_rx.status_clear[field].dword = (1ULL << offset);

	/* Cleans CNoC Interface Flags. */
	bostan_cnoc_clear_interface_flags(interface);

	/* Configures notification mode with interrupts for a specific cpu. */
	if (handler != NULL)
	{
		bostan_cnoc_rx_handlers[interface][tag] = handler;

		bostan_cnoc_rx_enable_it(interface, tag);
		bostan_cnoc_rx_update_notify(
			interface,
			tag,
			BOSTAN_CNOC_INTERRUPTS,
			(1 << core_get_id())
		);
	}

	/**
	 * Configures notification mode with events for a all cpu because
	 * perhaps another cpu, other than the master, waits for the event.
	 */
	else
		bostan_cnoc_rx_update_notify(interface, tag, BOSTAN_CNOC_EVENTS, (~0));

	return (0);
}

/*============================================================================*
 * bostan_cnoc_tx_alloc()                                                     *
 *============================================================================*/

/**
 * @brief Allocate C-NoC transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of receiver buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_cnoc_tx_alloc(int interface, int tag)
{
	if (!bostan_cnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_cnoc_tx_is_used(interface, tag))
		return (-EBUSY);

	bostan_cnoc_tx_set_used(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_cnoc_tx_free()                                                      *
 *============================================================================*/

/**
 * @brief Free C-NoC transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of transfer buffer.
 *
 * @return Zero if releases correctly and non zero otherwise.
 */
PUBLIC int bostan_cnoc_tx_free(int interface, int tag)
{
	if (!bostan_cnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	bostan_cnoc_tx_set_unused(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_cnoc_tx_config()                                                    *
 *============================================================================*/

/**
 * @brief Configure C-NoC transfer buffer.
 *
 * @param interface   Number of the DMA channel.
 * @param localid Source interface ID.
 * @param local_tag  Number of source buffer.
 * @param remoteid Target interface ID.
 * @param remote_tag  Number of target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_cnoc_tx_config(
	int interface,
	int localid,
	int local_tag,
	int remoteid,
	int remote_tag
)
{
	int src;
	int dest;
	mppa_cnoc_header_t header;
	mppa_cnoc_config_t config;

	if (!bostan_cnoc_tx_is_valid(interface, local_tag))
		return (-EINVAL);

	if (!bostan_cnoc_tx_is_used(interface, local_tag))
		return (-EINVAL);

	src  = __bsp_get_router_id(localid);
	dest = __bsp_get_router_id(remoteid);

	/* Configures header (default values by Kalray). */
	header.dword   = 0ULL;
	header._.valid = 1;
	header._.tag   = remote_tag;
	header._.route = (__bsp_routing_table[src][dest] >> 3);

	/* Configures first direction (default values by Kalray). */
	config.dword       = 0ULL;
	config._.first_dir = (__bsp_routing_table[src][dest] & 0x7);

	k1b_dcache_flush();
	k1b_dcache_fence();

	/* Writes on hardware registers. */
	mppa_cnoc[interface]->message_tx[local_tag].config.dword = config.dword;
	mppa_cnoc[interface]->message_tx[local_tag].header.dword = header.dword;

	return (0);
}

/*============================================================================*
 * bostan_cnoc_tx_write()                                                     *
 *============================================================================*/

/**
 * @brief Write into C-NoC transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number tag.
 * @param value     Data to send.
 */
PUBLIC int bostan_cnoc_tx_write(int interface, int tag, uint64_t value)
{
	if (!bostan_cnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_tx_is_used(interface, tag))
		return (-EINVAL);

	mppa_cnoc[interface]->message_tx[tag].push_eot.dword = value;

	return (0);
}
