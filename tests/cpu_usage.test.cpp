// Standard includes
#include <thread>

// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"

TEST(cpu_usage_test, usage_update_zero) {
    syst::cpu_usage_t cpu_usage;
    auto usage = cpu_usage.get();
    ASSERT_FALSE(usage.has_value());
}

TEST(cpu_usage_test, usage_update_one) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update());
    auto usage = cpu_usage.get();
    ASSERT_FALSE(usage.has_value());
}

TEST(cpu_usage_test, usage_update_two) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update());
    ASSERT_TRUE(cpu_usage.update());
    auto usage = cpu_usage.get();
    ASSERT_TRUE(usage.has_value());
    ASSERT_GE(usage.value(), 0.F);
    ASSERT_LE(usage.value(), 100.F);
}
