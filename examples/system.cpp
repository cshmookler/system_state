// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto system_info = syst::system_info();
    if (! system_info.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }

    std::cout << "Uptime: " << system_info->uptime.count() << "s" << std::endl;
    std::cout << "1 minute load: " << system_info->load_1 << std::endl;
    std::cout << "5 minute load: " << system_info->load_5 << std::endl;
    std::cout << "15 minute load: " << system_info->load_15 << std::endl;
    std::cout << "RAM total: " << system_info->ram_total << " bytes"
              << std::endl;
    std::cout << "RAM free: " << system_info->ram_free << " bytes" << std::endl;
    std::cout << "RAM shared: " << system_info->ram_shared << " bytes"
              << std::endl;
    std::cout << "RAM buffered: " << system_info->ram_buffered << " bytes"
              << std::endl;
    std::cout << "SWAP total: " << system_info->swap_total << " bytes"
              << std::endl;
    std::cout << "SWAP free: " << system_info->swap_free << " bytes"
              << std::endl;
    std::cout << "Processes: " << system_info->procs << std::endl;
    std::cout << "RAM usage: " << system_info->ram_usage << "%" << std::endl;
    std::cout << "SWAP usage: " << system_info->swap_usage << "%" << std::endl;
}
