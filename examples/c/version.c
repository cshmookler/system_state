// Standard includes
#include <stdio.h>

// External includes
#include "../build/version.h"

int main() {
    printf("Compile-time version: %s\n", syst_compiletime_version);
    printf("Runtime version: %s\n", syst_get_runtime_version());
}
