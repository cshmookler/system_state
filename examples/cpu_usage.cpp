// Standard includes
#include <chrono>
#include <iostream>
#include <thread>

// External includes
#include <system_state/core.hpp>
#include <system_state/error.hpp>

int main() {
    syst::cpu_usage_t usage;
    if (! usage.update()) {
        std::cout << syst::error << std::endl;
        return 1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (! usage.update()) {
        std::cout << syst::error << std::endl;
        return 1;
    }

    auto total_usage = usage.get_total();
    if (! total_usage.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }
    std::cout << "Total CPU usage: " << total_usage.value() << "%" << std::endl;
    auto core_usages = usage.get_per_core();
    if (! core_usages.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }
    std::cout << "CPU usage per core:\n";
    for (auto usage : core_usages.value()) {
        std::cout << "\t" << usage << "%" << std::endl;
    }
}
