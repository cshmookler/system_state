// Standard includes
#include <iostream>

// External includes
#include "../build/version.hpp"

int main() {
    std::cout << "Compile-time version: " << syst::compiletime_version
              << std::endl;
    std::cout << "Runtime version: " << syst::get_runtime_version()
              << std::endl;
}
