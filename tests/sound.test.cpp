// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

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

        ASSERT_TRUE(control.set_playback_status(new_status));
        ASSERT_TRUE(control.set_playback_status(old_status.value()));
    }

    ASSERT_TRUE(has_playback_status);
}

TEST(sound_test, sound_control_set_playback_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::volume_t new_volume;
    new_volume.front_left = 0;
    new_volume.front_right = 0;
    new_volume.rear_left = 0;
    new_volume.rear_right = 0;
    new_volume.front_center = 0;
    new_volume.woofer = 0;
    new_volume.side_left = 0;
    new_volume.side_right = 0;
    new_volume.rear_center = 0;

    bool has_playback_volume = false;

    for (auto& control : controls) {
        if (! control.has_playback_volume()) {
            continue;
        }

        has_playback_volume = true;

        auto old_volume = control.get_playback_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_playback_volume(new_volume));
        ASSERT_TRUE(control.set_playback_volume(old_volume.value()));
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

        ASSERT_TRUE(control.set_capture_status(new_status));
        ASSERT_TRUE(control.set_capture_status(old_status.value()));
    }

    ASSERT_TRUE(has_capture_status);
}

TEST(sound_test, sound_control_set_capture_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    syst::sound_control_t::volume_t new_volume;
    new_volume.front_left = 0;
    new_volume.front_right = 0;
    new_volume.rear_left = 0;
    new_volume.rear_right = 0;
    new_volume.front_center = 0;
    new_volume.woofer = 0;
    new_volume.side_left = 0;
    new_volume.side_right = 0;
    new_volume.rear_center = 0;

    bool has_capture_volume = false;

    for (auto& control : controls) {
        if (! control.has_capture_volume()) {
            continue;
        }

        has_capture_volume = true;

        auto old_volume = control.get_capture_volume();
        ASSERT_TRUE(old_volume.has_value());
        // Status can be any value.

        ASSERT_TRUE(control.set_capture_volume(new_volume));
        ASSERT_TRUE(control.set_capture_volume(old_volume.value()));
    }

    ASSERT_TRUE(has_capture_volume);
}
