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
    std::optional<std::vector<cpu_usage_stat_t>> old_stat;
    std::optional<std::vector<cpu_usage_stat_t>> new_stat;
};

[[nodiscard]] double get_usage_percentage(
  const cpu_usage_stat_t& old_stat, const cpu_usage_stat_t& new_stat) {
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

    return ratio_to_percent(total - idle, total);
}

cpu_usage_t::cpu_usage_t() : impl_(std::make_unique<impl>()) {
}

cpu_usage_t::~cpu_usage_t() = default;

bool cpu_usage_t::update() const {
    // documentation for /proc/stat
    //     https://www.kernel.org/doc/html/latest/filesystems/proc.html#miscellaneous-kernel-statistics-in-proc-stat

    const fs::path proc_stat_path = "/proc/stat";
    const auto lines = get_all_lines(proc_stat_path);
    if (! lines.has_value()) {
        // get_all_lines sets syst::error
        return false;
    }
    if (lines->size() < 2) {
        syst::error = "The process statistics file must contain at least two "
                      "lines.\nlines: '"
          + std::to_string(lines->size()) + "'\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }

    std::vector<cpu_usage_stat_t> temp_stats;

    for (const std::string& line : lines.value()) {
        // All lines with CPU statistics are at the beginning of the file
        // and start with a specific prefix.
        const std::string fields_prefix = "cpu";
        if (! has_prefix(line, fields_prefix)) {
            break;
        }

        std::stringstream stream;

        if ((stream << line).fail()) {
            syst::error = "Failed to load process statistics into a "
                          "std::stringstream object.\nstats: '"
              + line + "'";
            return false;
        }

        std::string cpu_name; // Currently unused
        if ((stream >> cpu_name).fail()) {
            syst::error = "Failed to read the 'user_mode' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }

        cpu_usage_stat_t temp{};

        if ((stream >> temp.user_mode).fail()) {
            syst::error = "Failed to read the 'user_mode' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.low_priority_user_mode).fail()) {
            syst::error = "Failed to read the 'low_priority_user_mode' "
                          "statistic from the process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.system_mode).fail()) {
            syst::error = "Failed to read the 'system_mode' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.idle).fail()) {
            syst::error = "Failed to read the 'idle' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.io_idle).fail()) {
            syst::error = "Failed to read the 'io_idle' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.interrupt).fail()) {
            syst::error = "Failed to read the 'interrupt' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.soft_interrupt).fail()) {
            syst::error = "Failed to read the 'soft_interrupt' statistic from "
                          "the process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.stolen).fail()) {
            syst::error = "Failed to read the 'stolen' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.guest).fail()) {
            syst::error = "Failed to read the 'guest' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }
        if ((stream >> temp.niced_guest).fail()) {
            syst::error = "Failed to read the 'niced_guest' statistic from the "
                          "process statistics file.\nfile: '"
              + proc_stat_path.string() + "'";
            return false;
        }

        temp_stats.push_back(temp);
    }

    if (temp_stats.size() < 2) {
        syst::error = "Failed to process at least two lines extracted from the "
                      "process statistics file\nlines processed: '"
          + std::to_string(temp_stats.size()) + "'\nfile: '"
          + proc_stat_path.string() + "'";
        return false;
    }

    // Do not modify the internal state of this class unless all operations
    // succeed.

    if (this->impl_->new_stat.has_value()) {
        this->impl_->old_stat = this->impl_->new_stat;
    } else {
        this->impl_->new_stat = std::vector<cpu_usage_stat_t>{};
    }

    this->impl_->new_stat = temp_stats;

    return true;
}

std::optional<double> cpu_usage_t::get_total() const {
    if (! this->impl_->new_stat.has_value()) {
        syst::error = "No statistics samples are stored. Call the 'update' "
                      "method twice before calling the 'get_total' method.";
        return std::nullopt;
    }
    if (! this->impl_->old_stat.has_value()) {
        syst::error =
          "Only one statistics sample is stored. Call the 'update' method one "
          "more time before calling the 'get_total' method.";
        return std::nullopt;
    }

    // The first values represent the total CPU statistics.
    // Both new_stat and old_stat are guaranteed to have at least two values.
    const cpu_usage_stat_t& old_stat = this->impl_->old_stat->front();
    const cpu_usage_stat_t& new_stat = this->impl_->new_stat->front();

    return get_usage_percentage(old_stat, new_stat);
}

std::optional<std::list<double>> cpu_usage_t::get_per_core() const {
    if (! this->impl_->new_stat.has_value()) {
        syst::error = "No statistics samples are stored. Call the 'update' "
                      "method twice before calling the 'get_per_core' method.";
        return std::nullopt;
    }
    if (! this->impl_->old_stat.has_value()) {
        syst::error =
          "Only one statistics sample is stored. Call the 'update' method one "
          "more time before calling the 'get_per_core' method.";
        return std::nullopt;
    }

    if (this->impl_->new_stat->size() != this->impl_->old_stat->size()) {
        syst::error = "The number of new statistics does not match the number "
                      "of old statistics.\nnew: '"
          + std::to_string(this->impl_->new_stat->size()) + "'\nold: '"
          + std::to_string(this->impl_->old_stat->size()) + "'";
        return std::nullopt;
    }

    std::list<double> cores;

    // The first values in old_stat and new_stat represent the total CPU usage
    // across all cores. This method only returns the usage for individual cores
    // so the first values are skipped (i = 1).
    for (size_t i = 1; i < this->impl_->new_stat->size(); ++i) {
        const cpu_usage_stat_t& old_stat = this->impl_->old_stat->at(i);
        const cpu_usage_stat_t& new_stat = this->impl_->new_stat->at(i);

        cores.push_back(get_usage_percentage(old_stat, new_stat));
    }

    return cores;
}

} // namespace syst
