// Standard includes
#include <fstream>

// Local includes
#include "../system_state/system_state.hpp"
#include "util.hpp"

namespace syst {

[[nodiscard]] res::optional_t<fs::path> devfs_path(const fs::path& sysfs_path) {
    fs::path devfs_path = "/dev" / sysfs_path.filename();

    if (! fs::is_block_file(devfs_path)) {
        return RES_NEW_ERROR(
          "The path to the devfs block device corresponding to a sysfs block "
          "device does not exist or is not a block file.\n\tdevfs: '"
          + devfs_path.string() + "'\n\tsysfs: '" + sysfs_path.string() + "'");
    }

    return devfs_path;
}

[[nodiscard]] res::optional_t<uint64_t> size(const fs::path& sysfs_path) {
    auto size = syst::get_int(sysfs_path / "size");
    if (! size.has_value()) {
        return RES_TRACE(size.error());
    }

    const uint64_t bytes_per_sector = 512; // UNIX sectors
    size.value() *= bytes_per_sector;

    return size;
}

[[nodiscard]] res::optional_t<uint64_t> start(const fs::path& sysfs_path) {
    auto start = syst::get_int(sysfs_path / "start");
    if (! start.has_value()) {
        return RES_TRACE(start.error());
    }

    const uint64_t bytes_per_sector = 512; // UNIX sectors
    start.value() *= bytes_per_sector;

    return start;
}

[[nodiscard]] res::optional_t<bool> read_only(const fs::path& sysfs_path) {
    auto read_only = syst::get_bool(sysfs_path / "ro");

    if (read_only.has_error()) {
        return RES_TRACE(read_only.error());
    }

    return read_only;
}

[[nodiscard]] res::optional_t<inflight_stat_t> inflight_stat(
  const fs::path& sysfs_path) {
    // documentation for /sys/block/<dev>/inflight
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const fs::path inflight_path = sysfs_path / "inflight";
    if (! fs::is_regular_file(inflight_path)) {
        return RES_NEW_ERROR("The path is not a regular file.\n\tpath: '"
          + inflight_path.string() + "'");
    }

    inflight_stat_t inflight_stat{};

    std::ifstream file{ inflight_path };

    if ((file >> inflight_stat.reads).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'reads' statistic from the inflight "
          "statistics file.\n\tfile: '"
          + inflight_path.string() + "'");
    }
    if ((file >> inflight_stat.writes).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'writes' statistic from the inflight "
          "statistics file.\n\tfile: '"
          + inflight_path.string() + "'");
    }

    return inflight_stat;
}

[[nodiscard]] res::optional_t<io_stat_t> io_stat(
  const fs::path& sysfs_path, const fs::path& disk_sysfs_path) {
    // documentation for /sys/block/<dev>/stat
    //     https://www.kernel.org/doc/html/latest/block/stat.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    const fs::path io_stat_status_path = disk_sysfs_path / "queue/iostats";
    const auto io_stat_status = syst::get_bool(io_stat_status_path);
    if (io_stat_status.has_error()) {
        return RES_TRACE(io_stat_status.error());
    }
    if (io_stat_status.has_error()) {
        return RES_NEW_ERROR(
          "The I/O statistics file is disabled. Write '1' to the "
          "I/O statistics status file to enable it.\n\tfile: '"
          + io_stat_status_path.string() + "'");
    }

    const fs::path io_stat_path = sysfs_path / "stat";
    if (! fs::is_regular_file(io_stat_path)) {
        return RES_NEW_ERROR("The path is not a regular file.\n\tpath: '"
          + io_stat_path.string() + "'");
    }

    io_stat_t io_stat{};

    uint64_t temp_int = 0;

    std::ifstream file{ io_stat_path };

    if ((file >> io_stat.reads_completed).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'reads_completed' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.reads_merged).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'reads_merged' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.sectors_read).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'sectors_read' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> temp_int).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'time_by_reads' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    io_stat.time_by_reads = ch::milliseconds(temp_int);
    if ((file >> io_stat.writes_completed).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'writes_completed' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.writes_merged).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'writes_merged' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.sectors_written).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'sectors_written' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> temp_int).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'time_by_writes' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    io_stat.time_by_writes = ch::milliseconds(temp_int);
    if ((file >> io_stat.io_in_flight).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'io_in_flight' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> temp_int).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'time_spent_queued' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    io_stat.time_spent_queued = ch::milliseconds(temp_int);
    if ((file >> temp_int).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'time_by_queued_io' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    io_stat.time_by_queued_io = ch::milliseconds(temp_int);
    if ((file >> io_stat.discards_completed).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'discards_completed' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.discards_merged).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'discards_merged' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> io_stat.sectors_discarded).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'sectors_discarded' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    if ((file >> temp_int).fail()) {
        return RES_NEW_ERROR(
          "Failed to read the 'time_by_discards' statistic from the "
          "I/O statistics file.\n\tfile: '"
          + io_stat_path.string() + "'");
    }
    io_stat.time_by_discards = ch::milliseconds(temp_int);

    return io_stat;
}

disk_t::disk_t(const fs::path& sysfs_path, const fs::path& devfs_path)
: sysfs_path_(sysfs_path), devfs_path_(devfs_path) {
}

res::optional_t<std::vector<disk_t>> get_disks() {
    // documentation for /sys/block/
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h
    //     https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/blk_types.h
    //     https://docs.kernel.org/block/index.html
    //     https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-block

    std::vector<disk_t> disks;

    const std::string blocks_path = "/sys/block";

    if (! fs::is_directory(blocks_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + blocks_path + "'");
    }

    for (const auto& block : fs::directory_iterator(blocks_path)) {
        if (! fs::is_directory(block)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(block)) {
            // Ignore paths that are not symbolic links.
            continue;
        }

        const fs::path& sysfs_path = block.path();

        auto devfs_path = syst::devfs_path(sysfs_path);
        if (devfs_path.has_error()) {
            return RES_TRACE(devfs_path.error());
        }

        disks.push_back(disk_t{ sysfs_path, devfs_path.value() });
    }

    return disks;
}

res::optional_t<std::vector<part_t>> disk_t::get_parts() const {
    std::vector<part_t> parts;

    const std::string blocks_path = "/sys/class/block";
    if (! fs::is_directory(blocks_path)) {
        return RES_NEW_ERROR(
          "The path is not a directory.\n\tpath: '" + blocks_path + "'");
    }

    for (const auto& block : fs::directory_iterator(blocks_path)) {
        if (! fs::is_directory(block)) {
            // Ignore paths that are not directories.
            continue;
        }
        if (! fs::is_symlink(block)) {
            // Ignore paths that are not symbolic links.
            continue;
        }

        const fs::path disk_name = this->sysfs_path_.filename();
        const fs::path part_name = block.path().filename();
        if (disk_name == part_name) {
            // Ignore this disk (it's a disk, not a partition).
            continue;
        }
        if (! syst::has_prefix(part_name.string(), disk_name.string())) {
            // Ignore block devices that are not associated with this disk.
            continue;
        }

        if (! fs::is_regular_file(block.path() / "partition")) {
            // Ignore block devices that are not partitions.
            continue;
        }

        const fs::path& sysfs_path = block.path();

        auto devfs_path = syst::devfs_path(sysfs_path);
        if (! devfs_path.has_value()) {
            return RES_TRACE(devfs_path.error());
        }

        parts.push_back(part_t{ sysfs_path,
          devfs_path.value(),
          this->sysfs_path_,
          this->devfs_path_ });
    }

    return parts;
}

fs::path disk_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

fs::path disk_t::get_devfs_path() const {
    return this->devfs_path_;
}

std::string disk_t::get_name() const {
    return this->sysfs_path_.filename();
}

res::optional_t<uint64_t> disk_t::get_size() const {
    auto size = syst::size(this->sysfs_path_);

    if (size.has_error()) {
        return RES_TRACE(size.error());
    }

    return size;
}

res::optional_t<bool> disk_t::is_removable() const {
    auto removable = syst::get_bool(this->sysfs_path_ / "removable");

    if (removable.has_error()) {
        return RES_TRACE(removable.error());
    }

    return removable;
}

res::optional_t<bool> disk_t::is_read_only() const {
    auto read_only = syst::read_only(this->sysfs_path_);

    if (read_only.has_error()) {
        return RES_TRACE(read_only.error());
    }

    return read_only;
}

res::optional_t<bool> disk_t::is_rotational() const {
    auto rotational = syst::get_bool(this->sysfs_path_ / "queue/rotational");

    if (rotational.has_error()) {
        return RES_TRACE(rotational.error());
    }

    return rotational;
}

res::optional_t<inflight_stat_t> disk_t::get_inflight_stat() const {
    auto inflight_stat = syst::inflight_stat(this->sysfs_path_);

    if (inflight_stat.has_error()) {
        return RES_TRACE(inflight_stat.error());
    }

    return inflight_stat;
}

res::optional_t<io_stat_t> disk_t::get_io_stat() const {
    auto io_stat = syst::io_stat(this->sysfs_path_, this->sysfs_path_);

    if (io_stat.has_error()) {
        return RES_TRACE(io_stat.error());
    }

    return io_stat;
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

fs::path part_t::get_sysfs_path() const {
    return this->sysfs_path_;
}

fs::path part_t::get_devfs_path() const {
    return this->devfs_path_;
}

std::string part_t::get_name() const {
    return this->sysfs_path_.filename();
}

disk_t part_t::get_disk() const {
    return disk_t{ this->disk_sysfs_path_, this->disk_devfs_path_ };
}

res::optional_t<uint64_t> part_t::get_size() const {
    auto size = syst::size(this->sysfs_path_);

    if (size.has_error()) {
        return RES_TRACE(size.error());
    }

    return size;
}

res::optional_t<uint64_t> part_t::get_start_position() const {
    auto start = syst::start(this->sysfs_path_);

    if (start.has_error()) {
        return RES_TRACE(start.error());
    }

    return start;
}

res::optional_t<bool> part_t::is_read_only() const {
    auto read_only = syst::read_only(this->sysfs_path_);

    if (read_only.has_error()) {
        return RES_TRACE(read_only.error());
    }

    return read_only;
}

res::optional_t<inflight_stat_t> part_t::get_inflight_stat() const {
    auto inflight_stat = syst::inflight_stat(this->sysfs_path_);

    if (inflight_stat.has_error()) {
        return RES_TRACE(inflight_stat.error());
    }

    return inflight_stat;
}

res::optional_t<io_stat_t> part_t::get_io_stat() const {
    auto io_stat = syst::io_stat(this->sysfs_path_, this->disk_sysfs_path_);

    if (io_stat.has_error()) {
        return RES_TRACE(io_stat.error());
    }

    return io_stat;
}

res::optional_t<bool> part_t::is_mounted() const {
    // documentation for /proc/mounts
    //     man proc_pid_mounts
    //     https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/4/html-single/introduction_to_system_administration/index#s4-storage-mounting-proc

    std::string proc_mounts = "/proc/mounts";

    auto mounts = syst::get_all_lines(proc_mounts);
    if (! mounts.has_value()) {
        return RES_TRACE(mounts.error());
    }

    // Search for the devfs path of this partition in the mounts flie.
    // All paths in the mounts file end with a space ' '.
    auto devfs_path = this->devfs_path_.string() + ' ';

    for (const std::string& mount : mounts.value()) {
        if (! syst::has_prefix(mount, devfs_path)) {
            continue;
        }

        return true;
    }

    return false;
}

res::optional_t<mount_info_t> part_t::get_mount_info() const {
    // documentation for /proc/mounts
    //     man proc_pid_mounts
    //     https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/4/html-single/introduction_to_system_administration/index#s4-storage-mounting-proc

    std::string proc_mounts = "/proc/mounts";

    auto mounts = syst::get_all_lines(proc_mounts);
    if (! mounts.has_value()) {
        return RES_TRACE(mounts.error());
    }

    char space = ' ';

    // Search for the devfs path of this partition in the mounts flie.
    // All paths in the mounts file end with a space.
    std::string buffered_devfs_path = this->devfs_path_.string() + space;

    for (const std::string& mount : mounts.value()) {
        if (! syst::has_prefix(mount, buffered_devfs_path)) {
            continue;
        }

        std::string mount_info_str = mount.substr(buffered_devfs_path.size());

        // Isolate all fields by looking for spaces in reverse order.

        auto unused_2_pos = mount_info_str.rfind(space);
        if (unused_2_pos >= mount_info_str.size()) {
            return RES_NEW_ERROR(
              "Failed to find the 2nd unused field for a partition in '"
              + proc_mounts + "'.\n\tdevfs: '" + this->devfs_path_.string()
              + "'\n\tsysfs: '" + this->sysfs_path_.string()
              + "'\n\tmount_info_str: '" + mount_info_str + "'");
        }

        mount_info_str = mount_info_str.substr(0, unused_2_pos);

        auto unused_1_pos = mount_info_str.rfind(space);
        if (unused_1_pos >= mount_info_str.size()) {
            return RES_NEW_ERROR(
              "Failed to find the 1st unused field for a partition in '"
              + proc_mounts + "'.\n\tdevfs: '" + this->devfs_path_.string()
              + "'\n\tsysfs: '" + this->sysfs_path_.string()
              + "'\n\tmount_info_str: '" + mount_info_str + "'");
        }

        mount_info_str = mount_info_str.substr(0, unused_1_pos);

        auto mount_options_pos = mount_info_str.rfind(space);
        if (mount_options_pos >= mount_info_str.size()) {
            return RES_NEW_ERROR(
              "Failed to find the mount options for a partition in '"
              + proc_mounts + "'.\n\tdevfs: '" + this->devfs_path_.string()
              + "'\n\tsysfs: '" + this->sysfs_path_.string()
              + "'\n\tmount_info_str: '" + mount_info_str + "'");
        }

        mount_info_t mount_info{};

        mount_info.options = mount_info_str.substr(mount_options_pos + 1);
        mount_info_str = mount_info_str.substr(0, mount_options_pos);

        auto fs_type_pos = mount_info_str.rfind(space);
        if (fs_type_pos >= mount_info_str.size()) {
            return RES_NEW_ERROR(
              "Failed to find the filesystem type for a partition in '"
              + proc_mounts + "'.\n\tdevfs: '" + this->devfs_path_.string()
              + "'\n\tsysfs: '" + this->sysfs_path_.string()
              + "'\n\tmount_info_str: '" + mount_info_str + "'");
        }

        mount_info.fs_type = mount_info_str.substr(fs_type_pos + 1);
        mount_info.mount_path = mount_info_str.substr(0, fs_type_pos);

        return mount_info;
    }

    return RES_NEW_ERROR("Failed to get the mount path of a partition because "
                         "it is not mounted to the filesystem.\n\tdevfs: '"
      + this->devfs_path_.string() + "'\n\tsysfs: '"
      + this->sysfs_path_.string() + "'");
}

} // namespace syst
