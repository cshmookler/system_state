// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/core.hpp"

TEST(sound_test, sound_mixer_get) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
}

TEST(sound_test, sound_mixer_all_controls) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();
    ASSERT_EQ(controls.size(), 0);
}

TEST(sound_test, sound_control_get_playback_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    for (auto& control : controls) {
        auto status = control.get_playback_status();
        ASSERT_TRUE(status.has_value());
        // Status can be any value.
    }
}

TEST(sound_test, sound_control_get_playback_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    for (auto& control : controls) {
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
}

TEST(sound_test, sound_control_get_capture_status) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    for (auto& control : controls) {
        auto status = control.get_capture_status();
        ASSERT_TRUE(status.has_value());
        // Status can be any value.
    }
}

TEST(sound_test, sound_control_get_capture_volume) {
    auto mixer = syst::sound_mixer_t::get();
    ASSERT_TRUE(mixer.has_value());
    auto controls = mixer->all_controls();

    // For testing purposes, there must be at least one sound control element.
    ASSERT_NE(controls.size(), 0);

    for (auto& control : controls) {
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
}

// TODO: REMOVE ALL BELOW

// TEST(sound_mixer_test, get_playback_status) {
//     auto mixer = syst::sound_mixer_t::get();
//     ASSERT_TRUE(mixer.has_value());
//     auto playback_status = mixer.();
//     ASSERT_TRUE(playback_status.has_value());
//     // Playback status can be any value.
// }
//
// TEST(sound_mixer_test, get_playback_volume) {
//     syst::sound_mixer_t mixer;
//     auto playback_volume = mixer.playback_volume();
//     ASSERT_TRUE(playback_volume.has_value());
//     // Playback volume values must be between 0 and 100.
//     ASSERT_GE(playback_volume->left, 0);
//     ASSERT_LE(playback_volume->left, 100);
//     ASSERT_GE(playback_volume->right, 0);
//     ASSERT_LE(playback_volume->right, 100);
// }
//
// TEST(sound_mixer_test, get_capture_status) {
//     syst::sound_mixer_t mixer;
//     auto capture_status = mixer.capture_status();
//     ASSERT_TRUE(capture_status.has_value());
//     // Capture status can be any value.
// }
//
// TEST(sound_mixer_test, get_capture_volume) {
//     syst::sound_mixer_t mixer;
//     auto capture_volume = mixer.capture_volume();
//     ASSERT_TRUE(capture_volume.has_value());
//     // Capture volume values must be between 0 and 100.
//     ASSERT_GE(capture_volume->left, 0);
//     ASSERT_LE(capture_volume->left, 100);
//     ASSERT_GE(capture_volume->right, 0);
//     ASSERT_LE(capture_volume->right, 100);
// }
//
// TEST(sound_mixer_test, set_playback_status) {
//     syst::sound_mixer_t mixer;
//
//     // Get the initial playback status.
//     auto old_status = mixer.playback_status();
//     ASSERT_TRUE(old_status.has_value());
//     // Playback status can be any value.
//
//     // Set a new playback status.
//     syst::sound_mixer_t::status_t new_status{ ! old_status->left,
//         ! old_status->right };
//     ASSERT_TRUE(mixer.playback_status(new_status));
//
//     // Verify that the reported status matches the previously set status.
//     auto new_reported_status = mixer.playback_status();
//     EXPECT_TRUE(new_reported_status.has_value());
//     if (new_reported_status.has_value()) {
//         // This playback status must match the previously set playback
//         status. EXPECT_EQ(new_reported_status->left, new_status.left);
//         EXPECT_EQ(new_reported_status->right, new_status.right);
//     }
//
//     ASSERT_TRUE(mixer.playback_status(old_status.value()));
// }
//
// TEST(sound_mixer_test, set_playback_volume) {
//     syst::sound_mixer_t mixer;
//
//     // Get the initial playback volume.
//     auto old_volume = mixer.playback_volume();
//     ASSERT_TRUE(old_volume.has_value());
//     // Playback volume values must be between 0 and 100.
//     ASSERT_GE(old_volume->left, 0);
//     ASSERT_LE(old_volume->left, 100);
//     ASSERT_GE(old_volume->right, 0);
//     ASSERT_LE(old_volume->right, 100);
//
//     // Set a new playback volume.
//     syst::sound_mixer_t::volume_t new_volume{ 100.F - old_volume->left,
//         100.F - old_volume->right };
//     ASSERT_TRUE(mixer.playback_volume(new_volume));
//
//     // Verify that the reported volume matches the previously set volume.
//     auto new_reported_volume = mixer.playback_volume();
//     EXPECT_TRUE(new_reported_volume.has_value());
//     if (new_reported_volume.has_value()) {
//         // This playback volume must match the previously set playback
//         volume. EXPECT_EQ(new_reported_volume->left, new_volume.left);
//         EXPECT_EQ(new_reported_volume->right, new_volume.right);
//     }
//
//     ASSERT_TRUE(mixer.playback_volume(old_volume.value()));
// }
//
// TEST(sound_mixer_test, set_capture_status) {
//     syst::sound_mixer_t mixer;
//
//     // Get the initial capture status.
//     auto old_status = mixer.capture_status();
//     ASSERT_TRUE(old_status.has_value());
//     // Capture status can be any value.
//
//     // Set a new capture status.
//     syst::sound_mixer_t::status_t new_status{ ! old_status->left,
//         ! old_status->right };
//     ASSERT_TRUE(mixer.capture_status(new_status));
//
//     // Verify that the reported status matches the previously set status.
//     auto new_reported_status = mixer.capture_status();
//     EXPECT_TRUE(new_reported_status.has_value());
//     if (new_reported_status.has_value()) {
//         // This capture status must match the previously set capture status.
//         EXPECT_EQ(new_reported_status->left, new_status.left);
//         EXPECT_EQ(new_reported_status->right, new_status.right);
//     }
//
//     ASSERT_TRUE(mixer.capture_status(old_status.value()));
// }
//
// TEST(sound_mixer_test, set_capture_volume) {
//     syst::sound_mixer_t mixer;
//
//     // Get the initial capture volume.
//     auto old_volume = mixer.capture_volume();
//     ASSERT_TRUE(old_volume.has_value());
//     // Capture volume values must be between 0 and 100.
//     ASSERT_GE(old_volume->left, 0);
//     ASSERT_LE(old_volume->left, 100);
//     ASSERT_GE(old_volume->right, 0);
//     ASSERT_LE(old_volume->right, 100);
//
//     // Set a new capture volume.
//     syst::sound_mixer_t::volume_t new_volume{ 100.F - old_volume->left,
//         100.F - old_volume->right };
//     ASSERT_TRUE(mixer.capture_volume(new_volume));
//
//     // Verify that the reported volume matches the previously set volume.
//     auto new_reported_volume = mixer.capture_volume();
//     EXPECT_TRUE(new_reported_volume.has_value());
//     if (new_reported_volume.has_value()) {
//         // This capture volume must match the previously set capture volume.
//         EXPECT_EQ(new_reported_volume->left, new_volume.left);
//         EXPECT_EQ(new_reported_volume->right, new_volume.right);
//     }
//
//     ASSERT_TRUE(mixer.capture_volume(old_volume.value()));
// }
