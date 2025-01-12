// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"

int main() {
    auto network_interfaces = syst::network_interface_t::all();
    if (network_interfaces.has_error()) {
        std::cerr << network_interfaces.error() << std::endl;
        return 1;
    }

    std::cout << std::boolalpha; // Pretty print boolean values.

    for (const auto& interface : network_interfaces.value()) {
        std::cout << "sysfs path: " << interface.sysfs_path() << std::endl;
        std::cout << "Name: " << interface.name() << std::endl;

        auto physical = interface.physical();
        if (physical.has_value()) {
            std::cout << "Physical: " << physical.value() << std::endl;
        } else {
            std::cerr << physical.error() << std::endl;
        }

        auto loopback = interface.loopback();
        if (loopback.has_value()) {
            std::cout << "Loopback: " << loopback.value() << std::endl;
        } else {
            std::cerr << loopback.error() << std::endl;
        }

        auto status = interface.status();
        if (status.has_value()) {
            std::cout << "Status: ";
            switch (status.value()) {
                case syst::network_interface_t::status_t::unknown:
                    std::cout << "Unknown";
                    break;
                case syst::network_interface_t::status_t::up:
                    std::cout << "Up";
                    break;
                case syst::network_interface_t::status_t::dormant:
                    std::cout << "Dormant";
                    break;
                case syst::network_interface_t::status_t::down:
                    std::cout << "Down";
                    break;
            }
            std::cout << std::endl;
        } else {
            std::cerr << status.error() << std::endl;
        }

        auto stat = interface.stat();
        if (stat.has_value()) {
            std::cout << "Down: " << stat->bytes_down << " bytes ("
                      << stat->packets_down << " packets)" << std::endl;
            std::cout << "Up: " << stat->bytes_up << " bytes ("
                      << stat->packets_up << " packets)" << std::endl;
        } else {
            std::cerr << stat.error() << std::endl;
        }

        std::cout << std::endl;
    }
}
