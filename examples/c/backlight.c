// Standard includes
#include <stdio.h>

// External includes
#include "../../system_state/system_state.h"
#include "macro.h"

int get_backlight(void) {
    char* error = NULL;

    syst_backlight_list_t* backlight_list = NULL;
    ASSERT_SUCCESS(syst_get_backlights(&backlight_list));

    unsigned long backlight_count = 0;
    ASSERT_SUCCESS(
      syst_backlight_list_get_size(backlight_list, &backlight_count));

    for (unsigned long idx = 0; idx < backlight_count; ++idx) {
        syst_backlight_t* backlight = NULL;
        ASSERT_SUCCESS(
          syst_backlight_list_get(backlight_list, idx, &backlight));

        char* sysfs_path = NULL;
        ASSERT_SUCCESS(syst_backlight_get_sysfs_path(backlight, &sysfs_path));
        printf("sysfs path: %s\n", sysfs_path);
        syst_string_free(sysfs_path);

        char* name = NULL;
        ASSERT_SUCCESS(syst_backlight_get_name(backlight, &name));
        printf("Name: %s\n", name);
        syst_string_free(name);

        double brightness = 0;
        ASSERT_SUCCESS(syst_backlight_get_brightness(backlight, &brightness));
        printf("Brightness: %.4f%%\n", brightness);

        printf("\n");
    }

    syst_backlight_list_free(backlight_list);

    return 0;
}

int set_backlight(void) {
    char* error = NULL;

    syst_backlight_list_t* backlight_list = NULL;
    ASSERT_SUCCESS(syst_get_backlights(&backlight_list));

    unsigned long backlight_count = 0;
    ASSERT_SUCCESS(
      syst_backlight_list_get_size(backlight_list, &backlight_count));

    for (unsigned long idx = 0; idx < backlight_count; ++idx) {
        syst_backlight_t* backlight = NULL;
        ASSERT_SUCCESS(
          syst_backlight_list_get(backlight_list, idx, &backlight));

        char* sysfs_path = NULL;
        ASSERT_SUCCESS(syst_backlight_get_sysfs_path(backlight, &sysfs_path));
        printf("sysfs path: %s\n", sysfs_path);
        syst_string_free(sysfs_path);

        char* name = NULL;
        ASSERT_SUCCESS(syst_backlight_get_name(backlight, &name));
        printf("Name: %s\n", name);
        syst_string_free(name);

        double old_brightness = 0;
        ASSERT_SUCCESS(
          syst_backlight_get_brightness(backlight, &old_brightness));

        ASSERT_SUCCESS(syst_backlight_set_brightness(backlight, 0));
        ASSERT_SUCCESS(syst_backlight_set_brightness_relative(backlight, 20));
        ASSERT_SUCCESS(
          syst_backlight_set_brightness(backlight, old_brightness));

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
