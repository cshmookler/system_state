// Standard includes
#include <cstring>

// External includes
#include <sys/utsname.h>

// Local includes
#include "../system_state/system_state.hpp"
#include "strerror.hpp"

namespace syst {

res::optional_t<std::string> get_running_kernel() {
    utsname utsname_info{};

    if (uname(&utsname_info) != 0) {
        int err = errno;
        return RES_NEW_ERROR(std::string{ "uname(): " } + syst::strerror(err));
    }

    return std::string{ static_cast<char*>(utsname_info.release) };
}

res::optional_t<std::vector<std::string>> get_installed_kernels() {
    std::vector<std::string> installed_kernels;

    const fs::path modules_path = "/usr/lib/modules";
    if (! fs::is_directory(modules_path)) {
        return RES_NEW_ERROR("The path is not a directory.\n\tpath: '"
          + modules_path.string() + "'");
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
