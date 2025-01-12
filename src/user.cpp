// External includes
#include <cstring>
#include <pwd.h>
#include <unistd.h>

// Local includes
#include "../system_state/core.hpp"

namespace syst {

syst::optional_t<std::string> username() {
    auto uid = geteuid();
    struct passwd* passwd_info = getpwuid(uid);

    if (passwd_info == nullptr) {
        int err = errno;
        return SYST_NEW_ERROR(
          "Failed to get passwd information from 'getpwuid'.\nreason: '"
          + std::string{ strerror(err) } + "'");
    }

    return std::string{ passwd_info->pw_name };
}

} // namespace syst
