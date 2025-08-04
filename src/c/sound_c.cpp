// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"
#include "assert_c.hpp"

extern "C" {

syst_sound_mixer_t* syst_get_sound_mixer(char** error) {
    auto result = syst::get_sound_mixer();
    ASSERT_HAS_VALUE(result, NULL);
    return result.release();
}

syst_sound_control_list_t* syst_sound_mixer_get_controls(
  syst_sound_mixer_t* sound_mixer, char** error) {
    ASSERT_NOT_NULL(sound_mixer, NULL);
    auto result = sound_mixer->get_controls();
    syst_sound_control_list_t* sound_control_list =
      new (std::nothrow) syst_sound_control_list_t;
    *sound_control_list = std::move(result);
    return sound_control_list;
}

void syst_sound_mixer_free(syst_sound_mixer_t* sound_mixer) {
    delete sound_mixer;
}

unsigned long syst_sound_control_list_get_size(
  syst_sound_control_list_t* sound_control_list, char** error) {
    ASSERT_NOT_NULL(sound_control_list, 0);
    return sound_control_list->size();
}

syst_sound_control_t* syst_sound_control_list_get(
  syst_sound_control_list_t* sound_control_list,
  unsigned long index,
  char** error) {
    ASSERT_NOT_NULL(sound_control_list, NULL);
    ASSERT_HAS_INDEX(sound_control_list, index, NULL);
    return &((*sound_control_list)[index]);
}

void syst_sound_control_list_free(
  syst_sound_control_list_t* sound_control_list) {
    delete sound_control_list;
}

char* syst_sound_control_get_name(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, NULL);
    return strdup(sound_control->get_name().c_str());
}

int syst_sound_control_has_playback_status(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, 0);
    return sound_control->has_playback_status();
}

int syst_sound_control_has_playback_volume(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, 0);
    return sound_control->has_playback_volume();
}

int syst_sound_control_has_capture_status(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, 0);
    return sound_control->has_capture_status();
}

int syst_sound_control_has_capture_volume(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, 0);
    return sound_control->has_capture_volume();
}

void syst_sound_control_set_playback_status_all(
  syst_sound_control_t* sound_control, int status, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_playback_status_all(status), );
}

void syst_sound_control_toggle_playback_status(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->toggle_playback_status(), );
}

void syst_sound_control_set_playback_volume_all(
  syst_sound_control_t* sound_control, double volume, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_playback_volume_all(volume), );
}

void syst_sound_control_set_playback_volume_all_relative(
  syst_sound_control_t* sound_control, double volume, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_playback_volume_all_relative(volume), );
}

void syst_sound_control_set_capture_status_all(
  syst_sound_control_t* sound_control, int status, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_capture_status_all(status), );
}

void syst_sound_control_toggle_capture_status(
  syst_sound_control_t* sound_control, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->toggle_capture_status(), );
}

void syst_sound_control_set_capture_volume_all(
  syst_sound_control_t* sound_control, double volume, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_capture_volume_all(volume), );
}

void syst_sound_control_set_capture_volume_all_relative(
  syst_sound_control_t* sound_control, double volume, char** error) {
    ASSERT_NOT_NULL(sound_control, );
    ASSERT_SUCCESS(sound_control->set_capture_volume_all_relative(volume), );
}

} // extern "C"
