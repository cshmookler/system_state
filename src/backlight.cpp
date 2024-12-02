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
    const std::string_view brightness_filename = "brightness";
    fs::path brightness_path = this->sysfs_path_ / brightness_filename;
    if (! fs::is_regular_file(brightness_path)) {
        return std::nullopt;
    }
    std::optional<uint64_t> brightness = get_int(brightness_path);
    if (! brightness.has_value()) {
        return std::nullopt;
    }

    const std::string_view max_brightness_filename = "max_brightness";
    fs::path max_brightness_path = this->sysfs_path_ / max_brightness_filename;
    if (! fs::is_regular_file(max_brightness_path)) {
        return std::nullopt;
    }
    std::optional<uint64_t> max_brightness = get_int(max_brightness_path);
    if (! max_brightness.has_value()) {
        return std::nullopt;
    }

    return ratio_to_percent(brightness.value(), max_brightness.value());
}

} // namespace syst
