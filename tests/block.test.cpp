// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(block_test, disk_get_disks) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());
}

TEST(block_test, disk_get_disks_one_disk) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);
}

TEST(block_test, disk_sysfs_path) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        std::filesystem::path sysfs_path = disk.get_sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(block_test, disk_devfs_path) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        std::filesystem::path devfs_path = disk.get_devfs_path();
        ASSERT_TRUE(std::filesystem::is_block_file(devfs_path));
    }
}

TEST(block_test, disk_name) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        std::string name = disk.get_name();
        ASSERT_TRUE(name.size() > 0);
    }
}

TEST(block_test, disk_size) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto size = disk.get_size();
        ASSERT_TRUE(size.has_value());
        // Partition size can be any value.
    }
}

TEST(block_test, disk_removable) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto removable = disk.is_removable();
        ASSERT_TRUE(removable.has_value());
        // The removable flag can be any value.
    }
}

TEST(block_test, disk_read_only) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto read_only = disk.is_read_only();
        ASSERT_TRUE(read_only.has_value());
        // The read-only flag can be any value.
    }
}

TEST(block_test, disk_rotational) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto rotational = disk.is_rotational();
        ASSERT_TRUE(rotational.has_value());
        // The rotational flag can be any value.
    }
}

TEST(block_test, disk_inflight_stat) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto inflight_stat = disk.get_inflight_stat();
        ASSERT_TRUE(inflight_stat.has_value());
        // All in-flight statistics can be any value.
    }
}

TEST(block_test, disk_io_stat) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    for (const syst::disk_t& disk : disks.value()) {
        auto io_stat = disk.get_io_stat();
        ASSERT_TRUE(io_stat.has_value());
        // All I/O statistics can be any value.
    }
}

TEST(block_test, disk_get_parts) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_sysfs_path) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            std::filesystem::path sysfs_path = part.get_sysfs_path();
            ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_devfs_path) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            std::filesystem::path devfs_path = part.get_devfs_path();
            ASSERT_TRUE(std::filesystem::is_block_file(devfs_path));
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_disk) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            syst::disk_t part_disk = part.get_disk();
            ASSERT_STREQ(disk.get_sysfs_path().c_str(),
              part_disk.get_sysfs_path().c_str());
            ASSERT_STREQ(disk.get_devfs_path().c_str(),
              part_disk.get_devfs_path().c_str());
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_name) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            std::string part_name = part.get_name();
            ASSERT_TRUE(part_name.size() > 0);
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_size) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto size = part.get_size();
            ASSERT_TRUE(size.has_value());
            // Partition size can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_start) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto start = part.get_start_position();
            ASSERT_TRUE(start.has_value());
            // Partition size can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_read_only) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto read_only = part.is_read_only();
            ASSERT_TRUE(read_only.has_value());
            // The read-only flag can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_inflight_stat) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto inflight_stat = part.get_inflight_stat();
            ASSERT_TRUE(inflight_stat.has_value());
            // All in-flight statistics can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_io_stat) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto io_stat = part.get_io_stat();
            ASSERT_TRUE(io_stat.has_value());
            // All I/O statistics can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_is_mounted) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto is_mounted = part.is_mounted();
            ASSERT_TRUE(is_mounted.has_value());
            // The is_mounted flag can be any value.
        }
    }

    ASSERT_TRUE(partition_found);
}

TEST(block_test, part_mount_info) {
    auto disks = syst::get_disks();
    ASSERT_TRUE(disks.has_value());

    // For testing purposes, there must be at least one disk.
    ASSERT_GE(disks->size(), 1);

    bool partition_found = false;

    for (const syst::disk_t& disk : disks.value()) {
        auto parts = disk.get_parts();
        ASSERT_TRUE(parts.has_value());

        // For testing purposes, one of the identified disks must contain at
        // least one partition.
        if (parts->size() == 0) {
            continue;
        }
        partition_found = true;

        for (const syst::part_t& part : parts.value()) {
            auto mount_info = part.get_mount_info();
            ASSERT_TRUE(mount_info.has_value());
            ASSERT_TRUE(mount_info->mount_path.string().size() > 0);
            ASSERT_TRUE(mount_info->fs_type.size() > 0);
            ASSERT_TRUE(mount_info->options.size() > 0);
        }
    }

    ASSERT_TRUE(partition_found);
}
