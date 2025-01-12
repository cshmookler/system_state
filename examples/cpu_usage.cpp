// Standard includes
#include <chrono>
#include <iostream>
#include <thread>

// External includes
#include "../system_state/core.hpp"

int main() {
    syst::cpu_usage_t usage;
    auto result = usage.update();
    if (result.failure()) {
        std::cerr << result.error() << std::endl;
        return 1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    result = usage.update();
    if (result.failure()) {
        std::cerr << result.error() << std::endl;
        return 1;
    }

    auto total_usage = usage.get_total();
    if (total_usage.has_error()) {
        std::cerr << total_usage.error() << std::endl;
        return 1;
    }
    std::cout << "Total CPU usage: " << total_usage.value() << "%" << std::endl;

    auto core_usages = usage.get_per_core();
    if (core_usages.has_error()) {
        std::cerr << core_usages.error() << std::endl;
        return 1;
    }
    std::cout << "CPU usage per core:\n";
    for (auto usage : core_usages.value()) {
        std::cout << "\t" << usage << "%" << std::endl;
    }
}
