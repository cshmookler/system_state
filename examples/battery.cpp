// Standard includes
#include <iomanip>
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int main() {
    auto batteries = syst::get_batteries();
    if (batteries.has_error()) {
        std::cerr << batteries.error().string() << '\n';
        return 1;
    }

    for (const auto& device : batteries.value()) {
        std::cout << "sysfs path: " << device.get_sysfs_path() << '\n';
        std::cout << "Name: " << device.get_name() << '\n';

        auto status = device.get_status();
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
            std::cout << '\n';
        } else {
            std::cerr << status.error().string() << '\n';
        }

        auto current = device.get_current();
        if (current.has_value()) {
            std::cout << "Current: " << current.value() << " A" << '\n';
        } else {
            std::cerr << current.error().string() << '\n';
        }

        auto power = device.get_power();
        if (power.has_value()) {
            std::cout << "Power: " << power.value() << " W" << '\n';
        } else {
            std::cerr << power.error().string() << '\n';
        }

        auto charge = device.get_charge();
        if (charge.has_value()) {
            std::cout << "Charge: " << charge.value() << "%" << '\n';
        } else {
            std::cerr << charge.error().string() << '\n';
        }

        auto capacity = device.get_capacity();
        if (capacity.has_value()) {
            std::cout << "Capacity: " << capacity.value() << "%" << '\n';
        } else {
            std::cerr << capacity.error().string() << '\n';
        }

        auto time_remaining = device.get_time_remaining();
        if (time_remaining.has_value()) {
            const uint64_t hours = time_remaining->count() / 3600;
            const uint64_t minutes = (time_remaining->count() % 3600) / 60;
            const uint64_t seconds =
              ((time_remaining->count() % 3600) % 60) / 60;
            std::cout << std::setfill('0');
            std::cout << "Time Remaining: " << std::setw(2) << hours << ":"
                      << std::setw(2) << minutes << ":" << std::setw(2)
                      << seconds << '\n';
            std::cout << std::setfill(' ');
        } else {
            std::cerr << time_remaining.error().string() << '\n';
        }

        std::cout << '\n';
    }
}
