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

    return value_to_percent(
      energy_empty.value(), energy_full.value(), energy_now.value());
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

    return value_to_percent(
      charge_empty.value(), charge_full.value(), charge_now.value());
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

        auto type = get_first_line(battery.path() / "type");
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

std::optional<double> battery_t::current() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    std::string error;

    const auto current_now = get_int(this->sysfs_path_ / "current_now");
    if (current_now.has_value()) {
        const double microamperes_per_ampere = static_cast<double>(1e6);
        return static_cast<double>(current_now.value())
          / microamperes_per_ampere;
    }
    error = syst::error;

    // If the current_now file is missing, dividing power_now by voltage_now
    // produces the approximate current draw from the battery in amperes.

    const auto power_now = get_int(this->sysfs_path_ / "power_now");
    if (! power_now.has_value()) {
        error += "\n" + syst::error;
        syst::error = error;
        return std::nullopt;
    }

    const auto voltage_now = get_int(this->sysfs_path_ / "voltage_now");
    if (! voltage_now.has_value()) {
        error += "\n" + syst::error;
        syst::error = error;
        return std::nullopt;
    }

    const double approx_current_now = static_cast<double>(power_now.value())
      / static_cast<double>(voltage_now.value());

    return approx_current_now;
}

std::optional<double> battery_t::power() const {
    // documentation for /sys/class/power_supply
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/power_supply.h
    //     https://www.kernel.org/doc/html/latest/power/power_supply_class.html

    std::string error;

    const auto power_now = get_int(this->sysfs_path_ / "power_now");
    if (power_now.has_value()) {
        const double microwatts_per_watt = static_cast<double>(1e6);
        return static_cast<double>(power_now.value()) / microwatts_per_watt;
    }
    error = syst::error;

    // If the power_now file is missing, multiplying current_now by voltage_now
    // produces the approximate power in picowatts.

    auto current_now = get_int(this->sysfs_path_ / "current_now");
    if (! current_now.has_value()) {
        error += "\n" + syst::error;
        syst::error = error;
        return std::nullopt;
    }

    auto voltage_now = get_int(this->sysfs_path_ / "voltage_now");
    if (! voltage_now.has_value()) {
        error += "\n" + syst::error;
        syst::error = error;
        return std::nullopt;
    }

    double approx_power_now_pico = static_cast<double>(current_now.value())
      * static_cast<double>(voltage_now.value());

    const double picowatts_per_watt = static_cast<double>(1e12);
    double approx_power_now = approx_power_now_pico / picowatts_per_watt;

    return approx_power_now;
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
    error = syst::error;

    auto charge_capacity = syst::charge_capacity(this->sysfs_path_);
    if (charge_capacity.has_value()) {
        return charge_capacity.value();
    }
    error += "\n" + syst::error;

    syst::error = error;
    return std::nullopt;
}

std::optional<double> energy_stored(const fs::path& sysfs_path) {
    auto energy_empty = get_int(sysfs_path / "energy_empty");
    if (! energy_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        energy_empty = 0;
    }

    auto energy_now = get_int(sysfs_path / "energy_now");
    if (! energy_now.has_value()) {
        return std::nullopt;
    }

    double energy_microwatt_hours =
      static_cast<double>(energy_now.value() - energy_empty.value());

    const double microwatt_hours_per_watt_hour = static_cast<double>(1e6);
    double energy_watt_hours =
      energy_microwatt_hours / microwatt_hours_per_watt_hour;

    return energy_watt_hours;
}

std::optional<double> energy_missing(const fs::path& sysfs_path) {
    auto energy_now = get_int(sysfs_path / "energy_now");
    if (! energy_now.has_value()) {
        return std::nullopt;
    }

    auto energy_full = get_int(sysfs_path / "energy_full");
    if (! energy_full.has_value()) {
        return std::nullopt;
    }

    double energy_microwatt_hours =
      static_cast<double>(energy_full.value() - energy_now.value());

    const double microwatt_hours_per_watt_hour = static_cast<double>(1e6);
    double energy_watt_hours =
      energy_microwatt_hours / microwatt_hours_per_watt_hour;

    return energy_watt_hours;
}

std::optional<double> charge_stored(const fs::path& sysfs_path) {
    auto charge_empty = get_int(sysfs_path / "charge_empty");
    if (! charge_empty.has_value()) {
        // If this file doesn't exist, assume its value is zero.
        charge_empty = 0;
    }

    auto charge_now = get_int(sysfs_path / "charge_now");
    if (! charge_now.has_value()) {
        return std::nullopt;
    }

    double charge_microamperes =
      static_cast<double>(charge_now.value() - charge_empty.value());

    const double microamperes_per_ampere = static_cast<double>(1e6);
    double charge_amperes = charge_microamperes / microamperes_per_ampere;

    return charge_amperes;
}

std::optional<double> charge_missing(const fs::path& sysfs_path) {
    auto charge_now = get_int(sysfs_path / "charge_now");
    if (! charge_now.has_value()) {
        return std::nullopt;
    }

    auto charge_full = get_int(sysfs_path / "charge_full");
    if (! charge_full.has_value()) {
        return std::nullopt;
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

std::optional<ch::seconds> battery_t::time_remaining() const {
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

    auto status = this->status();
    if (! status.has_value()) {
        // status sets syst::error
        return std::nullopt;
    }

    std::string error;

    if (status.value() == status_t::discharging) {
        // Method #1 for discharging
        auto time_to_empty = get_int(this->sysfs_path_ / "time_to_empty");
        if (time_to_empty.has_value()) {
            return ch::seconds{ time_to_empty.value() };
        }
        error = syst::error;

        // Method #2 for discharging
        auto energy = energy_stored(this->sysfs_path_);
        if (energy.has_value()) {
            auto power = this->power();
            if (power.has_value()) {
                return hours_to_seconds(energy.value() / power.value());
            }
        }
        error += "\n" + syst::error;

        // Method #3 for discharging
        auto charge = charge_stored(this->sysfs_path_);
        if (charge.has_value()) {
            auto current = this->current();
            if (current.has_value()) {
                return hours_to_seconds(charge.value() / current.value());
            }
        }
        error += "\n" + syst::error;

        syst::error = error;
        return std::nullopt;
    }

    if (status.value() == status_t::charging) {
        // Method #1 for charging
        auto time_to_full = get_int(this->sysfs_path_ / "time_to_full");
        if (time_to_full.has_value()) {
            return ch::seconds{ time_to_full.value() };
        }
        error = syst::error;

        // Method #2 for charging
        auto energy = energy_missing(this->sysfs_path_);
        if (energy.has_value()) {
            auto power = this->power();
            if (power.has_value()) {
                return hours_to_seconds(energy.value() / power.value());
            }
        }
        error += "\n" + syst::error;

        // Method #3 for charging
        auto charge = charge_missing(this->sysfs_path_);
        if (charge.has_value()) {
            auto current = this->current();
            if (current.has_value()) {
                return hours_to_seconds(charge.value() / current.value());
            }
        }
        error += "\n" + syst::error;

        syst::error = error;
        return std::nullopt;
    }

    syst::error = "Cannot calculate the time remaining for a battery that "
                  "is neither charging nor discharging.\nsysfs path: '"
      + this->sysfs_path_.string() + "'";
    return std::nullopt;
}

} // namespace syst
