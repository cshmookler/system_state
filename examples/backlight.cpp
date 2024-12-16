// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto backlights = syst::backlight_t::all();
    if (! backlights.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }

    for (const auto& device : backlights.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << std::endl;
        std::cout << "Name: " << device.name() << std::endl;

        auto brightness = device.brightness();
        if (brightness.has_value()) {
            std::cout << "Brightness: " << brightness.value() << "%"
                      << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        std::cout << std::endl;
    }
}
