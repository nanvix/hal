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
#include "../test.h"

#if (__TARGET_HAS_PORTAL)

/**
 * @brief ID of master NoC node.
 */
#define NODES_AMOUNT   2
#define NODENUM_MASTER 0
#define NODENUM_SLAVE  1
#define PORTAL_SIZE    256

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/**
 * @brief API Test: Portal Create Unlink
 */
static void test_portal_create_unlink(void)
{
	int portalid;

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);
	KASSERT(portal_unlink(portalid) == 0);
}

/**
 * @brief API Test: Portal Open Close
 */
static void test_portal_open_close(void)
{
	int portalid;

	KASSERT((portalid = portal_open(NODENUM_MASTER, NODENUM_SLAVE)) >= 0);
	KASSERT(portal_close(portalid) == 0);
}

/**
 * @brief API Test: Portal Allow
 */
static void test_portal_allow(void)
{
	int portalid;

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);

		KASSERT(portal_allow(portalid, NODENUM_SLAVE) == 0);

	KASSERT(portal_unlink(portalid) == 0);
}

/*============================================================================*/

/**
 * @brief API unit tests.
 */
struct test portal_tests_api[] = {
	{test_portal_create_unlink, "Create Unlink" },
	{test_portal_open_close,    "Open Close"    },
	{test_portal_allow,         "Open Allow"    },
	{NULL,                       NULL           },
};

/*============================================================================*
 * Fault Injection Tests                                                      *
 *============================================================================*/

/**
 * @brief Fault Injection Test: Portal Invalid Create
 */
static void test_portal_invalid_create(void)
{
	/* Invalid local NoC node. */
	KASSERT(portal_create(-1) == -EINVAL);
	KASSERT(portal_create(PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Invalid Open
 */
static void test_portal_invalid_open(void)
{
	/* Invalid local NoC node. */
	KASSERT(portal_open(-1, NODENUM_SLAVE) == -EINVAL);
	KASSERT(portal_open(PROCESSOR_NOC_NODES_NUM, NODENUM_SLAVE) == -EINVAL);

	/* Invalid remote NoC node. */
	KASSERT(portal_open(NODENUM_MASTER, -1) == -EINVAL);
	KASSERT(portal_open(NODENUM_MASTER, PROCESSOR_NOC_NODES_NUM) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Invalid Allow
 */
static void test_portal_invalid_allow(void)
{
	int portalid;

	/* Invalid portal ID. */
	KASSERT(portal_allow(-1, NODENUM_SLAVE) == -EINVAL);
	KASSERT(portal_allow(PORTAL_CREATE_MAX, NODENUM_SLAVE) == -EINVAL);

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);

		/* Invalid remote node. */
		KASSERT(portal_allow(portalid, -1) == -EINVAL);
		KASSERT(portal_allow(portalid, PROCESSOR_NOC_NODES_NUM) == -EINVAL);

	KASSERT(portal_unlink(portalid) == 0);
}

/**
 * @brief Fault Injection Test: Portal Invalid Unlink
 */
static void test_portal_invalid_unlink(void)
{
	/* Invalid portal ID. */
	KASSERT(portal_unlink(-1) == -EINVAL);
	KASSERT(portal_unlink(PORTAL_CREATE_MAX) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Invalid Close
 */
static void test_portal_invalid_close(void)
{
	/* Invalid portal ID. */
	KASSERT(portal_close(-1) == -EINVAL);
	KASSERT(portal_close(PORTAL_OPEN_MAX) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Invalid Read
 */
static void test_portal_invalid_read(void)
{
	int portalid;
	char buf[PORTAL_SIZE];

	/* Invalid portal ID */
	KASSERT(portal_aread(-1, buf, PORTAL_SIZE) == -EINVAL);
	KASSERT(portal_aread(PORTAL_CREATE_MAX, buf, PORTAL_SIZE) == -EINVAL);

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);

		/* Invalid buffer. */
		KASSERT(portal_aread(portalid, NULL, PORTAL_SIZE) == -EINVAL);

		/* Invalid buffer size. */
		KASSERT(portal_aread(portalid, buf, 0) == -EINVAL);
		KASSERT(portal_aread(portalid, buf, PORTAL_MAX_SIZE + 1) == -EINVAL);

	KASSERT(portal_unlink(portalid) == 0);

}

/**
 * @brief Fault Injection Test: Portal Invalid Write
 */
static void test_portal_invalid_write(void)
{
	int portalid;
	char buf[PORTAL_SIZE];

	/* Invalid portal ID */
	KASSERT(portal_awrite(-1, buf, PORTAL_SIZE) == -EINVAL);
	KASSERT(portal_awrite(PORTAL_OPEN_MAX, buf, PORTAL_SIZE) == -EINVAL);

	KASSERT((portalid = portal_open(NODENUM_MASTER, NODENUM_SLAVE)) >= 0);

		/* Invalid buffer. */
		KASSERT(portal_awrite(portalid, NULL, PORTAL_SIZE) == -EINVAL);

		/* Invalid buffer size. */
		KASSERT(portal_awrite(portalid, buf, 0) == -EINVAL);
		KASSERT(portal_awrite(portalid, buf, PORTAL_MAX_SIZE + 1) == -EINVAL);

	KASSERT(portal_close(portalid) == 0);
}

/**
 * @brief Fault Injection Test: Portal Bad Create
 */
static void test_portal_bad_create(void)
{
	/* Bad local NoC node. */
	KASSERT(portal_create(NODENUM_SLAVE) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Bad Open
 */
static void test_portal_bad_open(void)
{
	/* Bad local NoC node. */
	KASSERT(portal_open(NODENUM_SLAVE, NODENUM_SLAVE) == -EINVAL);

	/* Bad remote NoC node. */
	KASSERT(portal_open(NODENUM_MASTER, NODENUM_MASTER) == -EINVAL);
}

/**
 * @brief Fault Injection Test: Portal Bad Allow
 */
static void test_portal_bad_allow(void)
{
	int portalid;

	/* Bad portal ID. */
	KASSERT(portal_allow(0, NODENUM_SLAVE) == -EBADF);
	KASSERT(portal_allow(PORTAL_CREATE_MAX - 1, NODENUM_SLAVE) == -EBADF);

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);

		/* Bad remote NoC node. */
		KASSERT(portal_allow(portalid, NODENUM_MASTER) == -EINVAL);

	KASSERT(portal_unlink(portalid) == 0);
}

/**
 * @brief Fault Injection Test: Portal Bad Unlink
 */
static void test_portal_bad_unlink(void)
{
	/* Bad portal ID. */
	KASSERT(portal_unlink(0) == -EBADF);
	KASSERT(portal_unlink(PORTAL_CREATE_MAX - 1) == -EBADF);
}

/**
 * @brief Fault Injection Test: Portal Bad Close
 */
static void test_portal_bad_close(void)
{
	/* Bad portal ID. */
	KASSERT(portal_close(0) == -EBADF);
	KASSERT(portal_close(PORTAL_OPEN_MAX - 1) == -EBADF);
}

/**
 * @brief Fault Injection Test: Portal Double Unlink
 */
static void test_portal_double_unlink(void)
{
	int portalid;

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);
	KASSERT(portal_unlink(portalid) == 0);
	KASSERT(portal_unlink(portalid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Portal Double Close
 */
static void test_portal_double_close(void)
{
	int portalid;

	KASSERT((portalid = portal_open(NODENUM_MASTER, NODENUM_SLAVE)) >= 0);
	KASSERT(portal_close(portalid) == 0);
	KASSERT(portal_close(portalid) == -EBADF);
}

/**
 * @brief Fault Injection Test: Portal Double Allow
 */
static void test_portal_double_allow(void)
{
	int portalid;

	KASSERT((portalid = portal_create(NODENUM_MASTER)) >= 0);

		KASSERT(portal_allow(portalid, NODENUM_SLAVE) == 0);
		KASSERT(portal_allow(portalid, NODENUM_SLAVE) == -EBUSY);

	KASSERT(portal_unlink(portalid) == 0);
}

/**
 * @brief Unit tests.
 */
struct test portal_tests_fault[] = {
	{test_portal_invalid_create, "Invalid Create" },
	{test_portal_invalid_open,   "Invalid Open"   },
	{test_portal_invalid_allow,  "Invalid Allow"  },
	{test_portal_invalid_unlink, "Invalid Unlink" },
	{test_portal_invalid_close,  "Invalid Close"  },
	{test_portal_invalid_read,   "Invalid Read"   },
	{test_portal_invalid_write,  "Invalid Write"  },
	{test_portal_bad_create,     "Bad Create"     },
	{test_portal_bad_open,       "Bad Open"       },
	{test_portal_bad_allow,      "Bad Allow"      },
	{test_portal_bad_unlink,     "Bad Unlink"     },
	{test_portal_bad_close,      "Bad Close"      },
	{test_portal_double_unlink,  "Double Unlink"  },
	{test_portal_double_close,   "Double Close"   },
	{test_portal_double_allow,   "Double Allow"   },
	{NULL,                        NULL            },
};

#endif /* __TARGET_HAS_PORTAL */

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * The test_portal() function launches testing units on the portal
 * interface of the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void test_portal(void)
{
#if (__TARGET_HAS_PORTAL)

	/* API Tests */
	for (int i = 0; portal_tests_api[i].test_fn != NULL; i++)
	{
		portal_tests_api[i].test_fn();
		kprintf("[test][api][portal] %s [passed]", portal_tests_api[i].name);
	}

	/* FAULT Tests */
	for (int i = 0; portal_tests_fault[i].test_fn != NULL; i++)
	{
		portal_tests_fault[i].test_fn();
		kprintf("[test][fault][portal] %s [passed]", portal_tests_fault[i].name);
	}

#endif /* __TARGET_HAS_PORTAL */
}
