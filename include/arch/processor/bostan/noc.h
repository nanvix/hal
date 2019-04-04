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

	#include <arch/processor/bostan/noc/node.h>

	/**
	 * @name Number of NoC nodes attached to an IO device.
	 */
	#define BOSTAN_NR_NOC_IONODES 8

	/**
	 * @name Number of NoC nodes not attached to an IO device.
	 */
	#define BOSTAN_NR_NOC_CNODES 16

	/**
	 * @name Number of NoC nodes.
	 */
	#define BOSTAN_NR_NOC_NODES (BOSTAN_NR_NOC_IONODES + BOSTAN_NR_NOC_CNODES)

	/**
	 * @name Number of NoC nodes per DMA Channel.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_CREATE_PER_DMA 1                   /**< 1 D-NoC RX and 1 C-NoC TX */
	#define BOSTAN_MAILBOX_OPEN_PER_DMA   4                   /**< 1 D-NoC TX and 1 C-NoC RX */
	#define BOSTAN_PORTAL_CREATE_PER_DMA  2                   /**< 1 D-NoC RX and 1 C-NoC TX */
	#define BOSTAN_PORTAL_OPEN_PER_DMA    4                   /**< 1 D-NoC TX and 1 C-NoC RX */
	#define BOSTAN_SYNC_CREATE_PER_DMA    BOSTAN_NR_NOC_NODES /**< 1 C-NoC RX */
	#define BOSTAN_SYNC_OPEN_PER_DMA      1                   /**< 1 C-NoC TX */
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
	#define BOSTAN_MAILBOX_RX_OFF  BOSTAN_NR_RESERVED_RX_TAGS                   /**< Mailbox. */
	#define BOSTAN_PORTAL_RX_OFF  (BOSTAN_MAILBOX_RX_OFF + BOSTAN_NR_NOC_NODES) /**< Portal.  */
	#define BOSTAN_SYNC_RX_OFF    (BOSTAN_PORTAL_RX_OFF + BOSTAN_NR_NOC_NODES)  /**< Sync.    */
	/**@}*/

	/**
	 * @brief Transfer C-NoC tags offsets.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_CNOC_TX_OFF  BOSTAN_NR_RESERVED_TX_TAGS                              /**< Mailbox. */
	#define BOSTAN_PORTAL_CNOC_TX_OFF  (BOSTAN_MAILBOX_CNOC_TX_OFF + BOSTAN_MAILBOX_CREATE_MAX) /**< Portal.  */
	#define BOSTAN_SYNC_CNOC_TX_OFF    (BOSTAN_PORTAL_CNOC_TX_OFF + BOSTAN_PORTAL_CREATE_MAX)   /**< Sync.    */
	/**@}*/

	/**
	 * @brief Transfer D-NoC tags offsets.
	 */
	/**@{*/
	#define BOSTAN_MAILBOX_DNOC_TX_OFF  BOSTAN_NR_RESERVED_TX_TAGS                            /**< Mailbox. */
	#define BOSTAN_PORTAL_DNOC_TX_OFF  (BOSTAN_MAILBOX_DNOC_TX_OFF + BOSTAN_MAILBOX_OPEN_MAX) /**< Portal.  */
	/**@}*/

	/**
	 * @brief IDs of NoC nodes.
	 */
	EXTERN const int bostan_noc_nodes[BOSTAN_NR_NOC_NODES];

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 0.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 0,
	 * and zero otherwise.
	 */
	static inline int bostan_noc_is_ionode0(int nodeid)
	{
		return WITHIN(nodeid, BOSTAN_IOCLUSTER0, BOSTAN_IOCLUSTER0 + K1BIO_CORES_NUM);
	}

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 1.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 1,
	 * and zero otherwise.
	 */
	static inline int bostan_noc_is_ionode1(int nodeid)
	{
		return WITHIN(nodeid, BOSTAN_IOCLUSTER1, BOSTAN_IOCLUSTER1 + K1BIO_CORES_NUM);
	}

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
	static inline int bostan_noc_is_ionode(int nodeid)
	{
		return (bostan_noc_is_ionode0(nodeid) || bostan_noc_is_ionode1(nodeid));
	}

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
	static inline int bostan_noc_is_cnode(int nodeid)
	{
		return WITHIN(nodeid, BOSTAN_CCLUSTER0, BOSTAN_CCLUSTER15 + 1);
	}

	/**
	 * @brief Gets the logic number of the target NoC node.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The logic number of the target NoC node.
	 */
	EXTERN int bostan_node_get_num(int nodeid);

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
	EXTERN int bostan_nodes_convert(int *_nodes, const int *nodes, int nnodes);

	/**
	 * @brief Gets the virtual number of the target NoC node.
	 *
	 * @param nodenum Logic ID of the target NoC node.
	 * @returns The virtual number of the target NoC node.
	 */
	EXTERN int bostan_node_convert_id(int nodenum);

	/**
	 * @brief Returns the synchronization NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_node_sync_tag(int nodeid);

    /**
	 * @brief Returns the mailbox NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_node_mailbox_tag(int nodeid);

    /**
	 * @brief Returns the portal NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	EXTERN int bostan_node_portal_tag(int nodeid);

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
	#define NR_NOC_IONODES BOSTAN_NR_NOC_IONODES
	#define NR_NOC_CNODES BOSTAN_NR_NOC_CNODES
	#define NR_NOC_NODES BOSTAN_NR_NOC_NODES
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __processor_noc_is_ionode0_fn /**< processor_noc_is_ionode0() */
	#define __processor_noc_is_ionode1_fn /**< processor_noc_is_ionode1() */
	#define __processor_noc_is_ionode_fn  /**< processor_noc_is_ionode()  */
	#define __processor_noc_is_cnode_fn   /**< processor_noc_is_cnode()   */
	#define __processor_node_get_num_fn   /**< processor_node_get_num()   */
	#define __processor_nodes_convert_fn  /**< processor_nodes_convert()  */
	/**@}*/

	/**
	 * @see bostan_noc_is_ionode0()
	 */
	static inline int processor_noc_is_ionode0(int nodeid)
	{
		return (bostan_noc_is_ionode0(nodeid));
	}

	/**
	 * @see bostan_noc_is_ionode1()
	 */
	static inline int processor_noc_is_ionode1(int nodeid)
	{
		return (bostan_noc_is_ionode1(nodeid));
	}

	/**
	 * @see bostan_noc_is_ionode()
	 */
	static inline int processor_noc_is_ionode(int nodeid)
	{
		return (bostan_noc_is_ionode(nodeid));
	}

	/**
	 * @see bostan_noc_is_cnode()
	 */
	static inline int processor_noc_is_cnode(int nodeid)
	{
		return (bostan_noc_is_cnode(nodeid));
	}

	/**
	 * @see bostan_node_get_num()
	 */
	static inline int processor_node_get_num(int nodeid)
	{
		return bostan_node_get_num(nodeid);
	}

	/**
	 * @see bostan_nodes_convert()
	 */
	static inline int processor_nodes_convert(int *_nodes, const int *nodes, int nnodes)
	{
		return bostan_nodes_convert(_nodes, nodes, nnodes);
	}

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_NOC_H_ */

