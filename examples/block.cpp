// Standard includes
#include <ios>
#include <iostream>

// External includes
#include "../system_state/core.hpp"
#include "../system_state/error.hpp"

int main() {
    auto disks = syst::disk_t::all();
    if (! disks.has_value()) {
        std::cout << syst::error << std::endl;
        return 1;
    }

    std::cout << std::boolalpha; // Pretty print boolean values.

    for (const auto& disk : disks.value()) {
        std::cout << "sysfs path: " << disk.sysfs_path() << std::endl;
        std::cout << "devfs path: " << disk.devfs_path() << std::endl;
        std::cout << "Name: " << disk.name() << std::endl;

        auto size = disk.size();
        if (size.has_value()) {
            std::cout << "Size: " << size.value() << " bytes" << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto removable = disk.removable();
        if (removable.has_value()) {
            std::cout << "Removable: " << removable.value() << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto read_only = disk.read_only();
        if (read_only.has_value()) {
            std::cout << "Read-only: " << read_only.value() << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto rotational = disk.rotational();
        if (rotational.has_value()) {
            std::cout << "Rotational: " << rotational.value() << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto inflight_stat = disk.inflight_stat();
        if (inflight_stat.has_value()) {
            std::cout << "In-Flight Reads: " << inflight_stat->reads
                      << std::endl;
            std::cout << "In-Flight Writes: " << inflight_stat->writes
                      << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        auto io_stat = disk.io_stat();
        if (io_stat.has_value()) {
            std::cout << "Reads Completed: " << io_stat->reads_completed
                      << std::endl;
            std::cout << "Reads Merged: " << io_stat->reads_merged << std::endl;
            std::cout << "Sectors Read: " << io_stat->sectors_read << std::endl;
            std::cout << "Time by Reads: " << io_stat->time_by_reads.count()
                      << "ms" << std::endl;
            std::cout << "Writes Completed: " << io_stat->writes_completed
                      << std::endl;
            std::cout << "Writes Merged: " << io_stat->writes_merged
                      << std::endl;
            std::cout << "Sectors Written: " << io_stat->sectors_written
                      << std::endl;
            std::cout << "Time by Writes: " << io_stat->time_by_writes.count()
                      << "ms" << std::endl;
            std::cout << "I/O In-Flight: " << io_stat->io_in_flight
                      << std::endl;
            std::cout << "Time Spent Queued: "
                      << io_stat->time_spent_queued.count() << "ms"
                      << std::endl;
            std::cout << "Time by Queued I/O: "
                      << io_stat->time_by_queued_io.count() << "ms"
                      << std::endl;
            std::cout << "Discards Completed: " << io_stat->discards_completed
                      << std::endl;
            std::cout << "Discards Merged: " << io_stat->discards_merged
                      << std::endl;
            std::cout << "Sectors Discarded: " << io_stat->sectors_discarded
                      << std::endl;
            std::cout << "Time by Discards: "
                      << io_stat->time_by_discards.count() << "ms" << std::endl;
        } else {
            std::cout << syst::error << std::endl;
        }

        std::cout << std::endl;

        auto parts = disk.parts();
        if (! parts.has_value()) {
            std::cout << syst::error << std::endl;
            continue;
        }

        for (const auto& part : parts.value()) {
            std::cout << "sysfs path: " << part.sysfs_path() << std::endl;
            std::cout << "devfs path: " << part.devfs_path() << std::endl;
            std::cout << "Name: " << part.name() << std::endl;
            std::cout << "Disk Name: " << part.disk().name() << std::endl;

            auto part_size = part.size();
            if (part_size.has_value()) {
                std::cout << "Size: " << part_size.value() << " bytes"
                          << std::endl;
            } else {
                std::cout << syst::error << std::endl;
            }

            auto part_start = part.start();
            if (part_start.has_value()) {
                std::cout << "Start: " << part_start.value() << " bytes"
                          << std::endl;
            } else {
                std::cout << syst::error << std::endl;
            }

            auto part_read_only = part.read_only();
            if (part_read_only.has_value()) {
                std::cout << "Read-only: " << part_read_only.value()
                          << std::endl;
            } else {
                std::cout << syst::error << std::endl;
            }

            auto part_inflight_stat = part.inflight_stat();
            if (part_inflight_stat.has_value()) {
                std::cout << "In-Flight Reads: " << part_inflight_stat->reads
                          << std::endl;
                std::cout << "In-Flight Writes: " << part_inflight_stat->writes
                          << std::endl;
            } else {
                std::cout << syst::error << std::endl;
            }

            auto part_io_stat = disk.io_stat();
            if (part_io_stat.has_value()) {
                std::cout << "Reads Completed: "
                          << part_io_stat->reads_completed << std::endl;
                std::cout << "Reads Merged: " << part_io_stat->reads_merged
                          << std::endl;
                std::cout << "Sectors Read: " << part_io_stat->sectors_read
                          << std::endl;
                std::cout << "Time by Reads: "
                          << part_io_stat->time_by_reads.count() << "ms"
                          << std::endl;
                std::cout << "Writes Completed: "
                          << part_io_stat->writes_completed << std::endl;
                std::cout << "Writes Merged: " << part_io_stat->writes_merged
                          << std::endl;
                std::cout << "Sectors Written: "
                          << part_io_stat->sectors_written << std::endl;
                std::cout << "Time by Writes: "
                          << part_io_stat->time_by_writes.count() << "ms"
                          << std::endl;
                std::cout << "I/O In-Flight: " << part_io_stat->io_in_flight
                          << std::endl;
                std::cout << "Time Spent Queued: "
                          << part_io_stat->time_spent_queued.count() << "ms"
                          << std::endl;
                std::cout << "Time by Queued I/O: "
                          << part_io_stat->time_by_queued_io.count() << "ms"
                          << std::endl;
                std::cout << "Discards Completed: "
                          << part_io_stat->discards_completed << std::endl;
                std::cout << "Discards Merged: "
                          << part_io_stat->discards_merged << std::endl;
                std::cout << "Sectors Discarded: "
                          << part_io_stat->sectors_discarded << std::endl;
                std::cout << "Time by Discards: "
                          << part_io_stat->time_by_discards.count() << "ms"
                          << std::endl;
            } else {
                std::cout << syst::error << std::endl;
            }

            std::cout << std::endl;
        }
    }
}
