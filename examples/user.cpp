// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"

int main() {
    auto username = syst::username();
    if (username.has_error()) {
        std::cerr << username.error() << std::endl;
        return 1;
    }
    std::cout << "Username: " << username.value() << std::endl;
}
