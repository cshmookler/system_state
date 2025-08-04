#pragma once

namespace syst {

/**
 * @brief Get a human-readable description of a POSIX-compatible error code.
 *
 * This function returns a C-string instead of a std::string because the
 * definition of this function uses the POSIX version of the C++ standard
 * library which might conflict with the GNU version.  If this function returned
 * a std::string, and the POSIX implementation of std::string differed from the
 * GNU implementation, then calling this function would result in undefined
 * behavior or a segmentation fault.
 *
 * @param[in] errnum - A POSIX error code (usually from errno).
 * @return a string describing the error corresponding to the given error code.
 */
char* strerror(int errnum);

} // namespace syst
