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

#include <arch/processor/bostan/clusters.h>
#include <arch/processor/bostan/noc.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * IDs of NoC nodes.
 */
PUBLIC const int bostan_noc_nodes[BOSTAN_NR_NOC_NODES] = {
	BOSTAN_CCLUSTER0,
	BOSTAN_CCLUSTER1,
	BOSTAN_CCLUSTER2,
	BOSTAN_CCLUSTER3,
	BOSTAN_CCLUSTER4,
	BOSTAN_CCLUSTER5,
	BOSTAN_CCLUSTER6,
	BOSTAN_CCLUSTER7,
	BOSTAN_CCLUSTER8,
	BOSTAN_CCLUSTER9,
	BOSTAN_CCLUSTER10,
	BOSTAN_CCLUSTER11,
	BOSTAN_CCLUSTER12,
	BOSTAN_CCLUSTER13,
	BOSTAN_CCLUSTER14,
	BOSTAN_CCLUSTER15,
	BOSTAN_IOCLUSTER0 + 0,
	BOSTAN_IOCLUSTER0 + 1,
	BOSTAN_IOCLUSTER0 + 2,
	BOSTAN_IOCLUSTER0 + 3,
	BOSTAN_IOCLUSTER1 + 0,
	BOSTAN_IOCLUSTER1 + 1,
	BOSTAN_IOCLUSTER1 + 2,
	BOSTAN_IOCLUSTER1 + 3
};

/**
 * @brief Gets the logic number of a NoC node.
 *
 * @param nodeid ID of the target NoC node.
 *
 * @returns The logic number of the target NoC node.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_node_get_num(int nodeid)
{
	/* Lookup table of NoC node IDs. */
	for (int i = 1; i < BOSTAN_NR_NOC_NODES; i++)
	{
		/* Found. */
		if (nodeid == bostan_noc_nodes[i])
			return (i);
	}

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
PUBLIC int bostan_nodes_convert(int *_nodes, const int *nodes, int nnodes)
{
	/* Convert NoC node numbers into IDs. */
	for (int i = 0; i < nnodes; i++)
	{
		/* Invalid nodes list. */
		if ((nodes[i] < 0) || (nodes[i] >= BOSTAN_NR_NOC_NODES))
			return (-EINVAL);

		_nodes[i] = bostan_noc_nodes[nodes[i]];
	}

	return (0);
}

/*=======================================================================*
 * bostan_node_convert_id()                                             *
 *=======================================================================*/
/**
 * @brief Gets the virtual number of a NoC node.
 *
 * @param nodenum Logic ID of the target NoC node.
 *
 * @returns The virtual number of the target NoC node.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_node_convert_id(int nodenum)
{
	if (!WITHIN(nodenum, 0, BOSTAN_NR_NOC_NODES))
		return (-EINVAL);

	return (bostan_noc_nodes[nodenum]);
}

/*=======================================================================*
 * bostan_node_mailbox_tag()                                             *
 *=======================================================================*/

/**
 * @brief Returns the mailbox NoC tag for a target NoC node ID.
 *
 * @param nodeid ID of the target NoC node.
 *
 * @returns The NoC tag attached to the underlying node ID is
 * returned.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_node_mailbox_tag(int nodeid)
{
	if (bostan_noc_is_ionode0(nodeid))
		return (BOSTAN_MAILBOX_RX_OFF + (nodeid % K1BIO_CORES_NUM));
	else if (bostan_noc_is_ionode1(nodeid))
		return (BOSTAN_MAILBOX_RX_OFF + K1BIO_CORES_NUM + (nodeid % K1BIO_CORES_NUM));

	return (BOSTAN_MAILBOX_RX_OFF + K1BIO_CORES_NUM + K1BIO_CORES_NUM + nodeid);
}

/*=======================================================================*
 * bostan_node_portal_tag()                                              *
 *=======================================================================*/

/**
 * @brief Returns the portal NoC tag for a target NoC node ID.
 *
 * @param nodeid     ID of the target NoC node.
 *
 * @returns The NoC tag attached to the underlying node ID is
 * returned.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_node_portal_tag(int nodeid)
{
	if (bostan_noc_is_ionode0(nodeid))
		return (BOSTAN_PORTAL_RX_OFF + (nodeid % K1BIO_CORES_NUM));
	else if (bostan_noc_is_ionode1(nodeid))
		return (BOSTAN_PORTAL_RX_OFF + K1BIO_CORES_NUM + (nodeid % K1BIO_CORES_NUM));

	return (BOSTAN_PORTAL_RX_OFF + K1BIO_CORES_NUM + K1BIO_CORES_NUM + nodeid);
}

/*=======================================================================*
 * bostan_node_sync_tag()                                                *
 *=======================================================================*/

/**
 * @brief Returns the synchronization NoC tag for a target NoC node ID.
 *
 * @param nodeid ID of the target NoC node.
 *
 * @note This function is non-blocking.
 * @note This function is thread-safe.
 */
PUBLIC int bostan_node_sync_tag(int nodeid)
{
	if (bostan_noc_is_ionode0(nodeid))
		return (BOSTAN_SYNC_RX_OFF + (nodeid % K1BIO_CORES_NUM));
	else if (bostan_noc_is_ionode1(nodeid))
		return (BOSTAN_SYNC_RX_OFF + K1BIO_CORES_NUM + (nodeid % K1BIO_CORES_NUM));

	return (BOSTAN_SYNC_RX_OFF + K1BIO_CORES_NUM + K1BIO_CORES_NUM + nodeid);
}
