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
		return ((nodeid >= BOSTAN_IOCLUSTER0) && (nodeid < BOSTAN_IOCLUSTER0 + 4));
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
		return ((nodeid >= BOSTAN_IOCLUSTER1) && (nodeid < BOSTAN_IOCLUSTER1 + 4));
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
		return ((nodeid >= BOSTAN_CCLUSTER0) && (nodeid <= BOSTAN_CCLUSTER15));
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
	#define HAL_NR_NOC_IONODES BOSTAN_NR_NOC_IONODES
	#define HAL_NR_NOC_CNODES BOSTAN_NR_NOC_CNODES
	#define HAL_NR_NOC_NODES BOSTAN_NR_NOC_NODES
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

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_NOC_H_ */

