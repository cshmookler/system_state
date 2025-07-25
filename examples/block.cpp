// Standard includes
#include <ios>
#include <iostream>

// External includes
#include "../system_state/system_state.hpp"

int main() {
    auto disks = syst::get_disks();
    if (disks.has_error()) {
        std::cerr << disks.error().string() << '\n';
        return 1;
    }

    std::cout << std::boolalpha; // Pretty print boolean values.

    for (const auto& disk : disks.value()) {
        std::cout << "sysfs path: " << disk.get_sysfs_path().string() << '\n';
        std::cout << "devfs path: " << disk.get_devfs_path().string() << '\n';
        std::cout << "Name: " << disk.get_name() << '\n';

        auto size = disk.get_size();
        if (size.has_value()) {
            std::cout << "Size: " << size.value() << " bytes" << '\n';
        } else {
            std::cerr << size.error().string() << '\n';
        }

        auto removable = disk.is_removable();
        if (removable.has_value()) {
            std::cout << "Removable: " << removable.value() << '\n';
        } else {
            std::cerr << removable.error().string() << '\n';
        }

        auto read_only = disk.is_read_only();
        if (read_only.has_value()) {
            std::cout << "Read-only: " << read_only.value() << '\n';
        } else {
            std::cerr << read_only.error().string() << '\n';
        }

        auto rotational = disk.is_rotational();
        if (rotational.has_value()) {
            std::cout << "Rotational: " << rotational.value() << '\n';
        } else {
            std::cerr << rotational.error().string() << '\n';
        }

        auto inflight_stat = disk.get_inflight_stat();
        if (inflight_stat.has_value()) {
            std::cout << "In-Flight Reads: " << inflight_stat->reads << '\n';
            std::cout << "In-Flight Writes: " << inflight_stat->writes << '\n';
        } else {
            std::cerr << inflight_stat.error().string() << '\n';
        }

        auto io_stat = disk.get_io_stat();
        if (io_stat.has_value()) {
            std::cout << "Reads Completed: " << io_stat->reads_completed
                      << '\n';
            std::cout << "Reads Merged: " << io_stat->reads_merged << '\n';
            std::cout << "Sectors Read: " << io_stat->sectors_read << '\n';
            std::cout << "Time by Reads: " << io_stat->time_by_reads.count()
                      << "ms" << '\n';
            std::cout << "Writes Completed: " << io_stat->writes_completed
                      << '\n';
            std::cout << "Writes Merged: " << io_stat->writes_merged << '\n';
            std::cout << "Sectors Written: " << io_stat->sectors_written
                      << '\n';
            std::cout << "Time by Writes: " << io_stat->time_by_writes.count()
                      << "ms" << '\n';
            std::cout << "I/O In-Flight: " << io_stat->io_in_flight << '\n';
            std::cout << "Time Spent Queued: "
                      << io_stat->time_spent_queued.count() << "ms" << '\n';
            std::cout << "Time by Queued I/O: "
                      << io_stat->time_by_queued_io.count() << "ms" << '\n';
            std::cout << "Discards Completed: " << io_stat->discards_completed
                      << '\n';
            std::cout << "Discards Merged: " << io_stat->discards_merged
                      << '\n';
            std::cout << "Sectors Discarded: " << io_stat->sectors_discarded
                      << '\n';
            std::cout << "Time by Discards: "
                      << io_stat->time_by_discards.count() << "ms" << '\n';
        } else {
            std::cerr << io_stat.error().string() << '\n';
        }

        std::cout << '\n';

        auto parts = disk.get_parts();
        if (parts.has_error()) {
            std::cerr << parts.error().string() << '\n';
            continue;
        }

        for (const auto& part : parts.value()) {
            std::cout << "sysfs path: " << part.get_sysfs_path().string()
                      << '\n';
            std::cout << "devfs path: " << part.get_devfs_path().string()
                      << '\n';
            std::cout << "Name: " << part.get_name() << '\n';
            std::cout << "Disk Name: " << part.get_disk().get_name() << '\n';

            auto part_size = part.get_size();
            if (part_size.has_value()) {
                std::cout << "Size: " << part_size.value() << " bytes" << '\n';
            } else {
                std::cerr << part_size.error().string() << '\n';
            }

            auto part_start = part.get_start_position();
            if (part_start.has_value()) {
                std::cout << "Start: " << part_start.value() << " bytes"
                          << '\n';
            } else {
                std::cerr << part_start.error().string() << '\n';
            }

            auto part_read_only = part.is_read_only();
            if (part_read_only.has_value()) {
                std::cout << "Read-only: " << part_read_only.value() << '\n';
            } else {
                std::cerr << part_read_only.error().string() << '\n';
            }

            auto part_inflight_stat = part.get_inflight_stat();
            if (part_inflight_stat.has_value()) {
                std::cout << "In-Flight Reads: " << part_inflight_stat->reads
                          << '\n';
                std::cout << "In-Flight Writes: " << part_inflight_stat->writes
                          << '\n';
            } else {
                std::cerr << part_inflight_stat.error().string() << '\n';
            }

            auto part_io_stat = disk.get_io_stat();
            if (part_io_stat.has_value()) {
                std::cout << "Reads Completed: "
                          << part_io_stat->reads_completed << '\n';
                std::cout << "Reads Merged: " << part_io_stat->reads_merged
                          << '\n';
                std::cout << "Sectors Read: " << part_io_stat->sectors_read
                          << '\n';
                std::cout << "Time by Reads: "
                          << part_io_stat->time_by_reads.count() << "ms"
                          << '\n';
                std::cout << "Writes Completed: "
                          << part_io_stat->writes_completed << '\n';
                std::cout << "Writes Merged: " << part_io_stat->writes_merged
                          << '\n';
                std::cout << "Sectors Written: "
                          << part_io_stat->sectors_written << '\n';
                std::cout << "Time by Writes: "
                          << part_io_stat->time_by_writes.count() << "ms"
                          << '\n';
                std::cout << "I/O In-Flight: " << part_io_stat->io_in_flight
                          << '\n';
                std::cout << "Time Spent Queued: "
                          << part_io_stat->time_spent_queued.count() << "ms"
                          << '\n';
                std::cout << "Time by Queued I/O: "
                          << part_io_stat->time_by_queued_io.count() << "ms"
                          << '\n';
                std::cout << "Discards Completed: "
                          << part_io_stat->discards_completed << '\n';
                std::cout << "Discards Merged: "
                          << part_io_stat->discards_merged << '\n';
                std::cout << "Sectors Discarded: "
                          << part_io_stat->sectors_discarded << '\n';
                std::cout << "Time by Discards: "
                          << part_io_stat->time_by_discards.count() << "ms"
                          << '\n';
            } else {
                std::cerr << part_io_stat.error().string() << '\n';
            }

            auto part_is_mounted = part.is_mounted();
            if (part_is_mounted.has_value()) {
                std::cout << "Mounted: " << part_is_mounted.value() << '\n';
            } else {
                std::cerr << part_is_mounted.error().string() << '\n';
            }

            auto part_mount_info = part.get_mount_info();
            if (part_mount_info.has_value()) {
                std::cout << "Mount Path: "
                          << part_mount_info->mount_path.string() << '\n';
                std::cout << "Filesystem Type: " << part_mount_info->fs_type
                          << '\n';
                std::cout << "Options: " << part_mount_info->options << '\n';
            } else {
                std::cerr << part_mount_info.error().string() << '\n';
            }

            std::cout << '\n';
        }
    }
}
