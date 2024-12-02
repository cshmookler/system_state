// Standard includes
#include <fstream>

// Local includes
#include "util.hpp"

namespace syst {

std::optional<std::string> get_first_line(const std::filesystem::path& path) {
    std::ifstream file{ path };
    std::string first_line;
    if (! std::getline(file, first_line).good()) {
        return std::nullopt;
    }
    return first_line;
}

std::optional<uint64_t> get_int(const std::filesystem::path& path) {
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

std::optional<bool> get_bool(const std::filesystem::path& path) {
    std::optional<uint64_t> integer = get_int(path);

    return integer.value() == 1UL;
}

bool has_prefix(
  const std::string_view& target, const std::string_view& prefix) {
    return target.rfind(prefix, 0) == 0;
}

std::string_view remove_prefix(
  const std::string_view& target, const std::string_view& prefix) {
    if (! has_prefix(target, prefix)) {
        return target;
    }

    return target.substr(prefix.size());
}

} // namespace syst
