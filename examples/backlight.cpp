// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"

int get_backlight() {
    auto backlights = syst::backlight_t::all();
    if (backlights.has_error()) {
        std::cerr << backlights.error() << std::endl;
        return 1;
    }

    for (const auto& device : backlights.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << std::endl;
        std::cout << "Name: " << device.name() << std::endl;

        auto brightness = device.get_brightness();
        if (brightness.has_value()) {
            std::cout << "Brightness: " << brightness.value() << "%"
                      << std::endl;
        } else {
            std::cerr << brightness.error() << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}

int set_backlight() {
    auto backlights = syst::backlight_t::all();
    if (backlights.has_error()) {
        std::cerr << backlights.error() << std::endl;
        return 1;
    }

    for (auto& device : backlights.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << std::endl;
        std::cout << "Name: " << device.name() << std::endl;

        auto old_brightness = device.get_brightness();
        if (old_brightness.has_error()) {
            std::cerr << old_brightness.error() << std::endl;
            continue;
        }

        auto result = device.set_brightness(0);
        if (result.failure()) {
            std::cerr << result.error() << std::endl;
            continue;
        }

        result = device.set_brightness_relative(20);
        if (result.failure()) {
            std::cerr << result.error() << std::endl;
            continue;
        }

        result = device.set_brightness(old_brightness.value());
        if (result.failure()) {
            std::cerr << result.error() << std::endl;
            continue;
        }

        std::cout << std::endl;
    }

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
