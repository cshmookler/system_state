// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

network_interface_t::network_interface_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

std::optional<std::list<network_interface_t>> network_interface_t::all() {
    const std::string net_path = "/sys/class/net";

    if (! fs::is_directory(net_path)) {
        syst::error = "The path is not a directory.\npath: '" + net_path + "'";
        return std::nullopt;
    }

    std::list<network_interface_t> network_interfaces;

    for (const fs::directory_entry& interface :
      fs::directory_iterator(net_path)) {
        if (! fs::is_symlink(interface)) {
            syst::error = "The path is not a symbolic link.\npath: '"
              + interface.path().string() + "'";
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
        syst::error = "Failed to read the symlink at a path corresponding to a "
                      "sysfs device.\npath: '"
          + this->sysfs_path_.string() + "'";
        return std::nullopt;
    }

    return has_prefix(real_path.string(), "/sys/devices/virtual");
}

std::optional<bool> network_interface_t::loopback() const {
    // documentation for /sys/class/net/<dev>/type
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if_arp.h

    const auto type = get_int(this->sysfs_path_ / "type");
    if (! type.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    const uint64_t loopback_type = 772;
    return type.value() == loopback_type;
}

std::optional<network_interface_t::status_t> network_interface_t::status()
  const {
    // documentation for /sys/class/net/<dev>/operstate:
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if.h

    const fs::path status_path = this->sysfs_path_ / "operstate";
    const auto status = get_first_line(status_path);
    if (! status.has_value()) {
        // get_first_line sets syst::error
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

    syst::error = "An invalid status was read from a network interface status "
                  "file.\nstatus: '"
      + status.value() + "'\nfile: '" + status_path.string() + "'";
    return std::nullopt;
}

std::optional<network_interface_t::stat_t> network_interface_t::stat() const {
    const fs::path stat_path = this->sysfs_path_ / "statistics";
    if (! fs::is_directory(stat_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + stat_path.string() + "'";
        return std::nullopt;
    }

    stat_t stat{};

    const auto rx_bytes = get_int(stat_path / "rx_bytes");
    if (! rx_bytes.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }
    stat.bytes_down = rx_bytes.value();

    const auto tx_bytes = get_int(stat_path / "tx_bytes");
    if (! tx_bytes.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }
    stat.bytes_up = tx_bytes.value();

    const auto rx_packets = get_int(stat_path / "rx_packets");
    if (! rx_packets.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }
    stat.packets_down = rx_packets.value();

    const auto tx_packets = get_int(stat_path / "tx_packets");
    if (! tx_packets.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }
    stat.packets_up = tx_packets.value();

    return stat;
}

} // namespace syst
