// Standard includes
#include <memory>
#include <sstream>

// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

struct cpu_usage_stat_t {
    uint64_t user_mode;
    uint64_t low_priority_user_mode;
    uint64_t system_mode;
    uint64_t idle;
    uint64_t io_idle;
    uint64_t interrupt;
    uint64_t soft_interrupt;
    uint64_t stolen;
    uint64_t guest;
    uint64_t niced_guest;
};

struct cpu_usage_t::impl {
    std::optional<cpu_usage_stat_t> old_stat;
    std::optional<cpu_usage_stat_t> new_stat;
};

cpu_usage_t::cpu_usage_t() : impl_(std::make_unique<impl>()) {
}

cpu_usage_t::~cpu_usage_t() = default;

bool cpu_usage_t::update() const {
    // documentation for /proc/stat
    //     https://www.kernel.org/doc/html/latest/filesystems/proc.html#miscellaneous-kernel-statistics-in-proc-stat

    const fs::path proc_stat_path = "/proc/stat";
    const auto first_line = get_first_line(proc_stat_path);
    if (! first_line.has_value()) {
        // get_first_line sets syst::error
        return false;
    }

    const std::string fields_prefix = "cpu ";
    const std::string fields = remove_prefix(first_line.value(), fields_prefix);
    if (fields == first_line.value()) {
        syst::error = "The first line of the process statistics file does not "
                      "start with the expected prefix.\nline: '"
          + first_line.value() + "'\nprefix: '" + fields_prefix
          + "'\nfile: " + proc_stat_path.string() + "'";
        return false;
    }

    std::stringstream stream;

    if ((stream << fields).fail()) {
        syst::error =
          "Failed to load process statistics into a std::stringstream "
          "object.\nstats: '"
          + fields + "'";
        return false;
    }

    if (this->impl_->new_stat.has_value()) {
        this->impl_->old_stat = this->impl_->new_stat;
    } else {
        this->impl_->new_stat = cpu_usage_stat_t{};
    }

    if ((stream >> this->impl_->new_stat->user_mode).fail()) {
        syst::error = "Failed to read the 'user_mode' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->low_priority_user_mode).fail()) {
        syst::error =
          "Failed to read the 'low_priority_user_mode' statistic from the "
          "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->system_mode).fail()) {
        syst::error = "Failed to read the 'system_mode' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->idle).fail()) {
        syst::error = "Failed to read the 'idle' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->io_idle).fail()) {
        syst::error = "Failed to read the 'io_idle' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->interrupt).fail()) {
        syst::error = "Failed to read the 'interrupt' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->soft_interrupt).fail()) {
        syst::error = "Failed to read the 'soft_interrupt' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->stolen).fail()) {
        syst::error = "Failed to read the 'stolen' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->guest).fail()) {
        syst::error = "Failed to read the 'guest' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }
    if ((stream >> this->impl_->new_stat->niced_guest).fail()) {
        syst::error = "Failed to read the 'niced_guest' statistic from the "
                      "process statistics file.\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }

    return true;
}

std::optional<double> cpu_usage_t::get() const {
    if (! this->impl_->new_stat.has_value()) {
        syst::error = "No statistics samples are stored. Call the 'update' "
                      "method twice before calling the 'get' method.";
        return std::nullopt;
    }
    if (! this->impl_->old_stat.has_value()) {
        syst::error = "Only one statistics sample is stored. Call the 'update' "
                      "method one more time before calling the 'get' method.";
        return std::nullopt;
    }

    const cpu_usage_stat_t& old_stat = this->impl_->old_stat.value();
    const cpu_usage_stat_t& new_stat = this->impl_->new_stat.value();

    const uint64_t idle = new_stat.idle - old_stat.idle;

    uint64_t total = 0;
    total += new_stat.user_mode - old_stat.user_mode;
    total += new_stat.low_priority_user_mode - old_stat.low_priority_user_mode;
    total += new_stat.system_mode - old_stat.system_mode;
    total += idle;
    total += new_stat.io_idle - old_stat.io_idle;
    total += new_stat.interrupt - old_stat.interrupt;
    total += new_stat.soft_interrupt - old_stat.soft_interrupt;
    total += new_stat.stolen - old_stat.stolen;
    total += new_stat.guest - old_stat.guest;
    total += new_stat.niced_guest - old_stat.niced_guest;

    return ratio_to_percent(idle, total);
}

} // namespace syst
