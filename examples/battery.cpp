// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto batteries = syst::battery_t::all();
    if (! batteries.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }

    for (const auto& device : batteries.value()) {
        std::cout << "sysfs path: " << device.sysfs_path() << std::endl;
        std::cout << "Name: " << device.name() << std::endl;

        auto status = device.status();
        if (status.has_value()) {
            std::cout << "Status: ";
            switch (status.value()) {
                case syst::battery_t::status_t::unknown:
                    std::cout << "Unknown";
                    break;
                case syst::battery_t::status_t::charging:
                    std::cout << "Charging";
                    break;
                case syst::battery_t::status_t::discharging:
                    std::cout << "Discharging";
                    break;
                case syst::battery_t::status_t::not_charging:
                    std::cout << "Not Charging";
                    break;
                case syst::battery_t::status_t::full:
                    std::cout << "Full";
                    break;
            }
            std::cout << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto charge = device.charge();
        if (charge.has_value()) {
            std::cout << "Charge: " << charge.value() << "%" << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto capacity = device.capacity();
        if (capacity.has_value()) {
            std::cout << "Capacity: " << capacity.value() << "%" << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        std::cout << std::endl;
    }
}
