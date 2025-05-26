// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(backlight_test, all) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());
}

TEST(backlight_test, all_one_backlight) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);
}

TEST(backlight_test, sysfs_path) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (const syst::backlight_t& backlight : backlights.value()) {
        std::filesystem::path sysfs_path = backlight.get_sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(backlight_test, get_brightness) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (const syst::backlight_t& backlight : backlights.value()) {
        auto brightness = backlight.get_brightness();
        ASSERT_TRUE(brightness.has_value()) << RES_TRACE(brightness.error());
        // Backlight brightness must always be between 0 and 100.
        ASSERT_GE(brightness.value(), 0.F);
        ASSERT_LE(brightness.value(), 100.F);
    }
}

TEST(backlight_test, set_brightness) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (syst::backlight_t& backlight : backlights.value()) {
        auto brightness = backlight.get_brightness();
        ASSERT_TRUE(brightness.has_value()) << RES_TRACE(brightness.error());
        // Backlight brightness must always be between 0 and 100.
        ASSERT_GE(brightness.value(), 0.F);
        ASSERT_LE(brightness.value(), 100.F);

        auto result = backlight.set_brightness(0);
        EXPECT_TRUE(result.success());

        result = backlight.set_brightness(brightness.value());
        EXPECT_TRUE(result.success());
    }
}

TEST(backlight_test, set_brightness_relative) {
    auto backlights = syst::get_backlights();
    ASSERT_TRUE(backlights.has_value()) << RES_TRACE(backlights.error());

    // For testing purposes, there must be at least one backlight.
    ASSERT_GE(backlights->size(), 1);

    for (syst::backlight_t& backlight : backlights.value()) {
        auto brightness = backlight.get_brightness();
        ASSERT_TRUE(brightness.has_value()) << RES_TRACE(brightness.error());
        // Backlight brightness must always be between 0 and 100.
        ASSERT_GE(brightness.value(), 0.F);
        ASSERT_LE(brightness.value(), 100.F);

        auto result = backlight.set_brightness(0);
        EXPECT_TRUE(result.success());

        result = backlight.set_brightness_relative(-100);
        EXPECT_TRUE(result.success());

        result = backlight.set_brightness_relative(101);
        EXPECT_TRUE(result.success());

        result = backlight.set_brightness_relative(-100);
        EXPECT_TRUE(result.success());

        result = backlight.set_brightness(brightness.value());
        EXPECT_TRUE(result.success());
    }
}
