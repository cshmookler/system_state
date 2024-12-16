// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto running_version = syst::get_running_kernel();
    if (running_version.has_value()) {
        std::cout << "Running Version: " << running_version.value()
                  << std::endl;
    } else {
        std::cout << syst::error << std::endl;
    }

    auto installed_versions = syst::get_installed_kernels();
    if (installed_versions.has_value()) {
        std::cout << "Installed Versions:\n";
        for (const auto& version : installed_versions.value()) {
            std::cout << "\t" << version << std::endl;
        }
    } else {
        std::cout << syst::error << std::endl;
    }
}
