// Standard includes
#include <string_view>

// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

battery_t::battery_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

std::optional<std::list<battery_t>> battery_t::all() {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    const std::string_view power_supply_path = "/sys/class/power_supply";

    if (! fs::is_directory(power_supply_path)) {
        return std::nullopt;
    }

    std::list<battery_t> batteries;

    for (const fs::directory_entry& battery :
      fs::directory_iterator(power_supply_path)) {
        auto type = get_first_line(battery.path() / "type");
        if (! type.has_value()) {
            return std::nullopt;
        }
        if (type.value() != "Battery") {
            // Ignore power supply devices that are not batteries.
            continue;
        }

        batteries.push_back(battery_t{ battery });
    }

    return batteries;
}

fs::path battery_t::sysfs_path() const {
    return this->sysfs_path_;
}

std::optional<battery_status_t> battery_t::status() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto status = get_first_line(this->sysfs_path_ / "status");
    if (! status.has_value()) {
        return std::nullopt;
    }

    if (status.value() == "Unknown") {
        return battery_status_t::unknown;
    }
    if (status.value() == "Charging") {
        return battery_status_t::charging;
    }
    if (status.value() == "Discharging") {
        return battery_status_t::discharging;
    }
    if (status.value() == "Not charging") {
        return battery_status_t::not_charging;
    }
    if (status.value() == "Full") {
        return battery_status_t::full;
    }

    // The given status is invalid.
    return std::nullopt;
}

std::optional<double> battery_t::charge() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_now = get_int(this->sysfs_path_ / "energy_now");
    if (! energy_now.has_value()) {
        return std::nullopt;
    }

    auto energy_full = get_int(this->sysfs_path_ / "energy_full");
    if (! energy_full.has_value()) {
        return std::nullopt;
    }

    return ratio_to_percent(energy_now.value(), energy_full.value());
}

std::optional<double> battery_t::capacity() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_full = get_int(this->sysfs_path_ / "energy_full");
    if (! energy_full.has_value()) {
        return std::nullopt;
    }

    auto energy_full_design = get_int(this->sysfs_path_ / "energy_full_design");
    if (! energy_full_design.has_value()) {
        return std::nullopt;
    }

    return ratio_to_percent(energy_full.value(), energy_full_design.value());
}

} // namespace syst
