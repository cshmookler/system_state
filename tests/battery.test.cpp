// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(battery_test, all) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());
}

TEST(battery_test, all_one_battery) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);
}

TEST(battery_test, sysfs_path) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        std::filesystem::path sysfs_path = battery.get_sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(battery_test, name) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        std::string name = battery.get_name();
        ASSERT_TRUE(name.size() > 0);
    }
}

TEST(battery_test, status) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        auto status = battery.get_status();
        ASSERT_TRUE(status.has_value());
        // The status can be any value.
    }
}

TEST(battery_test, current) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        auto current = battery.get_current();
        ASSERT_TRUE(current.has_value());
        // The current must be a positive number.
        ASSERT_GE(current.value(), 0);
    }
}

TEST(battery_test, power) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    for (const syst::battery_t& battery : batteries.value()) {
        auto power = battery.get_power();
        ASSERT_TRUE(power.has_value());
        // The power must be a positive number.
        ASSERT_GE(power.value(), 0);
    }
}

TEST(battery_test, charge) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    bool charge_found = false;

    for (const syst::battery_t& battery : batteries.value()) {
        auto charge = battery.get_charge();

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
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    bool capacity_found = false;

    for (const syst::battery_t& battery : batteries.value()) {
        auto capacity = battery.get_capacity();

        // For testing purposes, at least one of the identified batteries must
        // have a reported capacity level.
        if (capacity.has_value()) {
            capacity_found = true;
        }

        // The capacity can be any value.
    }

    ASSERT_TRUE(capacity_found);
}

TEST(battery_test, time_remaining) {
    auto batteries = syst::get_batteries();
    ASSERT_TRUE(batteries.has_value());

    // For testing purposes, there must be at least one battery.
    ASSERT_GE(batteries->size(), 1);

    bool time_remaining_found = false;

    for (const syst::battery_t& battery : batteries.value()) {
        auto status = battery.get_status();
        ASSERT_TRUE(status.has_value());
        if (status.value() != syst::battery_t::status_t::discharging
          && status.value() != syst::battery_t::status_t::charging) {
            continue;
        }

        // For testing purposes, at least one of the identified batteries must
        // be discharging or charging.
        time_remaining_found = true;

        auto time_remaining = battery.get_time_remaining();
        ASSERT_TRUE(time_remaining.has_value());
        // The time_remaining must be a positive number.
        ASSERT_GE(time_remaining.value().count(), 0);
    }

    ASSERT_TRUE(time_remaining_found);
}
