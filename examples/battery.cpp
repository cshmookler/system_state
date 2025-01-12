// Standard includes
#include <iomanip>
#include <iostream>

// External includes
#include "../system_state/core.hpp"

int main() {
    auto batteries = syst::battery_t::all();
    if (batteries.has_error()) {
        std::cerr << batteries.error() << std::endl;
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
            std::cerr << status.error() << std::endl;
        }

        auto current = device.current();
        if (current.has_value()) {
            std::cout << "Current: " << current.value() << " A" << std::endl;
        } else {
            std::cerr << current.error() << std::endl;
        }

        auto power = device.power();
        if (power.has_value()) {
            std::cout << "Power: " << power.value() << " W" << std::endl;
        } else {
            std::cerr << power.error() << std::endl;
        }

        auto charge = device.charge();
        if (charge.has_value()) {
            std::cout << "Charge: " << charge.value() << "%" << std::endl;
        } else {
            std::cerr << charge.error() << std::endl;
        }

        auto capacity = device.capacity();
        if (capacity.has_value()) {
            std::cout << "Capacity: " << capacity.value() << "%" << std::endl;
        } else {
            std::cerr << capacity.error() << std::endl;
        }

        auto time_remaining = device.time_remaining();
        if (time_remaining.has_value()) {
            const uint64_t hours = time_remaining->count() / 3600;
            const uint64_t minutes = (time_remaining->count() % 3600) / 60;
            const uint64_t seconds =
              ((time_remaining->count() % 3600) % 60) / 60;
            std::cout << std::setfill('0');
            std::cout << "Time Remaining: " << std::setw(2) << hours << ":"
                      << std::setw(2) << minutes << ":" << std::setw(2)
                      << seconds << std::endl;
            std::cout << std::setfill(' ');
        } else {
            std::cerr << time_remaining.error() << std::endl;
        }

        std::cout << std::endl;
    }
}
