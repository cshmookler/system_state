// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

[[nodiscard]] std::optional<double> energy(const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_now = get_int(sysfs_path / "energy_now");
    if (! energy_now.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    auto energy_empty = get_int(sysfs_path / "energy_empty");
    if (! energy_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_full = get_int(sysfs_path / "energy_full");
    if (! energy_full.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return ratio_to_percent(energy_now.value() - energy_empty.value(),
      energy_full.value() - energy_empty.value());
}

[[nodiscard]] std::optional<double> charge(const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto charge_now = get_int(sysfs_path / "charge_now");
    if (! charge_now.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    auto charge_empty = get_int(sysfs_path / "charge_empty");
    if (! charge_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_full = get_int(sysfs_path / "charge_full");
    if (! charge_full.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return ratio_to_percent(charge_now.value() - charge_empty.value(),
      charge_full.value() - charge_empty.value());
}

[[nodiscard]] std::optional<double> energy_capacity(
  const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_empty = get_int(sysfs_path / "energy_empty");
    if (! energy_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_full = get_int(sysfs_path / "energy_full");
    if (! energy_full.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    auto energy_empty_design = get_int(sysfs_path / "energy_empty_design");
    if (! energy_empty_design.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty_design = 0;
    }

    auto energy_full_design = get_int(sysfs_path / "energy_full_design");
    if (! energy_full_design.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return ratio_to_percent(energy_full.value() - energy_empty.value(),
      energy_full_design.value() - energy_empty_design.value());
}

[[nodiscard]] std::optional<double> charge_capacity(
  const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto charge_empty = get_int(sysfs_path / "charge_empty");
    if (! charge_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_full = get_int(sysfs_path / "charge_full");
    if (! charge_full.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    auto charge_empty_design = get_int(sysfs_path / "charge_empty_design");
    if (! charge_empty_design.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty_design = 0;
    }

    auto charge_full_design = get_int(sysfs_path / "charge_full_design");
    if (! charge_full_design.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    return ratio_to_percent(charge_full.value() - charge_empty.value(),
      charge_full_design.value() - charge_empty_design.value());
}

battery_t::battery_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

std::optional<std::list<battery_t>> battery_t::all() {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    const std::string power_supply_path = "/sys/class/power_supply";

    if (! fs::is_directory(power_supply_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + power_supply_path + "'";
        return std::nullopt;
    }

    std::list<battery_t> batteries;

    for (const fs::directory_entry& battery :
      fs::directory_iterator(power_supply_path)) {
        if (! fs::is_symlink(battery)) {
            syst::error = "The path is not a symbolic link.\npath: '"
              + battery.path().string() + "'";
            return std::nullopt;
        }

        const auto type = get_first_line(battery.path() / "type");
        if (! type.has_value()) {
            // get_first_line sets syst::error
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

std::string battery_t::name() const {
    return this->sysfs_path_.filename();
}

std::optional<battery_t::status_t> battery_t::status() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    const fs::path status_path = this->sysfs_path_ / "status";
    const auto status = get_first_line(status_path);
    if (! status.has_value()) {
        // get_first_line sets syst::error
        return std::nullopt;
    }

    if (status.value() == "Unknown") {
        return status_t::unknown;
    }
    if (status.value() == "Charging") {
        return status_t::charging;
    }
    if (status.value() == "Discharging") {
        return status_t::discharging;
    }
    if (status.value() == "Not charging") {
        return status_t::not_charging;
    }
    if (status.value() == "Full") {
        return status_t::full;
    }

    syst::error =
      "An invalid status was read from a battery status file.\nstatus: '"
      + status.value() + "'\nfile: '" + status_path.string() + "'";
    return std::nullopt;
}

std::optional<double> battery_t::charge() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    // Attempt to calculate the current charge level of this device with energy,
    // then charge, and finally with capacity if all else fails.

    std::string error;

    auto energy = syst::energy(this->sysfs_path_);
    if (energy.has_value()) {
        return energy.value();
    }
    error += syst::error;

    auto charge = syst::charge(this->sysfs_path_);
    if (charge.has_value()) {
        return charge.value();
    }
    error += syst::error;

    // NOTE: This value is the just the current charge level of this device.
    // This is not the same as returned by the 'capacity' method!
    // https://www.kernel.org/doc/html/latest/power/power_supply_class.html#attributes-properties-detailed
    auto capacity = get_int(this->sysfs_path_ / "capacity");
    if (capacity.has_value()) {
        return capacity.value();
    }
    error += syst::error;

    syst::error = error;
    return std::nullopt;
}

std::optional<double> battery_t::capacity() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    // Attempt to calculate the capacity of this device (compared to original
    // capacity at manufacture) with energy and then charge if all else fails.

    std::string error;

    auto energy_capacity = syst::energy_capacity(this->sysfs_path_);
    if (energy_capacity.has_value()) {
        return energy_capacity.value();
    }
    error += syst::error;

    auto charge_capacity = syst::charge_capacity(this->sysfs_path_);
    if (charge_capacity.has_value()) {
        return charge_capacity.value();
    }
    error += syst::error;

    syst::error = error;
    return std::nullopt;
}

} // namespace syst
