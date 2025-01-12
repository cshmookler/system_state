// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

thermal_zone_t::thermal_zone_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

syst::optional_t<std::list<thermal_zone_t>> thermal_zone_t::all() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        return SYST_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + thermal_path + "'");
    }

    std::list<thermal_zone_t> thermal_zones;

    for (const fs::directory_entry& zone :
      fs::directory_iterator(thermal_path)) {
        if (! fs::is_directory(zone)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(zone)) {
            // Ignore paths that are not symbolic links.
            continue;
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

syst::optional_t<std::string> thermal_zone_t::type() const {
    auto type = syst::get_first_line(this->sysfs_path_ / "type");

    if (type.has_error()) {
        return SYST_TRACE(type.error());
    }

    return type;
}

syst::optional_t<double> thermal_zone_t::temperature() const {
    auto temp_millicelsius = syst::get_int(this->sysfs_path_ / "temp");
    if (temp_millicelsius.has_error()) {
        return SYST_TRACE(temp_millicelsius.error());
    }

    const double millicelsius_per_celsius = static_cast<double>(1e3);
    double temp_celsius =
      static_cast<double>(temp_millicelsius.value()) / millicelsius_per_celsius;

    return temp_celsius;
}

cooling_device_t::cooling_device_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

syst::optional_t<std::list<cooling_device_t>> cooling_device_t::all() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        return SYST_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + thermal_path + "'");
    }

    std::list<cooling_device_t> cooling_devices;

    for (const fs::directory_entry& device :
      fs::directory_iterator(thermal_path)) {
        if (! fs::is_directory(device)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(device)) {
            // Ignore paths that are not symbolic links.
            continue;
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

syst::optional_t<std::string> cooling_device_t::type() const {
    auto type = syst::get_first_line(this->sysfs_path_ / "type");

    if (type.has_error()) {
        return SYST_TRACE(type.error());
    }

    return type;
}

syst::optional_t<double> cooling_device_t::get_state() const {
    auto current_state = syst::get_int(this->sysfs_path_ / "cur_state");
    if (current_state.has_error()) {
        return SYST_TRACE(current_state.error());
    }

    auto maximum_state = syst::get_int(this->sysfs_path_ / "max_state");
    if (maximum_state.has_error()) {
        return SYST_TRACE(maximum_state.error());
    }

    return ratio_to_percent(current_state.value(), maximum_state.value());
}

result_t cooling_device_t::set_state(double state) {
    auto maximum_state = get_int(this->sysfs_path_ / "max_state");
    if (maximum_state.has_error()) {
        return SYST_TRACE(maximum_state.error());
    }

    if (state < static_cast<double>(0) || state > static_cast<double>(100)) {
        return SYST_NEW_ERROR(
          "The new state given for a cooling device is out of "
          "bounds.\n\tdevice path: '"
          + this->sysfs_path_.string() + "'\n\tstate: '" + std::to_string(state)
          + "'");
    }

    const uint64_t current_state =
      percent_to_value(static_cast<uint64_t>(0), maximum_state.value(), state);

    result_t result = write_int(this->sysfs_path_ / "cur_state", current_state);
    if (result.failure()) {
        return SYST_TRACE(result.error());
    }

    return syst::success;
}

} // namespace syst
