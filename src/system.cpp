// Standard includes
#include <cstring>

// External includes
#include <sys/sysinfo.h>

// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

template<typename integral_t>
[[nodiscard]] double load_to_double(integral_t load) {
    static_assert(std::is_integral_v<integral_t>);

    return static_cast<double>(load) / static_cast<double>(1U << SI_LOAD_SHIFT);
}

template<typename integral_lhs_t, typename integral_rhs_t>
[[nodiscard]] uint64_t mem_to_bytes(
  integral_lhs_t mem, integral_rhs_t mem_unit) {
    static_assert(std::is_integral_v<integral_lhs_t>);
    static_assert(std::is_integral_v<integral_rhs_t>);

    return static_cast<uint64_t>(mem) * static_cast<uint64_t>(mem_unit);
}

syst::optional_t<system_info_t> system_info() {
    struct sysinfo raw_info {};
    if (sysinfo(&raw_info) != 0) {
        int err = errno;
        return SYST_NEW_ERROR(
          "Failed to get system information from 'sysinfo'.\n\treason: '"
          + std::string{ strerror(err) } + "'");
    }

    system_info_t system_info{};

    system_info.uptime = ch::seconds(raw_info.uptime);
    system_info.load_1 = load_to_double(raw_info.loads[0]);
    system_info.load_5 = load_to_double(raw_info.loads[1]);
    system_info.load_15 = load_to_double(raw_info.loads[2]);
    system_info.ram_total = mem_to_bytes(raw_info.totalram, raw_info.mem_unit);
    system_info.ram_free = mem_to_bytes(raw_info.freeram, raw_info.mem_unit);
    system_info.ram_shared =
      mem_to_bytes(raw_info.sharedram, raw_info.mem_unit);
    system_info.ram_buffered =
      mem_to_bytes(raw_info.bufferram, raw_info.mem_unit);
    system_info.swap_total =
      mem_to_bytes(raw_info.totalswap, raw_info.mem_unit);
    system_info.swap_free = mem_to_bytes(raw_info.freeswap, raw_info.mem_unit);
    system_info.procs = static_cast<uint64_t>(raw_info.procs);
    system_info.ram_usage =
      ratio_to_percent(raw_info.totalram - raw_info.freeram, raw_info.totalram);
    system_info.swap_usage = ratio_to_percent(
      raw_info.totalswap - raw_info.freeswap, raw_info.totalswap);

    return system_info;
}

} // namespace syst
