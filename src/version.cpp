// Local includes
#include "../build/version.h"

extern "C" {

const char* syst_get_runtime_version() {
    return syst_compiletime_version;
}

} // extern "C"
