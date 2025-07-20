// Standard includes
#include <cstring>

// Local includes
#include "../../system_state/system_state.h"

extern "C" {

void syst_string_free(char* error) {
    free(error);
}

} // extern "C"
