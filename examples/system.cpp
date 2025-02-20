// Standard includes
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int main() {
    auto system_info = syst::get_system_info();
    if (system_info.has_error()) {
        std::cerr << system_info.error().string() << '\n';
        return 1;
    }

    std::cout << "Uptime: " << system_info->uptime.count() << "s" << '\n';
    std::cout << "1 minute load: " << system_info->load_1 << '\n';
    std::cout << "5 minute load: " << system_info->load_5 << '\n';
    std::cout << "15 minute load: " << system_info->load_15 << '\n';
    std::cout << "RAM total: " << system_info->ram_total << " bytes" << '\n';
    std::cout << "RAM free: " << system_info->ram_free << " bytes" << '\n';
    std::cout << "RAM shared: " << system_info->ram_shared << " bytes" << '\n';
    std::cout << "RAM buffered: " << system_info->ram_buffered << " bytes"
              << '\n';
    std::cout << "SWAP total: " << system_info->swap_total << " bytes" << '\n';
    std::cout << "SWAP free: " << system_info->swap_free << " bytes" << '\n';
    std::cout << "Processes: " << system_info->procs << '\n';
    std::cout << "RAM usage: " << system_info->ram_usage << "%" << '\n';
    std::cout << "SWAP usage: " << system_info->swap_usage << "%" << '\n';
}
