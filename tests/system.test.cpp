// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(system_test, system_info) {
    auto system_info = syst::get_system_info();
    ASSERT_TRUE(system_info.has_value()) << RES_TRACE(system_info.error());

    // Uptime should (almost) never be zero.
    EXPECT_GT(system_info->uptime.count(), 0UL);

    // Load must always be positive.
    EXPECT_GT(system_info->load_1, 0.F);
    EXPECT_GT(system_info->load_5, 0.F);
    EXPECT_GT(system_info->load_15, 0.F);

    // These can be any value:
    //  - ram_total
    //  - ram_free
    //  - ram_shared
    //  - ram_buffered
    //  - swap_total
    //  - swap_free

    // The number of running processes should always be at least one.
    EXPECT_GT(system_info->procs, 0UL);

    // RAM and swap usage should always be between 0 and 100.
    // RAM usage must always be greater than 0.
    EXPECT_GT(system_info->ram_usage, 0.F);
    EXPECT_LE(system_info->ram_usage, 100.F);
    EXPECT_GE(system_info->swap_usage, 0.F);
    EXPECT_LE(system_info->swap_usage, 100.F);
}
