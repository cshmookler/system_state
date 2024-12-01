// Standard includes
#include <fstream>
#include <string_view>

// Local includes
#include "../system_state/core.hpp"
#include "util.hpp"

namespace syst {

[[nodiscard]] std::optional<fs::path> devfs_path(const fs::path& sysfs_path) {
    fs::path devfs_root_path = "/dev";
    fs::path devfs_path = devfs_root_path / sysfs_path.filename();

    if (! fs::is_block_file(devfs_path)) {
        // The path to the devfs device must exist.
        return std::nullopt;
    }

    return std::make_optional<fs::path>(std::move(devfs_path));
}

[[nodiscard]] std::optional<uint64_t> size(const fs::path& sysfs_path) {
    const std::string_view size_filename = "size";
    fs::path size_path = sysfs_path / size_filename;

    std::optional<uint64_t> size = get_int(size_path);

    const uint64_t bytes_per_sector = 512; // UNIX sectors
    size.value() *= bytes_per_sector;

    return size;
}

[[nodiscard]] std::optional<bool> read_only(const fs::path& sysfs_path) {
    const std::string_view read_only_filename = "ro";
    fs::path read_only_path = sysfs_path / read_only_filename;

    return get_bool(read_only_path);
}

[[nodiscard]] std::optional<inflight_stat_t> inflight_stat(
  const fs::path& sysfs_path) {
    // documentation for /sys/block/<dev>/inflight
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const std::string_view inflight_filename = "inflight";
    fs::path inflight_path = sysfs_path / inflight_filename;

    if (! fs::is_regular_file(inflight_path)) {
        // The in-flight statistics file must exist.
        return std::nullopt;
    }

    inflight_stat_t inflight_stat{};

    std::ifstream file{ inflight_path };

    if (! (file >> inflight_stat.reads)) {
        return std::nullopt;
    }
    if (! (file >> inflight_stat.writes)) {
        return std::nullopt;
    }

    return inflight_stat;
}

[[nodiscard]] std::optional<io_stat_t> io_stat(
  const fs::path& sysfs_path, const fs::path& disk_sysfs_path) {
    // documentation for /sys/block/<dev>/stat
    //     https://www.kernel.org/doc/html/latest/block/stat.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const std::string_view queue_path = "queue";
    const std::string_view io_stat_status_filename = "iostats";
    fs::path io_stat_status_path =
      disk_sysfs_path / queue_path / io_stat_status_filename;

    std::optional<bool> io_stat_status = get_bool(io_stat_status_path);

    if ((! io_stat_status.has_value()) || (! io_stat_status.value())) {
        // I/O statistics must be enabled.
        return std::nullopt;
    }

    const std::string_view io_stat_filename = "stat";
    fs::path io_stat_path = sysfs_path / io_stat_filename;

    if (! fs::is_regular_file(io_stat_path)) {
        // The I/O statistics file must exist.
        return std::nullopt;
    }

    io_stat_t io_stat{};

    uint64_t temp_int = 0;

    std::ifstream file{ io_stat_path };

    if (! (file >> io_stat.reads_completed)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.reads_merged)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.sectors_read)) {
        return std::nullopt;
    }
    if (! (file >> temp_int)) {
        return std::nullopt;
    }
    io_stat.time_by_reads = ch::milliseconds(temp_int);
    if (! (file >> io_stat.writes_completed)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.writes_merged)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.sectors_written)) {
        return std::nullopt;
    }
    if (! (file >> temp_int)) {
        return std::nullopt;
    }
    io_stat.time_by_writes = ch::milliseconds(temp_int);
    if (! (file >> io_stat.io_in_flight)) {
        return std::nullopt;
    }
    if (! (file >> temp_int)) {
        return std::nullopt;
    }
    io_stat.time_spent_queued = ch::milliseconds(temp_int);
    if (! (file >> temp_int)) {
        return std::nullopt;
    }
    io_stat.time_by_queued_io = ch::milliseconds(temp_int);
    if (! (file >> io_stat.discards_completed)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.discards_merged)) {
        return std::nullopt;
    }
    if (! (file >> io_stat.sectors_discarded)) {
        return std::nullopt;
    }
    if (! (file >> temp_int)) {
        return std::nullopt;
    }
    io_stat.time_by_discards = ch::milliseconds(temp_int);

    return io_stat;
}

disk_t::disk_t(const fs::path& sysfs_path, const fs::path& devfs_path) noexcept
: sysfs_path_(sysfs_path), devfs_path_(devfs_path) {
}

std::optional<std::list<disk_t>> disk_t::all() {
    // documentation for /sys/block/
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/blk_types.h
    //     https://docs.kernel.org/block/index.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    std::list<disk_t> disks;

    const std::string_view devices_path = "/sys/block";

    if (! fs::is_directory(devices_path)) {
        // The path to the devices directory must exist.
        return std::nullopt;
    }

    for (const auto& device : fs::directory_iterator(devices_path)) {
        const fs::path& sysfs_path = device.path();

        std::optional<fs::path> devfs_path = ::syst::devfs_path(sysfs_path);
        if (! devfs_path.has_value()) {
            // The path to the devfs device must be valid.
            return std::nullopt;
        }

        disks.push_back(disk_t{ sysfs_path, devfs_path.value() });
    }

    return std::optional<std::list<disk_t>>{ std::move(disks) };
}

std::optional<std::list<part_t>> disk_t::parts() const {
    std::list<part_t> parts;

    const std::string_view devices_path = "/sys/class/block";

    if (! fs::is_directory(devices_path)) {
        // The path to the devices directory must exist.
        return std::nullopt;
    }

    for (const auto& device : fs::directory_iterator(devices_path)) {
        const fs::path disk_name = this->sysfs_path_.filename();
        const fs::path part_name = device.path().filename();
        if (disk_name == part_name) {
            // Ignore this disk (it's a disk, not a partition).
            continue;
        }
        if (part_name.string().rfind(disk_name, 0) != 0) {
            // Ignore block devices that are not associated with this disk.
            continue;
        }

        const std::string_view device_partition_filename = "partition";
        if (! fs::is_regular_file(device.path() / device_partition_filename)) {
            // Ignore block devices that are not partitions.
            continue;
        }

        const fs::path& sysfs_path = device.path();

        std::optional<fs::path> devfs_path = ::syst::devfs_path(sysfs_path);
        if (! devfs_path.has_value()) {
            // The path to the devfs device must be valid.
            return std::nullopt;
        }

        parts.push_back(part_t{ sysfs_path,
          devfs_path.value(),
          this->sysfs_path_,
          this->devfs_path_ });
    }

    return std::optional<std::list<part_t>>{ std::move(parts) };
}

fs::path disk_t::sysfs_path() const {
    return this->sysfs_path_;
}

fs::path disk_t::devfs_path() const {
    return this->devfs_path_;
}

std::optional<uint64_t> disk_t::size() const {
    return ::syst::size(this->sysfs_path_);
}

std::optional<bool> disk_t::removable() const {
    const std::string_view removable_filename = "removable";
    fs::path removable_path = this->sysfs_path_ / removable_filename;

    return get_bool(removable_path);
}

std::optional<bool> disk_t::read_only() const {
    return ::syst::read_only(this->sysfs_path_);
}

std::optional<bool> disk_t::rotational() const {
    // documentation for /sys/block/<dev>/stat
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const std::string_view queue_path = "queue";
    const std::string_view rotational_filename = "rotational";
    fs::path rotational_path =
      this->sysfs_path_ / queue_path / rotational_filename;

    return get_bool(rotational_path);
}

std::optional<inflight_stat_t> disk_t::inflight_stat() const {
    return ::syst::inflight_stat(this->sysfs_path_);
}

std::optional<io_stat_t> disk_t::io_stat() const {
    return ::syst::io_stat(this->sysfs_path_, this->sysfs_path_);
}

part_t::part_t(const fs::path& sysfs_path,
  const fs::path& devfs_path,
  const fs::path& disk_sysfs_path,
  const fs::path& disk_devfs_path) noexcept
: sysfs_path_(sysfs_path)
, devfs_path_(devfs_path)
, disk_sysfs_path_(disk_sysfs_path)
, disk_devfs_path_(disk_devfs_path) {
}

fs::path part_t::sysfs_path() const {
    return this->sysfs_path_;
}

fs::path part_t::devfs_path() const {
    return this->devfs_path_;
}

disk_t part_t::disk() const {
    return disk_t{ this->disk_sysfs_path_, this->disk_devfs_path_ };
}

std::optional<uint64_t> part_t::size() const {
    return ::syst::size(this->sysfs_path_);
}

std::optional<bool> part_t::read_only() const {
    return ::syst::read_only(this->sysfs_path_);
}

std::optional<inflight_stat_t> part_t::inflight_stat() const {
    return ::syst::inflight_stat(this->sysfs_path_);
}

std::optional<io_stat_t> part_t::io_stat() const {
    return ::syst::io_stat(this->sysfs_path_, this->disk_sysfs_path_);
}

} // namespace syst
