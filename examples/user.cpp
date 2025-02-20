// Standard includes
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int main() {
    auto username = syst::get_username();
    if (username.has_error()) {
        std::cerr << username.error().string() << '\n';
        return 1;
    }
    std::cout << "Username: " << username.value() << '\n';
}
