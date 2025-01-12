#pragma once

/*****************************************************************************/
/*  Copyright (c) 2024 Caden Shmookler                                       */
/*                                                                           */
/*  This software is provided 'as-is', without any express or implied        */
/*  warranty. In no event will the authors be held liable for any damages    */
/*  arising from the use of this software.                                   */
/*                                                                           */
/*  Permission is granted to anyone to use this software for any purpose,    */
/*  including commercial applications, and to alter it and redistribute it   */
/*  freely, subject to the following restrictions:                           */
/*                                                                           */
/*  1. The origin of this software must not be misrepresented; you must not  */
/*     claim that you wrote the original software. If you use this software  */
/*     in a product, an acknowledgment in the product documentation would    */
/*     be appreciated but is not required.                                   */
/*  2. Altered source versions must be plainly marked as such, and must not  */
/*     be misrepresented as being the original software.                     */
/*  3. This notice may not be removed or altered from any source             */
/*     distribution.                                                         */
/*****************************************************************************/

/**
 * @file optional.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief An alternative to std::optional that stores an error message.
 * @date 2025-01-05
 */

// Standard includes
#include <memory>
#include <string>

// Append a trace to an error. Each trace contains the file name, function name,
// and line number where this macro is expanded.
#define SYST_TRACE(trace)                                                      \
    syst::error_t {                                                            \
        std::string{ trace } + "\n" + __FILE__ + ":" + __FUNCTION__            \
          + "():" + std::to_string(__LINE__)                                   \
    }

// Append a trace to an error with an additional error message.
#define SYST_ERROR(trace, error)                                               \
    syst::error_t {                                                            \
        SYST_TRACE(trace).get() + " -> " + error                               \
    }

// Create a new error with a trace.
#define SYST_NEW_ERROR(error) SYST_ERROR("", error)

namespace syst {

/**
 * @brief Represents an error with traces.
 */
class error_t {
    std::string error_;

  public:
    // All constructors must be explicit so construction is never ambiguous. If
    // a function returns a syst::optional_t<std::string>, then returning a
    // std::string must never construct a syst::error_t unless explicitly
    // casted to a syst::error_t.
    // For example:
    //
    // syst::optional_t<std::string> example(bool success) {
    //     if (success) {
    //         return "success";
    //     } else {
    //         return syst::error_t("error");
    //     }
    // }

    explicit error_t(const std::string& error) : error_(error) {
    }

    explicit error_t(std::string&& error) : error_(std::move(error)) {
    }

    /**
     * @brief Get a const reference to the stored error message and stack trace.
     */
    [[nodiscard]] const std::string& get() const {
        return this->error_;
    }

    /**
     * @brief Get a reference to the stored error message and stack trace.
     */
    [[nodiscard]] std::string& get() {
        return this->error_;
    }
};

/**
 * @brief Indicates success or failure. Contains an error message for failure
 * and an empty string for success.
 */
class result_t {
    std::unique_ptr<std::string> error_;

    static const std::string success_message;

  public:
    result_t() {
    }

    result_t(const error_t& error)
    : error_(std::make_unique<std::string>(error.get())) {
    }

    result_t(error_t&& error)
    : error_(
        std::make_unique<std::string>(std::forward<std::string>(error.get()))) {
    }

    result_t(const result_t& result) {
        if (result.success()) {
            this->error_ = nullptr;
        } else {
            this->error_ = std::make_unique<std::string>(*(result.error_));
        }
    }
    result_t(result_t&&) = default;
    result_t& operator=(const result_t& result) {
        if (this == &result) {
            return *this;
        }

        if (result.success()) {
            this->error_ = nullptr;
        } else {
            this->error_ = std::make_unique<std::string>(*(result.error_));
        }

        return *this;
    }
    result_t& operator=(result_t&&) = default;
    ~result_t() = default;

    /**
     * @return true if this result represents success and false otherwise.
     */
    [[nodiscard]] bool success() const {
        return this->error_ == nullptr;
    }

    /**
     * @return true if this result represents failure and false otherwise.
     */
    [[nodiscard]] bool failure() const {
        return ! this->success();
    }

    /**
     * @return a copy of the error stored within this result or a
     * generic success message if this result represents success.
     */
    [[nodiscard]] std::string error() {
        if (this->success()) {
            return success_message;
        }

        return *(this->error_);
    }
};

extern const result_t success;

/*
 * @brief An exception thrown when attempting to access a value stored within a
 * syst::optional_t that does not exist.
 */
class bad_optional_access_t : public std::exception {
    std::string error_;

  public:
    bad_optional_access_t(const std::string& error) : error_(error) {
    }

    bad_optional_access_t(std::string&& error) : error_(std::move(error)) {
    }

    const char* what() const noexcept override {
        return this->error_.c_str();
    }
};

/**
 * @brief Represents a value that may or may not exist. Contains an error
 * message explaining why the value does not exist.
 */
template<typename type_t>
class optional_t {
    std::unique_ptr<type_t> value_;
    std::unique_ptr<std::string> error_;

    static const std::string has_value_message;
    static const std::string bad_optional_access_message;

  public:
    // If a syst::optional_t does not contain a value, it is guaranteed to
    // contain an error.

    optional_t(const error_t& error)
    : value_(nullptr), error_(std::make_unique<std::string>(error.get())) {
    }
    optional_t(error_t&& error)
    : value_(nullptr)
    , error_(
        std::make_unique<std::string>(std::forward<std::string>(error.get()))) {
    }

    optional_t(const type_t& value)
    : value_(std::make_unique<type_t>(value)), error_(nullptr) {
    }
    optional_t(type_t&& value)
    : value_(std::make_unique<type_t>(std::forward<type_t>(value)))
    , error_(nullptr) {
    }

    optional_t& operator=(const type_t& value) {
        this->value_ = std::make_unique<type_t>(value);
        this->error_ = nullptr;
        return *this;
    }
    optional_t& operator=(type_t&& value) {
        this->value_ = std::make_unique<type_t>(std::forward<type_t>(value));
        this->error_ = nullptr;
        return *this;
    }

    optional_t& operator=(const error_t& error) {
        this->value_ = nullptr;
        this->error_ = std::make_unique<std::string>(error.get());
        return *this;
    }
    optional_t& operator=(error_t&& error) {
        this->value_ = nullptr;
        this->error_ =
          std::make_unique<std::string>(std::forward<std::string>(error.get()));
        return *this;
    }

    optional_t(const optional_t& result) {
        if (result.success()) {
            this->value_ = std::make_unique<type_t>(*(result.value_));
            this->error_ = nullptr;
        } else {
            this->value_ = nullptr;
            this->error_ = std::make_unique<std::string>(*(result.error_));
        }
    }
    optional_t(optional_t&&) = default;
    optional_t& operator=(const optional_t& result) {
        if (this == &result) {
            return *this;
        }

        if (result.success()) {
            this->value_ =
              std::make_unique<type_t>(std::forward<type_t>(*(result.value_)));
            this->error_ = nullptr;
        } else {
            this->value_ = nullptr;
            this->error_ = std::make_unique<std::string>(
              std::forward<type_t>(*(result.error_)));
        }

        return *this;
    }
    optional_t& operator=(optional_t&&) = default;
    ~optional_t() = default;

    [[nodiscard]] const type_t* operator->() const {
        if (! this->has_value()) {
            throw bad_optional_access_t{
                SYST_ERROR(this->error(), bad_optional_access_message).get()
            };
        }

        return this->value_.get();
    }

    [[nodiscard]] type_t* operator->() {
        if (! this->has_value()) {
            throw bad_optional_access_t{
                SYST_ERROR(this->error(), bad_optional_access_message).get()
            };
        }

        return this->value_.get();
    }

    /**
     * @return true if this object contains a value and false otherwise.
     */
    [[nodiscard]] bool has_value() const {
        return this->value_ != nullptr;
    }

    /**
     * @throw bad_optional_access_t if this object does not contain a value.
     * @return a const reference to the value stored within this object.
     */
    [[nodiscard]] const type_t& value() const {
        return *(this->operator->());
    }

    /**
     * @throw bad_optional_access_t if this object does not contain a value.
     * @return a reference to the value stored within this object.
     */
    [[nodiscard]] type_t& value() {
        return *(this->operator->());
    }

    /**
     * @return true if this object contains an error and false otherwise.
     */
    [[nodiscard]] bool has_error() const {
        return this->error_ != nullptr;
    }

    /**
     * @return a copy of the value stored within this object or a generic
     * success message if this object does not contain an error.
     */
    [[nodiscard]] std::string error() const {
        if (! this->has_error()) {
            return has_value_message;
        }

        return *(this->error_);
    }
};

template<typename type_t>
const std::string optional_t<type_t>::has_value_message = "Has value";

template<typename type_t>
const std::string optional_t<type_t>::bad_optional_access_message =
  "Attempted to access a value from a syst::optional_t that does not exist.";

} // namespace syst
