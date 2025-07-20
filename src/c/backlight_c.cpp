// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"
#include "assert_c.hpp"

extern "C" {

char* syst_get_backlights(syst_backlight_list_t** backlight_list) {
    auto result = syst::get_backlights();
    ASSERT_HAS_VALUE(result);
    *backlight_list = result.release();
    return NULL;
}

char* syst_backlight_list_get_size(
  syst_backlight_list_t* backlight_list, unsigned long* size) {
    ASSERT_NOT_NULL(backlight_list);
    *size = backlight_list->size();
    return NULL;
}

char* syst_backlight_list_get(syst_backlight_list_t* backlight_list,
  unsigned long index,
  syst_backlight_t** backlight) {
    ASSERT_NOT_NULL(backlight_list);
    ASSERT_HAS_INDEX(backlight_list, index);
    *backlight = &((*backlight_list)[index]);
    return NULL;
}

void syst_backlight_list_free(syst_backlight_list_t* backlight_list) {
    delete backlight_list;
}

char* syst_backlight_get_sysfs_path(syst_backlight_t* backlight, char** path) {
    ASSERT_NOT_NULL(backlight);
    *path = strdup(backlight->get_sysfs_path().c_str());
    return NULL;
}

char* syst_backlight_get_name(syst_backlight_t* backlight, char** name) {
    ASSERT_NOT_NULL(backlight);
    *name = strdup(backlight->get_name().c_str());
    return NULL;
}

char* syst_backlight_get_brightness(
  syst_backlight_t* backlight, double* brightness) {
    ASSERT_NOT_NULL(backlight);
    auto result = backlight->get_brightness();
    ASSERT_HAS_VALUE(result);
    *brightness = result.value();
    return NULL;
}

char* syst_backlight_set_brightness(
  syst_backlight_t* backlight, double brightness) {
    ASSERT_NOT_NULL(backlight);
    ASSERT_SUCCESS(backlight->set_brightness(brightness));
    return NULL;
}

char* syst_backlight_set_brightness_relative(
  syst_backlight_t* backlight, double brightness) {
    ASSERT_NOT_NULL(backlight);
    ASSERT_SUCCESS(backlight->set_brightness(brightness));
    return NULL;
}

} // extern "C"
