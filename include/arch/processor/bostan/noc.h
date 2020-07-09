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

#ifndef PROCESSOR_BOSTAN_NOC_H_
#define PROCESSOR_BOSTAN_NOC_H_

	/* Cluster API. */
	#include <arch/processor/bostan/_bostan.h>

/**
 * @addtogroup processor-bostan-noc NoC
 * @ingroup processor-bostan
 *
 * @brief NoC
 */
/**@{*/

	#include <arch/processor/bostan/noc/dma.h>

	/**
	 * @name Number of NoC nodes attached to an IO device.
	 */
	#define BOSTAN_PROCESSOR_NOC_IONODES_NUM 8

	/**
	 * @name Number of NoC nodes not attached to an IO device.
	 */
	#define BOSTAN_PROCESSOR_NOC_CNODES_NUM 16

	/**
	 * @name Number of NoC nodes not attached to an IO device.
	 */
	#define BOSTAN_PROCESSOR_NOC_NODES_NUM (BOSTAN_PROCESSOR_NOC_IONODES_NUM + BOSTAN_PROCESSOR_NOC_CNODES_NUM)

	/**
	 * @brief Logical NoC node ID of master.
	 */
	#define BOSTAN_PROCESSOR_NODENUM_MASTER 0

	/**
	 * @brief Logical NoC node ID of leader.
	 */
	#define BOSTAN_PROCESSOR_NODENUM_LEADER (BOSTAN_PROCESSOR_NOC_IONODES_NUM + 0)

	/**
	 * @name Number of NoC nodes per DMA Channel.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_CREATE_PER_DMA 1                              /**< 1 D-NoC RX and 1 C-NoC TX             */
	#define BOSTAN_MAILBOX_OPEN_PER_DMA   BOSTAN_PROCESSOR_NOC_NODES_NUM /**< 1 D-NoC TX (by demand) and 1 C-NoC RX */
	#define BOSTAN_PORTAL_CREATE_PER_DMA  2                              /**< 1 D-NoC RX and 1 C-NoC TX             */
	#define BOSTAN_PORTAL_OPEN_PER_DMA    BOSTAN_PROCESSOR_NOC_NODES_NUM /**< 1 D-NoC TX (by demand) and 1 C-NoC RX */
	#define BOSTAN_SYNC_CREATE_PER_DMA    BOSTAN_PROCESSOR_NOC_NODES_NUM /**< 1 C-NoC RX                            */
	#define BOSTAN_SYNC_OPEN_PER_DMA      BOSTAN_PROCESSOR_NOC_NODES_NUM /**< 1 C-NoC TX                            */
	/**@}*/

	/**
	 * @brief Receiver NoC tags offsets.
	 *
	 * All NoC connectors that are listed bellow support 1:N
	 * single-direction communication. Therefore, we need K1B_NR_NOC_NODES
	 * NoC tags for each. The first two tags are used by the hardware and
	 * thus are skipped.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_RX_OFF (BOSTAN_PROCESSOR_NOC_RESERVED_RXS_NUM)                  /**< Mailbox. */
	#define BOSTAN_PORTAL_RX_OFF  (BOSTAN_MAILBOX_RX_OFF + BOSTAN_PROCESSOR_NOC_NODES_NUM) /**< Portal.  */
	#define BOSTAN_SYNC_RX_OFF    (BOSTAN_PORTAL_RX_OFF + BOSTAN_PROCESSOR_NOC_NODES_NUM)  /**< Sync.    */
	/**@}*/

	/**
	 * @brief Transfer C-NoC tags offsets.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_CNOC_TX_OFF (BOSTAN_PROCESSOR_NOC_RESERVED_TXS_NUM)                  /**< Mailbox. */
	#define BOSTAN_PORTAL_CNOC_TX_OFF  (BOSTAN_MAILBOX_CNOC_TX_OFF + BOSTAN_MAILBOX_CREATE_MAX) /**< Portal.  */
	#define BOSTAN_SYNC_CNOC_TX_OFF    (BOSTAN_PORTAL_CNOC_TX_OFF + BOSTAN_PORTAL_CREATE_MAX)   /**< Sync.    */
	/**@}*/

	/**
	 * @brief Transfer D-NoC tags offsets.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_DNOC_TX_OFF (BOSTAN_PROCESSOR_NOC_RESERVED_TXS_NUM)                /**< Mailbox. */
	#define BOSTAN_PORTAL_DNOC_TX_OFF  (BOSTAN_MAILBOX_DNOC_TX_OFF + BOSTAN_MAILBOX_OPEN_MAX) /**< Portal.  */
	/**@}*/

	/**
	 * @brief Converts a nodes list.
	 *
	 * @param _nodes Place to store converted list.
	 * @param nodes  Target nodes list.
	 * @param nnodes Number of nodes in the list.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int bostan_processor_noc_node_num_to_id(int nodenum);

	/**
	 * @brief Converts a cluster number to NoC node number.
	 *
	 * @param clusternum Target node number.
	 *
	 * @returns The logical cluster number in which the node number @p
	 * nodenum is located.
	 */
	EXTERN int bostan_processor_noc_cluster_to_node_num(int clusternum);

	/**
	 * @brief Converts a NoC node number to cluster number.
	 *
	 * @param nodenum Target node number.
	 *
	 * @returns The logical cluster number in which the node number @p
	 * nodenum is located.
	 */
	EXTERN int bostan_processor_noc_node_to_cluster_num(int nodenum);

	/**
	 * @brief Returns the synchronization NoC tag for a target NoC node ID.
	 *
	 * @param nodenum Logical ID of the target NoC node.
	 * @param slot    Slot of tags (0 or 1).
	 *
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_processor_node_sync_tag(int nodenum, int slot);

    /**
	 * @brief Returns the mailbox NoC tag for a target NoC node ID.
	 *
	 * @param nodenum Logical ID of the target NoC node.
	 *
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_processor_node_mailbox_tag(int nodenum);

    /**
	 * @brief Returns the portal NoC tag for a target NoC node ID.
	 *
	 * @param nodenum Logical ID of the target NoC node.
	 *
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_processor_node_portal_tag(int nodenum);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond bostan
 */

	/**
	 * @name Exported Constans
	 */
	/**@{*/
	#define PROCESSOR_NOC_IONODES_NUM BOSTAN_PROCESSOR_NOC_IONODES_NUM /**< BOSTAN_PROCESSOR_NOC_IONODES_NUM */
	#define PROCESSOR_NOC_CNODES_NUM  BOSTAN_PROCESSOR_NOC_CNODES_NUM  /**< BOSTAN_PROCESSOR_NOC_CNODES_NUM  */
	#define PROCESSOR_NODENUM_MASTER  BOSTAN_PROCESSOR_NODENUM_MASTER  /**< BOSTAN_PROCESSOR_NODENUM_MASTER  */
	#define PROCESSOR_NODENUM_LEADER  BOSTAN_PROCESSOR_NODENUM_LEADER  /**< BOSTAN_PROCESSOR_NODENUM_LEADER  */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __processor_noc_setup_fn     /**< processor_noc_setup()     */
	#define __processor_noc_is_ionode_fn /**< processor_noc_is_ionode() */
	#define __processor_noc_is_cnode_fn  /**< processor_noc_is_cnode()  */
	#define __processor_node_get_num_fn  /**< processor_node_get_num()  */
	/**@}*/

	/**
	 * @brief Initializes the noc interface.
	 */
	static inline void processor_noc_setup(void)
	{
		bostan_dma_init();
	}

	/**
	 * @brief Mask NoC interrupts.
	 */
	static inline void bostan_noc_it_mask(void)
	{
		interrupt_mask(K1B_INT_CNOC);
		interrupt_mask(K1B_INT_DNOC);
	}

	/**
	 * @brief Unnask NoC interrupts.
	 */
	static inline void bostan_noc_it_unmask(void)
	{
		interrupt_unmask(K1B_INT_DNOC);
		interrupt_unmask(K1B_INT_CNOC);
	}

	/**
	 * @brief Verify lost interrupts.
	 */
	static inline void bostan_noc_it_verify(void)
	{
		bostan_cnoc_it_verify();
		bostan_dnoc_it_verify();
	}

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodenum Logic ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
	static inline int processor_noc_is_ionode(int nodenum)
	{
		return (WITHIN(nodenum, 0, PROCESSOR_NOC_IONODES_NUM));
	}

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodenum Logic ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
	static inline int processor_noc_is_cnode(int nodenum)
	{
		return (
			WITHIN(
				nodenum,
				PROCESSOR_NOC_IONODES_NUM,
				PROCESSOR_NOC_IONODES_NUM + PROCESSOR_NOC_CNODES_NUM
			)
		);
	}

	/**
	 * @brief Gets the logic number of the target NoC node
	 * attached with a core.
	 *
	 * @returns The logic number of the target NoC node attached
	 * with the @p coreid.
	 */
	static inline int processor_node_get_num(void)
	{
		return (bostan_processor_noc_cluster_to_node_num(cluster_get_num()));
	}

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_NOC_H_ */

