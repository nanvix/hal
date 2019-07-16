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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/*============================================================================*
 * Definitions                                                                *
 *============================================================================*/

/**
 * @name Resource flags.
 */
/**@{*/
#define BOSTAN_CNOC_TAG_USED  1ULL /**< Used tag?          */
#define BOSTAN_CNOC_IT_ENABLE 1ULL /**< Enabled interrupt? */
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
PRIVATE uint64_t bostan_cnoc_rx_tags[BOSTAN_NR_INTERFACES][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT]       ALIGN(sizeof(uint64_t)); /**< Receiver tags.                  */
PRIVATE uint64_t bostan_cnoc_barrier_modes[BOSTAN_NR_INTERFACES][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT] ALIGN(sizeof(uint64_t)); /**< Receiver tags modes.            */
PRIVATE uint64_t bostan_cnoc_rx_its[BOSTAN_NR_INTERFACES][BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT]        ALIGN(sizeof(uint64_t)); /**< Receiver tags interrupt status. */
PRIVATE bostan_noc_handler_fn bostan_cnoc_rx_handlers[BOSTAN_NR_INTERFACES][BOSTAN_CNOC_RX_MAX]   ALIGN(sizeof(uint64_t)); /**< Receiver tags handlers.         */
/**@}*/

/**
 * @name Transfer resources.
 */
/**@{*/
PRIVATE uint8_t bostan_cnoc_tx_tags[BOSTAN_NR_INTERFACES] ALIGN(sizeof(uint64_t)); /**< Transfer tags. */
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
static inline int bostan_cnoc_rx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_NR_INTERFACES)
		&& WITHIN(tag, BOSTAN_CNOC_RX_BASE, BOSTAN_NR_CNOC_RX);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_rx_set_used(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] |= (BOSTAN_CNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_rx_set_unused(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] &= ~(BOSTAN_CNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets interrupt enable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_rx_enable_it(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] |= (BOSTAN_CNOC_IT_ENABLE << OFFSET(tag));
}

/**
 * @brief Sets interrupt disable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_rx_disable_it(int interface, int tag)
{
	bostan_cnoc_rx_tags[interface][FIELD(tag)] &= ~(BOSTAN_CNOC_IT_ENABLE << OFFSET(tag));
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
static inline int bostan_cnoc_tx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_NR_INTERFACES)
		&& WITHIN(tag, BOSTAN_CNOC_TX_BASE, BOSTAN_NR_CNOC_TX);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_tx_set_used(int interface, int tag)
{
	bostan_cnoc_tx_tags[interface] |= (BOSTAN_CNOC_TAG_USED << tag);
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
static inline void bostan_cnoc_tx_set_unused(int interface, int tag)
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
static inline int bostan_cnoc_rx_is_used(const int interface, int tag)
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
static inline int bostan_cnoc_rx_it_is_enable(const int interface, int tag)
{
	return ((bostan_cnoc_rx_its[interface][FIELD(tag)] >> OFFSET(tag)) & BOSTAN_CNOC_IT_ENABLE);
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
static inline int bostan_cnoc_tx_is_used(const int interface, int tag)
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
static inline void bostan_cnoc_rx_update_notify(int interface, int tag, int events, int cores_mask)
{
	mppa_cnoc_mailbox_notif_t notif;

	notif.dword    = 0ULL;
	notif._.enable = 1;
	notif._.evt_en = (events == BOSTAN_CNOC_EVENTS);
	notif._.it_en  = (events != BOSTAN_CNOC_EVENTS);
#ifdef __k1dp__
	notif._.pe     = cores_mask;
#else
	notif._.rm     = cores_mask;
#endif

	mppa_cnoc[interface]->message_mailbox_notif[tag].dword = notif.dword;
}

/*============================================================================*
 * bostan_cnoc_setup()                                                       *
 *============================================================================*/

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
	uint64_t tags_set;
	bostan_noc_handler_fn handler;

	UNUSED(ev_src);

	for (unsigned int interface = 0; interface < BOSTAN_NR_INTERFACES; interface++)
	{
		for (unsigned int field = 0; field < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; field++)
		{
			/* Gets the set of tags that generated an interrupt. */
			tags_set = mppa_cnoc[interface]->message_rx.status[field].dword;
			tags_set &= ~(bostan_cnoc_rx_its[interface][field]);

			if (tags_set)
			{
				/* Cleans RX Tags flags for incoming interrupts. */
				__k1_io_write64((void *)(&mppa_cnoc[interface]->message_rx.status_clear[field]), tags_set);
				__k1_cnoc_clear_it_status(interface);

				/* For each tag that generated an interrupt, executes the call of its specific handler. */
				while (tags_set)
				{
					offset    = __builtin_k1_ctzdl(tags_set);
					tags_set &= ~(0x1ULL << offset);

					tag = field * (sizeof(uint64_t) * 8) + offset;

					handler = bostan_cnoc_rx_handlers[interface][tag];

					if (handler)
						handler(interface, tag);
				}
			}
		}
	}
}

/*============================================================================*
 * bostan_cnoc_setup()                                                        *
 *============================================================================*/

/**
 * @brief Initializes the control structures and configures the interrupt
 * handlers.
 */
PUBLIC void bostan_cnoc_setup(void)
{
	interrupt_register(K1B_INT_CNOC, bostan_cnoc_it_handler);

	for (int interface = 0; interface < BOSTAN_NR_INTERFACES; interface++)
	{
		/* All tags are unused. */
		for (unsigned int field = 0; field < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; field++)
			bostan_cnoc_rx_tags[interface][field] = 0ULL;

		/* All tags do not have handlers. */
		for (unsigned int field = 0; field < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; field++)
			bostan_cnoc_rx_its[interface][field] = 0ULL;

		for (unsigned int tag = 0; tag < BOSTAN_CNOC_RX_BIT_FIELD_AMOUNT; tag++)
			bostan_cnoc_rx_handlers[interface][tag] = NULL;

		__builtin_k1_wpurge();
		__builtin_k1_fence();
	}
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
	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	bostan_cnoc_rx_set_unused(interface, tag);

	if (bostan_cnoc_rx_it_is_enable(interface, tag))
	{
		bostan_cnoc_rx_handlers[interface][tag] = NULL;
		bostan_cnoc_rx_disable_it(interface, tag);
	}

	/* Configures the tag to generate an event but the event does not notify anyone. */
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
	bostan_noc_handler_fn handler
)
{
	int field;
	int offset;

	if (!bostan_cnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_cnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	field  = FIELD(tag);
	offset = OFFSET(tag);

	/* Updates barrier modes of the tags field. */
	if (mode == BOSTAN_CNOC_BARRIER_MODE)
		bostan_cnoc_barrier_modes[interface][field] |= (0x1ULL << offset);
	else
		bostan_cnoc_barrier_modes[interface][field] &= ~(0x1ULL << offset);

	mppa_cnoc[interface]->message_rx.barrier_mode[field].dword = bostan_cnoc_barrier_modes[interface][field];

	__builtin_k1_wpurge();
	__builtin_k1_fence();

	/* Sets initial value. */
	mppa_cnoc[interface]->message_ram[tag].dword = mask;

	/* Configures notification mode. */
	if (handler != NULL)
	{
		bostan_cnoc_rx_handlers[interface][tag] = handler;

		bostan_cnoc_rx_enable_it(interface, tag);
		bostan_cnoc_rx_update_notify(interface, tag, BOSTAN_CNOC_INTERRUPTS, (1 << core_get_id()));
	}
	else
		bostan_cnoc_rx_update_notify(interface, tag, BOSTAN_CNOC_EVENTS, (~0));

	__builtin_k1_wpurge();
	__builtin_k1_fence();

	return (0);
}

/*============================================================================*
 * bostan_cnoc_tx_alloc()                                                     *
 *============================================================================*/

/**
 * @brief Free C-NoC transfer buffer.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Number of transfer buffer.

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
 * @param source_node Source interface ID.
 * @param source_tag  Number of source buffer.
 * @param target_node Target interface ID.
 * @param target_tag  Number of target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 */
PUBLIC int bostan_cnoc_tx_config(
	int interface,
	int source_node,
	int source_tag,
	int target_node,
	int target_tag
)
{
	int src;
	int dest;
	mppa_cnoc_header_t header;
	mppa_cnoc_config_t config;

	if (!bostan_cnoc_tx_is_valid(interface, source_tag))
		return (-EINVAL);

	if (!bostan_cnoc_tx_is_used(interface, source_tag))
		return (-EINVAL);

	src  = __bsp_get_router_id(source_node);
	dest = __bsp_get_router_id(target_node);

	/* Configures header (default values by Kalray). */
	header.dword   = 0ULL;
	header._.valid = 1;
	header._.tag   = target_tag;
	header._.route = (__bsp_routing_table[src][dest] >> 3);

	/* Configures first direction (default values by Kalray). */
	config.dword       = 0ULL;
	config._.first_dir = (__bsp_routing_table[src][dest] & 0x7);

	__builtin_k1_wpurge();
	__builtin_k1_fence();

	/* Writes on hardware registers. */
	mppa_cnoc[interface]->message_tx[source_tag].config.dword = config.dword;
	mppa_cnoc[interface]->message_tx[source_tag].header.dword = header.dword;

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
PUBLIC void bostan_cnoc_tx_write(int interface, int tag, uint64_t value)
{
	if (bostan_cnoc_tx_is_valid(interface, tag) && bostan_cnoc_tx_is_used(interface, tag))
		mppa_cnoc[interface]->message_tx[tag].push_eot.dword = value;
}
