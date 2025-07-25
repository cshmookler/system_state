// Standard includes
#include <thread>

// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(cpu_usage_test, get_total_update_zero) {
    syst::cpu_usage_t cpu_usage;
    auto usage = cpu_usage.get_total();
    ASSERT_FALSE(usage.has_value()) << RES_TRACE(usage.error());
}

TEST(cpu_usage_test, get_total_usage_update_one) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update().success());
    auto usage = cpu_usage.get_total();
    ASSERT_FALSE(usage.has_value()) << RES_TRACE(usage.error());
}

TEST(cpu_usage_test, get_total_usage_update_two) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update().success());
    ASSERT_TRUE(cpu_usage.update().success());
    auto usage = cpu_usage.get_total();
    ASSERT_TRUE(usage.has_value()) << RES_TRACE(usage.error());
    ASSERT_GE(usage.value(), 0.F);
    ASSERT_LE(usage.value(), 100.F);
}

TEST(cpu_usage_test, get_per_core_update_zero) {
    syst::cpu_usage_t cpu_usage;
    auto cores = cpu_usage.get_per_core();
    ASSERT_FALSE(cores.has_value()) << RES_TRACE(cores.error());
}

TEST(cpu_usage_test, get_per_core_update_one) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update().success());
    auto cores = cpu_usage.get_per_core();
    ASSERT_FALSE(cores.has_value()) << RES_TRACE(cores.error());
}

TEST(cpu_usage_test, get_per_core_update_two) {
    syst::cpu_usage_t cpu_usage;
    ASSERT_TRUE(cpu_usage.update().success());
    ASSERT_TRUE(cpu_usage.update().success());
    auto cores = cpu_usage.get_per_core();
    ASSERT_TRUE(cores.has_value()) << RES_TRACE(cores.error());
    for (auto usage : cores.value()) {
        ASSERT_GE(usage, 0.F);
        ASSERT_LE(usage, 100.F);
    }
    // The number of cores returned by get_per_core must match the number of
    // cores returned by std::thread::hardware_concurrency.
    ASSERT_EQ(cores->size(), std::thread::hardware_concurrency());
}
