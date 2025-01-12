// Standard includes
#include <fstream>

// Local includes
#include "util.hpp"

namespace syst {

syst::optional_t<std::vector<std::string>> get_all_lines(
  const std::filesystem::path& path) {
    if (! std::filesystem::is_regular_file(path)) {
        return SYST_NEW_ERROR(
          "The path is not a regular file.\n\tpath: '" + path.string() + "'");
    }

    std::ifstream file{ path };
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line).good()) {
        lines.push_back(line);
    }

    return lines;
}

syst::optional_t<std::string> get_first_line(
  const std::filesystem::path& path) {
    if (! std::filesystem::is_regular_file(path)) {
        return SYST_NEW_ERROR(
          "The path is not a regular file.\n\tpath: '" + path.string() + "'");
    }

    std::ifstream file{ path };
    std::string first_line;
    if (! std::getline(file, first_line).good()) {
        return SYST_NEW_ERROR(
          "Failed to read the first line of a file.\n\tfile: '" + path.string()
          + "'");
    }

    return first_line;
}

syst::optional_t<uint64_t> get_int(const std::filesystem::path& path) {
    syst::optional_t<std::string> value_str = syst::get_first_line(path);
    if (value_str.has_error()) {
        return SYST_ERROR(value_str.error(),
          "Failed to read an integer from a file.\n\tfile: '" + path.string()
            + "'");
    }

    unsigned long value = 0;
    try {
        value = std::stoul(value_str.value());
    } catch (const std::invalid_argument& exception) {
        return SYST_NEW_ERROR(
          "Encountered a std::invalid_argument exception while "
          "attempting to convert a value to an integer which was "
          "read from a file.\n\tvalue: '"
          + value_str.value() + "'\n\tfile: '" + path.string()
          + "'\n\texception: '" + exception.what() + "'");
    } catch (const std::out_of_range& exception) {
        return SYST_NEW_ERROR(
          "Encountered a std::out_of_range exception while "
          "attempting to convert a value to an integer which was "
          "read from a file.\n\tvalue: '"
          + value_str.value() + "'\n\tfile: '" + path.string()
          + "'\n\texception: '" + exception.what() + "'");
    }

    return value;
}

syst::optional_t<bool> get_bool(const std::filesystem::path& path) {
    syst::optional_t<uint64_t> integer = syst::get_int(path);
    if (integer.has_error()) {
        return SYST_ERROR(integer.error(),
          "Failed to read a boolean from a file.\n\tpath: '" + path.string()
            + "'");
    }

    if (integer.value() != 0 && integer.value() != 1) {
        return SYST_NEW_ERROR(
          "Expected a boolean value (either 0 or 1) from a file.\n\tvalue: '"
          + std::to_string(integer.value()) + "'\n\tfile: '" + path.string()
          + "'");
    }

    return integer.value() == 1UL;
}

result_t write_int(const std::filesystem::path& path, uint64_t integer) {
    if (! std::filesystem::is_regular_file(path)) {
        return SYST_NEW_ERROR(
          "The path is not a regular file.\n\tpath: '" + path.string() + "'");
    }

    std::ofstream file{ path };
    if ((file << integer).fail()) {
        return SYST_NEW_ERROR("Failed to write an integer to a file.\n\tpath: '"
          + path.string() + "'\n\tinteger: '" + std::to_string(integer) + "'");
    }

    return syst::success;
}

bool has_prefix(const std::string& target, const std::string& prefix) {
    return target.rfind(prefix, 0) == 0;
}

std::string remove_prefix(
  const std::string& target, const std::string& prefix) {
    if (! syst::has_prefix(target, prefix)) {
        return target;
    }

    return target.substr(prefix.size());
}

} // namespace syst
