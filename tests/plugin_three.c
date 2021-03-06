// SPDX-License-Identifier: BSD-3-Clause
/**
 * @file plugin_three.c
 *
 * @brief MPTCP test plugin.
 *
 * Copyright (c) 2019, Intel Corporation
 */

#undef NDEBUG

#include <assert.h>

#include <ell/plugin.h>
#include <ell/util.h>  // For L_STRINGIFY needed by l_error().
#include <ell/log.h>

#ifdef HAVE_CONFIG_H
# include <mptcpd/config-private.h>
#endif

#include <mptcpd/plugin.h>

#include "test-plugin.h"

// ----------------------------------------------------------------

static struct plugin_call_count call_count;

// ----------------------------------------------------------------

static void plugin_three_new_connection(mptcpd_cid_t connection_id,
                                        struct mptcpd_addr const *laddr,
                                        struct mptcpd_addr const *raddr,
                                        bool backup,
                                        struct mptcpd_pm *pm)
{
        (void) connection_id;
        (void) laddr;
        (void) raddr;
        (void) backup;
        (void) pm;

        ++call_count.new_connection;
}

static void plugin_three_new_address(mptcpd_cid_t connection_id,
                                     mptcpd_aid_t addr_id,
                                     struct mptcpd_addr const *addr,
                                     struct mptcpd_pm *pm)
{
        (void) connection_id;
        (void) addr_id;
        (void) addr;
        (void) pm;

        ++call_count.new_address;
}

static void plugin_three_new_subflow(mptcpd_cid_t connection_id,
                                     mptcpd_aid_t laddr_id,
                                     struct mptcpd_addr const *laddr,
                                     mptcpd_aid_t raddr_id,
                                     struct mptcpd_addr const *raddr,
                                     struct mptcpd_pm *pm)
{
        (void) connection_id;
        (void) laddr_id;
        (void) laddr;
        (void) raddr_id;
        (void) raddr;
        (void) pm;

        ++call_count.new_subflow;
}

static void plugin_three_subflow_closed(mptcpd_cid_t connection_id,
                                        struct mptcpd_addr const *laddr,
                                        struct mptcpd_addr const *raddr,
                                        struct mptcpd_pm *pm)
{
        (void) connection_id;
        (void) laddr;
        (void) raddr;
        (void) pm;

        ++call_count.subflow_closed;
}

static void plugin_three_connection_closed(mptcpd_cid_t connection_id,
                                           struct mptcpd_pm *pm)
{
        (void) connection_id;
        (void) pm;

        ++call_count.connection_closed;
}

static struct mptcpd_plugin_ops const pm_ops = {
        .new_connection    = plugin_three_new_connection,
        .new_address       = plugin_three_new_address,
        .new_subflow       = plugin_three_new_subflow,
        .subflow_closed    = plugin_three_subflow_closed,
        .connection_closed = plugin_three_connection_closed
};

static int plugin_three_init(void)
{
        static char const name[] = TEST_PLUGIN;

        if (!mptcpd_plugin_register_ops(name, &pm_ops)) {
                l_error("Failed to initialize test "
                        "plugin \"" TEST_PLUGIN "\".");

                return -1;
        }

        return 0;
}

static void plugin_three_exit(void)
{
        struct plugin_call_count const count = { .new_connection = 0 };

        assert(plugin_call_count_is_sane(&call_count, &count));
        assert(plugin_call_count_is_equal(&call_count, &count));

        plugin_call_count_reset(&call_count);
}

L_PLUGIN_DEFINE(MPTCPD_PLUGIN_DESC,
                plugin_three,
                "test plugin three",
                VERSION,
                L_PLUGIN_PRIORITY_DEFAULT,  // Between the other
                                            // plugins.
                plugin_three_init,
                plugin_three_exit)


/*
  Local Variables:
  c-file-style: "linux"
  End:
*/
