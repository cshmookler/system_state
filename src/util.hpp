#pragma once

// Standard includes
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace syst {

/**
 * @brief Extract all lines from the file at the given path.
 *
 * @param[in] path - The path to the file to extract all lines from.
 * @return an dynamic array of strings with each string corresponding to one
 * line or std::nullopt if an error occured.
 */
[[nodiscard]] std::optional<std::vector<std::string>> get_all_lines(
  const std::filesystem::path& path);

/**
 * @brief Extract the first line from the file at the given path.
 *
 * @param[in] path - The path to the file to extract the first line from.
 * @return a string or std::nullopt if an error occured.
 */
[[nodiscard]] std::optional<std::string> get_first_line(
  const std::filesystem::path& path);

/**
 * @brief Extract an integer from the file at the given path.
 *
 * @param[in] path - The path to the file to extract an integer from.
 * @return an integer or std::nullopt if an error occurred.
 */
[[nodiscard]] std::optional<uint64_t> get_int(
  const std::filesystem::path& path);

/**
 * @brief Extract a boolean (0 or 1) from the file at the given path.
 *
 * @param[in] path - The path to the file to extract a boolean from.
 * @return a boolean or std::nullopt if an error occurred.
 */
[[nodiscard]] std::optional<bool> get_bool(const std::filesystem::path& path);

/**
 * @brief Check whether a target string has the given prefix.
 *
 * @param[in] target - The string that may contain the prefix.
 * @param[in] prefix - The prefix to search for in the target string.
 * @return true if the target string has the given prefix and false otherwise.
 */
[[nodiscard]] bool has_prefix(
  const std::string& target, const std::string& prefix);

/**
 * @brief Removes the given prefix from the target string or does nothing if the
 * target string does not contain the given prefix.
 *
 * @param[in] target - The string to remove the given prefix from.
 * @param[in] prefix - The prefix to remove from the target string.
 * @return the target string with the given prefix removed.
 */
[[nodiscard]] std::string remove_prefix(
  const std::string& target, const std::string& prefix);

/**
 * @brief Calculate a percent (0 - 100) from the given ratio. If the denominator
 * is zero, then 100 is returned.
 *
 * @tparam integral_t - Any integral type that is convertable to a double.
 * @param[in] numerator - The numerator of the given ratio.
 * @param[in] denominator - The denominator of the given ratio.
 * @return the ratio represented as a percent ranging from 0 to 100.
 */
template<typename integral_t>
[[nodiscard]] double ratio_to_percent(
  integral_t numerator, integral_t denominator) {
    static_assert(std::is_integral_v<integral_t>);

    if (denominator == 0) {
        // Return 100% if the denominator is invalid.
        return static_cast<double>(1e2);
    }
    return (static_cast<double>(numerator) / static_cast<double>(denominator))
      * static_cast<double>(1e2);
}

} // namespace syst
