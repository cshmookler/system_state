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
 * @file core.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Core utilities for fetching and modifying the system state.
 * @date 2024-11-27
 */

// Standard includes
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <list>

namespace syst {

namespace fs = std::filesystem;
namespace ch = std::chrono;

struct system_info_t {
    // The number of seconds since the system booted.
    ch::seconds uptime;

    // The system load average over the last one minute.
    double load_1;

    // The system load average over the last five minutes.
    double load_5;

    // The system load average over the last fifteen minutes.
    double load_15;

    // Total addressable RAM in bytes (minus reserved bits and the kernel
    // binary code).
    uint64_t ram_total;

    // Free addressable RAM in bytes.
    uint64_t ram_free;

    // Shared addressable RAM in bytes.
    uint64_t ram_shared;

    // Buffered addressable RAM in bytes.
    uint64_t ram_buffered;

    // Total swap space in bytes.
    uint64_t swap_total;

    // Free swap space in bytes.
    uint64_t swap_free;

    // The number of currently running processes.
    uint64_t procs;

    // RAM usage percentage.
    // The ratio of free RAM to total RAM multiplied by 100.
    double ram_usage;

    // Swap usage percentage.
    // The ratio of free swap to total swap multiplied by 100.
    double swap_usage;
};

[[nodiscard]] std::optional<system_info_t> system_info();

struct inflight_stat_t {
    // The number of in-flight read requests for this device.
    uint64_t reads;

    // The number of in-flight write requests for this device.
    uint64_t writes;
};

struct io_stat_t {
    // The number of read requests that have completed successfully.
    uint64_t reads_completed;

    // The number of read requests merged with an already-queued I/O
    // request.
    uint64_t reads_merged;

    // The number of 512-byte UNIX sectors that have been read from.
    uint64_t sectors_read;

    // The number of milliseconds that read requests have waited on this
    // device multiplied by the number of waiting read requests.
    ch::milliseconds time_by_reads;

    // The number of write requests that have completed successfully.
    uint64_t writes_completed;

    // The number of write requests merged with an already-queued I/O
    // request.
    uint64_t writes_merged;

    // The number of 512-byte UNIX sectors that have been written to.
    uint64_t sectors_written;

    // The number of milliseconds that write requests have waited on this
    // device multiplied by the number of waiting write requests.
    ch::milliseconds time_by_writes;

    // The number of I/O requests that have been issued but not yet
    // completed.
    uint64_t io_in_flight;

    // The number of milliseconds that I/O requests have waited on this
    // device.
    ch::milliseconds time_spent_queued;

    // The number of milliseconds that I/O requests have waited on this
    // device multiplied by the number of waiting I/O requests.
    ch::milliseconds time_by_queued_io;

    // The number of discard requests that have completed successfully.
    uint64_t discards_completed;

    // The number of discard requests merged with an already-queued I/O
    // request.
    uint64_t discards_merged;

    // The number of 512-byte UNIX sectors that have been discarded.
    uint64_t sectors_discarded;

    // The number of milliseconds that discard requests have waited on this
    // device multiplied by the number of waiting discard requests.
    ch::milliseconds time_by_discards;
};

class part_t;

/**
 * @brief Represents a disk block device on this system.
 */
class disk_t {
    fs::path sysfs_path_;
    fs::path devfs_path_;

    disk_t(const fs::path& sysfs_path, const fs::path& devfs_path);

    // Some classes need to access the private constructor for this class, but
    // not all classes need access.
    friend part_t;

  public:
    /**
     * @return all disk block devices on this system.
     */
    [[nodiscard]] static std::optional<std::list<disk_t>> all();

    /**
     * @return all partitions associated with this disk block device.
     */
    [[nodiscard]] std::optional<std::list<part_t>> parts() const;

    /**
     * @return the path to this device in /sys. This provides access to various
     * device-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the path to this device in /dev. This provides access to the
     * device itself.
     */
    [[nodiscard]] fs::path devfs_path() const;

    /**
     * @return the size of this device in bytes or std::nullopt if the size
     * could not be determined.
     */
    [[nodiscard]] std::optional<uint64_t> size() const;

    /**
     * @return true if this device is removable, false if it is not, and
     * std::nullopt if the removable status could not be determined.
     */
    [[nodiscard]] std::optional<bool> removable() const;

    /**
     * @return true if this device is read-only, false if it is not, and
     * std::nullopt if the read-only status could not be determined.
     */
    [[nodiscard]] std::optional<bool> read_only() const;

    /**
     * @return true if this device is rotational (HDD), false if it is not
     * (SSD), and std::nullopt if the rotational type could not be determined.
     */
    [[nodiscard]] std::optional<bool> rotational() const;

    /**
     * @return in-flight statistics for this device or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] std::optional<inflight_stat_t> inflight_stat() const;

    /**
     * @return I/O statistics for this device or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] std::optional<io_stat_t> io_stat() const;
};

/**
 * @brief Represents a partition block device on this system.
 */
class part_t {
    fs::path sysfs_path_;
    fs::path devfs_path_;
    fs::path disk_sysfs_path_;
    fs::path disk_devfs_path_;

    part_t(const fs::path& sysfs_path,
      const fs::path& devfs_path,
      const fs::path& disk_sysfs_path,
      const fs::path& disk_devfs_path);

    // Some classes need to access the private constructor for this class, but
    // not all classes need access.
    friend disk_t;

  public:
    /**
     * @return the path to this partition in /sys. This provides access to
     * various partition-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the path to this partition in /dev. This provides access to the
     * partition itself.
     */
    [[nodiscard]] fs::path devfs_path() const;

    /**
     * @return the disk corresponding to this partition.
     */
    [[nodiscard]] disk_t disk() const;

    /**
     * @return the size of this partition in bytes or std::nullopt if the size
     * could not be determined.
     */
    [[nodiscard]] std::optional<uint64_t> size() const;

    /*
     * @return true if this partition is read-only, false if it is not, and
     * std::nullopt if the read-only status could not be determined.
     */
    [[nodiscard]] std::optional<bool> read_only() const;

    /**
     * @return in-flight statistics for this device or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] std::optional<inflight_stat_t> inflight_stat() const;

    /**
     * @return I/O statistics for this partition or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] std::optional<io_stat_t> io_stat() const;
};

/**
 * @brief Stores CPU usage information. The 'update' method must be called at
 * least twice before calling the 'get' method.
 */
class cpu_usage_t {
    struct impl;
    std::unique_ptr<impl> impl_;

  public:
    cpu_usage_t();
    cpu_usage_t(const cpu_usage_t&) = delete;
    cpu_usage_t(cpu_usage_t&&) noexcept = default;
    cpu_usage_t& operator=(const cpu_usage_t&) = delete;
    cpu_usage_t& operator=(cpu_usage_t&&) noexcept = default;
    // The destructor must be implemented where 'impl' is defined.
    ~cpu_usage_t();

    /**
     * @brief Attempt to update the CPU usage statistics.
     *
     * @return true if the statistics were successfully updated and false
     * otherwise.
     */
    [[nodiscard]] bool update() const;

    /**
     * @brief Attempt to calculate the total CPU usage percentage for this
     * system. The percentage is calculated by dividing the time spent idle by
     * the total time elapsed between the last two update calls and multiplying
     * the result by 100.
     *
     * @return the total CPU usage percentage or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] std::optional<double> get() const;
};

class backlight_t {
    fs::path sysfs_path_;

    backlight_t(const fs::path& sysfs_path);

  public:
    /**
     * @return all backlights on this system.
     */
    [[nodiscard]] static std::optional<std::list<backlight_t>> all();

    /**
     * @return the path to this backlight in /sys. This provides access to
     * various backlight-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @brief Attempt to calculate the brightness percentage of this backlight.
     * The percentage is calculated by dividing the current brightness by the
     * maximum brightness and multiplying the result by 100.
     *
     * @return the brightness percentage or std::nullopt if an error occurred.
     */
    [[nodiscard]] std::optional<double> brightness() const;
};

} // namespace syst