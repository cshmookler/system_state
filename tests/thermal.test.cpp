// External includes
#include <filesystem>
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"

TEST(thermal_test, thermal_zone_all) {
    auto thermal_zones = syst::thermal_zone_t::all();
    ASSERT_TRUE(thermal_zones.has_value());
}

TEST(thermal_test, thermal_zone_all_one_interface) {
    auto thermal_zones = syst::thermal_zone_t::all();
    ASSERT_TRUE(thermal_zones.has_value());

    // For testing purposes, there must be at least one thermal zone.
    ASSERT_GE(thermal_zones->size(), 1);
}

TEST(thermal_test, thermal_zone_sysfs_path) {
    auto thermal_zones = syst::thermal_zone_t::all();
    ASSERT_TRUE(thermal_zones.has_value());

    // For testing purposes, there must be at least one thermal zone.
    ASSERT_GE(thermal_zones->size(), 1);

    for (auto& zone : thermal_zones.value()) {
        std::filesystem::path sysfs_path = zone.sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(thermal_test, thermal_zone_type) {
    auto thermal_zones = syst::thermal_zone_t::all();
    ASSERT_TRUE(thermal_zones.has_value());

    // For testing purposes, there must be at least one thermal zone.
    ASSERT_GE(thermal_zones->size(), 1);

    for (auto& zone : thermal_zones.value()) {
        auto type = zone.type();
        ASSERT_TRUE(type.has_value());
        // All types must be at least one character long.
        ASSERT_GE(type->size(), 1);
    }
}

TEST(thermal_test, thermal_zone_temperature) {
    auto thermal_zones = syst::thermal_zone_t::all();
    ASSERT_TRUE(thermal_zones.has_value());

    // For testing purposes, there must be at least one thermal zone.
    ASSERT_GE(thermal_zones->size(), 1);

    for (auto& zone : thermal_zones.value()) {
        auto temperature = zone.temperature();
        ASSERT_TRUE(temperature.has_value());
        // Thermal zone temperatures must always be between 0 and 100.
        ASSERT_GE(temperature.value(), 0);
        ASSERT_LE(temperature.value(), 100);
    }
}

TEST(thermal_test, cooling_device_all) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());
}

TEST(thermal_test, cooling_device_all_one_interface) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());

    // For testing purposes, there must be at least one cooling device.
    ASSERT_GE(cooling_devices->size(), 1);
}

TEST(thermal_test, cooling_device_sysfs_path) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());

    // For testing purposes, there must be at least one cooling device.
    ASSERT_GE(cooling_devices->size(), 1);

    for (auto& device : cooling_devices.value()) {
        std::filesystem::path sysfs_path = device.sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(thermal_test, cooling_device_type) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());

    // For testing purposes, there must be at least one cooling device.
    ASSERT_GE(cooling_devices->size(), 1);

    for (auto& device : cooling_devices.value()) {
        auto type = device.type();
        ASSERT_TRUE(type.has_value());
        // All types must be at least one character long.
        ASSERT_GE(type->size(), 1);
    }
}

TEST(thermal_test, cooling_device_get_state) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());

    // For testing purposes, there must be at least one cooling device.
    ASSERT_GE(cooling_devices->size(), 1);

    for (auto& device : cooling_devices.value()) {
        auto state = device.get_state();
        ASSERT_TRUE(state.has_value());
        // Cooling device states must always be between 0 and 100.
        ASSERT_GE(state.value(), 0);
        ASSERT_LE(state.value(), 100);
    }
}

TEST(thermal_test, cooling_device_set_state) {
    auto cooling_devices = syst::cooling_device_t::all();
    ASSERT_TRUE(cooling_devices.has_value());

    // For testing purposes, there must be at least one cooling device.
    ASSERT_GE(cooling_devices->size(), 1);

    for (auto& device : cooling_devices.value()) {
        auto old_state = device.get_state();
        ASSERT_TRUE(old_state.has_value());

        EXPECT_FALSE(device.set_state(-1).success());
        EXPECT_TRUE(device.set_state(0).success());
        EXPECT_TRUE(device.set_state(100).success());
        EXPECT_FALSE(device.set_state(101).success());

        ASSERT_TRUE(device.set_state(old_state.value()).success());
    }
}
