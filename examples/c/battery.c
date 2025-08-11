// Standard includes
#include <stdio.h>

// External includes
#include "../../system_state/system_state.h"
#include "macro.h"

int main() {
    char* error = NULL;

    syst_battery_list_t* battery_list = syst_get_batteries(&error);
    ASSERT_SUCCESS();

    unsigned long battery_count =
      syst_battery_list_get_size(battery_list, &error);
    ASSERT_SUCCESS();

    for (unsigned long idx = 0; idx < battery_count; ++idx) {
        syst_battery_t* battery =
          syst_battery_list_get(battery_list, idx, &error);
        ASSERT_SUCCESS();

        char* sysfs_path = syst_battery_get_sysfs_path(battery, &error);
        ASSERT_SUCCESS();
        printf("sysfs path: %s\n", sysfs_path);
        syst_string_free(sysfs_path);

        char* name = syst_battery_get_name(battery, &error);
        ASSERT_SUCCESS();
        printf("Name: %s\n", name);
        syst_string_free(name);

        syst_battery_status_t status = syst_battery_get_status(battery, &error);
        ASSERT_SUCCESS();
        printf("Status: ");
        switch (status) {
            case syst_battery_status_unknown:
                printf("Unknown\n");
                break;
            case syst_battery_status_charging:
                printf("Charging\n");
                break;
            case syst_battery_status_discharging:
                printf("Discharging\n");
                break;
            case syst_battery_status_not_charging:
                printf("Not Charging\n");
                break;
            case syst_battery_status_full:
                printf("Full\n");
                break;
        }

        double current = syst_battery_get_current(battery, &error);
        ASSERT_SUCCESS();
        printf("Current: %.5f A\n", current);

        double power = syst_battery_get_power(battery, &error);
        ASSERT_SUCCESS();
        printf("Power: %.3f W\n", power);

        double charge = syst_battery_get_charge(battery, &error);
        ASSERT_SUCCESS();
        printf("Charge: %.4f%%\n", charge);

        double capacity = syst_battery_get_capacity(battery, &error);
        ASSERT_SUCCESS();
        printf("Capacity: %.4f%%\n", capacity);

        unsigned long total_seconds =
          syst_battery_get_time_remaining(battery, &error);
        ASSERT_SUCCESS();
        unsigned long hours = total_seconds / 3600;
        unsigned long minutes = (total_seconds % 3600) / 60;
        unsigned long seconds = ((total_seconds % 3600) % 60) / 60;
        printf("Time Remaining: %02lu:%02lu:%02lu\n", hours, minutes, seconds);

        printf("\n");
    }

    syst_battery_list_free(battery_list);

    return 0;
}
