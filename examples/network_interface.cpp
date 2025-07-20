// Standard includes
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int main() {
    auto network_interfaces = syst::get_network_interfaces();
    if (network_interfaces.has_error()) {
        std::cerr << network_interfaces.error().string() << '\n';
        return 1;
    }

    std::cout << std::boolalpha; // Pretty print boolean values.

    for (const auto& interface : network_interfaces.value()) {
        std::cout << "sysfs path: " << interface.get_sysfs_path().string()
                  << '\n';
        std::cout << "Name: " << interface.get_name() << '\n';

        auto physical = interface.is_physical();
        if (physical.has_value()) {
            std::cout << "Physical: " << physical.value() << '\n';
        } else {
            std::cerr << physical.error().string() << '\n';
        }

        auto loopback = interface.is_loopback();
        if (loopback.has_value()) {
            std::cout << "Loopback: " << loopback.value() << '\n';
        } else {
            std::cerr << loopback.error().string() << '\n';
        }

        auto status = interface.get_status();
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
            std::cout << '\n';
        } else {
            std::cerr << status.error().string() << '\n';
        }

        auto stat = interface.get_stat();
        if (stat.has_value()) {
            std::cout << "Down: " << stat->bytes_down << " bytes ("
                      << stat->packets_down << " packets)" << '\n';
            std::cout << "Up: " << stat->bytes_up << " bytes ("
                      << stat->packets_up << " packets)" << '\n';
        } else {
            std::cerr << stat.error().string() << '\n';
        }

        std::cout << '\n';
    }
}
