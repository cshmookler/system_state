// Local includes
#include "../build/version.hpp"

namespace syst {

const char* get_runtime_version() {
    return ::syst::compiletime_version;
}

} // namespace syst