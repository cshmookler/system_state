// Local includes
#include "../system_state/system_state.hpp"
#include "util.hpp"

namespace syst {

[[nodiscard]] res::optional_t<double> energy(const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_now = syst::get_int(sysfs_path / "energy_now");
    if (energy_now.has_error()) {
        return RES_TRACE(energy_now.error());
    }

    auto energy_empty = syst::get_int(sysfs_path / "energy_empty");
    if (energy_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_full = syst::get_int(sysfs_path / "energy_full");
    if (energy_full.has_error()) {
        return RES_TRACE(energy_full.error());
    }

    return syst::value_to_percent(
      energy_empty.value(), energy_full.value(), energy_now.value());
}

[[nodiscard]] res::optional_t<double> charge(const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto charge_now = syst::get_int(sysfs_path / "charge_now");
    if (charge_now.has_error()) {
        return RES_TRACE(charge_now.error());
    }

    auto charge_empty = syst::get_int(sysfs_path / "charge_empty");
    if (charge_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_full = syst::get_int(sysfs_path / "charge_full");
    if (charge_full.has_error()) {
        return RES_TRACE(charge_full.error());
    }

    return syst::value_to_percent(
      charge_empty.value(), charge_full.value(), charge_now.value());
}

[[nodiscard]] res::optional_t<double> energy_capacity(
  const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto energy_empty = syst::get_int(sysfs_path / "energy_empty");
    if (energy_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_full = syst::get_int(sysfs_path / "energy_full");
    if (energy_full.has_error()) {
        return RES_TRACE(energy_full.error());
    }

    auto energy_empty_design =
      syst::get_int(sysfs_path / "energy_empty_design");
    if (energy_empty_design.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty_design = 0;
    }

    auto energy_full_design = syst::get_int(sysfs_path / "energy_full_design");
    if (energy_full_design.has_error()) {
        return RES_TRACE(energy_full_design.error());
    }

    return syst::ratio_to_percent(energy_full.value() - energy_empty.value(),
      energy_full_design.value() - energy_empty_design.value());
}

[[nodiscard]] res::optional_t<double> charge_capacity(
  const fs::path& sysfs_path) {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto charge_empty = syst::get_int(sysfs_path / "charge_empty");
    if (charge_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_full = syst::get_int(sysfs_path / "charge_full");
    if (charge_full.has_error()) {
        return RES_TRACE(charge_full.error());
    }

    auto charge_empty_design =
      syst::get_int(sysfs_path / "charge_empty_design");
    if (charge_empty_design.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty_design = 0;
    }

    auto charge_full_design = syst::get_int(sysfs_path / "charge_full_design");
    if (charge_full_design.has_error()) {
        return RES_TRACE(charge_full_design.error());
    }

    return syst::ratio_to_percent(charge_full.value() - charge_empty.value(),
      charge_full_design.value() - charge_empty_design.value());
}

battery_t::battery_t(const fs::path& sysfs_path) : sysfs_path_(sysfs_path) {
}

res::optional_t<std::list<battery_t>> get_batteries() {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    const std::string power_supply_path = "/sys/class/power_supply";

    if (! fs::is_directory(power_supply_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + power_supply_path + "'");
    }

    std::list<battery_t> batteries;

    for (const fs::directory_entry& battery :
      fs::directory_iterator(power_supply_path)) {
        if (! fs::is_directory(battery)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(battery)) {
            // Ignore paths that are not symbolic links.
            continue;
        }

        auto type = syst::get_first_line(battery.path() / "type");
        if (type.has_error()) {
            return RES_TRACE(type.error());
        }
        if (type.value() != "Battery") {
            // Ignore power supply devices that are not batteries.
            continue;
        }

        batteries.push_back(battery_t{ battery });
    }

    return batteries;
}

fs::path battery_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

std::string battery_t::get_name() const {
    return this->sysfs_path_.filename();
}

res::optional_t<battery_t::status_t> battery_t::get_status() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    const fs::path status_path = this->sysfs_path_ / "status";
    auto status = syst::get_first_line(status_path);
    if (status.has_error()) {
        return RES_TRACE(status.error());
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

    return RES_NEW_ERROR(
      "An invalid status was read from a battery status file.\n\tstatus: '"
      + status.value() + "'\n\tfile: '" + status_path.string() + "'");
}

res::optional_t<double> battery_t::get_current() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto current_now = syst::get_int(this->sysfs_path_ / "current_now");
    if (current_now.has_value()) {
        const double microamperes_per_ampere = static_cast<double>(1e6);
        return static_cast<double>(current_now.value())
          / microamperes_per_ampere;
    }
    res::error_t error = RES_TRACE(current_now.error());

    // If the current_now file is missing, dividing power_now by voltage_now
    // produces the approximate current draw from the battery in amperes.

    auto power_now = syst::get_int(this->sysfs_path_ / "power_now");
    if (power_now.has_error()) {
        return RES_CONCAT(error, power_now.error());
    }

    auto voltage_now = syst::get_int(this->sysfs_path_ / "voltage_now");
    if (voltage_now.has_error()) {
        return RES_CONCAT(error, voltage_now.error());
    }

    const double approx_current_now = static_cast<double>(power_now.value())
      / static_cast<double>(voltage_now.value());

    return approx_current_now;
}

res::optional_t<double> battery_t::get_power() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    auto power_now = syst::get_int(this->sysfs_path_ / "power_now");
    if (power_now.has_value()) {
        const double microwatts_per_watt = static_cast<double>(1e6);
        return static_cast<double>(power_now.value()) / microwatts_per_watt;
    }
    res::error_t error = RES_TRACE(power_now.error());

    // If the power_now file is missing, multiplying current_now by voltage_now
    // produces the approximate power in picowatts.

    auto current_now = syst::get_int(this->sysfs_path_ / "current_now");
    if (current_now.has_error()) {
        return RES_CONCAT(error, current_now.error());
    }

    auto voltage_now = syst::get_int(this->sysfs_path_ / "voltage_now");
    if (voltage_now.has_error()) {
        return RES_CONCAT(error, voltage_now.error());
    }

    double approx_power_now_pico = static_cast<double>(current_now.value())
      * static_cast<double>(voltage_now.value());

    const double picowatts_per_watt = static_cast<double>(1e12);
    double approx_power_now = approx_power_now_pico / picowatts_per_watt;

    return approx_power_now;
}

res::optional_t<double> battery_t::get_charge() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    // Attempt to calculate the current charge level of this device with energy,
    // then charge, and finally with capacity if all else fails.

    auto energy = syst::energy(this->sysfs_path_);
    if (energy.has_value()) {
        return energy.value();
    }
    res::error_t error = RES_TRACE(energy.error());

    auto charge = syst::charge(this->sysfs_path_);
    if (charge.has_value()) {
        return charge.value();
    }
    error = RES_CONCAT(error, charge.error());

    // NOTE: This value is the just the current charge level of this device.
    // This is not the same as returned by the 'capacity' method!
    // https://www.kernel.org/doc/html/latest/power/power_supply_class.html#attributes-properties-detailed
    auto capacity = syst::get_int(this->sysfs_path_ / "capacity");
    if (capacity.has_value()) {
        return static_cast<double>(capacity.value());
    }
    error = RES_CONCAT(error, capacity.error());

    return error;
}

res::optional_t<double> battery_t::get_capacity() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    // Attempt to calculate the capacity of this device (compared to original
    // capacity at manufacture) with energy and then charge if all else fails.

    auto energy_capacity = syst::energy_capacity(this->sysfs_path_);
    if (energy_capacity.has_value()) {
        return energy_capacity.value();
    }
    res::error_t error = RES_TRACE(energy_capacity.error());

    auto charge_capacity = syst::charge_capacity(this->sysfs_path_);
    if (charge_capacity.has_value()) {
        return charge_capacity.value();
    }
    error = RES_CONCAT(error, charge_capacity.error());

    return error;
}

res::optional_t<double> energy_stored(const fs::path& sysfs_path) {
    auto energy_empty = syst::get_int(sysfs_path / "energy_empty");
    if (energy_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_now = syst::get_int(sysfs_path / "energy_now");
    if (energy_now.has_error()) {
        return RES_TRACE(energy_now.error());
    }

    double energy_microwatt_hours =
      static_cast<double>(energy_now.value() - energy_empty.value());

    const double microwatt_hours_per_watt_hour = static_cast<double>(1e6);
    double energy_watt_hours =
      energy_microwatt_hours / microwatt_hours_per_watt_hour;

    return energy_watt_hours;
}

res::optional_t<double> energy_missing(const fs::path& sysfs_path) {
    auto energy_now = syst::get_int(sysfs_path / "energy_now");
    if (energy_now.has_error()) {
        return RES_TRACE(energy_now.error());
    }

    auto energy_full = syst::get_int(sysfs_path / "energy_full");
    if (energy_full.has_error()) {
        return RES_TRACE(energy_full.error());
    }

    double energy_microwatt_hours =
      static_cast<double>(energy_full.value() - energy_now.value());

    const double microwatt_hours_per_watt_hour = static_cast<double>(1e6);
    double energy_watt_hours =
      energy_microwatt_hours / microwatt_hours_per_watt_hour;

    return energy_watt_hours;
}

res::optional_t<double> charge_stored(const fs::path& sysfs_path) {
    auto charge_empty = syst::get_int(sysfs_path / "charge_empty");
    if (charge_empty.has_error()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_now = syst::get_int(sysfs_path / "charge_now");
    if (charge_now.has_error()) {
        return RES_TRACE(charge_now.error());
    }

    double charge_microamperes =
      static_cast<double>(charge_now.value() - charge_empty.value());

    const double microamperes_per_ampere = static_cast<double>(1e6);
    double charge_amperes = charge_microamperes / microamperes_per_ampere;

    return charge_amperes;
}

res::optional_t<double> charge_missing(const fs::path& sysfs_path) {
    auto charge_now = syst::get_int(sysfs_path / "charge_now");
    if (charge_now.has_error()) {
        return RES_TRACE(charge_now.error());
    }

    auto charge_full = syst::get_int(sysfs_path / "charge_full");
    if (charge_full.has_error()) {
        return RES_TRACE(charge_full.error());
    }

    double charge_microamperes =
      static_cast<double>(charge_full.value() - charge_now.value());

    const double microamperes_per_ampere = static_cast<double>(1e6);
    double charge_amperes = charge_microamperes / microamperes_per_ampere;

    return charge_amperes;
}

ch::seconds hours_to_seconds(double hours) {
    const double seconds_per_minute = static_cast<double>(60);
    const double minutes_per_hour = static_cast<double>(60);
    const double seconds_per_hour = seconds_per_minute * minutes_per_hour;

    uint64_t seconds = static_cast<uint64_t>(hours * seconds_per_hour);

    return ch::seconds{ seconds };
}

res::optional_t<ch::seconds> battery_t::get_time_remaining() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    // If this battery is discharging, then power_now, current_now, and
    // voltage_now represent the flow out of the battery. If this battery is
    // charging, then these values represent the flow into the battery.
    // Therefore, these values can be used to calculate the time until empty as
    // well as the time until full.

    // If this battery is discharging:
    // Method #1 for calculating time remaining:
    //     time_to_empty contains the number of seconds remaining until this
    //     battery is empty.
    // Method #2 for calculating time remaining:
    //     When discharging, energy_now and power_now measure
    //     energy_now is measured in µWh (microwatt hours).
    //     power_now is measured in µW (microwatts).
    //     Dividing energy_now by power_now produces the number of hours until
    //     this battery is empty.
    // Method #3 for calculating time remaining:
    //     charge_now is measured in µAh (microampere hours).
    //     current_now is measured in µA (microamperes).
    //     Dividing charge_now by current_now produces the number of hours until
    //     this battery is empty.

    // If this battery is charging:
    // Method #1 for calculating time remaining:
    //     time_to_full contains the number of seconds remaining until this
    //     battery is full.
    // Method #2 for calculating time remaining:
    //     energy_full is measured in µWh (microwatt hours).
    //     energy_now is measured in µWh (microwatt hours).
    //     power_now is measured in µW (microwatts).
    //     Dividing (energy_full - energy_now) by power_now produces the number
    //     of hours until this battery is full.
    // Method #3 for calculating time remaining:
    //     charge_full is measured in µAh (microampere hours).
    //     charge_now is measured in µAh (microampere hours).
    //     current_now is measured in µA (microamperes).
    //     Dividing (charge_full - charge_now) by current_now produces the
    //     number of hours until this battery is full.

    // If one of power_now, current_now, or voltage_now is missing, the other
    // two can be used to calculate the missing one (P = I * V).

    auto status = this->get_status();
    if (status.has_error()) {
        return RES_TRACE(status.error());
    }

    if (status.value() == status_t::discharging) {
        // Method #1 for discharging
        auto time_to_empty = syst::get_int(this->sysfs_path_ / "time_to_empty");
        if (time_to_empty.has_value()) {
            return ch::seconds{ time_to_empty.value() };
        }
        res::error_t error = RES_TRACE(time_to_empty.error());

        // Method #2 for discharging
        auto energy = syst::energy_stored(this->sysfs_path_);
        if (energy.has_value()) {
            auto power = this->get_power();
            if (power.has_value()) {
                return syst::hours_to_seconds(energy.value() / power.value());
            }
        }
        error = RES_CONCAT(error, energy.error());

        // Method #3 for discharging
        auto charge = syst::charge_stored(this->sysfs_path_);
        if (charge.has_value()) {
            auto current = this->get_current();
            if (current.has_value()) {
                return syst::hours_to_seconds(charge.value() / current.value());
            }
        }
        error = RES_CONCAT(error, charge.error());

        return error;
    }

    if (status.value() == status_t::charging) {
        // Method #1 for charging
        auto time_to_full = syst::get_int(this->sysfs_path_ / "time_to_full");
        if (time_to_full.has_value()) {
            return ch::seconds{ time_to_full.value() };
        }
        res::error_t error = RES_TRACE(time_to_full.error());

        // Method #2 for charging
        auto energy = syst::energy_missing(this->sysfs_path_);
        if (energy.has_value()) {
            auto power = this->get_power();
            if (power.has_value()) {
                return syst::hours_to_seconds(energy.value() / power.value());
            }
        }
        error = RES_CONCAT(error, energy.error());

        // Method #3 for charging
        auto charge = syst::charge_missing(this->sysfs_path_);
        if (charge.has_value()) {
            auto current = this->get_current();
            if (current.has_value()) {
                return syst::hours_to_seconds(charge.value() / current.value());
            }
        }
        error = RES_CONCAT(error, charge.error());

        return error;
    }

    return RES_NEW_ERROR(
      "Cannot calculate the time remaining for a battery that "
      "is neither charging nor discharging.\n\tsysfs path: '"
      + this->sysfs_path_.string() + "'");
}

} // namespace syst
