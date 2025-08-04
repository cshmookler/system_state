// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"
#include "assert_c.hpp"

extern "C" {

syst_backlight_list_t* syst_get_backlights(char** error) {
    auto result = syst::get_backlights();
    ASSERT_HAS_VALUE(result, NULL);
    return result.release();
}

unsigned long syst_backlight_list_get_size(
  syst_backlight_list_t* backlight_list, char** error) {
    ASSERT_NOT_NULL(backlight_list, 0);
    return backlight_list->size();
}

syst_backlight_t* syst_backlight_list_get(
  syst_backlight_list_t* backlight_list, unsigned long index, char** error) {
    ASSERT_NOT_NULL(backlight_list, NULL);
    ASSERT_HAS_INDEX(backlight_list, index, NULL);
    return &((*backlight_list)[index]);
}

void syst_backlight_list_free(syst_backlight_list_t* backlight_list) {
    delete backlight_list;
}

char* syst_backlight_get_sysfs_path(syst_backlight_t* backlight, char** error) {
    ASSERT_NOT_NULL(backlight, NULL);
    return strdup(backlight->get_sysfs_path().c_str());
}

char* syst_backlight_get_name(syst_backlight_t* backlight, char** error) {
    ASSERT_NOT_NULL(backlight, NULL);
    return strdup(backlight->get_name().c_str());
}

double syst_backlight_get_brightness(
  syst_backlight_t* backlight, char** error) {
    ASSERT_NOT_NULL(backlight, 0);
    auto result = backlight->get_brightness();
    ASSERT_HAS_VALUE(result, 0);
    return result.value();
}

void syst_backlight_set_brightness(
  syst_backlight_t* backlight, double brightness, char** error) {
    ASSERT_NOT_NULL(backlight, );
    ASSERT_SUCCESS(backlight->set_brightness(brightness), );
}

void syst_backlight_set_brightness_relative(
  syst_backlight_t* backlight, double brightness, char** error) {
    ASSERT_NOT_NULL(backlight, );
    ASSERT_SUCCESS(backlight->set_brightness_relative(brightness), );
}

} // extern "C"
