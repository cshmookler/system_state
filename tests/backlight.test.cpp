// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"

TEST(backlight_test, all) {
    auto backlights = syst::backlight_t::all();
    ASSERT_TRUE(backlights.has_value());
}

TEST(backlight_test, all_one_backlight) {
    auto backlights = syst::backlight_t::all();
    ASSERT_TRUE(backlights.has_value());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);
}

TEST(backlight_test, sysfs_path) {
    auto backlights = syst::backlight_t::all();
    ASSERT_TRUE(backlights.has_value());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (const syst::backlight_t& backlight : backlights.value()) {
        std::filesystem::path sysfs_path = backlight.sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(backlight_test, brightness) {
    auto backlights = syst::backlight_t::all();
    ASSERT_TRUE(backlights.has_value());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (const syst::backlight_t& backlight : backlights.value()) {
        std::optional<double> brightness = backlight.brightness();
        ASSERT_TRUE(brightness.has_value());
        // Backlight brightness must always be between 0 and 100.
        ASSERT_GE(brightness.value(), 0.F);
        ASSERT_LE(brightness.value(), 100.F);
        std::cout << brightness.value() << std::endl;
    }
}
