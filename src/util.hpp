#pragma once

// Standard includes
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

// External includes
#include <cpp_result/all.hpp>

namespace syst {

/**
 * @brief Extract all lines from the file at the given path.
 *
 * @param[in] path - The path to the file to extract all lines from.
 * @return a dynamic array of string with each string corresponding to one line
 * if the operation succeeded or an error otherwise.
 */
[[nodiscard]] res::optional_t<std::vector<std::string>> get_all_lines(
  const std::filesystem::path& path);

/**
 * @brief Extract the first line from the file at the given path.
 *
 * @param[in] path - The path to the file to extract the first line from.
 * @return a string if the operation succeeded or an error otherwise.
 */
[[nodiscard]] res::optional_t<std::string> get_first_line(
  const std::filesystem::path& path);

/**
 * @brief Extract an integer from the file at the given path.
 *
 * @param[in] path - The path to the file to extract an integer from.
 * @return an integer if the operation succeeded or an error otherwise.
 */
[[nodiscard]] res::optional_t<uint64_t> get_int(
  const std::filesystem::path& path);

/**
 * @brief Extract a boolean (0 or 1) from the file at the given path.
 *
 * @param[in] path - The path to the file to extract a boolean from.
 * @return a boolean if the operation succeeded or an error otherwise.
 */
[[nodiscard]] res::optional_t<bool> get_bool(const std::filesystem::path& path);

/**
 * @brief Write an integer to the file at the given path.
 *
 * @param[in] path - The path to the file to write to.
 * @param[in] integer - The integer to write to the given file.
 * @return success if the given integer was successfully written to the given
 * file and failure otherwise.
 */
res::result_t write_int(const std::filesystem::path& path, uint64_t integer);

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
 * @brief Calculate a percentage (0 - 100) based on the given ratio. If the
 * denominator is zero, then 100 is returned.
 *
 * @param[in] numerator - The numerator (top) of the ratio.
 * @param[in] denominator - The denominator (bottom) of the ratio.
 * @return the given ratio represented as a percentage (0 to 100).
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

/**
 * @brief Calculate a percentage (0 - 100) based on where a given value falls on
 * the given range (min - max).
 *
 * @tparam integral_t - Any integral type that is convertable to a double.
 * @param[in] min - The minimum possible value.
 * @param[in] max - The maximum possible value.
 * @param[in] value - The value to compute the percentage for.
 * @return the value on the given range (min - max) represented as a percentage
 * (0 to 100).
 */
template<typename integral_t>
[[nodiscard]] double value_to_percent(
  integral_t min, integral_t max, integral_t value) {
    static_assert(std::is_integral_v<integral_t>);

    return syst::ratio_to_percent(value - min, max - min);
}

/**
 * @brief Calculate a value falling on the given range (min - max) based on a
 * given percentage (0 - 100).
 *
 * @tparam integral_t - Any integral type that is convertable to a double.
 * @param[in] min - The minimum possible value.
 * @param[in] max - The maximum possible value.
 * @param[in] percent - The percentage to compute the value for.
 * @return the percentage represented as a value on the given range (min - max).
 */
template<typename integral_t>
[[nodiscard]] integral_t percent_to_value(
  integral_t min, integral_t max, double percent) {
    static_assert(std::is_integral_v<integral_t>);

    return static_cast<integral_t>((percent / static_cast<double>(1e2))
             * static_cast<double>(max - min))
      + min;
}

} // namespace syst
