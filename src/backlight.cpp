// Standard includes
#include <algorithm>

// Local includes
#include "../system_state/system_state.hpp"
#include "util.hpp"

namespace syst {

backlight_t::backlight_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

res::optional_t<std::vector<backlight_t>> get_backlights() {
    // documentation for /sys/class/backlight
    //     https://www.kernel.org/doc/html/latest/gpu/backlight.html

    const std::string backlights_path = "/sys/class/backlight";

    if (! fs::is_directory(backlights_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + backlights_path + "'");
    }

    std::vector<backlight_t> backlights;

    for (const fs::directory_entry& backlight :
      fs::directory_iterator(backlights_path)) {
        if (! fs::is_directory(backlight)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(backlight)) {
            // Ignore paths that are not symbolic links.
            continue;
        }

        backlights.push_back(backlight_t{ backlight });
    }

    return backlights;
}

fs::path backlight_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

std::string backlight_t::get_name() const {
    return this->sysfs_path_.filename();
}

res::optional_t<double> backlight_t::get_brightness() const {
    auto brightness = syst::get_int(this->sysfs_path_ / "brightness");
    if (brightness.has_error()) {
        return RES_ERROR(brightness.error(),
          "The 'brightness' file is required to calculate the brightness "
          "percentage of a backlight.");
    }

    auto max_brightness = syst::get_int(this->sysfs_path_ / "max_brightness");
    if (max_brightness.has_error()) {
        return RES_ERROR(max_brightness.error(),
          "The 'max_brightness' file is required to calculate the brightness "
          "percentage of a backlight.");
    }

    return syst::value_to_percent(
      static_cast<uint64_t>(0), max_brightness.value(), brightness.value());
}

res::result_t backlight_t::set_brightness(double brightness) {
    double clamped_brightness = std::clamp(
      brightness, static_cast<double>(0.F), static_cast<double>(100.F));

    auto max_brightness = syst::get_int(this->sysfs_path_ / "max_brightness");
    if (max_brightness.has_error()) {
        return RES_ERROR(max_brightness.error(),
          "The 'max_brightness' file is required to set the brightness "
          "percentage of a backlight.");
    }

    uint64_t value = syst::percent_to_value(
      static_cast<uint64_t>(0), max_brightness.value(), clamped_brightness);

    auto result = syst::write_int(this->sysfs_path_ / "brightness", value);
    if (result.failure()) {
        return RES_TRACE(result.error());
    }

    return res::success;
}

res::result_t backlight_t::set_brightness_relative(double brightness) {
    auto old_brightness = this->get_brightness();
    if (old_brightness.has_error()) {
        return RES_TRACE(old_brightness.error());
    }

    auto result = this->set_brightness(old_brightness.value() + brightness);
    if (result.failure()) {
        return RES_TRACE(result.error());
    }

    return res::success;
}

} // namespace syst
