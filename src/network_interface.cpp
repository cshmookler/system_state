// Local includes
#include "../system_state/system_state.hpp"
#include "util.hpp"

namespace syst {

network_interface_t::network_interface_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

res::optional_t<std::list<network_interface_t>> get_network_interfaces() {
    const std::string net_path = "/sys/class/net";

    if (! fs::is_directory(net_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + net_path + "'");
    }

    std::list<network_interface_t> network_interfaces;

    for (const fs::directory_entry& interface :
      fs::directory_iterator(net_path)) {
        if (! fs::is_directory(interface)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(interface)) {
            // Ignore paths that are not symbolic links.
            continue;
        }

        network_interfaces.push_back(network_interface_t{ interface });
    }

    return network_interfaces;
}

fs::path network_interface_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

std::string network_interface_t::get_name() const {
    return this->sysfs_path_.filename();
}

res::optional_t<bool> network_interface_t::is_physical() const {
    fs::path real_path;
    try {
        real_path = fs::read_symlink(this->sysfs_path_);
    } catch (const fs::filesystem_error&) {
        return RES_NEW_ERROR("Failed to read the symlink at a path "
                             "corresponding to a sysfs device.\n\tpath: '"
          + this->sysfs_path_.string() + "'");
    }

    return ! syst::has_prefix(real_path.string(), "../../devices/virtual");
}

res::optional_t<bool> network_interface_t::is_loopback() const {
    // documentation for /sys/class/net/<dev>/type
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if_arp.h

    auto type = syst::get_int(this->sysfs_path_ / "type");
    if (type.has_error()) {
        return RES_TRACE(type.error());
    }

    const uint64_t loopback_type = 772;
    return type.value() == loopback_type;
}

res::optional_t<network_interface_t::status_t> network_interface_t::get_status()
  const {
    // documentation for /sys/class/net/<dev>/operstate:
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/if.h

    const fs::path status_path = this->sysfs_path_ / "operstate";
    const auto status = syst::get_first_line(status_path);
    if (status.has_error()) {
        return RES_TRACE(status.error());
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

    return RES_NEW_ERROR("An invalid status was read from a network interface "
                         "status file.\n\tstatus: '"
      + status.value() + "'\n\tfile: '" + status_path.string() + "'");
}

res::optional_t<network_interface_t::stat_t> network_interface_t::get_stat()
  const {
    const fs::path stat_path = this->sysfs_path_ / "statistics";
    if (! fs::is_directory(stat_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + stat_path.string() + "'");
    }

    stat_t stat{};

    auto rx_bytes = syst::get_int(stat_path / "rx_bytes");
    if (rx_bytes.has_error()) {
        return RES_TRACE(rx_bytes.error());
    }
    stat.bytes_down = rx_bytes.value();

    auto tx_bytes = syst::get_int(stat_path / "tx_bytes");
    if (tx_bytes.has_error()) {
        return RES_TRACE(tx_bytes.error());
    }
    stat.bytes_up = tx_bytes.value();

    auto rx_packets = syst::get_int(stat_path / "rx_packets");
    if (rx_packets.has_error()) {
        return RES_TRACE(rx_packets.error());
    }
    stat.packets_down = rx_packets.value();

    auto tx_packets = syst::get_int(stat_path / "tx_packets");
    if (tx_packets.has_error()) {
        return RES_TRACE(tx_packets.error());
    }
    stat.packets_up = tx_packets.value();

    return stat;
}

} // namespace syst
