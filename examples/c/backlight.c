// Standard includes
#include <stdio.h>

// External includes
#include "../../system_state/system_state.h"
#include "macro.h"

int get_backlight(void) {
    char* error = NULL;

    syst_backlight_list_t* backlight_list = syst_get_backlights(&error);
    ASSERT_SUCCESS();

    unsigned long backlight_count =
      syst_backlight_list_get_size(backlight_list, &error);
    ASSERT_SUCCESS();

    for (unsigned long idx = 0; idx < backlight_count; ++idx) {
        syst_backlight_t* backlight =
          syst_backlight_list_get(backlight_list, idx, &error);
        ASSERT_SUCCESS();

        char* sysfs_path = syst_backlight_get_sysfs_path(backlight, &error);
        ASSERT_SUCCESS();
        printf("sysfs path: %s\n", sysfs_path);
        syst_string_free(sysfs_path);

        char* name = syst_backlight_get_name(backlight, &error);
        ASSERT_SUCCESS();
        printf("Name: %s\n", name);
        syst_string_free(name);

        double brightness = syst_backlight_get_brightness(backlight, &error);
        ASSERT_SUCCESS();
        printf("Brightness: %.4f%%\n", brightness);

        printf("\n");
    }

    syst_backlight_list_free(backlight_list);

    return 0;
}

int set_backlight(void) {
    char* error = NULL;

    syst_backlight_list_t* backlight_list = syst_get_backlights(&error);
    ASSERT_SUCCESS();

    unsigned long backlight_count =
      syst_backlight_list_get_size(backlight_list, &error);
    ASSERT_SUCCESS();

    for (unsigned long idx = 0; idx < backlight_count; ++idx) {
        syst_backlight_t* backlight =
          syst_backlight_list_get(backlight_list, idx, &error);
        ASSERT_SUCCESS();

        char* sysfs_path = syst_backlight_get_sysfs_path(backlight, &error);
        ASSERT_SUCCESS();
        printf("sysfs path: %s\n", sysfs_path);
        syst_string_free(sysfs_path);

        char* name = syst_backlight_get_name(backlight, &error);
        ASSERT_SUCCESS();
        printf("Name: %s\n", name);
        syst_string_free(name);

        double old_brightness =
          syst_backlight_get_brightness(backlight, &error);
        ASSERT_SUCCESS();

        syst_backlight_set_brightness(backlight, 0, &error);
        ASSERT_SUCCESS();
        syst_backlight_set_brightness_relative(backlight, 20, &error);
        ASSERT_SUCCESS();
        syst_backlight_set_brightness(backlight, old_brightness, &error);
        ASSERT_SUCCESS();

        printf("\n");
    }

    syst_backlight_list_free(backlight_list);

    return 0;
}

int main(int argc, char** argv) {
    // If there are no arguments, get backlight information.
    // If there is at least one argument, set backlight state.

    if (argc == 1) {
        return get_backlight();
    }

    return set_backlight();
}
