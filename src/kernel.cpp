// Standard includes
#include <cstring>

// External includes
#include <sys/utsname.h>

// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

namespace syst {

std::optional<std::string> get_running_kernel() {
    utsname utsname_info{};

    if (uname(&utsname_info) != 0) {
        int err = errno;
        syst::error = std::string{ "uname(): " } + std::strerror(err) + '\n';
        return std::nullopt;
    }

    return utsname_info.release;
}

std::optional<std::list<std::string>> get_installed_kernels() {
    std::list<std::string> installed_kernels;

    const fs::path modules_path = "/usr/lib/modules";
    if (! fs::is_directory(modules_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + modules_path.string() + "'";
        return std::nullopt;
    }

    for (const fs::directory_entry& release :
      fs::directory_iterator(modules_path)) {
        if (! release.is_directory()) {
            continue;
        }
        installed_kernels.push_back(release.path().filename());
    }

    return installed_kernels;
}

} // namespace syst
