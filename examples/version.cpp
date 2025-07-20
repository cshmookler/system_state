// Standard includes
#include <iostream>

// External includes
#include "../build/version.h"

int main() {
    std::cout << "Compile-time version: " << syst_compiletime_version << '\n';
    std::cout << "Runtime version: " << syst_get_runtime_version() << '\n';
}
