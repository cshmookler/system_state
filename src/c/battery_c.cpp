// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"
#include "assert_c.hpp"

extern "C" {

syst_battery_list_t* syst_get_batteries(char** error) {
    auto result = syst::get_batteries();
    ASSERT_HAS_VALUE(result, NULL);
    return result.release();
}

unsigned long syst_battery_list_get_size(
  syst_battery_list_t* battery_list, char** error) {
    ASSERT_NOT_NULL(battery_list, 0);
    return battery_list->size();
}

syst_battery_t* syst_battery_list_get(
  syst_battery_list_t* battery_list, unsigned long index, char** error) {
    ASSERT_NOT_NULL(battery_list, NULL);
    ASSERT_HAS_INDEX(battery_list, index, NULL);
    return &((*battery_list)[index]);
}

void syst_battery_list_free(syst_battery_list_t* battery_list) {
    delete battery_list;
}

char* syst_battery_get_sysfs_path(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, NULL);
    return strdup(battery->get_sysfs_path().c_str());
}

char* syst_battery_get_name(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, NULL);
    return strdup(battery->get_name().c_str());
}

syst_battery_status_t syst_battery_get_status(
  syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, syst_battery_status_unknown);
    auto result = battery->get_status();
    ASSERT_HAS_VALUE(result, syst_battery_status_unknown);
    switch (result.value()) {
        case syst::battery_t::status_t::unknown:
            return syst_battery_status_unknown;
        case syst::battery_t::status_t::charging:
            return syst_battery_status_charging;
        case syst::battery_t::status_t::discharging:
            return syst_battery_status_discharging;
        case syst::battery_t::status_t::not_charging:
            return syst_battery_status_not_charging;
        case syst::battery_t::status_t::full:
            return syst_battery_status_full;
        default:
            return syst_battery_status_unknown;
    }
}

double syst_battery_get_current(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, 0);
    auto result = battery->get_current();
    ASSERT_HAS_VALUE(result, 0);
    return result.value();
}

double syst_battery_get_power(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, 0);
    auto result = battery->get_power();
    ASSERT_HAS_VALUE(result, 0);
    return result.value();
}

double syst_battery_get_charge(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, 0);
    auto result = battery->get_charge();
    ASSERT_HAS_VALUE(result, 0);
    return result.value();
}

double syst_battery_get_capacity(syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, 0);
    auto result = battery->get_capacity();
    ASSERT_HAS_VALUE(result, 0);
    return result.value();
}

unsigned long syst_battery_get_time_remaining(
  syst_battery_t* battery, char** error) {
    ASSERT_NOT_NULL(battery, 0);
    auto result = battery->get_time_remaining();
    ASSERT_HAS_VALUE(result, 0);
    return result->count();
}

} // extern "C"
