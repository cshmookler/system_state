// Standard includes
#include <cstdint>

// External includes
#include <sys/sysinfo.h>

// Local includes
#include "../system_state/core.hpp"

namespace syst {

struct System::Impl {
    struct sysinfo info {};
    bool good = false;

    static double load_to_double(unsigned long load) {
        return static_cast<double>(load)
          / static_cast<double>(1U << SI_LOAD_SHIFT);
    }

    static double ratio_to_perc(uint64_t numerator, uint64_t denominator) {
        return (static_cast<double>(numerator)
                 / static_cast<double>(denominator))
          * static_cast<double>(1e2);
    }

    uint64_t mem_to_bytes(unsigned long mem) const {
        return static_cast<uint64_t>(mem)
          * static_cast<uint64_t>(this->info.mem_unit);
    }

    Impl() : good(sysinfo(&this->info) == 0) {
    }
};

System::System() : inst_(std::make_unique<Impl>()) {
}

System::~System() = default;

std::optional<System> System::get() {
    System sys{};
    if (! sys.inst_->good) {
        return std::nullopt;
    }
    return sys;
}

uint64_t System::get_uptime() const {
    return static_cast<uint64_t>(inst_->info.uptime);
}

double System::get_load_1() const {
    return Impl::load_to_double(inst_->info.loads[0]);
}

double System::get_load_5() const {
    return Impl::load_to_double(inst_->info.loads[1]);
}

double System::get_load_15() const {
    return Impl::load_to_double(inst_->info.loads[2]);
}

uint64_t System::get_ram_total() const {
    return inst_->mem_to_bytes(inst_->info.totalram);
}

uint64_t System::get_ram_free() const {
    return inst_->mem_to_bytes(inst_->info.freeram);
}

uint64_t System::get_ram_shared() const {
    return inst_->mem_to_bytes(inst_->info.sharedram);
}

uint64_t System::get_ram_buffered() const {
    return inst_->mem_to_bytes(inst_->info.bufferram);
}

uint64_t System::get_swap_total() const {
    return inst_->mem_to_bytes(inst_->info.totalswap);
}

uint64_t System::get_swap_free() const {
    return inst_->mem_to_bytes(inst_->info.freeswap);
}

unsigned short System::get_procs() const {
    return inst_->info.procs;
}

double System::get_ram_perc() const {
    return Impl::ratio_to_perc(inst_->info.freeram, inst_->info.totalram);
}

double System::get_swap_perc() const {
    return Impl::ratio_to_perc(inst_->info.freeswap, inst_->info.totalswap);
}

} // namespace syst
