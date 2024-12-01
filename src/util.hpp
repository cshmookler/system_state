#pragma once

// Standard includes
#include <fstream>

// Local includes
#include "../system_state/core.hpp"

namespace syst {

std::optional<std::string> get_first_line(const fs::path& path) {
    std::ifstream file{ path };
    std::string first_line;
    if (! std::getline(file, first_line).good()) {
        return std::nullopt;
    }
    return first_line;
}

std::optional<uint64_t> get_int(const fs::path& path) {
    std::optional<std::string> value_str = get_first_line(path);
    if (! value_str.has_value()) {
        return std::nullopt;
    }

    unsigned long value = 0;
    try {
        value = std::stoul(value_str.value());
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }

    return value;
}

std::optional<bool> get_bool(const fs::path& path) {
    std::optional<uint64_t> integer = get_int(path);

    return integer.value() == 1UL;
}

} // namespace syst
