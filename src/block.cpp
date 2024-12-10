// Standard includes
#include <fstream>

// Local includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"
#include "util.hpp"

namespace syst {

[[nodiscard]] std::optional<fs::path> devfs_path(const fs::path& sysfs_path) {
    fs::path devfs_path = "/dev" / sysfs_path.filename();

    if (! fs::is_block_file(devfs_path)) {
        syst::error =
          "The path to the devfs block device corresponding to a sysfs block "
          "device does not exist or is not a block file.\ndevfs: '"
          + devfs_path.string() + "'\nsysfs: '" + sysfs_path.string() + "'";
        return std::nullopt;
    }

    return devfs_path;
}

[[nodiscard]] std::optional<uint64_t> size(const fs::path& sysfs_path) {
    auto size = get_int(sysfs_path / "size");
    if (! size.has_value()) {
        // get_int sets syst::error
        return std::nullopt;
    }

    const uint64_t bytes_per_sector = 512; // UNIX sectors
    size.value() *= bytes_per_sector;

    return size;
}

[[nodiscard]] std::optional<bool> read_only(const fs::path& sysfs_path) {
    return get_bool(sysfs_path / "ro"); // get_bool sets syst:error
}

[[nodiscard]] std::optional<inflight_stat_t> inflight_stat(
  const fs::path& sysfs_path) {
    // documentation for /sys/block/<dev>/inflight
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const fs::path inflight_path = sysfs_path / "inflight";
    if (! fs::is_regular_file(inflight_path)) {
        syst::error = "The path is not a regular file.\npath: '"
          + inflight_path.string() + "'";
        return std::nullopt;
    }

    inflight_stat_t inflight_stat{};

    std::ifstream file{ inflight_path };

    if ((file >> inflight_stat.reads).fail()) {
        syst::error = "Failed to read the 'reads' statistic from the inflight "
                      "statistics file.\nfile: '"
          + inflight_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> inflight_stat.writes).fail()) {
        syst::error = "Failed to read the 'writes' statistic from the inflight "
                      "statistics file.\nfile: '"
          + inflight_path.string() + "'";
        return std::nullopt;
    }

    return inflight_stat;
}

[[nodiscard]] std::optional<io_stat_t> io_stat(
  const fs::path& sysfs_path, const fs::path& disk_sysfs_path) {
    // documentation for /sys/block/<dev>/stat
    //     https://www.kernel.org/doc/html/latest/block/stat.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const fs::path io_stat_status_path = disk_sysfs_path / "queue/iostats";
    const auto io_stat_status = get_bool(io_stat_status_path);
    if (! io_stat_status.has_value()) {
        // get_bool sets syst::error
        return std::nullopt;
    }
    if (! io_stat_status.value()) {
        syst::error = "The I/O statistics file is disabled. Write '1' to the "
                      "I/O statistics status file to enable it.\nfile: '"
          + io_stat_status_path.string() + "'";
        return std::nullopt;
    }

    const fs::path io_stat_path = sysfs_path / "stat";
    if (! fs::is_regular_file(io_stat_path)) {
        syst::error = "The path is not a regular file.\npath: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }

    io_stat_t io_stat{};

    uint64_t temp_int = 0;

    std::ifstream file{ io_stat_path };

    if ((file >> io_stat.reads_completed).fail()) {
        syst::error = "Failed to read the 'reads_completed' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.reads_merged).fail()) {
        syst::error = "Failed to read the 'reads_merged' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.sectors_read).fail()) {
        syst::error = "Failed to read the 'sectors_read' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> temp_int).fail()) {
        syst::error = "Failed to read the 'time_by_reads' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    io_stat.time_by_reads = ch::milliseconds(temp_int);
    if ((file >> io_stat.writes_completed).fail()) {
        syst::error =
          "Failed to read the 'writes_completed' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.writes_merged).fail()) {
        syst::error = "Failed to read the 'writes_merged' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.sectors_written).fail()) {
        syst::error = "Failed to read the 'sectors_written' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> temp_int).fail()) {
        syst::error = "Failed to read the 'time_by_writes' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    io_stat.time_by_writes = ch::milliseconds(temp_int);
    if ((file >> io_stat.io_in_flight).fail()) {
        syst::error = "Failed to read the 'io_in_flight' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> temp_int).fail()) {
        syst::error =
          "Failed to read the 'time_spent_queued' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    io_stat.time_spent_queued = ch::milliseconds(temp_int);
    if ((file >> temp_int).fail()) {
        syst::error =
          "Failed to read the 'time_by_queued_io' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    io_stat.time_by_queued_io = ch::milliseconds(temp_int);
    if ((file >> io_stat.discards_completed).fail()) {
        syst::error =
          "Failed to read the 'discards_completed' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.discards_merged).fail()) {
        syst::error = "Failed to read the 'discards_merged' statistic from the "
                      "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> io_stat.sectors_discarded).fail()) {
        syst::error =
          "Failed to read the 'sectors_discarded' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    if ((file >> temp_int).fail()) {
        syst::error =
          "Failed to read the 'time_by_discards' statistic from the "
          "I/O statistics file.\nfile: '"
          + io_stat_path.string() + "'";
        return std::nullopt;
    }
    io_stat.time_by_discards = ch::milliseconds(temp_int);

    return io_stat;
}

disk_t::disk_t(const fs::path& sysfs_path, const fs::path& devfs_path)
: sysfs_path_(sysfs_path), devfs_path_(devfs_path) {
}

std::optional<std::list<disk_t>> disk_t::all() {
    // documentation for /sys/block/
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/blk_types.h
    //     https://docs.kernel.org/block/index.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    std::list<disk_t> disks;

    const std::string blocks_path = "/sys/block";

    if (! fs::is_directory(blocks_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + blocks_path + "'";
        return std::nullopt;
    }

    for (const auto& block : fs::directory_iterator(blocks_path)) {
        if (! fs::is_symlink(block)) {
            syst::error = "The path is not symbolic link.\npath: '"
              + block.path().string() + "'";
            return std::nullopt;
        }

        const fs::path& sysfs_path = block.path();

        const auto devfs_path = syst::devfs_path(sysfs_path);
        if (! devfs_path.has_value()) {
            // devfs_path sets syst::error
            return std::nullopt;
        }

        disks.push_back(disk_t{ sysfs_path, devfs_path.value() });
    }

    return disks;
}

std::optional<std::list<part_t>> disk_t::parts() const {
    std::list<part_t> parts;

    const std::string blocks_path = "/sys/class/block";
    if (! fs::is_directory(blocks_path)) {
        syst::error =
          "The path is not a directory.\npath: '" + blocks_path + "'";
        return std::nullopt;
    }

    for (const auto& block : fs::directory_iterator(blocks_path)) {
        const fs::path disk_name = this->sysfs_path_.filename();
        const fs::path part_name = block.path().filename();
        if (disk_name == part_name) {
            // Ignore this disk (it's a disk, not a partition).
            continue;
        }
        if (! has_prefix(part_name.string(), disk_name.string())) {
            // Ignore block devices that are not associated with this disk.
            continue;
        }

        if (! fs::is_symlink(block)) {
            syst::error = "The path is not a symbolic link.\npath: '"
              + block.path().string() + "'";
            return std::nullopt;
        }

        if (! fs::is_regular_file(block.path() / "partition")) {
            // Ignore block devices that are not partitions.
            continue;
        }

        const fs::path& sysfs_path = block.path();

        const auto devfs_path = syst::devfs_path(sysfs_path);
        if (! devfs_path.has_value()) {
            // devfs_path sets syst::error
            return std::nullopt;
        }

        parts.push_back(part_t{ sysfs_path,
          devfs_path.value(),
          this->sysfs_path_,
          this->devfs_path_ });
    }

    return parts;
}

fs::path disk_t::sysfs_path() const {
    return this->sysfs_path_;
}

fs::path disk_t::devfs_path() const {
    return this->devfs_path_;
}

std::optional<uint64_t> disk_t::size() const {
    return syst::size(this->sysfs_path_); // size sets syst::error
}

std::optional<bool> disk_t::removable() const {
    return get_bool(
      this->sysfs_path_ / "removable"); // get_bool sets syst::error
}

std::optional<bool> disk_t::read_only() const {
    return syst::read_only(this->sysfs_path_); // read_only sets syst::error
}

std::optional<bool> disk_t::rotational() const {
    return get_bool(
      this->sysfs_path_ / "queue/rotational"); // get_bool sets syst::error
}

std::optional<inflight_stat_t> disk_t::inflight_stat() const {
    return syst::inflight_stat(
      this->sysfs_path_); // inflight_stat sets syst::error
}

std::optional<io_stat_t> disk_t::io_stat() const {
    return syst::io_stat(
      this->sysfs_path_, this->sysfs_path_); // io_stat sets syst::error
}

part_t::part_t(const fs::path& sysfs_path,
  const fs::path& devfs_path,
  const fs::path& disk_sysfs_path,
  const fs::path& disk_devfs_path)
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
    return syst::size(this->sysfs_path_); // size sets syst::error
}

std::optional<bool> part_t::read_only() const {
    return syst::read_only(this->sysfs_path_); // read_only sets syst::error
}

std::optional<inflight_stat_t> part_t::inflight_stat() const {
    return syst::inflight_stat(
      this->sysfs_path_); // inflight_stat sets syst::error
}

std::optional<io_stat_t> part_t::io_stat() const {
    return syst::io_stat(
      this->sysfs_path_, this->disk_sysfs_path_); // io_stat sets syst::error
}

} // namespace syst
