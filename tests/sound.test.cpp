// Standard includes
#include <thread>

// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(sound_test, sound_mixer_get) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
}

TEST(sound_test, sound_mixer_all_controls) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);
}

TEST(sound_test, sound_control_name) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    for (auto& control : controls) {
        std::string name = control.name();
        // All sound control element names must be at least one character long.
        ASSERT_NE(name.size(), 0);
    }
}

TEST(sound_test, sound_control_get_playback_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_playback_status = false;

    for (auto& control : controls) {
        if (! control.has_playback_status()) {
            continue;
        }

        has_playback_status = true;

        auto status = control.get_playback_status();
        ASSERT_TRUE(status.has_value());
        // Status can be any value.
    }

    ASSERT_TRUE(has_playback_status);
}

TEST(sound_test, sound_control_get_playback_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_playback_volume = false;

    for (auto& control : controls) {
        if (! control.has_playback_volume()) {
            continue;
        }

        has_playback_volume = true;

        auto volume = control.get_playback_volume();
        ASSERT_TRUE(volume.has_value());
        // Volume values must be between 0 and 100.
        if (volume->front_left.has_value()) {
            ASSERT_GE(volume->front_left.value(), 0);
            ASSERT_LE(volume->front_left.value(), 100);
        }
        if (volume->front_right.has_value()) {
            ASSERT_GE(volume->front_right.value(), 0);
            ASSERT_LE(volume->front_right.value(), 100);
        }
        if (volume->rear_left.has_value()) {
            ASSERT_GE(volume->rear_left.value(), 0);
            ASSERT_LE(volume->rear_left.value(), 100);
        }
        if (volume->rear_right.has_value()) {
            ASSERT_GE(volume->rear_right.value(), 0);
            ASSERT_LE(volume->rear_right.value(), 100);
        }
        if (volume->front_center.has_value()) {
            ASSERT_GE(volume->front_center.value(), 0);
            ASSERT_LE(volume->front_center.value(), 100);
        }
        if (volume->woofer.has_value()) {
            ASSERT_GE(volume->woofer.value(), 0);
            ASSERT_LE(volume->woofer.value(), 100);
        }
        if (volume->side_left.has_value()) {
            ASSERT_GE(volume->side_left.value(), 0);
            ASSERT_LE(volume->side_left.value(), 100);
        }
        if (volume->side_right.has_value()) {
            ASSERT_GE(volume->side_right.value(), 0);
            ASSERT_LE(volume->side_right.value(), 100);
        }
        if (volume->rear_center.has_value()) {
            ASSERT_GE(volume->rear_center.value(), 0);
            ASSERT_LE(volume->rear_center.value(), 100);
        }
    }

    ASSERT_TRUE(has_playback_volume);
}

TEST(sound_test, sound_control_get_capture_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_capture_status = false;

    for (auto& control : controls) {
        if (! control.has_capture_status()) {
            continue;
        }

        has_capture_status = true;

        auto status = control.get_capture_status();
        ASSERT_TRUE(status.has_value());
        // Status can be any value.
    }

    ASSERT_TRUE(has_capture_status);
}

TEST(sound_test, sound_control_get_capture_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_capture_volume = false;

    for (auto& control : controls) {
        if (! control.has_capture_volume()) {
            continue;
        }

        has_capture_volume = true;

        auto volume = control.get_capture_volume();
        ASSERT_TRUE(volume.has_value());
        // Volume values must be between 0 and 100.
        if (volume->front_left.has_value()) {
            ASSERT_GE(volume->front_left.value(), 0);
            ASSERT_LE(volume->front_left.value(), 100);
        }
        if (volume->front_right.has_value()) {
            ASSERT_GE(volume->front_right.value(), 0);
            ASSERT_LE(volume->front_right.value(), 100);
        }
        if (volume->rear_left.has_value()) {
            ASSERT_GE(volume->rear_left.value(), 0);
            ASSERT_LE(volume->rear_left.value(), 100);
        }
        if (volume->rear_right.has_value()) {
            ASSERT_GE(volume->rear_right.value(), 0);
            ASSERT_LE(volume->rear_right.value(), 100);
        }
        if (volume->front_center.has_value()) {
            ASSERT_GE(volume->front_center.value(), 0);
            ASSERT_LE(volume->front_center.value(), 100);
        }
        if (volume->woofer.has_value()) {
            ASSERT_GE(volume->woofer.value(), 0);
            ASSERT_LE(volume->woofer.value(), 100);
        }
        if (volume->side_left.has_value()) {
            ASSERT_GE(volume->side_left.value(), 0);
            ASSERT_LE(volume->side_left.value(), 100);
        }
        if (volume->side_right.has_value()) {
            ASSERT_GE(volume->side_right.value(), 0);
            ASSERT_LE(volume->side_right.value(), 100);
        }
        if (volume->rear_center.has_value()) {
            ASSERT_GE(volume->rear_center.value(), 0);
            ASSERT_LE(volume->rear_center.value(), 100);
        }
    }

    ASSERT_TRUE(has_capture_volume);
}

TEST(sound_test, sound_control_set_playback_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::status_t new_status;
    new_status.front_left = false;
    new_status.front_right = false;
    new_status.rear_left = false;
    new_status.rear_right = false;
    new_status.front_center = false;
    new_status.woofer = false;
    new_status.side_left = false;
    new_status.side_right = false;
    new_status.rear_center = false;

    bool has_playback_status = false;

    for (auto& control : controls) {
        if (! control.has_playback_status()) {
            continue;
        }

        has_playback_status = true;

        auto old_status = control.get_playback_status();
        ASSERT_TRUE(old_status.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_playback_status(new_status).success());

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_playback_status);
}

TEST(sound_test, sound_control_set_playback_status_all) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_playback_status = false;

    for (auto& control : controls) {
        if (! control.has_playback_status()) {
            continue;
        }

        has_playback_status = true;

        auto old_status = control.get_playback_status();
        ASSERT_TRUE(old_status.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_playback_status_all(0).success());

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_playback_status);
}

TEST(sound_test, sound_control_toggle_playback_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_playback_status = false;

    for (auto& control : controls) {
        if (! control.has_playback_status()) {
            continue;
        }

        has_playback_status = true;

        auto old_status = control.get_playback_status();
        ASSERT_TRUE(old_status.has_value());

        ASSERT_TRUE(control.toggle_playback_status().success());

        // For some reason, setting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // All channels of the current status should be different from those of
        // the original status after toggling once.
        auto current_status = control.get_playback_status();
        ASSERT_TRUE(current_status.has_value());

        ASSERT_EQ(old_status->front_left.has_value(),
          current_status->front_left.has_value());
        if (current_status->front_left.has_value()) {
            EXPECT_NE(old_status->front_left.value(),
              current_status->front_left.value());
        }

        ASSERT_EQ(old_status->front_right.has_value(),
          current_status->front_right.has_value());
        if (current_status->front_right.has_value()) {
            EXPECT_NE(old_status->front_right.value(),
              current_status->front_right.value());
        }

        ASSERT_EQ(old_status->rear_left.has_value(),
          current_status->rear_left.has_value());
        if (current_status->rear_left.has_value()) {
            EXPECT_NE(
              old_status->rear_left.value(), current_status->rear_left.value());
        }

        ASSERT_EQ(old_status->rear_right.has_value(),
          current_status->rear_right.has_value());
        if (current_status->rear_right.has_value()) {
            EXPECT_NE(old_status->rear_right.value(),
              current_status->rear_right.value());
        }

        ASSERT_EQ(old_status->front_center.has_value(),
          current_status->front_center.has_value());
        if (current_status->front_center.has_value()) {
            EXPECT_NE(old_status->front_center.value(),
              current_status->front_center.value());
        }

        ASSERT_EQ(
          old_status->woofer.has_value(), current_status->woofer.has_value());
        if (current_status->woofer.has_value()) {
            EXPECT_NE(
              old_status->woofer.value(), current_status->woofer.value());
        }

        ASSERT_EQ(old_status->side_left.has_value(),
          current_status->side_left.has_value());
        if (current_status->side_left.has_value()) {
            EXPECT_NE(
              old_status->side_left.value(), current_status->side_left.value());
        }

        ASSERT_EQ(old_status->side_right.has_value(),
          current_status->side_right.has_value());
        if (current_status->side_right.has_value()) {
            EXPECT_NE(old_status->side_right.value(),
              current_status->side_right.value());
        }

        ASSERT_EQ(old_status->rear_center.has_value(),
          current_status->rear_center.has_value());
        if (current_status->rear_center.has_value()) {
            EXPECT_NE(old_status->rear_center.value(),
              current_status->rear_center.value());
        }

        ASSERT_TRUE(control.toggle_playback_status().success());

        // For some reason, setting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // All channels of the current status should match the those of the
        // original status after toggling twice.
        current_status = control.get_playback_status();
        ASSERT_TRUE(current_status.has_value());

        ASSERT_EQ(old_status->front_left.has_value(),
          current_status->front_left.has_value());
        if (current_status->front_left.has_value()) {
            EXPECT_EQ(old_status->front_left.value(),
              current_status->front_left.value());
        }

        ASSERT_EQ(old_status->front_right.has_value(),
          current_status->front_right.has_value());
        if (current_status->front_right.has_value()) {
            EXPECT_EQ(old_status->front_right.value(),
              current_status->front_right.value());
        }

        ASSERT_EQ(old_status->rear_left.has_value(),
          current_status->rear_left.has_value());
        if (current_status->rear_left.has_value()) {
            EXPECT_EQ(
              old_status->rear_left.value(), current_status->rear_left.value());
        }

        ASSERT_EQ(old_status->rear_right.has_value(),
          current_status->rear_right.has_value());
        if (current_status->rear_right.has_value()) {
            EXPECT_EQ(old_status->rear_right.value(),
              current_status->rear_right.value());
        }

        ASSERT_EQ(old_status->front_center.has_value(),
          current_status->front_center.has_value());
        if (current_status->front_center.has_value()) {
            EXPECT_EQ(old_status->front_center.value(),
              current_status->front_center.value());
        }

        ASSERT_EQ(
          old_status->woofer.has_value(), current_status->woofer.has_value());
        if (current_status->woofer.has_value()) {
            EXPECT_EQ(
              old_status->woofer.value(), current_status->woofer.value());
        }

        ASSERT_EQ(old_status->side_left.has_value(),
          current_status->side_left.has_value());
        if (current_status->side_left.has_value()) {
            EXPECT_EQ(
              old_status->side_left.value(), current_status->side_left.value());
        }

        ASSERT_EQ(old_status->side_right.has_value(),
          current_status->side_right.has_value());
        if (current_status->side_right.has_value()) {
            EXPECT_EQ(old_status->side_right.value(),
              current_status->side_right.value());
        }

        ASSERT_EQ(old_status->rear_center.has_value(),
          current_status->rear_center.has_value());
        if (current_status->rear_center.has_value()) {
            EXPECT_EQ(old_status->rear_center.value(),
              current_status->rear_center.value());
        }

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_playback_status);
}

TEST(sound_test, sound_control_set_playback_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::volume_t too_low_volume;
    too_low_volume.front_left = -1;
    too_low_volume.front_right = -1;
    too_low_volume.rear_left = -1;
    too_low_volume.rear_right = -1;
    too_low_volume.front_center = -1;
    too_low_volume.woofer = -1;
    too_low_volume.side_left = -1;
    too_low_volume.side_right = -1;
    too_low_volume.rear_center = -1;

    syst::sound_control_t::volume_t low_volume;
    low_volume.front_left = 0;
    low_volume.front_right = 0;
    low_volume.rear_left = 0;
    low_volume.rear_right = 0;
    low_volume.front_center = 0;
    low_volume.woofer = 0;
    low_volume.side_left = 0;
    low_volume.side_right = 0;
    low_volume.rear_center = 0;

    syst::sound_control_t::volume_t high_volume;
    high_volume.front_left = 100;
    high_volume.front_right = 100;
    high_volume.rear_left = 100;
    high_volume.rear_right = 100;
    high_volume.front_center = 100;
    high_volume.woofer = 100;
    high_volume.side_left = 100;
    high_volume.side_right = 100;
    high_volume.rear_center = 100;

    syst::sound_control_t::volume_t too_high_volume;
    too_high_volume.front_left = 101;
    too_high_volume.front_right = 101;
    too_high_volume.rear_left = 101;
    too_high_volume.rear_right = 101;
    too_high_volume.front_center = 101;
    too_high_volume.woofer = 101;
    too_high_volume.side_left = 101;
    too_high_volume.side_right = 101;
    too_high_volume.rear_center = 101;

    bool has_playback_volume = false;

    for (auto& control : controls) {
        if (! control.has_playback_volume()) {
            continue;
        }

        has_playback_volume = true;

        auto old_volume = control.get_playback_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_FALSE(control.set_playback_volume(too_low_volume).success());
        EXPECT_TRUE(control.set_playback_volume(low_volume).success());
        EXPECT_TRUE(control.set_playback_volume(high_volume).success());
        EXPECT_FALSE(control.set_playback_volume(too_high_volume).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_playback_volume);
}

TEST(sound_test, sound_control_set_playback_volume_all) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    const double too_low_volume = -1;
    const double low_volume = 0;
    const double high_volume = 100;
    const double too_high_volume = 101;

    bool has_playback_volume = false;

    for (auto& control : controls) {
        if (! control.has_playback_volume()) {
            continue;
        }

        has_playback_volume = true;

        auto old_volume = control.get_playback_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_FALSE(control.set_playback_volume_all(too_low_volume).success());
        EXPECT_TRUE(control.set_playback_volume_all(low_volume).success());
        EXPECT_TRUE(control.set_playback_volume_all(high_volume).success());
        EXPECT_FALSE(
          control.set_playback_volume_all(too_high_volume).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_playback_volume);
}

TEST(sound_test, sound_control_set_playback_volume_all_relative) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_playback_volume = false;

    for (auto& control : controls) {
        if (! control.has_playback_volume()) {
            continue;
        }

        has_playback_volume = true;

        auto old_volume = control.get_playback_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_TRUE(control.set_playback_volume_all(0).success());
        EXPECT_TRUE(control.set_playback_volume_all_relative(-5).success());
        EXPECT_TRUE(control.set_playback_volume_all_relative(+5).success());
        EXPECT_TRUE(control.set_playback_volume_all(100).success());
        EXPECT_TRUE(control.set_playback_volume_all_relative(+5).success());
        EXPECT_TRUE(control.set_playback_volume_all_relative(-5).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_playback_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_playback_volume);
}

TEST(sound_test, sound_control_set_capture_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::status_t new_status;
    new_status.front_left = false;
    new_status.front_right = false;
    new_status.rear_left = false;
    new_status.rear_right = false;
    new_status.front_center = false;
    new_status.woofer = false;
    new_status.side_left = false;
    new_status.side_right = false;
    new_status.rear_center = false;

    bool has_capture_status = false;

    for (auto& control : controls) {
        if (! control.has_capture_status()) {
            continue;
        }

        has_capture_status = true;

        auto old_status = control.get_capture_status();
        ASSERT_TRUE(old_status.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_capture_status(new_status).success());

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_capture_status);
}

TEST(sound_test, sound_control_set_capture_status_all) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_capture_status = false;

    for (auto& control : controls) {
        if (! control.has_capture_status()) {
            continue;
        }

        has_capture_status = true;

        auto old_status = control.get_capture_status();
        ASSERT_TRUE(old_status.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_capture_status_all(0).success());

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_capture_status);
}

TEST(sound_test, sound_control_toggle_capture_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_capture_status = false;

    for (auto& control : controls) {
        if (! control.has_capture_status()) {
            continue;
        }

        has_capture_status = true;

        auto old_status = control.get_capture_status();
        ASSERT_TRUE(old_status.has_value());

        ASSERT_TRUE(control.toggle_capture_status().success());

        // For some reason, setting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // All channels of the current status should be different from those of
        // the original status after toggling once.
        auto current_status = control.get_capture_status();
        ASSERT_TRUE(current_status.has_value());

        ASSERT_EQ(old_status->front_left.has_value(),
          current_status->front_left.has_value());
        if (current_status->front_left.has_value()) {
            EXPECT_NE(old_status->front_left.value(),
              current_status->front_left.value());
        }

        ASSERT_EQ(old_status->front_right.has_value(),
          current_status->front_right.has_value());
        if (current_status->front_right.has_value()) {
            EXPECT_NE(old_status->front_right.value(),
              current_status->front_right.value());
        }

        ASSERT_EQ(old_status->rear_left.has_value(),
          current_status->rear_left.has_value());
        if (current_status->rear_left.has_value()) {
            EXPECT_NE(
              old_status->rear_left.value(), current_status->rear_left.value());
        }

        ASSERT_EQ(old_status->rear_right.has_value(),
          current_status->rear_right.has_value());
        if (current_status->rear_right.has_value()) {
            EXPECT_NE(old_status->rear_right.value(),
              current_status->rear_right.value());
        }

        ASSERT_EQ(old_status->front_center.has_value(),
          current_status->front_center.has_value());
        if (current_status->front_center.has_value()) {
            EXPECT_NE(old_status->front_center.value(),
              current_status->front_center.value());
        }

        ASSERT_EQ(
          old_status->woofer.has_value(), current_status->woofer.has_value());
        if (current_status->woofer.has_value()) {
            EXPECT_NE(
              old_status->woofer.value(), current_status->woofer.value());
        }

        ASSERT_EQ(old_status->side_left.has_value(),
          current_status->side_left.has_value());
        if (current_status->side_left.has_value()) {
            EXPECT_NE(
              old_status->side_left.value(), current_status->side_left.value());
        }

        ASSERT_EQ(old_status->side_right.has_value(),
          current_status->side_right.has_value());
        if (current_status->side_right.has_value()) {
            EXPECT_NE(old_status->side_right.value(),
              current_status->side_right.value());
        }

        ASSERT_EQ(old_status->rear_center.has_value(),
          current_status->rear_center.has_value());
        if (current_status->rear_center.has_value()) {
            EXPECT_NE(old_status->rear_center.value(),
              current_status->rear_center.value());
        }

        ASSERT_TRUE(control.toggle_capture_status().success());

        // For some reason, setting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // All channels of the current status should match the those of the
        // original status after toggling twice.
        current_status = control.get_capture_status();
        ASSERT_TRUE(current_status.has_value());

        ASSERT_EQ(old_status->front_left.has_value(),
          current_status->front_left.has_value());
        if (current_status->front_left.has_value()) {
            EXPECT_EQ(old_status->front_left.value(),
              current_status->front_left.value());
        }

        ASSERT_EQ(old_status->front_right.has_value(),
          current_status->front_right.has_value());
        if (current_status->front_right.has_value()) {
            EXPECT_EQ(old_status->front_right.value(),
              current_status->front_right.value());
        }

        ASSERT_EQ(old_status->rear_left.has_value(),
          current_status->rear_left.has_value());
        if (current_status->rear_left.has_value()) {
            EXPECT_EQ(
              old_status->rear_left.value(), current_status->rear_left.value());
        }

        ASSERT_EQ(old_status->rear_right.has_value(),
          current_status->rear_right.has_value());
        if (current_status->rear_right.has_value()) {
            EXPECT_EQ(old_status->rear_right.value(),
              current_status->rear_right.value());
        }

        ASSERT_EQ(old_status->front_center.has_value(),
          current_status->front_center.has_value());
        if (current_status->front_center.has_value()) {
            EXPECT_EQ(old_status->front_center.value(),
              current_status->front_center.value());
        }

        ASSERT_EQ(
          old_status->woofer.has_value(), current_status->woofer.has_value());
        if (current_status->woofer.has_value()) {
            EXPECT_EQ(
              old_status->woofer.value(), current_status->woofer.value());
        }

        ASSERT_EQ(old_status->side_left.has_value(),
          current_status->side_left.has_value());
        if (current_status->side_left.has_value()) {
            EXPECT_EQ(
              old_status->side_left.value(), current_status->side_left.value());
        }

        ASSERT_EQ(old_status->side_right.has_value(),
          current_status->side_right.has_value());
        if (current_status->side_right.has_value()) {
            EXPECT_EQ(old_status->side_right.value(),
              current_status->side_right.value());
        }

        ASSERT_EQ(old_status->rear_center.has_value(),
          current_status->rear_center.has_value());
        if (current_status->rear_center.has_value()) {
            EXPECT_EQ(old_status->rear_center.value(),
              current_status->rear_center.value());
        }

        // For some reason, resetting the status too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_status(old_status.value()).success());
    }

    ASSERT_TRUE(has_capture_status);
}

TEST(sound_test, sound_control_set_capture_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::volume_t too_low_volume;
    too_low_volume.front_left = -1;
    too_low_volume.front_right = -1;
    too_low_volume.rear_left = -1;
    too_low_volume.rear_right = -1;
    too_low_volume.front_center = -1;
    too_low_volume.woofer = -1;
    too_low_volume.side_left = -1;
    too_low_volume.side_right = -1;
    too_low_volume.rear_center = -1;

    syst::sound_control_t::volume_t low_volume;
    low_volume.front_left = 0;
    low_volume.front_right = 0;
    low_volume.rear_left = 0;
    low_volume.rear_right = 0;
    low_volume.front_center = 0;
    low_volume.woofer = 0;
    low_volume.side_left = 0;
    low_volume.side_right = 0;
    low_volume.rear_center = 0;

    syst::sound_control_t::volume_t high_volume;
    high_volume.front_left = 100;
    high_volume.front_right = 100;
    high_volume.rear_left = 100;
    high_volume.rear_right = 100;
    high_volume.front_center = 100;
    high_volume.woofer = 100;
    high_volume.side_left = 100;
    high_volume.side_right = 100;
    high_volume.rear_center = 100;

    syst::sound_control_t::volume_t too_high_volume;
    too_high_volume.front_left = 101;
    too_high_volume.front_right = 101;
    too_high_volume.rear_left = 101;
    too_high_volume.rear_right = 101;
    too_high_volume.front_center = 101;
    too_high_volume.woofer = 101;
    too_high_volume.side_left = 101;
    too_high_volume.side_right = 101;
    too_high_volume.rear_center = 101;

    bool has_capture_volume = false;

    for (auto& control : controls) {
        if (! control.has_capture_volume()) {
            continue;
        }

        has_capture_volume = true;

        auto old_volume = control.get_capture_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_FALSE(control.set_capture_volume(too_low_volume).success());
        EXPECT_TRUE(control.set_capture_volume(low_volume).success());
        EXPECT_TRUE(control.set_capture_volume(high_volume).success());
        EXPECT_FALSE(control.set_capture_volume(too_high_volume).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_capture_volume);
}

TEST(sound_test, sound_control_set_capture_volume_all) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    const double too_low_volume = -1;
    const double low_volume = 0;
    const double high_volume = 100;
    const double too_high_volume = 101;

    bool has_capture_volume = false;

    for (auto& control : controls) {
        if (! control.has_capture_volume()) {
            continue;
        }

        has_capture_volume = true;

        auto old_volume = control.get_capture_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_FALSE(control.set_capture_volume_all(too_low_volume).success());
        EXPECT_TRUE(control.set_capture_volume_all(low_volume).success());
        EXPECT_TRUE(control.set_capture_volume_all(high_volume).success());
        EXPECT_FALSE(control.set_capture_volume_all(too_high_volume).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_capture_volume);
}

TEST(sound_test, sound_control_set_capture_volume_all_relative) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    bool has_capture_volume = false;

    for (auto& control : controls) {
        if (! control.has_capture_volume()) {
            continue;
        }

        has_capture_volume = true;

        auto old_volume = control.get_capture_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        EXPECT_TRUE(control.set_capture_volume_all(0).success());
        EXPECT_TRUE(control.set_capture_volume_all_relative(-5).success());
        EXPECT_TRUE(control.set_capture_volume_all_relative(+5).success());
        EXPECT_TRUE(control.set_capture_volume_all(100).success());
        EXPECT_TRUE(control.set_capture_volume_all_relative(+5).success());
        EXPECT_TRUE(control.set_capture_volume_all_relative(-5).success());

        // For some reason, resetting the volume too quickly fails but does
        // not provide a reason why.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ASSERT_TRUE(control.set_capture_volume(old_volume.value()).success());
    }

    ASSERT_TRUE(has_capture_volume);
}
