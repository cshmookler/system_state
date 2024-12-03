// Standard includes
#include <string_view>

// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

backlight_t::backlight_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

std::optional<std::list<backlight_t>> backlight_t::all() {
    // documentation for /sys/class/backlight
    //     https://www.kernel.org/doc/html/latest/gpu/backlight.html

    const std::string_view backlights_path = "/sys/class/backlight";

    if (! fs::is_directory(backlights_path)) {
        return std::nullopt;
    }

    std::list<backlight_t> backlights;

    for (const fs::directory_entry& backlight :
      fs::directory_iterator(backlights_path)) {
        backlights.push_back(backlight_t{ backlight });
    }

    return backlights;
}

fs::path backlight_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::optional<double> backlight_t::brightness() const {
    auto brightness = get_int(this->sysfs_path_ / "brightness");
    if (! brightness.has_value()) {
        return std::nullopt;
    }

    auto max_brightness = get_int(this->sysfs_path_ / "max_brightness");
    if (! max_brightness.has_value()) {
        return std::nullopt;
    }

    return ratio_to_percent(brightness.value(), max_brightness.value());
}

} // namespace syst
