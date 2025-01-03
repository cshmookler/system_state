// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

thermal_zone_t::thermal_zone_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

std::optional<std::list<thermal_zone_t>> thermal_zone_t::all() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + thermal_path + "'";
        return std::nullopt;
    }

    std::list<thermal_zone_t> thermal_zones;

    for (const fs::directory_entry& zone :
      fs::directory_iterator(thermal_path)) {
        if (! fs::is_symlink(zone)) {
            syst::error = "The path is not a symbolic link.\npath: '"
              + zone.path().string() + "'";
            return std::nullopt;
        }

        const std::string thermal_zone_prefix = "thermal_zone";
        if (! syst::has_prefix(zone.path().filename(), thermal_zone_prefix)) {
            // Ignore devices that are not thermal zones.
            continue;
        }

        thermal_zones.push_back(thermal_zone_t{ zone });
    }

    return thermal_zones;
}

fs::path thermal_zone_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::optional<std::string> thermal_zone_t::type() const {
    return get_first_line(this->sysfs_path_ / "type");
}

std::optional<double> thermal_zone_t::temperature() const {
    const auto temperature = get_int(this->sysfs_path_ / "temp");
    if (! temperature.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return static_cast<double>(temperature.value()) / static_cast<double>(1e3);
}

cooling_device_t::cooling_device_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

std::optional<std::list<cooling_device_t>> cooling_device_t::all() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + thermal_path + "'";
        return std::nullopt;
    }

    std::list<cooling_device_t> cooling_devices;

    for (const fs::directory_entry& device :
      fs::directory_iterator(thermal_path)) {
        if (! fs::is_symlink(device)) {
            syst::error = "The path is not a symbolic link.\npath: '"
              + device.path().string() + "'";
            return std::nullopt;
        }

        const std::string cooling_device_prefix = "cooling_device";
        if (! syst::has_prefix(
              device.path().filename(), cooling_device_prefix)) {
            // Ignore devices that are not cooling devices.
            continue;
        }

        cooling_devices.push_back(cooling_device_t{ device });
    }

    return cooling_devices;
}

fs::path cooling_device_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::optional<std::string> cooling_device_t::type() const {
    return get_first_line(this->sysfs_path_ / "type");
}

std::optional<double> cooling_device_t::get_state() const {
    const auto current_state = get_int(this->sysfs_path_ / "cur_state");
    if (! current_state.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    const auto maximum_state = get_int(this->sysfs_path_ / "max_state");
    if (! maximum_state.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return ratio_to_percent(current_state.value(), maximum_state.value());
}

bool cooling_device_t::set_state(double state) const {
    const auto maximum_state = get_int(this->sysfs_path_ / "max_state");
    if (! maximum_state.has_value()) {
        // get_int sets syst::error
        return false;
    }

    if (state < static_cast<double>(0) || state > static_cast<double>(100)) {
        syst::error = "The new state given for a cooling device is out of "
                      "bounds.\ndevice path: '"
          + this->sysfs_path_.string() + "'\nstate: '" + std::to_string(state)
          + "'";
        return false;
    }

    const uint64_t current_state =
      percent_to_value(static_cast<uint64_t>(0), maximum_state.value(), state);

    return write_int(this->sysfs_path_ / "cur_state", current_state);
}

} // namespace syst
