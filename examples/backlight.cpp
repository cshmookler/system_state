// Standard includes
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int get_backlight() {
    auto backlights = syst::backlight_t::all();
    if (backlights.has_error()) {
        std::cerr << backlights.error().string() << '\n';
        return 1;
    }

    for (const auto& device : backlights.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << '\n';
        std::cout << "Name: " << device.name() << '\n';

        auto brightness = device.get_brightness();
        if (brightness.has_value()) {
            std::cout << "Brightness: " << brightness.value() << "%" << '\n';
        } else {
            std::cerr << brightness.error().string() << '\n';
        }

        std::cout << '\n';
    }

    return 0;
}

int set_backlight() {
    auto backlights = syst::backlight_t::all();
    if (backlights.has_error()) {
        std::cerr << backlights.error().string() << '\n';
        return 1;
    }

    for (auto& device : backlights.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << '\n';
        std::cout << "Name: " << device.name() << '\n';

        auto old_brightness = device.get_brightness();
        if (old_brightness.has_error()) {
            std::cerr << old_brightness.error().string() << '\n';
            continue;
        }

        auto result = device.set_brightness(0);
        if (result.failure()) {
            std::cerr << result.error().string() << '\n';
            continue;
        }

        result = device.set_brightness_relative(20);
        if (result.failure()) {
            std::cerr << result.error().string() << '\n';
            continue;
        }

        result = device.set_brightness(old_brightness.value());
        if (result.failure()) {
            std::cerr << result.error().string() << '\n';
            continue;
        }

        std::cout << '\n';
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
