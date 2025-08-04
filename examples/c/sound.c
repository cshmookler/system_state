// Standard includes
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>

// External includes
#include "../../system_state/system_state.h"
#include "macro.h"

int get_sound() {
    char* error = NULL;

    syst_sound_mixer_t* sound_mixer = syst_get_sound_mixer(&error);
    ASSERT_SUCCESS();

    syst_sound_control_list_t* sound_control_list =
      syst_sound_mixer_get_controls(sound_mixer, &error);
    ASSERT_SUCCESS();

    unsigned long sound_control_count =
      syst_sound_control_list_get_size(sound_control_list, &error);
    ASSERT_SUCCESS();

    for (unsigned long idx = 0; idx < sound_control_count; ++idx) {
        syst_sound_control_t* sound_control =
          syst_sound_control_list_get(sound_control_list, idx, &error);
        ASSERT_SUCCESS();

        char* name = syst_sound_control_get_name(sound_control, &error);
        ASSERT_SUCCESS();
        printf("Name: %s\n", name);
        syst_string_free(name);

        int has_playback_status =
          syst_sound_control_has_playback_status(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_playback_status == 1) {
            // TODO: get playback status
        }

        int has_playback_volume =
          syst_sound_control_has_playback_volume(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_playback_volume == 1) {
            // TODO: get playback volume
        }

        int has_capture_status =
          syst_sound_control_has_capture_status(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_capture_status == 1) {
            // TODO: get capture status
        }

        int has_capture_volume =
          syst_sound_control_has_capture_volume(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_capture_volume == 1) {
            // TODO: get capture volume
        }

        printf("\n");
    }

    syst_sound_mixer_free(sound_mixer);

    return 0;
}

int set_sound() {
    char* error = NULL;

    syst_sound_mixer_t* sound_mixer = syst_get_sound_mixer(&error);
    ASSERT_SUCCESS();

    syst_sound_control_list_t* sound_control_list =
      syst_sound_mixer_get_controls(sound_mixer, &error);
    ASSERT_SUCCESS();

    unsigned long sound_control_count =
      syst_sound_control_list_get_size(sound_control_list, &error);
    ASSERT_SUCCESS();

    for (unsigned long idx = 0; idx < sound_control_count; ++idx) {
        syst_sound_control_t* sound_control =
          syst_sound_control_list_get(sound_control_list, idx, &error);
        ASSERT_SUCCESS();

        char* name = syst_sound_control_get_name(sound_control, &error);
        ASSERT_SUCCESS();
        printf("Name: %s\n", name);
        syst_string_free(name);

        int has_playback_status =
          syst_sound_control_has_playback_status(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_playback_status == 1) {
            syst_sound_control_toggle_playback_status(sound_control, &error);
            ASSERT_SUCCESS();

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            usleep(100 * 1000);

            syst_sound_control_toggle_playback_status(sound_control, &error);
            ASSERT_SUCCESS();
        }

        int has_playback_volume =
          syst_sound_control_has_playback_volume(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_playback_volume == 1) {
            syst_sound_control_set_playback_volume_all_relative(
              sound_control, 20, &error);
            ASSERT_SUCCESS();

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            usleep(100 * 1000);

            syst_sound_control_set_playback_volume_all_relative(
              sound_control, -20, &error);
            ASSERT_SUCCESS();
        }

        int has_capture_status =
          syst_sound_control_has_capture_status(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_capture_status == 1) {
            syst_sound_control_toggle_capture_status(sound_control, &error);
            ASSERT_SUCCESS();

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            usleep(100 * 1000);

            syst_sound_control_toggle_capture_status(sound_control, &error);
            ASSERT_SUCCESS();
        }

        int has_capture_volume = 0;
        syst_sound_control_has_capture_volume(sound_control, &error);
        ASSERT_SUCCESS();
        if (has_capture_volume == 1) {
            syst_sound_control_set_capture_volume_all_relative(
              sound_control, 20, &error);
            ASSERT_SUCCESS();

            // For some reason, setting the status too quickly fails but does
            // not provide a reason why.
            usleep(100 * 1000);

            syst_sound_control_set_capture_volume_all_relative(
              sound_control, -20, &error);
            ASSERT_SUCCESS();
        }

        printf("\n");
    }

    syst_sound_mixer_free(sound_mixer);

    return 0;
}

int main(int argc, char** argv) {
    // If there are no arguments, get sound information.
    // If there is at least one argument, set sound status and volume.

    if (argc == 1) {
        return get_sound();
    }

    return set_sound();
}
