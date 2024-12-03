// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"
#include <filesystem>

namespace syst {

network_interface_t::network_interface_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

std::optional<std::list<network_interface_t>> network_interface_t::all() {
    const std::string_view net_path = "/sys/class/net";

    if (! fs::is_directory(net_path)) {
        return std::nullopt;
    }

    std::list<network_interface_t> network_interfaces;

    for (const fs::directory_entry& interface :
      fs::directory_iterator(net_path)) {
        if (! fs::is_symlink(interface)) {
            return std::nullopt;
        }

        network_interfaces.push_back(network_interface_t{ interface });
    }

    return network_interfaces;
}

fs::path network_interface_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::optional<bool> network_interface_t::physical() const {
    fs::path real_path;
    try {
        real_path = fs::read_symlink(this->sysfs_path_);
    } catch (const fs::filesystem_error&) {
        return std::nullopt;
    }

    return has_prefix(real_path.string(), "/sys/devices/virtual");
}

std::optional<bool> network_interface_t::loopback() const {
    // documentation for /sys/class/net/<dev>/type
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if_arp.h

    auto type = get_int(this->sysfs_path_ / "type");
    if (! type.has_value()) {
        return std::nullopt;
    }

    const uint64_t loopback_type = 772;
    return type.value() == loopback_type;
}

std::optional<network_interface_t::status_t> network_interface_t::status()
  const {
    // documentation for /sys/class/net/:
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if.h

    auto status = get_first_line(this->sysfs_path_ / "operstate");
    if (! status.has_value()) {
        return std::nullopt;
    }

    if (status.value() == "unknown") {
        return status_t::unknown;
    }
    if (status.value() == "up") {
        return status_t::up;
    }
    if (status.value() == "dormant") {
        return status_t::dormant;
    }
    if (status.value() == "down") {
        return status_t::down;
    }

    // The given status is invalid.
    return std::nullopt;
}

} // namespace syst