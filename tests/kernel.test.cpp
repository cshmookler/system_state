// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(kernel_test, get_running_kernel) {
    auto version = syst::get_running_kernel();
    ASSERT_TRUE(version.has_value());
    // The kernel version must be at least one character long.
    ASSERT_GE(version->size(), 1);
}

TEST(kernel_test, get_installed_kernel) {
    auto versions = syst::get_installed_kernels();
    ASSERT_TRUE(versions.has_value());
    for (const auto& version : versions.value()) {
        // All kernel versions must be at least one character long.
        ASSERT_GE(version.size(), 1);
    }
}
