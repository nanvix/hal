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

#include <arch/processor/bostan/noc/dtag.h>
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
#define BOSTAN_DNOC_TAG_USED  0x1ULL /**< Used tag?          */
#define BOSTAN_DNOC_IT_ENABLE 0x1ULL /**< Enabled interrupt? */
/**@}*/

/**
 * @name Notification modes.
 */
/**@{*/
#define BOSTAN_DNOC_EVENTS     BSP_EV_LINE /**< Event mode.     */
#define BOSTAN_DNOC_INTERRUPTS BSP_IT_LINE /**< Interrupt mode. */
/**@}*/

/**
 * @name Auxiliar macros.
 */
/**@{*/
#define FIELD(tag)  (tag / (sizeof(uint64_t) * 8)) /**< Bit field that includes the tag.              */
#define OFFSET(tag) (tag % (sizeof(uint64_t) * 8)) /**< Displacement of the tag within the bit field. */
/**@}*/

/**
 * @brief Amount of bit fields for DNoC rx resources.
 */
#define BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT (BOSTAN_DNOC_RX_MAX / (sizeof(uint64_t) * 8))

/*============================================================================*
 * Global variables                                                           *
 *============================================================================*/

/**
 * @name Reciever resources.
 */
/**@{*/
PRIVATE uint64_t bostan_dnoc_rx_tags[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT]               ALIGN(sizeof(dword_t)); /**< Receiver tags.                  */
PRIVATE uint64_t bostan_dnoc_rx_its[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT]                ALIGN(sizeof(dword_t)); /**< Receiver tags interrupt status. */
PRIVATE bostan_processor_noc_handler_fn bostan_dnoc_rx_handlers[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_RX_MAX] ALIGN(sizeof(dword_t)); /**< Receiver tags handlers.         */
/**@}*/

/**
 * @name Transfer resources.
 */
/**@{*/
PRIVATE uint8_t bostan_dnoc_tx_tags[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM]                     ALIGN(sizeof(dword_t)); /**< Transfer tags.                                    */
PRIVATE int bostan_dnoc_tx_old_fdir[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_TX_MAX] ALIGN(sizeof(dword_t)); /**< History of the First Direction of the TX Channel. */
/**@}*/

/**
 * @name Ucore resources.
 *
 * @todo Ucore doesn't trigger events/interrupts yet.
 */
/**@{*/
PRIVATE uint8_t bostan_dnoc_uc_tags[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM]                        ALIGN(sizeof(dword_t)); /**< Ucore tags.                                           */
PRIVATE int bostan_dnoc_uc_job[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX]         ALIGN(sizeof(dword_t)); /**< Job queue control.                                    */
PRIVATE uint8_t bostan_dnoc_tx_uc_link[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX] ALIGN(sizeof(dword_t)); /**< Control the association of a TX channel with a Ucore. */
//! PRIVATE uint8_t bostan_dnoc_uc_its[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT]                 ALIGN(sizeof(dword_t)); /**< Ucore interrupt status.  */
//! PRIVATE bostan_processor_noc_handler_fn bostan_dnoc_uc_handlers[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX] ALIGN(sizeof(dword_t)); /**< Ucore handlers.          */

 /* These variables are to simulate Compute Cluster behavior on IO Clusters (Maybe it can be removed). */
#ifdef __k1io__
	mOS_uc_desc_t bostan_dnoc_uc_descriptor[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX]    ALIGN(sizeof(dword_t)); /**< Information about which set of resources must be updated.          */
	mOS_uc_path_t bostan_dnoc_uc_path[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX]          ALIGN(sizeof(dword_t)); /**< Defines ths NoC route and tag to be used to reach the destination. */
	mOS_uc_notify_mode_t bostan_dnoc_uc_notify[BOSTAN_PROCESSOR_NOC_INTERFACES_NUM][BOSTAN_DNOC_UC_MAX] ALIGN(sizeof(dword_t)); /**< Information about which pe set will be notified.                   */
#endif
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
 * @param interface Number of the interface.
 * @param tag Number of the interface.
 *
 * @return Zero if tag is valid non zero otherwise.
 */
PRIVATE inline int bostan_dnoc_rx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)
	    && WITHIN(tag, BOSTAN_DNOC_RX_BASE, BOSTAN_DNOC_RXS_NUM);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_rx_set_used(int interface, int tag)
{
	bostan_dnoc_rx_tags[interface][FIELD(tag)] |= (BOSTAN_DNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_rx_set_unused(int interface, int tag)
{
	bostan_dnoc_rx_tags[interface][FIELD(tag)] &= ~(BOSTAN_DNOC_TAG_USED << OFFSET(tag));
}

/**
 * @brief Sets interrupt enable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_rx_enable_it(int interface, int tag)
{
	bostan_dnoc_rx_its[interface][FIELD(tag)] |= (BOSTAN_DNOC_IT_ENABLE << OFFSET(tag));
}

/**
 * @brief Sets interrupt disable.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_rx_disable_it(int interface, int tag)
{
	bostan_dnoc_rx_its[interface][FIELD(tag)] &= ~(BOSTAN_DNOC_IT_ENABLE << OFFSET(tag));
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline int bostan_dnoc_rx_is_used(const int interface, int tag)
{
	return ((bostan_dnoc_rx_tags[interface][FIELD(tag)] >> OFFSET(tag)) & BOSTAN_DNOC_TAG_USED);
}

/**
 * @brief Asserts whether or not a interrupt is enabled for a given resource.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target interrupt is enabled, and zero otherwise.
 */
PRIVATE inline int bostan_dnoc_rx_it_is_enable(const int interface, int tag)
{
	return ((bostan_dnoc_rx_its[interface][FIELD(tag)] >> OFFSET(tag)) & BOSTAN_DNOC_IT_ENABLE);
}

/*----------------------------------------------------------------------------*
 * Transfer functions                                                         *
 *----------------------------------------------------------------------------*/

/**
 * @brief Assess reciver tag limits.
 *
 * @param tag Number of the interface.
 *
 * @return Zero if tag is valid non zero otherwise.
 */
PRIVATE inline int bostan_dnoc_tx_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)
	    && WITHIN(tag, BOSTAN_DNOC_TX_BASE, BOSTAN_DNOC_TXS_NUM);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_tx_set_used(int interface, int tag)
{
	bostan_dnoc_tx_tags[interface] |= (BOSTAN_DNOC_TAG_USED << tag);
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_tx_set_unused(int interface, int tag)
{
	bostan_dnoc_tx_tags[interface] &= ~(BOSTAN_DNOC_TAG_USED << tag);
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline int bostan_dnoc_tx_is_used(const int interface, int tag)
{
	return ((bostan_dnoc_tx_tags[interface] >> tag) & BOSTAN_DNOC_TAG_USED);
}

/*----------------------------------------------------------------------------*
 * Ucore functions                                                            *
 *----------------------------------------------------------------------------*/

/**
 * @brief Assess ucore tag limits.
 *
 * @param tag Number of the interface.
 *
 * @return Zero if tag is valid non zero otherwise.
 */
PRIVATE inline int bostan_dnoc_uc_is_valid(int interface, int tag)
{
	return WITHIN(interface, 0, BOSTAN_PROCESSOR_NOC_INTERFACES_NUM)
	    && WITHIN(tag, BOSTAN_DNOC_UC_BASE, BOSTAN_DNOC_UCS_NUM);
}

/**
 * @brief Sets a resource as used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_uc_set_used(int interface, int tag)
{
	bostan_dnoc_uc_tags[interface] |= (BOSTAN_DNOC_TAG_USED << tag);
}

/**
 * @brief Sets a resource as not used.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 */
PRIVATE inline void bostan_dnoc_uc_set_unused(int interface, int tag)
{
	bostan_dnoc_uc_tags[interface] &= ~(BOSTAN_DNOC_TAG_USED << tag);
}

/**
 * @brief Asserts whether or not a resource is in use.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline int bostan_dnoc_uc_is_used(const int interface, int tag)
{
	return ((bostan_dnoc_uc_tags[interface] >> tag) & BOSTAN_DNOC_TAG_USED);
}

/**
 * @brief Check if a TX channel is linked to a Ucore.
 *
 * @param interface Underlying DMA interface.
 * @param uc        Underlying ucore tag.
 * @param tx        Underlying transfer tag.
 *
 * @returns Non zero if the Ucore is linked to the Tx channel, and zero otherwise.
 */
PRIVATE inline int bostan_dnoc_uc_is_linked(int interface, int uc, int tx)
{
	return ((bostan_dnoc_tx_uc_link[interface][uc] >> tx) & BOSTAN_DNOC_TAG_USED);
}

/**
 * @brief Link a TX channel to a Ucore.
 *
 * @param interface Underlying DMA interface.
 * @param uc        Underlying ucore tag.
 * @param tx        Underlying transfer tag.
 *
 * @returns Zero if link successfully, and non zero otherwise.
 */
PRIVATE inline int bostan_dnoc_uc_link(int interface, int uc, int tx)
{
	uint32_t channel_id;

	if (bostan_dnoc_uc_is_linked(interface, uc, tx))
		return (0);

	/* Build channel identification on IO, i.e, repeats tx tag on an int (4 bits each). */
	channel_id = __k1_dnoc_ucore_set_indirection_table(tx);

	/* Compute clusters need explicit linking the Ucore with the TX channel. */
#ifdef __k1dp__
	mOS_async_uc_link_tx(uc, tx, channel_id, BOSTAN_DNOC_EVENTS);

	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EINVAL);
#else
	mOS_async_uc_get_cfg_perm(interface, uc, BOSTAN_DNOC_EVENTS);

	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EINVAL);

	mppa_dnoc[interface]->threads[uc].channel_ids.word = channel_id;
#endif

	bostan_dnoc_tx_uc_link[interface][uc] |= (BOSTAN_DNOC_TAG_USED << tx);

	return (0);
}

/**
 * @brief Unlink a TX channel from a Ucore.
 *
 * @param interface Underlying DMA interface.
 * @param uc        Underlying ucore tag.
 * @param tx        Underlying transfer tag.
 *
 * @returns Zero if unlink successfully, and non zero otherwise.
 */
PRIVATE inline int bostan_dnoc_uc_unlink(int interface, int uc, int tx)
{
	if (!bostan_dnoc_uc_is_linked(interface, uc, tx))
		return (0);

	/* Compute clusters need explicit unlinking the Ucore with the TX channel. */
#ifdef __k1dp__
	mOS_async_uc_unlink_tx(uc, BOSTAN_DNOC_EVENTS);

	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EINVAL);
#endif

	bostan_dnoc_tx_uc_link[interface][uc] &= ~(BOSTAN_DNOC_TAG_USED << tx);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_rx_update_notify()                                             *
 *============================================================================*/

/**
 * @brief Update underlying notification mode.
 *
 * @param interface Underlying DMA interface.
 * @param tag       Underlying resource ID.
 * @param events    0 to use events, 1 to use interrupts.
 * @param core_mask Defines what cores will be notified.
 *
 * @returns Non zero if the target resource is in used, and zero otherwise.
 */
PRIVATE inline void bostan_dnoc_rx_update_notify(int interface, int tag, int events, int cores_mask)
{
	mOS_async_rx_get_cfg_perm(interface, tag, BOSTAN_DNOC_EVENTS);
	mOS_async_rx_rda_event_waitclear(BOSTAN_DNOC_EVENTS);
	mOS_async_rx_get_rw_perm(interface, tag, BOSTAN_DNOC_EVENTS);
	mOS_async_rx_rda_event_waitclear(BOSTAN_DNOC_EVENTS);

	mOS_rx_cfg_line(interface, tag, events, cores_mask);
}

/**
 * @brief Check if has any interrupt event on dnoc interfaces.
 *
 * @return Non-zero if has received a interrupt, zero otherwise.
 */
PRIVATE int bostan_dnoc_it_received(void)
{
	for (unsigned interface = 0; interface < BOSTAN_PROCESSOR_NOC_INTERFACES_NUM; interface++)
		for (unsigned int field = 0; field < BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT; field++)
			if (mppa_dnoc[interface]->rx_global.events[field].dword)
				return (1);

	return (0);
}

/**
 * @brief Underlying handler for DNoC interrupts.
 *
 * @details When a DNoC interrupt is triggered, we do not know what is the
 * interface that generated it. So, for each interface, we need to search
 * the RX tags fields to find the correct tag to call its handler.
 *
 * @param ev_src Interrupt ID
 */
PRIVATE void bostan_dnoc_it_handler(int ev_src)
{
	int tag;
	int offset;
	uint64_t tags_status;
	volatile dword_t * field_status;
	bostan_processor_noc_handler_fn handler;

	if (ev_src < 0)
		interrupt_mask(K1B_INT_DNOC);

	dcache_invalidate();

	while (bostan_dnoc_it_received())
	{
		for (unsigned int interface = 0; interface < BOSTAN_PROCESSOR_NOC_INTERFACES_NUM; interface++)
		{
			for (unsigned int field = 0; field < BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT; field++)
			{
				/**
				* Masks the DNOC interrupt to get the triggered tags and clear them
				* without intermediary interrupts.
				*/
				tags_status  = 0ULL;
				field_status = &mppa_dnoc[interface]->rx_global.events[field].dword;

				/* Gets the set of tags that generated an interrupt. */
				while ((tags_status = (*field_status)) != 0ULL)
				{
					/* For each tag that generated an interrupt, cleans its flags. */
					while (tags_status)
					{
						offset       = __builtin_k1_ctzdl(tags_status);
						tags_status &= ~(1ULL << offset);

						tag = field * (sizeof(uint64_t) * 8) + offset;

						mppa_dnoc[interface]->rx_queues[tag].event_lac.hword;

						__k1_dnoc_event_clear_rx(interface);

						handler = (bostan_processor_noc_handler_fn) __k1_umem_read32(
							(void *) &bostan_dnoc_rx_handlers[interface][tag]
						);

						if (handler)
							handler(interface, tag);
					}
				}
			}
		}
	}

	if (ev_src < 0)
		interrupt_unmask(K1B_INT_DNOC);
}

/*============================================================================*
 * bostan_dnoc_it_verify()                                                    *
 *============================================================================*/

/**
 * @brief Verify lost interrupts.
 */
PUBLIC void bostan_dnoc_it_verify(void)
{
	bostan_dnoc_it_handler(-1);
}

/*============================================================================*
 * bostan_dnoc_setup()                                                        *
 *============================================================================*/

/**
 * @brief Dummy DNoC handler.
 */
PRIVATE void bostan_dnoc_dummy_noc_handler(int interface, int tag)
{
	UNUSED(interface);
	UNUSED(tag);
}

/**
 * @brief Initializes the control structures and configures the interrupt
 * handlers.
 */
PUBLIC void bostan_dnoc_setup(void)
{
	interrupt_register(K1B_INT_DNOC, bostan_dnoc_it_handler);

	for (int interface = 0; interface < BOSTAN_PROCESSOR_NOC_INTERFACES_NUM; interface++)
	{
		/* IO Cluster need initializes the boot status of the interface. */
#ifdef __k1io__
		mOS_dnoc_set_boot_status(interface, 1);
#endif

		/* All RX tags are unused. */
		for (unsigned int field = 0; field < BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT; field++)
			bostan_dnoc_rx_tags[interface][field] = 0ULL;

		/* All tags do not have handlers. */
		for (unsigned int field = 0; field < BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT; field++)
			bostan_dnoc_rx_its[interface][field] = 0ULL;

		for (unsigned int tag = 0; tag < BOSTAN_DNOC_RX_BIT_FIELD_AMOUNT; tag++)
			bostan_dnoc_rx_handlers[interface][tag] = &bostan_dnoc_dummy_noc_handler;

		/* All TX tags are unused. */
		bostan_dnoc_tx_tags[interface] = 0;

		/* Old first direction of each TX tag are -2 by Kalray. */
		for (int tag = 0; tag < BOSTAN_DNOC_TX_MAX; tag++)
			bostan_dnoc_tx_old_fdir[interface][tag] = -2;

		/* All Ucore tags are unused. */
		bostan_dnoc_uc_tags[interface] = 0;

		/* Initializes Ucore control structures. */
		for (int tag = 0; tag < BOSTAN_DNOC_UC_MAX; tag++)
		{
			bostan_dnoc_uc_job[interface][tag]     = 0;
			bostan_dnoc_tx_uc_link[interface][tag] = 0;
#ifdef __k1io__
			bostan_dnoc_uc_descriptor[interface][tag]._word = 0;
			bostan_dnoc_uc_notify[interface][tag]._word     = 0;
			bostan_dnoc_uc_path[interface][tag].header.dword          = 0ULL;
			bostan_dnoc_uc_path[interface][tag].header_ext.dword      = 0ULL;
			bostan_dnoc_uc_path[interface][tag].min_max_task_id.dword = 0ULL;
#endif
			bostan_dnoc_rx_update_notify(interface, tag, BOSTAN_DNOC_EVENTS, 0);

			/* Maybe this is not necessary but in some mOS functions, they set the activation register. */
			mOS_async_uc_get_cfg_perm(interface, tag, BOSTAN_DNOC_EVENTS);
			if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
				kpanic("Error on ucore configuration!");

			__k1_io_write32(
				(void *) &mppa_dnoc[interface]->threads[tag].activation.word,
				_K1_NOCV2_ENABLE_DIRECT_UCORE
			);
		}
	}

	/* Flush dcache to sram. */
	dcache_invalidate();
}

/*============================================================================*
 * Data Receiver                                                              *
 *============================================================================*/

/*============================================================================*
 * bostan_dnoc_rx_alloc()                                                     *
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
	if (!bostan_dnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_dnoc_rx_is_used(interface, tag))
		return (-EBUSY);

	bostan_dnoc_rx_set_used(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_rx_free()                                                      *
 *============================================================================*/

/**
 * @brief Free D-NoC receiver buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof receiver buffer.
 *
 * @return Zero if releases correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_rx_free(int interface, int tag)
{
	if (!bostan_dnoc_rx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_dnoc_rx_is_used(interface, tag))
		return (-EINVAL);

	bostan_dnoc_rx_set_unused(interface, tag);

	if (bostan_dnoc_rx_it_is_enable(interface, tag))
	{
		bostan_dnoc_rx_handlers[interface][tag] = &bostan_dnoc_dummy_noc_handler;
		bostan_dnoc_rx_disable_it(interface, tag);
	}

	/* Configures the tag to generate an event but the event does not notify anyone. */
	bostan_dnoc_rx_update_notify(interface, tag, BOSTAN_DNOC_EVENTS, 0);

	/* Cleans event and item counters. */
	mppa_dnoc[interface]->rx_queues[tag].event_lac.hword;
	mppa_dnoc[interface]->rx_queues[tag].item_lac.dword;

	return (0);
}

/*============================================================================*
 * bostan_dnoc_rx_wait()                                                      *
 *============================================================================*/

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
	/**
	 * @note: The distinction of actions for each type of CPU is because
	 * the mOS function does not wait correctly in the IO cluster.
	 */

#ifdef __k1dp__
	mOS_rx_event_waitclear(interface, BOSTAN_DNOC_EVENTS, tag);
#else
	int voffset;
	volatile uint64_t *chunck64_ptr;

	voffset = OFFSET(tag);
	chunck64_ptr = &mppa_dnoc[interface]->rx_global.events[FIELD(tag)].dword;

	do
	{
		if ((*chunck64_ptr >> voffset) & 0x1)
			break;

		__builtin_k1_waitclr1((sizeof(__k1_uint32_t) * 8) + _K1_NOC_RX_IDX + interface);
	} while (1);
#endif

	/* Cleans event counter. */
	mppa_dnoc[interface]->rx_queues[tag].event_lac.hword;

	/* Invalites the dcache for get the incoming data. */
	dcache_invalidate();

	return (0);
}

/*============================================================================*
 * bostan_dnoc_rx_config()                                                    *
 *============================================================================*/

/**
 * @brief Configure D-NoC receiver buffer.
 *
 * @details The mode used for triggers events/interrupts is identified by the
 * number 3. This mode decrement the item_counter, when it reaches 0, it will
 * generate an event/interrupt and reloads the item_counter with the value on
 * item_reload.
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
	uint64_t min_size,
	uint64_t max_size,
	uint64_t offset,
	bostan_processor_noc_handler_fn handler
)
{
	/* Gets permissions to configure on underlying RX resources. */
	mOS_async_rx_get_cfg_perm(interface, tag, BOSTAN_DNOC_EVENTS);
	if (mOS_async_rx_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EPERM);

	/* Gets permissions to read/write on underlying RX resources. */
	mOS_async_rx_get_rw_perm(interface, tag, BOSTAN_DNOC_EVENTS);
	if (mOS_async_rx_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EPERM);

#ifdef __k1io__
	/* Sets manually the underlying registers. */
	mppa_dnoc[interface]->rx_queues[tag].buffer_base.dword    = (uint64_t) (uintptr_t) buffer;
	mppa_dnoc[interface]->rx_queues[tag].buffer_size.dword    = (uint64_t) max_size;
	mppa_dnoc[interface]->rx_queues[tag].current_offset.dword = (uint64_t) offset;
	mppa_dnoc[interface]->rx_queues[tag].item_counter.dword   = min_size;
	mppa_dnoc[interface]->rx_queues[tag].item_reload.dword    = min_size;
	mppa_dnoc[interface]->rx_queues[tag].event_counter.hword  = 0;
	mppa_dnoc[interface]->rx_queues[tag].reload_mode.bword    = 3;
	mppa_dnoc[interface]->rx_queues[tag].activation.bword     = 1;
#else
	/* Calls mOS function to configure the underlying registers. */
	mOS_async_rx_configure(
		interface,
		tag,
		((uintptr_t) buffer),
		max_size,
		offset,
		min_size,
		min_size,
		0,
		3,
		1,
		BOSTAN_DNOC_EVENTS
	);

	if (mOS_async_rx_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EINVAL);
#endif

	/* Configures notification mode. */
	if (handler != NULL)
	{
		bostan_dnoc_rx_handlers[interface][tag] = handler;
		bostan_dnoc_rx_enable_it(interface, tag);

		bostan_dnoc_rx_update_notify(interface, tag, BOSTAN_DNOC_INTERRUPTS, (1 << core_get_id()));
	}
	else
		bostan_dnoc_rx_update_notify(interface, tag, BOSTAN_DNOC_EVENTS, (~0));

	return (0);
}

/*============================================================================*
 * Data Transfer                                                              *
 *============================================================================*/

/*============================================================================*
 * bostan_dnoc_tx_alloc()                                                     *
 *============================================================================*/

/**
 * @brief Allocate D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof transfer buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_tx_alloc(int interface, int tag)
{
	if (!bostan_dnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (bostan_dnoc_tx_is_used(interface, tag))
		return (-EBUSY);

	bostan_dnoc_tx_set_used(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_tx_free()                                                      *
 *============================================================================*/

/**
 * @brief Free D-NoC transfer buffer.
 *
 * @param interface Number of the interface.
 * @param tag       Number of transfer buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_tx_free(int interface, int tag)
{
	mppa_dnoc_channel_config_t config;

	if (!bostan_dnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_dnoc_tx_is_used(interface, tag))
		return (-EINVAL);

	/* Default configuration (default values by Kalray). */
	config.reg             = 0ULL;
	config._.payload_max   = 62;
	config._.payload_min   = 2;
	config._.cfg_pe_en     = 1;
	config._.cfg_user_en   = 1;
	config._.write_pe_en   = 1;
	config._.write_user_en = 1;

	/* Writes on underlying registers. */
	mppa_dnoc[interface]->tx_channels[tag].header.reg = 0ULL;
	mppa_dnoc[interface]->tx_channels[tag].config.reg = config.reg;

	bostan_dnoc_tx_set_unused(interface, tag);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_tx_config()                                                    *
 *============================================================================*/

/**
 * @brief Configure D-NoC transfer tag.
 *
 * @param interface   Number of the interface.
 * @param localid Source interface ID.
 * @param local_tag  Number of source buffer.
 * @param remoteid Target interface ID.
 * @param remote_tag  Number of target buffer.
 *
 * @return Zero if configure successfully and non zero otherwise.
 *
 * @todo Discover why and if old_fdir verification is necessary.
 */
PUBLIC int bostan_dnoc_tx_config(
	int interface,
	int localid,
	int local_tag,
	int remoteid,
	int remote_tag
)
{
	int src;
	int dest;
	mppa_dnoc_header_t header;
	mppa_dnoc_channel_config_t config;

	if (!bostan_dnoc_tx_is_valid(interface, local_tag))
		return (-EINVAL);

	if (!bostan_dnoc_tx_is_used(interface, local_tag))
		return (-EINVAL);

	src  = __bsp_get_router_id(localid);
	dest = __bsp_get_router_id(remoteid);

	/* Sets header values (default values by Kalray). */
	header.dword   = 0ULL;
	header._.valid = 1;
	header._.route = (__bsp_routing_table[src][dest] >> 3);
	header._.tag   = remote_tag;

	/* Sets configuration values (default values by Kalray). */
	config.dword               = 0;
	config._.bw_current_credit = 190;
	config._.bw_max_credit     = 190;
	config._.payload_max       = 32;
	config._.payload_min       = 6;
	config._.first_dir         = (__bsp_routing_table[src][dest] & 0x7);
	config._.cfg_pe_en         = 1;
	config._.cfg_user_en       = 1;
	config._.write_pe_en       = 1;
	config._.write_user_en     = 1;

	/*  Checks if the tx channel are saturated. */
	int old_fdir = bostan_dnoc_tx_old_fdir[interface][local_tag];

	if (old_fdir != -2)
	{
		/* Wait TX channel be free. */
		while (mppa_dnoc[interface]->tx_ports[local_tag].watermark.reg != 3);

		/**
		 * Kalray's comment: We still can't reprogram the TX due to #6937.
		 * We can only reprogram it once we know that the FIFO is not saturated
		 */
		while (mppa_dnoc[interface]->dma_global.router_fifo[5 * old_fdir].level.hword > 350);
	}

	/* Updates first direction of local_tag. */
	bostan_dnoc_tx_old_fdir[interface][local_tag] = config._.first_dir;

	/* Flush dcache to sram. */
	dcache_invalidate();

	/* Writes on underlying registers. */
	mppa_dnoc[interface]->tx_channels[local_tag].header.dword = header.dword;
	mppa_dnoc[interface]->tx_channels[local_tag].config.dword = config.dword;

	return (0);
}

/*============================================================================*
 * TX Channel Auxiliar Macros (manually transfers)                            *
 *============================================================================*/

/**
 * @brief Generic macro to write a data in a packet shaper.
 * @param __interface Number of the interface.
 * @param __tx        Number of transfer tag.
 * @param __data      Data to be written (8/16/32 or 64 bits).
 * @param __field     TX port to write the data.
 */
#define BOSTAN_DNOC_TX_WRITE_GENERIC(__interface, __tx, __data, __field) \
	{ \
		switch (sizeof(__data)) { \
		case 1: \
			*( volatile uint8_t* ) &mppa_dnoc[(__interface)]->tx_ports[(__tx)].__field = (__data); \
			break; \
		case 2: \
			*( volatile uint16_t* ) &mppa_dnoc[(__interface)]->tx_ports[(__tx)].__field = (__data); \
			break; \
		case 4: \
			*( volatile uint32_t* ) &mppa_dnoc[(__interface)]->tx_ports[(__tx)].__field = (__data); \
			break; \
		case 8: \
			*( volatile uint64_t* ) &mppa_dnoc[(__interface)]->tx_ports[(__tx)].__field = (__data); \
			break; \
		default: \
			break; \
		} \
	}

/**
 * @brief Macro to write a data in a packet shaper.
 * @param __interface Number of the interface.
 * @param __tx        Number of transfer tag.
 * @param __data      Data to be written (8/16/32 or 64 bits).
 */
#define BOSTAN_DNOC_TX_WRITE(__interface, __tx, __data) \
	BOSTAN_DNOC_TX_WRITE_GENERIC(__interface, __tx, __data, push_data)

/**
 * @brief Macro to write a data in a packet shaper. The data
 * will carry an EOT signal.
 * @param __interface Number of the interface.
 * @param __tx        Number of transfer tag.
 * @param __data      Data to be written (8/16/32 or 64 bits).
 */
#define BOSTAN_DNOC_TX_WRITE_EOT(__interface, __tx, __data) \
	BOSTAN_DNOC_TX_WRITE_GENERIC(__interface, __tx, __data, push_data_eot)

/**
 * @brief Macro to write a data in a packet shaper. Then,
 * the packet shaper will be flushed.
 * @param __interface Number of the interface.
 * @param __tx        Number of transfer tag.
 * @param __data      Data to be written (8/16/32 or 64 bits).
 */
#define BOSTAN_DNOC_TX_WRITE_FLUSH(__interface, __tx, __data) \
	BOSTAN_DNOC_TX_WRITE_GENERIC(__interface, __tx, __data, push_data_flush)

/**
 * @brief Macro to sent offset in a packet shaper.
 * @param __interface Number of the interface.
 * @param __tx        Number of transfer tag.
 * @param __offset    Offset to be sent to the destination.
 */
#define BOSTAN_DNOC_TX_WRITE_OFFSET(__interface, __tx, __offset) \
	BOSTAN_DNOC_TX_WRITE_GENERIC(__interface, __tx, __offset, push_offset)

/*============================================================================*
 * bostan_dnoc_tx_write()                                                     *
 *============================================================================*/

/**
 * @brief Writes on D-NoC transfer tag.
 *
 * @param interface Number of the DMA channel.
 * @param tag       Transfer buffer ID.
 * @param buffer    Local data pointer.
 * @param size      Amount of bytes to transfer.
 * @param offset    Offset on local buffer data.
 *
 * @todo Discover how to send offset before the data (Still not working).
 */
PUBLIC int bostan_dnoc_tx_write(
	int interface,
	int tag,
	const void *buffer,
	uint64_t size,
	uint64_t offset
)
{
	mppa_dnoc_address_t address_config;
	mppa_dnoc_push_offset_t offset_config;

	if (!bostan_dnoc_tx_is_valid(interface, tag))
		return (-EINVAL);

	if (!bostan_dnoc_tx_is_used(interface, tag))
		return (-EINVAL);

	/* Dynamic offset. */
	address_config.word       = 0;
	address_config._.offset   = offset;
	address_config._.protocol = 0x1; //! Absolute:0x1 | Relative:0x2
	address_config._.valid    = 1;

	/* Static offset. */
	offset_config.dword      = 0;
	offset_config._.offset   = offset;
	offset_config._.protocol = 0x1; //! Absolute:0x1 | Relative:0x2
	offset_config._.valid    = 1;

	/* Sends offset manually. */
	BOSTAN_DNOC_TX_WRITE_OFFSET(interface, tag, offset_config.dword);

	/* Uses default primitive to send data. */
	__k1_dnoc_send_data_force_offset(
		interface,
		tag,
		(void *) buffer,
		size,
		1,
		address_config
	);

	return (0);
}

/*============================================================================*
 * UCore                                                                      *
 *============================================================================*/

/*============================================================================*
 * bostan_dnoc_uc_alloc()                                                     *
 *============================================================================*/

/**
 * @brief Allocate D-NoC ucore sender.
 *
 * @param interface Number of the interface.
 * @param tag       Numberof of ucore.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_uc_alloc(int interface, int uctag, int txtag)
{
	mppa_dnoc_thread_event_it_target_t event_config;

	if (!bostan_dnoc_uc_is_valid(interface, uctag))
		return (-EINVAL);

	if (bostan_dnoc_uc_is_used(interface, uctag))
		return (-EBUSY);

	if (bostan_dnoc_tx_alloc(interface, txtag) != 0)
		return (-EBUSY);

	/* Gets permission to configure notification mode. */
	mOS_async_uc_get_cfg_perm(interface, uctag, BOSTAN_DNOC_EVENTS);

	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
	{
		bostan_dnoc_tx_free(interface, txtag);
		return (-EPERM);
	}

	/* Default notification mode. */
	event_config._.event_en = 1;
	event_config._.it_en    = 0;
#ifdef __k1dp__
	event_config._.cluster  = (~0); //! Alternative: (1 << core_get_id())
#else
	event_config._.rm       = (~0); //! Alternative: (1 << core_get_id())
#endif

	/* Writes on underlying registers. */
	mppa_dnoc[interface]->threads[uctag].event_it_target.reg = event_config.reg;

	bostan_dnoc_uc_set_used(interface, uctag);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_uc_free()                                                      *
 *============================================================================*/

/**
 * @brief Free D-NoC ucore sender.
 *
 * @param interface Number of the interface.
 * @param uctag     Number of ucore.
 * @param txtag     Number of transfer buffer.
 *
 * @return Zero if allocate correctly and non zero otherwise.
 */
PUBLIC int bostan_dnoc_uc_free(int interface, int uctag, int txtag)
{
	if (!bostan_dnoc_uc_is_valid(interface, uctag))
		return (-EINVAL);

	if (!bostan_dnoc_uc_is_used(interface, uctag))
		return (-EINVAL);

	/* We link they only if a write is executed. */
	if (bostan_dnoc_uc_is_linked(interface, uctag, txtag))
		if (bostan_dnoc_uc_unlink(interface, uctag, txtag) != 0)
			return (-EINVAL);

	/* We can only handle unlinked TX channel. */
	if (bostan_dnoc_tx_free(interface, txtag) != 0)
		return (-EINVAL);

	bostan_dnoc_uc_set_unused(interface, uctag);

	return (0);
}

/*============================================================================*
 * bostan_dnoc_uc_wait()                                                      *
 *============================================================================*/

/**
 * @brief Wait events on D-NoC sender buffer.
 *
 * @param interface Number of the interface.
 * @param uctag     Number of ucore.
 *
 * @return Amount of received events.
 *
 * @todo Ucore is not triggering an event/interrupt, so
 * we need to fix it and maybe change the wait condition.
 */
PUBLIC int bostan_dnoc_uc_wait(int interface, int uctag)
{
#if 0
	/**
	 * This implementation depends on Ucore generates events,
	 * but for some reason, it isn't generating.
	 */

#ifdef __k1dp__
	mOS_uc_event_pop_waitclear(BOSTAN_DNOC_EVENTS, uctag);
	// while(mOS_uc_pop_event(uctag) != 0); //! It does not clean the event counter.
#else
	while(mOS_ucore_event_lac(interface, uctag) == 0);
#endif
#else
	/**
	 * This implementation waits the Ucore thread change the status
	 * to suspend (value 0) and its works apparently.
	 */

	/* Gets permission to read the program status. */
	mOS_async_uc_get_write_perm(interface, uctag, BOSTAN_DNOC_EVENTS);
	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		return (-EINVAL);

	/* Wait Ucore thread be suspended. */
	while (__k1_dnoc_ucore_load_program_status(interface, uctag) != 0);

	/* Clears the event counter even though it is not incremented. */
	__k1_dnoc_ucore_loadnclear_event_counter(interface, uctag);
#endif

	return (0);
}

/*============================================================================*
 * Ucore Auxiliar Macros and Variables                                        *
 *============================================================================*/

/**
 * @name Auxiliar macro to hide theses huge variable names.
 */
/**@{*/
#define UC_VIRTUAL_FIFO_PATH(INTERFACE, UC, TX)   _scoreboard_start.SCB_UC.uc_virtual_fifo[INTERFACE][UC][TX].path
#define UC_VIRTUAL_FIFO_NOTIFY(INTERFACE, UC, TX) _scoreboard_start.SCB_UC.uc_virtual_fifo[INTERFACE][UC][TX].notify
/**@}*/

/**
 * @name Ucore programs.
 */
/**@{*/
/**
 * @brief Linear microcode (from Kalray).
 *
 * arg0: address of packet
 * arg1: number of 64 bits elements (payload size) for packet
 * arg2: undefined
 * arg3: offset
 */
unsigned long long firmware_linear[] ALIGN(128) = {
	0x0000001400000000ULL,  /* C_0: ptr_0=R[0];                                                       */
	0x0000007640000000ULL,  /* C_1: ptr_1=R[3];                                                       */
	0x0000000243800000ULL,  /* C_2: SEND_OFFSET(ptr_1,chan_0);                                        */
	0x0000003000680000ULL,  /* C_3: dcnt1=R[1];                                                       */
	0x0000000803c80013ULL,  /* C_4: READ8(ptr_0,chan_0); ptr_0+=8; if(dcnt1!=0) goto C_4; dcnt1--;    */
	0x0000000c00004000ULL,  /* C_5: SEND_EOT(chan_0); ptr_0+=0x0;                                     */
	0x0000000000022000ULL,  /* C_6: SEND_IT(); WAIT_TOKEN();                                          */
	0x0000000000000001ULL   /* C_7: goto C_0;                                                         */
};

/**
 * @brief Ethernet microcode (from Kalray).
 *
 * arg0: number of 64 bits elements for packet 1
 * arg1: number of 8 bits elements (payload size) for packet 1
 * arg2: number of 64 bits elements for packet 2
 * arg3: number of 8 bits elements (payload size) for packet 2
 * arg4: number of 64 bits elements for packet 3
 * arg5: number of 8 bits elements (payload size) for packet 3
 * arg6: number of 64 bits elements for packet 4
 * arg7: number of 8 bits elements (payload size) for packet 4
 * p0: address of packet 1
 * p1: address of packet 2
 * p2: address of packet 3
 * p3: address of packet 4
 */
unsigned long long ucode_eth[] ALIGN(128) = {
	0x0000000000000000ULL,  /* C_0:                                                                   */
	0x0000001000600000ULL,  /* C_1: dcnt0=R[0];                                                       */
	0x0000000000400012ULL,  /* C_2: if(dcnt0==0) goto C_4; dcnt0--;                                   */
	0x0000000803c0000fULL,  /* C_3: READ8(ptr_0,chan_0); ptr_0+=8; if(dcnt0!=0) goto C_3; dcnt0--;    */
	0x0000003000608000ULL,  /* C_4: FLUSH(chan_0); dcnt0=R[1];                                        */
	0x000000000040001eULL,  /* C_5: if(dcnt0==0) goto C_7; dcnt0--;                                   */
	0x000000080240001bULL,  /* C_6: READ1(ptr_0,chan_0); ptr_0+=1; if(dcnt0!=0) goto C_6; dcnt0--;    */
	0x0000000000004000ULL,  /* C_7: SEND_EOT(chan_0);                                                 */
	0x0000005000600000ULL,  /* C_8: dcnt0=R[2];                                                       */
	0x000000000040002eULL,  /* C_9: if(dcnt0==0) goto C_11; dcnt0--;                                  */
	0x0000000843c0002bULL,  /* C_10: READ8(ptr_1,chan_0); ptr_1+=8; if(dcnt0!=0) goto C_10; dcnt0--;  */
	0x0000007000608000ULL,  /* C_11: FLUSH(chan_0); dcnt0=R[3];                                       */
	0x000000000040003aULL,  /* C_12: if(dcnt0==0) goto C_14; dcnt0--;                                 */
	0x0000000842400037ULL,  /* C_13: READ1(ptr_1,chan_0); ptr_1+=1; if(dcnt0!=0) goto C_13; dcnt0--;  */
	0x0000000000004000ULL,  /* C_14: SEND_EOT(chan_0);                                                */
	0x0000009000600000ULL,  /* C_15: dcnt0=R[4];                                                      */
	0x000000000040004aULL,  /* C_16: if(dcnt0==0) goto C_18; dcnt0--;                                 */
	0x0000000883c00047ULL,  /* C_17: READ8(ptr_2,chan_0); ptr_2+=8; if(dcnt0!=0) goto C_17; dcnt0--;  */
	0x000000b000608000ULL,  /* C_18: FLUSH(chan_0); dcnt0=R[5];                                       */
	0x0000000000400056ULL,  /* C_19: if(dcnt0==0) goto C_21; dcnt0--;                                 */
	0x0000000882400053ULL,  /* C_20: READ1(ptr_2,chan_0); ptr_2+=1; if(dcnt0!=0) goto C_20; dcnt0--;  */
	0x0000000000004000ULL,  /* C_21: SEND_EOT(chan_0);                                                */
	0x000000d000600000ULL,  /* C_22: dcnt0=R[6];                                                      */
	0x0000000000400066ULL,  /* C_23: if(dcnt0==0) goto C_25; dcnt0--;                                 */
	0x00000008c3c00063ULL,  /* C_24: READ8(ptr_3,chan_0); ptr_3+=8; if(dcnt0!=0) goto C_24; dcnt0--;  */
	0x000000f000608000ULL,  /* C_25: FLUSH(chan_0); dcnt0=R[7];                                       */
	0x0000000000400072ULL,  /* C_26: if(dcnt0==0) goto C_28; dcnt0--;                                 */
	0x00000008c240006fULL,  /* C_27: READ1(ptr_3,chan_0); ptr_3+=1; if(dcnt0!=0) goto C_27; dcnt0--;  */
	0x0000000000004000ULL,  /* C_28: SEND_EOT(chan_0);                                                */
	0x0000000000000000ULL,  /* C_29:                                                                  */
	0x0000000000022000ULL,  /* C_30: SEND_IT(); WAIT_TOKEN();                                         */
	0x0000000000000001ULL   /* C_31: goto C_0;                                                        */
};

/**
 * @brief Ethernet microcode Version 2 (from Kalray).
 *
 * arg0: number of 64 bits elements for packet 1
 * arg1: number of 8 bits elements (payload size) for packet 1
 * arg2: number of 64 bits elements for packet 2
 * arg3: number of 8 bits elements (payload size) for packet 2
 * arg4: number of 64 bits elements for packet 3
 * arg5: number of 8 bits elements (payload size) for packet 3
 * arg6: number of 64 bits elements for packet 4
 * arg7: number of 8 bits elements (payload size) for packet 4
 * p0: address of packet 1
 * p1: address of packet 2
 * p2: address of packet 3
 * p3: address of packet 4
 */
PRIVATE unsigned long long ucode_eth_v2[] ALIGN(128) = {
	0x0000001000600000ULL, /* C_0: dcnt0=R[0];                                                        */
	0x0000003000680000ULL, /* C_1: dcnt1=R[1];                                                        */
	0x000000000008004aULL, /* C_2: if(dcnt1==0) goto C_18;                                            */
	0x000000000040001aULL, /* C_3: if(dcnt0==0) goto C_6; dcnt0--;                                    */
	0x0000000803c00013ULL, /* C_4: READ8(ptr_0,chan_0); ptr_0+=8; if(dcnt0!=0) goto C_4; dcnt0--;     */
	0x0000000000008000ULL, /* C_5: FLUSH(chan_0);                                                     */
	0x0000000000480041ULL, /* C_6: goto C_16; dcnt1--;                                                */
	0x0000000000000041ULL, /* C_7: goto C_16;                                                         */
	0x0000000000001000ULL, /* C_8: STOP(); ALIGN DROP ADDRESS                                         */
	0x0000000000001000ULL, /* C_9: STOP(); ALIGN DROP ADDRESS                                         */
	0x0000000000001000ULL, /* C_10: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_11: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_12: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_13: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_14: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_15: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000802480043ULL, /* C_16: READ1(ptr_0,chan_0); ptr_0+=1; if(dcnt1!=0) goto C_16; dcnt1--;   */
	0x0000000000004059ULL, /* C_17: SEND_EOT(chan_0); goto C_22;                                      */
	0x000000000040005aULL, /* C_18: if(dcnt0==0) goto C_22; dcnt0--;                                  */
	0x0000000000400000ULL, /* C_19: dcnt0--;                                                          */
	0x0000000803c00053ULL, /* C_20: READ8(ptr_0,chan_0); ptr_0+=8; if(dcnt0!=0) goto C_20; dcnt0--;   */
	0x0000000803804000ULL, /* C_21: SEND_EOT(chan_0); READ8(ptr_0,chan_0); ptr_0+=8;                  */
	0x0000005000600000ULL, /* C_22: dcnt0=R[2];                                                       */
	0x0000007000680081ULL, /* C_23: goto C_32; dcnt1=R[3];                                            */
	0x0000000000001000ULL, /* C_24: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_25: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_26: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_27: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_28: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_29: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_30: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_31: STOP(); ALIGN DROP ADDRESS                                        */
	0x00000000000800c2ULL, /* C_32: if(dcnt1==0) goto C_48;                                           */
	0x0000000000400092ULL, /* C_33: if(dcnt0==0) goto C_36; dcnt0--;                                  */
	0x0000000843c0008bULL, /* C_34: READ8(ptr_1,chan_0); ptr_1+=8; if(dcnt0!=0) goto C_34; dcnt0--;   */
	0x0000000000008000ULL, /* C_35: FLUSH(chan_0);                                                    */
	0x0000000000480000ULL, /* C_36: dcnt1--;                                                          */
	0x0000000842480097ULL, /* C_37: READ1(ptr_1,chan_0); ptr_1+=1; if(dcnt1!=0) goto C_37; dcnt1--;   */
	0x00000000000040d1ULL, /* C_38: SEND_EOT(chan_0); goto C_52;                                      */
	0x00000000000000c1ULL, /* C_39: goto C_48;                                                        */
	0x0000000000001000ULL, /* C_40: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_41: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_42: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_43: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_44: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_45: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_46: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_47: STOP(); ALIGN DROP ADDRESS                                        */
	0x00000000004000d2ULL, /* C_48: if(dcnt0==0) goto C_52; dcnt0--;                                  */
	0x0000000000400000ULL, /* C_49: dcnt0--;                                                          */
	0x0000000843c000cbULL, /* C_50: READ8(ptr_1,chan_0); ptr_1+=8; if(dcnt0!=0) goto C_50; dcnt0--;   */
	0x0000000843804000ULL, /* C_51: SEND_EOT(chan_0); READ8(ptr_1,chan_0); ptr_1+=8;                  */
	0x0000009000600000ULL, /* C_52: dcnt0=R[4];                                                       */
	0x000000b000680000ULL, /* C_53: dcnt1=R[5];                                                       */
	0x000000000008011aULL, /* C_54: if(dcnt1==0) goto C_70;                                           */
	0x0000000000000101ULL, /* C_55: goto C_64;                                                        */
	0x0000000000001000ULL, /* C_56: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_57: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_58: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_59: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_60: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_61: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_62: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_63: STOP(); ALIGN DROP ADDRESS                                        */
	0x000000000040010eULL, /* C_64: if(dcnt0==0) goto C_67; dcnt0--;                                  */
	0x0000000883c00107ULL, /* C_65: READ8(ptr_2,chan_0); ptr_2+=8; if(dcnt0!=0) goto C_65; dcnt0--;   */
	0x0000000000008000ULL, /* C_66: FLUSH(chan_0);                                                    */
	0x0000000000480000ULL, /* C_67: dcnt1--;                                                          */
	0x0000000882480113ULL, /* C_68: READ1(ptr_2,chan_0); ptr_2+=1; if(dcnt1!=0) goto C_68; dcnt1--;   */
	0x0000000000004149ULL, /* C_69: SEND_EOT(chan_0); goto C_82;                                      */
	0x000000000040014aULL, /* C_70: if(dcnt0==0) goto C_82; dcnt0--;                                  */
	0x0000000000400141ULL, /* C_71: goto C_80; dcnt0--;                                               */
	0x0000000000001000ULL, /* C_72: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_73: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_74: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_75: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_76: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_77: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_78: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_79: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000883c00143ULL, /* C_80: READ8(ptr_2,chan_0); ptr_2+=8; if(dcnt0!=0) goto C_80; dcnt0--;   */
	0x0000000883804000ULL, /* C_81: SEND_EOT(chan_0); READ8(ptr_2,chan_0); ptr_2+=8;                  */
	0x000000d000600000ULL, /* C_82: dcnt0=R[6];                                                       */
	0x000000f000680000ULL, /* C_83: dcnt1=R[7];                                                       */
	0x000000000008018eULL, /* C_84: if(dcnt1==0) goto C_99;                                           */
	0x0000000000400182ULL, /* C_85: if(dcnt0==0) goto C_96; dcnt0--;                                  */
	0x00000008c3c0015bULL, /* C_86: READ8(ptr_3,chan_0); ptr_3+=8; if(dcnt0!=0) goto C_86; dcnt0--;   */
	0x0000000000008181ULL, /* C_87: FLUSH(chan_0); goto C_96;                                         */
	0x0000000000001000ULL, /* C_88: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_89: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_90: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_91: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_92: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_93: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_94: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000001000ULL, /* C_95: STOP(); ALIGN DROP ADDRESS                                        */
	0x0000000000480000ULL, /* C_96: dcnt1--;                                                          */
	0x00000008c2480187ULL, /* C_97: READ1(ptr_3,chan_0); ptr_3+=1; if(dcnt1!=0) goto C_97; dcnt1--;   */
	0x000000000000419dULL, /* C_98: SEND_EOT(chan_0); goto C_103;                                     */
	0x000000000040019eULL, /* C_99: if(dcnt0==0) goto C_103; dcnt0--;                                 */
	0x0000000000400000ULL, /* C_100: dcnt0--;                                                         */
	0x00000008c3c00197ULL, /* C_101: READ8(ptr_3,chan_0); ptr_3+=8; if(dcnt0!=0) goto C_101; dcnt0--; */
	0x00000008c3804000ULL, /* C_102: SEND_EOT(chan_0); READ8(ptr_3,chan_0); ptr_3+=8;                 */
	0x00000000000221c1ULL, /* C_103: SEND_IT(); WAIT_TOKEN(); goto C_112;                             */
	0x0000000000001000ULL, /* C_104: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_105: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_106: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_107: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_108: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_109: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_110: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000001000ULL, /* C_111: STOP(); ALIGN DROP ADDRESS                                       */
	0x0000000000000001ULL  /* C_112: goto C_0;                                                        */
};
/**@}*/

/**
 * @name Auxiliar structure.
 */
/**@{*/
struct bostan_dnoc_uc_transaction
{
	mOS_uc_parameters_t  *parameter; /**< Registers with both array and named field accessors.               */
	mOS_uc_pointers_t    *pointer;   /**< Registers with both array and named field accessors.               */
	mOS_uc_path_t        *path;      /**< Defines ths NoC route and tag to be used to reach the destination. */
	mOS_uc_notify_mode_t *notify;    /**< Information about which pe set will be notified.                   */
	mOS_uc_desc_t        *desc;      /**< Information about which set of resources must be updated.          */
	mOS_uc_transaction_t *job;       /**< Parameters + Path + Notification.                                  */
};

/*============================================================================*
 * bostan_dnoc_uc_pre_config()                                                *
 *============================================================================*/

/**
 * @brief Pre-configurates the virtual ucore thread.
 *
 * @param interface  Number of the interface.
 * @param uctag      Number of Ucore.
 * @param txtag      Number of TX channel.
 * @param remote_tag Number of target RX tag.
 */
PRIVATE inline void bostan_dnoc_uc_pre_config(
	int interface,
	int uctag,
	int txtag,
	int remote_tag
)
{
	/* Pré-configurations. */
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).min_max_task_id._.min_max_task_id_en = 1;
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).min_max_task_id._.min_task_id        = remote_tag;
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).min_max_task_id._.max_task_id        = remote_tag;
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).min_max_task_id._.current_task_id    = remote_tag;

#if 0
	/* Test for configure the ucore to trigger an event (not works). */
	UC_VIRTUAL_FIFO_NOTIFY(interface, uctag, txtag).target_mask = (~0);
	UC_VIRTUAL_FIFO_NOTIFY(interface, uctag, txtag).line_nb     = 0;
#endif

	/* This write in hardware registers. */
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).legacy.header.reg =
		mppa_dnoc[interface]->tx_channels[txtag].header.dword;
	UC_VIRTUAL_FIFO_PATH(interface, uctag, txtag).legacy.config.reg =
		mppa_dnoc[interface]->tx_channels[txtag].config.dword;
}

/*============================================================================*
 * bostan_dnoc_uc_get_next_job()                                              *
 *============================================================================*/

/**
 * @brief Gets the next transaction on jobs queue.
 *
 * @details On the IO Clusters do not exist the transactions queue then
 * we only simulate ots behavior.
 *
 * @param interface Number of the interface.
 * @param uctag     Number of ucore.
 * @param trs       Transaction that will be configurated.
 */
PRIVATE inline void bostan_dnoc_uc_get_next_job(
	int interface,
	int uctag,
	struct bostan_dnoc_uc_transaction * trs
)
{
#ifdef __k1dp__
	int job = bostan_dnoc_uc_job[interface][uctag];

	mOS_uc_desc_t desc;

	/* Waits the transaction be completed. */
	do
		desc._word = __builtin_k1_lwu(&_scoreboard_start.SCB_UC.trs[uctag][job].desc._word);
	while (desc.commit != 0);

	/* Gets underlying transaction pointers. */
	trs->parameter = &_scoreboard_start.SCB_UC.trs[uctag][job].parameter;
	trs->pointer   = &_scoreboard_start.SCB_UC.trs[uctag][job].pointer;
	trs->notify    = &_scoreboard_start.SCB_UC.trs[uctag][job].notify;
	trs->path      = &_scoreboard_start.SCB_UC.trs[uctag][job].path;
	trs->desc      = &_scoreboard_start.SCB_UC.trs[uctag][job].desc;
	trs->job       = &_scoreboard_start.SCB_UC.trs[uctag][job];

	/* Updates the next free job (Round Robin behavior). */
	bostan_dnoc_uc_job[interface][uctag]  = job + 1;
	bostan_dnoc_uc_job[interface][uctag] %= MOS_NB_UC_TRS;
#else
	/* Gets underlying transaction pointers (only parameters and pointers). */
	trs->parameter = (mOS_uc_parameters_t *) (&mppa_dnoc[interface]->threads[uctag].parameter[0]);
	trs->pointer   = (mOS_uc_pointers_t *)   (&mppa_dnoc[interface]->threads[uctag].pointer[0]);
	trs->notify    = &bostan_dnoc_uc_notify[interface][uctag];
	trs->path      = &bostan_dnoc_uc_path[interface][uctag];
	trs->desc      = &bostan_dnoc_uc_descriptor[interface][uctag];

	mOS_async_uc_get_cfg_perm(interface, uctag, BOSTAN_DNOC_EVENTS);

	if (mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS) != 0)
		kpanic("Cannot get the next job!");

	/* Waits Ucore thread finishes previous job. */
	while (mOS_ucore_get_program_run(interface, uctag) != 0);

	/**
	 * This check was supposed to be taken in the IO clusters but they are
	 * blocked in the second tentative. Apparently, the operations are not
	 * being completed, since this check is more for debugging second Kalray
	 * includes, works without it.
	 */
	// while (mppa_dnoc[interface]->ucore_global.thread_outstanding_req[uctag].reg != 0); //! IO block here on second time.
#endif
}

/*============================================================================*
 * bostan_dnoc_uc_config()                                                    *
 *============================================================================*/

/**
 * @brief Configurates the underliyng Ucore thread.
 *
 * @param interface  Number of the interface.
 * @param uctag      Number of Ucore.
 * @param txtag      Number of TX channel.
 * @param buffer     Local data pointer.
 * @param size       Amount of bytes to transfer.
 * @param parameters Ucore program parameters (only IO Cluster).
 * @param pointers   Ucore program pointers (only IO Cluster).
 */
PRIVATE inline int bostan_dnoc_uc_config(
	int interface,
	int uctag,
	int txtag,
	void * buffer,
#ifdef __k1io__
	uint32_t size,
	uint64_t * parameters,
	uint64_t * pointers
#else
	uint32_t size
#endif
)
{
#ifdef __k1dp__
	UNUSED(interface);
	UNUSED(txtag);
#endif
	UNUSED(ucode_eth);
	UNUSED(ucode_eth_v2);
	UNUSED(firmware_linear);

	mOS_async_uc_configure(
#ifdef __k1io__
		interface,
#endif
		uctag,
		((uintptr_t) &ucode_eth_v2),
		((uintptr_t) buffer),
		size,
#ifdef __k1io__
		0,
		__k1_dnoc_ucore_set_indirection_table(txtag),
		__k1_dnoc_ucore_set_indirection_table(uctag),
		pointers,
		parameters,
#endif
		BOSTAN_DNOC_EVENTS
	);

	return mOS_async_uc_rda_event_waitclear(BOSTAN_DNOC_EVENTS);
}

/*============================================================================*
 * bostan_dnoc_uc_commit_job()                                                *
 *============================================================================*/

/**
 * @brief Liberates the Ucore thread to transfer the data.
 *
 * @param interface  Number of the interface.
 * @param uctag      Number of Ucore.
 * @param trs        Configurated transaction.
 */
PRIVATE inline void bostan_dnoc_uc_commit_job(
	int interface,
	int uctag,
	struct bostan_dnoc_uc_transaction * trs
)
{
	/* Flush cached values. */
	dcache_invalidate();

#ifdef __k1dp__
	UNUSED(interface);

	/* Primitive function to commit a jog, only exists in the Compute Clusters */
	mOS_ucore_commit(uctag, trs->job);
#else
	UNUSED(trs);

	/* Sets program status of the Ucore thread do RUN (0x3) */
	mOS_ucore_set_program_run(interface, uctag, 0x3);
#endif
}

/*============================================================================*
 * bostan_dnoc_uc_config_write()                                              *
 *============================================================================*/

/**
 * @brief Configure and async writes on D-NoC transfer tag.
 *
 * @param interface   Number of the DMA channel.
 * @param localid Local Node ID.
 * @param uctag       Ucore thread ID.
 * @param txtag       Transfer buffer ID.
 * @param remoteid Target Node ID.
 * @param remote_tag  Target receiver buffer.
 * @param buffer      Local data pointer.
 * @param size        Amount of bytes to transfer.
 * @param offset      Offset on target buffer.
 *
 * @return Zero if configure correctly and non zero otherwise.
 *
 * @todo Discover how to send offset before the data.
 */
PUBLIC int bostan_dnoc_uc_config_write(
	int interface,
	int localid,
	int uctag,
	int txtag,
	int remoteid,
	int remote_tag,
	const void *buffer,
	uint64_t size,
	uint64_t offset
)
{
	int ret;
	mOS_uc_desc_t desc;
	uint64_t * pointers;
	uint64_t * parameters;
	struct bostan_dnoc_uc_transaction next_trs;

	UNUSED(offset);

	if (!bostan_dnoc_uc_is_valid(interface, uctag))
		return (-EINVAL);

	if (!bostan_dnoc_uc_is_used(interface, uctag))
		return (-EINVAL);

	/**
	 * A linked TX channel cannot be configured by CPU. So, we
	 * need to unlink him to perform the required configuration.
	 */
	if (bostan_dnoc_uc_is_linked(interface, uctag, txtag))
		if (bostan_dnoc_uc_unlink(interface, uctag, txtag) != 0)
			return (-EINVAL);

	/* First, we need configurate the TX channel. */
	if (bostan_dnoc_tx_config(interface, localid, txtag, remoteid, remote_tag) != 0)
		return (-EINVAL);

	/* Pré-configurations. */
	bostan_dnoc_uc_pre_config(interface, uctag, txtag, remote_tag);

	/**
	 * Now that we no longer need to tweak the TX channel, we can
	 * link it to Ucore.
	 */
	if (bostan_dnoc_uc_link(interface, uctag, txtag) != 0)
		return (-EINVAL);

	/* Gets the next free transaction slot. */
	bostan_dnoc_uc_get_next_job(interface, uctag, &next_trs);

	/**
	 * On the Compute Clusters, the arguments of the Ucore program are
	 * configurated on transaction variable but on the IO Clusters,
	 * they are set manually on ucore configuration.
	 */
#ifdef __k1io__
	mOS_uc_pointers_t mos_pointers;
	mOS_uc_parameters_t mos_parameters;

	parameters = mos_parameters.array;
	pointers   = mos_pointers.array;
#else
	parameters = next_trs.parameter->array;
	pointers   = next_trs.pointer->array;
#endif

	/**
	 * Configurates the parameters and pointers
	 * to executes the ucode_eth and ucode_eth_v2.
	 */

	parameters[0] = (size >> 3);
	parameters[1] = (size %  8);
	parameters[2] = (size >> 3);
	parameters[3] = (size %  8);
	parameters[4] = (size >> 3);
	parameters[5] = (size %  8);
	parameters[6] = (size >> 3);
	parameters[7] = (size %  8);

	pointers[0] = 0;
	pointers[1] = size;
	pointers[2] = size * 2;
	pointers[3] = size * 3;

	/* Defines which parameters and pointers are valid. */
	desc.vid         = txtag;
	desc.param_set   = 0xff;
	desc.pointer_set = 0xf;
	next_trs.desc->_word = desc._word;

	/* Configures the underlying Ucore thread. */
	ret = bostan_dnoc_uc_config(
		interface,
		uctag,
		txtag,
		(void *) buffer,
#ifdef __k1io__
		size,
		parameters,
		pointers
#else
		size
#endif
	);

	if (ret != 0)
		return (-EINVAL);

	/* Dispars the Ucore thread to execute the data transfer. */
	bostan_dnoc_uc_commit_job(interface, uctag, &next_trs);

	return (0);
}
