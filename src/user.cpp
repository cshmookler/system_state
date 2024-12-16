// External includes
#include <cstring>
#include <pwd.h>
#include <unistd.h>

// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

namespace syst {

std::optional<std::string> username() {
    auto uid = geteuid();
    struct passwd* passwd_info = getpwuid(uid);

    if (passwd_info == nullptr) {
        syst::error =
          "Failed to get passwd information from 'getpwuid'.\nreason: '"
          + std::string{ strerror(errno) } + "'";
        return std::nullopt;
    }

    return passwd_info->pw_name;
}

} // namespace syst
