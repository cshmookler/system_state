// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"
#include "assert_c.hpp"

extern "C" {

char* syst_get_sound_mixer(syst_sound_mixer_t** sound_mixer) {
    ASSERT_NOT_NULL(sound_mixer);
    auto result = syst::get_sound_mixer();
    ASSERT_HAS_VALUE(result);
    *sound_mixer = result.release();
    return NULL;
}

char* syst_sound_mixer_get_controls(syst_sound_mixer_t* sound_mixer,
  syst_sound_control_list_t** sound_control_list) {
    ASSERT_NOT_NULL(sound_mixer);
    auto result = sound_mixer->get_controls();
    *sound_control_list = new (std::nothrow) syst_sound_control_list_t;
    *(*sound_control_list) = std::move(result);
    return NULL;
}

void syst_sound_mixer_free(syst_sound_mixer_t* sound_mixer) {
    delete sound_mixer;
}

char* syst_sound_control_list_get_size(
  syst_sound_control_list_t* sound_control_list, unsigned long* size) {
    ASSERT_NOT_NULL(sound_control_list);
    *size = sound_control_list->size();
    return NULL;
}

char* syst_sound_control_list_get(syst_sound_control_list_t* sound_control_list,
  unsigned long index,
  syst_sound_control_t** sound_control) {
    ASSERT_NOT_NULL(sound_control_list);
    ASSERT_HAS_INDEX(sound_control_list, index);
    *sound_control = &((*sound_control_list)[index]);
    return NULL;
}

void syst_sound_control_list_free(
  syst_sound_control_list_t* sound_control_list) {
    delete sound_control_list;
}

char* syst_sound_control_get_name(
  syst_sound_control_t* sound_control, char** name) {
    ASSERT_NOT_NULL(sound_control);
    *name = strdup(sound_control->get_name().c_str());
    return NULL;
}

char* syst_sound_control_has_playback_status(
  syst_sound_control_t* sound_control, int* has_playback_status) {
    ASSERT_NOT_NULL(sound_control);
    *has_playback_status = sound_control->has_playback_status();
    return NULL;
}

char* syst_sound_control_has_playback_volume(
  syst_sound_control_t* sound_control, int* has_playback_volume) {
    ASSERT_NOT_NULL(sound_control);
    *has_playback_volume = sound_control->has_playback_volume();
    return NULL;
}

char* syst_sound_control_has_capture_status(
  syst_sound_control_t* sound_control, int* has_capture_status) {
    ASSERT_NOT_NULL(sound_control);
    *has_capture_status = sound_control->has_capture_status();
    return NULL;
}

char* syst_sound_control_has_capture_volume(
  syst_sound_control_t* sound_control, int* has_capture_volume) {
    ASSERT_NOT_NULL(sound_control);
    *has_capture_volume = sound_control->has_capture_volume();
    return NULL;
}

char* syst_sound_control_set_playback_status_all(
  syst_sound_control_t* sound_control, int status) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_playback_status_all(status));
    return NULL;
}

char* syst_sound_control_toggle_playback_status(
  syst_sound_control_t* sound_control) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->toggle_playback_status());
    return NULL;
}

char* syst_sound_control_set_playback_volume_all(
  syst_sound_control_t* sound_control, double volume) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_playback_volume_all(volume));
    return NULL;
}

char* syst_sound_control_set_playback_volume_all_relative(
  syst_sound_control_t* sound_control, double volume) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_playback_volume_all_relative(volume));
    return NULL;
}

char* syst_sound_control_set_capture_status_all(
  syst_sound_control_t* sound_control, int status) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_capture_status_all(status));
    return NULL;
}

char* syst_sound_control_toggle_capture_status(
  syst_sound_control_t* sound_control) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->toggle_capture_status());
    return NULL;
}

char* syst_sound_control_set_capture_volume_all(
  syst_sound_control_t* sound_control, double volume) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_capture_volume_all(volume));
    return NULL;
}

char* syst_sound_control_set_capture_volume_all_relative(
  syst_sound_control_t* sound_control, double volume) {
    ASSERT_NOT_NULL(sound_control);
    ASSERT_SUCCESS(sound_control->set_capture_volume_all_relative(volume));
    return NULL;
}

} // extern "C"
