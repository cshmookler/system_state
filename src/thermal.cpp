// Standard includes
#include <algorithm>

// Local includes
#include "../system_state/system_state.hpp"
#include "util.hpp"

namespace syst {

thermal_zone_t::thermal_zone_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

res::optional_t<std::list<thermal_zone_t>> get_thermal_zones() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        return RES_NEW_ERROR(
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

fs::path thermal_zone_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

res::optional_t<std::string> thermal_zone_t::get_type() const {
    auto type = syst::get_first_line(this->sysfs_path_ / "type");

    if (type.has_error()) {
        return RES_TRACE(type.error());
    }

    return type;
}

res::optional_t<double> thermal_zone_t::get_temperature() const {
    auto temp_millicelsius = syst::get_int(this->sysfs_path_ / "temp");
    if (temp_millicelsius.has_error()) {
        return RES_TRACE(temp_millicelsius.error());
    }

    const double millicelsius_per_celsius = 1e3;
    double temp_celsius =
      static_cast<double>(temp_millicelsius.value()) / millicelsius_per_celsius;

    return temp_celsius;
}

cooling_device_t::cooling_device_t(const fs::path& sysfs_path)
: sysfs_path_(sysfs_path) {
}

res::optional_t<std::list<cooling_device_t>> get_cooling_devices() {
    // documentation for /sys/class/thermal
    //     https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-thermal
    //     https://www.kernel.org/doc/html/latest/driver-api/thermal/sysfs-api.html

    const std::string thermal_path = "/sys/class/thermal";

    if (! fs::is_directory(thermal_path)) {
        return RES_NEW_ERROR(
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

fs::path cooling_device_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

res::optional_t<std::string> cooling_device_t::get_type() const {
    auto type = syst::get_first_line(this->sysfs_path_ / "type");

    if (type.has_error()) {
        return RES_TRACE(type.error());
    }

    return type;
}

res::optional_t<double> cooling_device_t::get_state() const {
    auto current_state = syst::get_int(this->sysfs_path_ / "cur_state");
    if (current_state.has_error()) {
        return RES_TRACE(current_state.error());
    }

    auto maximum_state = syst::get_int(this->sysfs_path_ / "max_state");
    if (maximum_state.has_error()) {
        return RES_TRACE(maximum_state.error());
    }

    return ratio_to_percent(current_state.value(), maximum_state.value());
}

res::result_t cooling_device_t::set_state(double state) {
    double clamped_state =
      std::clamp(state, static_cast<double>(0.F), static_cast<double>(100.F));

    auto maximum_state = get_int(this->sysfs_path_ / "max_state");
    if (maximum_state.has_error()) {
        return RES_TRACE(maximum_state.error());
    }

    const uint64_t current_state = percent_to_value(
      static_cast<uint64_t>(0), maximum_state.value(), clamped_state);

    res::result_t result =
      write_int(this->sysfs_path_ / "cur_state", current_state);
    if (result.failure()) {
        return RES_TRACE(result.error());
    }

    return res::success;
}

} // namespace syst
