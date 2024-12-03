// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"

TEST(battery_test, all) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());
}

TEST(battery_test, all_one_battery) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);
}

TEST(battery_test, sysfs_path) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        std::filesystem::path sysfs_path = battery.sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(battery_test, status) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        auto status = battery.status();
        ASSERT_TRUE(status.has_value());
        // The status can be any value.
    }
}

TEST(battery_test, charge) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    bool charge_found = false;

    for (const syst::battery_t& battery : batteries.value()) {
        auto charge = battery.charge();

        // For testing purposes, at least one of the identified batteries must
        // have a reported charge level.
        if (charge.has_value()) {
            charge_found = true;
        }

        // The charge can be any value.
    }

    ASSERT_TRUE(charge_found);
}

TEST(battery_test, capacity) {
    auto batteries = syst::battery_t::all();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    bool capacity_found = false;

    for (const syst::battery_t& battery : batteries.value()) {
        auto capacity = battery.capacity();

        // For testing purposes, at least one of the identified batteries must
        // have a reported capacity level.
        if (capacity.has_value()) {
            capacity_found = true;
        }

        // The capacity can be any value.
    }

    ASSERT_TRUE(capacity_found);
}
