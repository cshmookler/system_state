// Standard includes
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto username = syst::username();
    if (! username.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }
    std::cout << "Username: " << username.value() << std::endl;
}
