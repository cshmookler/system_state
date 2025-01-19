// Standard includes
// Use the POSIX version of strerror_r instead of the GNU version.
#undef _GNU_SOURCE
// Use a recent version of the POSIX standard so strerror_r is supported.
// https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html#index-_005fPOSIX_005fC_005fSOURCE
#define _POSIX_C_SOURCE 200809L
#include <cstring>

// Local includes
#include "strerror.hpp"

namespace syst {

const char* strerror(int errnum) {
    int strerror_errnum = strerror_r(errnum, nullptr, 0);
}

} // namespace syst
