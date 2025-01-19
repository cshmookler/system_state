// Standard includes
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int get_thermal() {
    auto thermal_zones = syst::thermal_zone_t::all();
    if (thermal_zones.has_error()) {
        std::cerr << thermal_zones.error().string() << '\n';
        return 1;
    }

    std::cout << "Thermal Zones:" << '\n';
    for (auto& zone : thermal_zones.value()) {
        std::cout << "sysfs path: " << zone.sysfs_path() << '\n';

        auto type = zone.type();
        if (type.has_value()) {
            std::cout << "\tType: " << type.value() << '\n';
        } else {
            std::cerr << type.error().string() << '\n';
        }

        auto temperature = zone.temperature();
        if (temperature.has_value()) {
            std::cout << "\tTemperature: " << temperature.value() << "°C"
                      << '\n';
        } else {
            std::cerr << temperature.error().string() << '\n';
        }
    }

    std::cout << '\n';

    auto cooling_devices = syst::cooling_device_t::all();
    if (cooling_devices.has_error()) {
        std::cerr << cooling_devices.error().string() << '\n';
        return 1;
    }

    std::cout << "Cooling Devices:" << '\n';
    for (auto& device : cooling_devices.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << '\n';

        auto type = device.type();
        if (type.has_value()) {
            std::cout << "\tType: " << type.value() << '\n';
        } else {
            std::cerr << type.error().string() << '\n';
        }

        auto state = device.get_state();
        if (state.has_value()) {
            std::cout << "\tState: " << state.value() << "%" << '\n';
        } else {
            std::cerr << state.error().string() << '\n';
        }
    }

    return 0;
}

int set_thermal() {
    auto cooling_devices = syst::cooling_device_t::all();
    if (cooling_devices.has_error()) {
        std::cerr << cooling_devices.error().string() << '\n';
        return 1;
    }

    double new_state = 0;

    for (auto& device : cooling_devices.value()) {
        auto old_state = device.get_state();
        if (old_state.has_error()) {
            std::cerr << old_state.error().string() << '\n';
            return 1;
        }

        auto result = device.set_state(new_state);
        if (result.failure()) {
            std::cerr << result.error().string() << '\n';
            return 1;
        }

        result = device.set_state(old_state.value());
        if (result.failure()) {
            std::cerr << result.error().string() << '\n';
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    // If there are no arguments, get thermal and cooling information.
    // If there is at least one argument, set cooling state.

    if (argc == 1) {
        return get_thermal();
    }

    return set_thermal();
}
