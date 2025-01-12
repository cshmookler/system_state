// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

backlight_t::backlight_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

syst::optional_t<std::list<backlight_t>> backlight_t::all() {
    // documentation for /sys/class/backlight
    //     https://www.kernel.org/doc/html/latest/gpu/backlight.html

    const std::string backlights_path = "/sys/class/backlight";

    if (! fs::is_directory(backlights_path)) {
        return SYST_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + backlights_path + "'");
    }

    std::list<backlight_t> backlights;

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

fs::path backlight_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::string backlight_t::name() const {
    return this->sysfs_path_.filename();
}

syst::optional_t<double> backlight_t::brightness() const {
    auto brightness = syst::get_int(this->sysfs_path_ / "brightness");
    if (brightness.has_error()) {
        return SYST_ERROR(brightness.error(),
          "The 'brightness' file is required to calculate the brightness "
          "percentage of a backlight.");
    }

    auto max_brightness = syst::get_int(this->sysfs_path_ / "max_brightness");
    if (max_brightness.has_error()) {
        return SYST_ERROR(max_brightness.error(),
          "The 'max_brightness' file is required to calculate the brightness "
          "percentage of a backlight.");
    }

    return syst::value_to_percent(
      static_cast<uint64_t>(0), max_brightness.value(), brightness.value());
}

} // namespace syst
