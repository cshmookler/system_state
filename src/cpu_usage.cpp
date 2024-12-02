// Standard includes
#include <memory>
#include <string_view>
#include <sstream>

// Local includes
#include "../system_state/core.hpp"
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

    const std::string_view proc_stat_path = "/proc/stat";
    std::optional<std::string> first_line = get_first_line(proc_stat_path);
    if (! first_line.has_value()) {
        return false;
    }

    const std::string_view proc_stat_cpu_field = "cpu ";
    std::string_view fields =
      remove_prefix(first_line.value(), proc_stat_cpu_field);

    if (fields == first_line.value()) {
        // The first line did not contain the target prefix.
        return false;
    }

    std::stringstream stream;

    if ((stream << fields).fail()) {
        return false;
    }

    if (this->impl_->new_stat.has_value()) {
        this->impl_->old_stat = this->impl_->new_stat;
    } else {
        this->impl_->new_stat = cpu_usage_stat_t{};
    }

    if ((stream >> this->impl_->new_stat->user_mode).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->low_priority_user_mode).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->system_mode).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->idle).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->io_idle).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->interrupt).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->soft_interrupt).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->stolen).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->guest).fail()) {
        return false;
    }
    if ((stream >> this->impl_->new_stat->niced_guest).fail()) {
        return false;
    }

    return true;
}

std::optional<double> cpu_usage_t::get() const {
    if ((! this->impl_->old_stat.has_value())
      || (! this->impl_->new_stat.has_value())) {
        return std::nullopt;
    }

    const cpu_usage_stat_t& old_stat = this->impl_->old_stat.value();
    const cpu_usage_stat_t& new_stat = this->impl_->new_stat.value();

    uint64_t idle = new_stat.idle - old_stat.idle;

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
