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
#include <list>
#include <string>
#include <optional>

// Local includes
#include "optional.hpp"

namespace syst {

namespace fs = std::filesystem;
namespace ch = std::chrono;

/**
 * @return the username of the owner of this process or std::nullopt if an error
 * occurred.
 */
syst::optional_t<std::string> username();

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

/**
 * @return system information from sysinfo or std::nullopt if an error occurred.
 */
[[nodiscard]] syst::optional_t<system_info_t> system_info();

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
     * @return all disk block devices on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<disk_t>> all();

    /**
     * @return all partitions associated with this disk block device or
     * std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<std::list<part_t>> parts() const;

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
     * @return the name of this device.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @return the size of this device in bytes or std::nullopt if the size
     * could not be determined.
     */
    [[nodiscard]] syst::optional_t<uint64_t> size() const;

    /**
     * @return true if this device is removable, false if it is not, and
     * std::nullopt if the removable status could not be determined.
     */
    [[nodiscard]] syst::optional_t<bool> removable() const;

    /**
     * @return true if this device is read-only, false if it is not, and
     * std::nullopt if the read-only status could not be determined.
     */
    [[nodiscard]] syst::optional_t<bool> read_only() const;

    /**
     * @return true if this device is rotational (HDD), false if it is not
     * (SSD), and std::nullopt if the rotational type could not be determined.
     */
    [[nodiscard]] syst::optional_t<bool> rotational() const;

    /**
     * @return in-flight statistics for this device or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] syst::optional_t<inflight_stat_t> inflight_stat() const;

    /**
     * @return I/O statistics for this device or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] syst::optional_t<io_stat_t> io_stat() const;
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
     * @return the name of this partition.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @return the disk corresponding to this partition.
     */
    [[nodiscard]] disk_t disk() const;

    /**
     * @return the size of this partition in bytes or std::nullopt if the size
     * could not be determined.
     */
    [[nodiscard]] syst::optional_t<uint64_t> size() const;

    /**
     * @return the start position of this partition on the disk corresponding to
     * this partition in bytes or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<uint64_t> start() const;

    /*
     * @return true if this partition is read-only, false if it is not, and
     * std::nullopt if the read-only status could not be determined.
     */
    [[nodiscard]] syst::optional_t<bool> read_only() const;

    /**
     * @return in-flight statistics for this device or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] syst::optional_t<inflight_stat_t> inflight_stat() const;

    /**
     * @return I/O statistics for this partition or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] syst::optional_t<io_stat_t> io_stat() const;
};

/**
 * @brief Stores CPU usage information. The 'update' method must be called at
 * least twice before calling the 'get' method.
 */
class cpu_usage_t {
    struct impl_t;
    std::unique_ptr<impl_t> impl_;

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
     * @return a result indicating success or failure.
     */
    [[nodiscard]] result_t update() const;

    /**
     * @brief Attempt to calculate the total CPU usage percentage for this
     * system. The percentage is calculated by dividing the time not spent idle
     * by the total time elapsed between the last two update calls and
     * multiplying the result by 100.
     *
     * @return the total CPU usage percentage or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] syst::optional_t<double> get_total() const;

    /**
     * @brief Attempt to calculate the CPU usage percentage of each core for
     * this system. The percentages for each core are calculated by dividing the
     * time not spent idle by the total time elapsed between the last two update
     * calls and multiplying the result by 100.
     *
     * @return a dynamic array of doubles with each double representing the CPU
     * usage percentage of a specific core or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<std::list<double>> get_per_core() const;
};

/**
 * @brief Represents a device with thermal information, such as a temperature
 * sensor.
 */
class thermal_zone_t {
    fs::path sysfs_path_;

    thermal_zone_t(const fs::path& sysfs_path);

  public:
    /**
     * @return all thermal zones on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<thermal_zone_t>> all();

    /**
     * @return the path to this thermal zone in /sys. This provides access to
     * various zone-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the type of this thermal zone represented as a string or
     * std::nullopt if the type could not be determined.
     */
    [[nodiscard]] syst::optional_t<std::string> type() const;

    /**
     * @return the temperature at this thermal zone in degrees Celsius or
     * std::nullopt if the temperature could not be determined.
     */
    [[nodiscard]] syst::optional_t<double> temperature() const;
};

/**
 * @brief Represents a thermal management device, such as a fan.
 */
class cooling_device_t {
    fs::path sysfs_path_;

    cooling_device_t(const fs::path& sysfs_path);

  public:
    /**
     * @return all cooling devices on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<cooling_device_t>> all();

    /**
     * @return the path to this cooling device in /sys. This provides access to
     * various zone-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the type of this cooling device represented as a string or
     * std::nullopt if the type could not be determined.
     */
    [[nodiscard]] syst::optional_t<std::string> type() const;

    /**
     * @return get the current state of this cooling device as a percentage (0 -
     * 100) or std::nullopt if the current state could not be determined.
     */
    [[nodiscard]] syst::optional_t<double> get_state() const;

    /**
     * @brief Attempt to set the state of this cooling device.
     *
     * This function requires root privileges.
     *
     * @param[in] state - The new state in percents (0 - 100).
     * @return a result indicating success or failure.
     */
    result_t set_state(double state);
};

class backlight_t {
    fs::path sysfs_path_;

    backlight_t(const fs::path& sysfs_path);

  public:
    /**
     * @return all backlights on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<backlight_t>> all();

    /**
     * @return the path to this backlight in /sys. This provides access to
     * various backlight-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the name of this backlight.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @brief Attempt to calculate the brightness percentage of this backlight.
     * The percentage is calculated by dividing the current brightness by the
     * maximum brightness and multiplying the result by 100.
     *
     * @return the brightness percentage or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<double> get_brightness() const;

    /**
     * @brief Attempt to set the brightness percentage of this backlight.
     *
     * @return a result indicating success or failure.
     */
    syst::result_t set_brightness(double brightness);
};

/**
 * @brief Represents a battery connected to this system.
 */
class battery_t {
    fs::path sysfs_path_;

    battery_t(const fs::path& sysfs_path);

  public:
    enum class status_t {
        unknown,
        charging,
        discharging,
        not_charging,
        full,
    };

    /**
     * @return all batteries on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<battery_t>> all();

    /**
     * @return the path to this battery in /sys. This provides access to
     * various battery-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the name of this battery.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @return the current status of this battery or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] syst::optional_t<status_t> status() const;

    /**
     * @return the amount of current (in amperes) being drawn from this battery
     * or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<double> current() const;

    /**
     * @return the amount of power (in watts) being drawn from this battery or
     * std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<double> power() const;

    /**
     * @brief Attempt to calculate the current energy percentage of this
     * battery. This is calculated by dividing the current energy level by the
     * available energy capacity and multiplying by 100.
     *
     * @return the current energy percentage or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] syst::optional_t<double> charge() const;

    /**
     * @brief Attempt to calculate the percentage of energy still storable
     * within this battery compared to when this battery was new. This
     * percentage is calculated by dividing the current energy capacity by the
     * original energy capacity given by the manufacturer and multiplying by
     * 100.
     *
     * @return the capacity percentage or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<double> capacity() const;

    /**
     * @brief If this battery is discharging, attempt to calculate the number of
     * seconds until this battery is empty. If this battery is charging, attempt
     * to calculate the number of seconds until this battery is full.
     *
     * @return the number of seconds until this battery is empty or full
     * (depending on charge status) or std::nullopt if the time remaining could
     * not be determined.
     */
    [[nodiscard]] syst::optional_t<ch::seconds> time_remaining() const;
};

/**
 * @brief Represents a network interface (either physical and virtual) on this
 * system.
 */
class network_interface_t {
    fs::path sysfs_path_;

    network_interface_t(const fs::path& sysfs_path);

  public:
    enum class status_t {
        unknown,
        up,
        dormant,
        down,
    };

    struct stat_t {
        uint64_t bytes_down;
        uint64_t bytes_up;
        uint64_t packets_down;
        uint64_t packets_up;
    };

    /**
     * @return all network interfaces on this system or std::nullopt if an error
     * occurred.
     */
    [[nodiscard]] static syst::optional_t<std::list<network_interface_t>> all();

    /**
     * @return the path to this network interface in /sys. This provides access
     * to various interface-specific information exposed by the kernel.
     */
    [[nodiscard]] fs::path sysfs_path() const;

    /**
     * @return the name of this network interface.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @return true if this network interface represents a physical device,
     * false if it is virtual, and std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<bool> physical() const;

    /**
     * @return true if this network interface is a loopback device, false
     * otherwise, and std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<bool> loopback() const;

    /**
     * @brief Attempt to get the current status of this network interface.
     *
     * @return the current status or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<status_t> status() const;

    /**
     * @brief Attempt to get the statistics for transmitted and received data
     * for this network interface.
     *
     * @return the interface statistics or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<stat_t> stat() const;
};

class sound_control_t;

/**
 * @brief Represents a sound mixer device, which is used to access sound control
 * elements.
 */
class sound_mixer_t {
    struct impl_t;
    std::unique_ptr<impl_t> impl_;

    sound_mixer_t(const impl_t& impl);

  public:
    sound_mixer_t(const sound_mixer_t&) = delete;
    // Classes with custom destructors must also have custom move constructors.
    sound_mixer_t(sound_mixer_t&& sound_mixer) noexcept;
    sound_mixer_t& operator=(const sound_mixer_t&) = delete;
    sound_mixer_t& operator=(sound_mixer_t&&) noexcept = default;
    // The destructor must be implemented where 'impl' is defined.
    ~sound_mixer_t();

    /**
     * @return a new sound mixer object or std::nullopt if an error occurred.
     */
    [[nodiscard]] static syst::optional_t<sound_mixer_t> get();

    /**
     * @return all active sound control elements.
     */
    [[nodiscard]] std::list<sound_control_t> all_controls() const;
};

/**
 * @brief Represents a sound control element, which manages the volume and
 * status of an individual playback/capture device.
 */
class sound_control_t {
    struct impl_t;
    std::unique_ptr<impl_t> impl_;

    sound_control_t(const impl_t& impl);

    // Some classes need to access the private constructor for this class, but
    // not all classes need access.
    friend sound_mixer_t;

    template<typename type_t>
    struct state_t {
        type_t front_left;
        type_t front_right;
        type_t rear_left;
        type_t rear_right;
        type_t front_center;
        type_t woofer;
        type_t side_left;
        type_t side_right;
        type_t rear_center;
    };

  public:
    using status_t = state_t<std::optional<bool>>;
    using volume_t = state_t<std::optional<double>>;

    sound_control_t(const sound_control_t&) = delete;
    sound_control_t(sound_control_t&&) noexcept = default;
    sound_control_t& operator=(const sound_control_t&) = delete;
    sound_control_t& operator=(sound_control_t&&) noexcept = default;
    // The destructor must be implemented where 'impl' is defined.
    ~sound_control_t();

    /**
     * @return the name of this sound control element.
     */
    [[nodiscard]] std::string name() const;

    /**
     * @return true if this control has a playback status and false otherwise.
     */
    [[nodiscard]] bool has_playback_status() const;

    /**
     * @return true if this control has a playback volume and false
     * otherwise.
     */
    [[nodiscard]] bool has_playback_volume() const;

    /**
     * @return true if this control has a capture status and false otherwise.
     */
    [[nodiscard]] bool has_capture_status() const;

    /**
     * @return true if this control has a capture volume and false
     * otherwise.
     */
    [[nodiscard]] bool has_capture_volume() const;

    /**
     * @return the current playback status or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<status_t> get_playback_status() const;

    /**
     * @return the current playback volume as a percentage or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] syst::optional_t<volume_t> get_playback_volume() const;

    /**
     * @return the current capture status or std::nullopt if an error occurred.
     */
    [[nodiscard]] syst::optional_t<status_t> get_capture_status() const;

    /**
     * @return the current capture volume as a percentage or std::nullopt if an
     * error occurred.
     */
    [[nodiscard]] syst::optional_t<volume_t> get_capture_volume() const;

    /**
     * @brief Attempt to set the playback status.
     *
     * @param[in] status - The new playback status.
     * @return a result indicating success or failure.
     */
    result_t set_playback_status(const status_t& status);

    /**
     * @brief Attempt to set the playback status for all channels.
     *
     * @param[in] status - The new playback status.
     * @return a result indicating success or failure.
     */
    result_t set_playback_status_all(bool status);

    /**
     * @brief Attempt to toggle the playback status of each individual channel.
     *
     * @return a result indicating success or failure.
     */
    result_t toggle_playback_status();

    /**
     * @brief Attempt to set the playback volume.
     *
     * @param[in] volume - The new playback volume.
     * @return a result indicating success or failure.
     */
    result_t set_playback_volume(const volume_t& volume);

    /**
     * @brief Attempt to set the playback volume for all channels.
     *
     * @param[in] volume - The new playback volume.
     * @return a result indicating success or failure.
     */
    result_t set_playback_volume_all(double volume);

    /**
     * @brief Attempt to set the capture status.
     *
     * @param[in] status - The new capture status.
     * @return a result indicating success or failure.
     */
    result_t set_capture_status(const status_t& status);

    /**
     * @brief Attempt to set the capture status for all channels.
     *
     * @param[in] status - The new capture status.
     * @return a result indicating success or failure.
     */
    result_t set_capture_status_all(bool status);

    /**
     * @brief Attempt to toggle the capture status of each individual channel.
     *
     * @return a result indicating success or failure.
     */
    result_t toggle_capture_status();

    /**
     * @brief Attempt to set the capture volume.
     *
     * @param[in] volume - The new capture volume in percents.
     * @return a result indicating success or failure.
     */
    result_t set_capture_volume(const volume_t& volume);

    /**
     * @brief Attempt to set the capture volume for all channels.
     *
     * @param[in] volume - The new capture volume.
     * @return a result indicating success or failure.
     */
    result_t set_capture_volume_all(double volume);
};

/**
 * @return the release version of the currently running kernel or std::nullopt
 * if an error occurred.
 */
[[nodiscard]] syst::optional_t<std::string> get_running_kernel();

/**
 * @return the release versions of all installed kernels or std::nullopt if an
 * error occurred.
 */
[[nodiscard]] syst::optional_t<std::list<std::string>> get_installed_kernels();

} // namespace syst
