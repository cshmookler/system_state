// Standard includes
#include <fstream>

// Local includes
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

std::optional<std::vector<std::string>> get_all_lines(
  const std::filesystem::path& path) {
    if (! std::filesystem::is_regular_file(path)) {
        syst::error =
          "The path is not a regular file.\npath: '" + path.string() + "'";
        return std::nullopt;
    }

    std::ifstream file{ path };
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line).good()) {
        lines.push_back(line);
    }

    return lines;
}

std::optional<std::string> get_first_line(const std::filesystem::path& path) {
    if (! std::filesystem::is_regular_file(path)) {
        syst::error =
          "The path is not a regular file.\npath: '" + path.string() + "'";
        return std::nullopt;
    }

    std::ifstream file{ path };
    std::string first_line;
    if (! std::getline(file, first_line).good()) {
        syst::error = "Failed to read the first line of a file.\nfile: '"
          + path.string() + "'";
        return std::nullopt;
    }

    return first_line;
}

std::optional<uint64_t> get_int(const std::filesystem::path& path) {
    std::optional<std::string> value_str = get_first_line(path);
    if (! value_str.has_value()) {
        // get_first_line sets syst::error
        return std::nullopt;
    }

    unsigned long value = 0;
    try {
        value = std::stoul(value_str.value());
    } catch (const std::invalid_argument& exception) {
        syst::error = "Encountered a std::invalid_argument exception while "
                      "attempting to convert a value to an integer which was "
                      "read from a file.\nvalue: '"
          + value_str.value() + "'\nfile: '" + path.string() + "'";
        return std::nullopt;
    } catch (const std::out_of_range&) {
        syst::error = "Encountered a std::out_of_range exception while "
                      "attempting to convert a value to an integer which was "
                      "read from a file.\nvalue: '"
          + value_str.value() + "'\nfile: '" + path.string() + "'";
        return std::nullopt;
    }

    return value;
}

std::optional<bool> get_bool(const std::filesystem::path& path) {
    std::optional<uint64_t> integer = get_int(path);
    if (! integer.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    if (integer.value() != 0 && integer.value() != 1) {
        syst::error =
          "Expected a boolean value (either 0 or 1) from a file.\nvalue: '"
          + std::to_string(integer.value()) + "'\nfile: '" + path.string()
          + "'";
        return std::nullopt;
    }

    return integer.value() == 1UL;
}

bool has_prefix(const std::string& target, const std::string& prefix) {
    return target.rfind(prefix, 0) == 0;
}

std::string remove_prefix(
  const std::string& target, const std::string& prefix) {
    if (! has_prefix(target, prefix)) {
        return target;
    }

    return target.substr(prefix.size());
}

} // namespace syst
