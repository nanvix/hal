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

#ifndef PROCESSOR_OR1K_QEMU_NOC_H_
#define PROCESSOR_OR1K_QEMU_NOC_H_

	/* Cluster API. */
	#include <arch/processor/or1k-qemu/_or1k-qemu.h>

/**
 * @addtogroup processor-or1k-noc NoC
 * @ingroup processor-or1k
 *
 * @brief NoC
 */
/**@{*/

	#include <arch/processor/or1k-qemu/noc/node.h>

	/**
	 * @name Number of NoC nodes attached to an IO device.
	 */
	#define OR1K_QEMU_NR_NOC_IONODES 1

	/**
	 * @name Number of NoC nodes not attached to an IO device.
	 */
	#define OR1K_QEMU_NR_NOC_CNODES 1

	/**
	 * @name Number of NoC nodes.
	 */
	#define OR1K_QEMU_NR_NOC_NODES (OR1K_QEMU_NR_NOC_IONODES + OR1K_QEMU_NR_NOC_CNODES)

	/**
	 * @brief NoC tags offsets.
	 *
	 * All NoC connectors that are listed bellow support 1:N
	 * single-direction communication. Therefore, we need K1B_NR_NOC_NODES
	 * NoC tags for each. The first two tags are used by the hardware and
	 * thus are skipped.
	 */
	/**@{*/
	#define OR1K_QEMU_TAG_MAILBOX_OFF  2                                                   /**< Mailbox. */
	#define OR1K_QEMU_TAG_PORTAL_OFF  (OR1K_QEMU_TAG_MAILBOX_OFF + OR1K_QEMU_NR_NOC_NODES) /**< Portal.  */
	#define OR1K_QEMU_TAG_SYNC_OFF    (OR1K_QEMU_TAG_PORTAL_OFF + OR1K_QEMU_NR_NOC_NODES)  /**< Sync.    */
	/**@}*/

	/**
	 * @brief IDs of NoC nodes.
	 */
	static inline const int or1k_qemu_noc_nodes[OR1K_QEMU_NR_NOC_NODES];

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 0.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 0,
	 * and zero otherwise.
	 */
	static inline int or1k_qemu_noc_is_ionode0(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 1.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 1,
	 * and zero otherwise.
	 */
	static inline int or1k_qemu_noc_is_ionode1(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
	static inline int or1k_qemu_noc_is_ionode(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
	static inline int or1k_qemu_noc_is_cnode(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

	/**
	 * @brief Gets the logic number of the target NoC node.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The logic number of the target NoC node.
	 */
	static inline int or1k_qemu_node_get_num(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

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
	static inline int or1k_qemu_nodes_convert(int *_nodes, const int *nodes, int nnodes)
	{
		UNUSED(_nodes);
		UNUSED(nodes);
		UNUSED(nnodes);

		return 0
	}

	/**
	 * @brief Returns the synchronization NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	static inline int or1k_qemu_node_sync_tag(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

    /**
	 * @brief Returns the mailbox NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	static inline int or1k_qemu_node_mailbox_tag(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

    /**
	 * @brief Returns the portal NoC tag for a target NoC node ID.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The NoC tag attached to the underlying node ID is
	 * returned.
	 */
	static inline int or1k_qemu_node_portal_tag(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/**
	 * @name Exported Constans
	 */
	/**@{*/
	#define HAL_NR_NOC_IONODES OR1K_QEMU_NR_NOC_IONODES
	#define HAL_NR_NOC_CNODES OR1K_QEMU_NR_NOC_CNODES
	#define HAL_NR_NOC_NODES OR1K_QEMU_NR_NOC_NODES
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __hal_processor_noc
	#define __processor_node_get_num   /**< processor_node_get_num()  */
	#define __processor_nodes_convert  /**< processor_nodes_convert() */
	#define __processor_noc_is_ionode0 /**< processor_noc_is_ionode0() */
	#define __processor_noc_is_ionode1 /**< processor_noc_is_ionode1() */
	#define __processor_noc_is_ionode  /**< processor_noc_is_ionode()  */
	#define __processor_noc_is_cnode   /**< processor_noc_is_cnode()   */
	/**@}*/

    /**
	 * @see or1k_qemu_node_get_num()
	 */
	static inline int processor_node_get_num(int nodeid)
	{
		return or1k_qemu_node_get_num(nodeid);
	}

	/**
	 * @see or1k_qemu_nodes_convert()
	 */
	static inline int processor_nodes_convert(int *_nodes, const int *nodes, int nnodes)
	{
		return or1k_qemu_nodes_convert(_nodes, nodes, nnodes);
	}

	/**
	 * @see or1k_qemu_noc_is_ionode0()
	 */
	static inline int processor_noc_is_ionode0(int nodeid)
	{
		return (or1k_qemu_noc_is_ionode0(nodeid));
	}

	/**
	 * @see or1k_qemu_noc_is_ionode1()
	 */
	static inline int processor_noc_is_ionode1(int nodeid)
	{
		return (or1k_qemu_noc_is_ionode1(nodeid));
	}

	/**
	 * @see or1k_qemu_noc_is_ionode()
	 */
	static inline int processor_noc_is_ionode(int nodeid)
	{
		return (or1k_qemu_noc_is_ionode(nodeid));
	}

	/**
	 * @see or1k_qemu_noc_is_cnode()
	 */
	static inline int processor_noc_is_cnode(int nodeid)
	{
		return (or1k_qemu_noc_is_cnode(nodeid));
	}

/**@endcond*/

#endif /* PROCESSOR_OR1K_QEMU_NOC_H_ */

