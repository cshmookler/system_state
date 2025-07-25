// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(network_interface_test, all) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());
}

TEST(network_interface_test, all_one_interface) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);
}

TEST(network_interface_test, sysfs_path) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    for (const syst::network_interface_t& interface : interfaces.value()) {
        std::filesystem::path sysfs_path = interface.get_sysfs_path();
        ASSERT_TRUE(std::filesystem::is_directory(sysfs_path));
    }
}

TEST(network_interface_test, name) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    for (const syst::network_interface_t& interface : interfaces.value()) {
        std::string name = interface.get_name();
        ASSERT_TRUE(name.size() > 0);
    }
}

TEST(network_interface_test, physical) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    for (const syst::network_interface_t& interface : interfaces.value()) {
        auto physical = interface.is_physical();
        ASSERT_TRUE(physical.has_value()) << RES_TRACE(physical.error());
        // Network interfaces can be either physical or virtual.
    }
}

TEST(network_interface_test, loopback) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    bool loopback_found = false;

    for (const syst::network_interface_t& interface : interfaces.value()) {
        auto loopback = interface.is_loopback();
        ASSERT_TRUE(loopback.has_value()) << RES_TRACE(loopback.error());

        // For testing purposes, there must be at least one loopback interface.
        if (loopback.value()) {
            loopback_found = true;
        }
    }

    ASSERT_TRUE(loopback_found);
}

TEST(network_interface_test, status) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    for (const syst::network_interface_t& interface : interfaces.value()) {
        auto status = interface.get_status();
        ASSERT_TRUE(status.has_value()) << RES_TRACE(status.error());
        // The status can be any value.
    }
}

TEST(network_interface_test, stat) {
    auto interfaces = syst::get_network_interfaces();
    ASSERT_TRUE(interfaces.has_value()) << RES_TRACE(interfaces.error());

    // For testing purposes, there must be at least one network interface.
    ASSERT_GE(interfaces->size(), 1);

    bool bytes_down_found = false;
    bool bytes_up_found = false;
    bool packets_down_found = false;
    bool packets_up_found = false;

    for (const syst::network_interface_t& interface : interfaces.value()) {
        auto status = interface.get_stat();
        ASSERT_TRUE(status.has_value()) << RES_TRACE(status.error());
        // The status can be any value.

        // For testing purposes, each field must be non-zero for at least one
        // network interface.
        if (status->bytes_down > 0) {
            bytes_down_found = true;
        }
        if (status->bytes_up > 0) {
            bytes_up_found = true;
        }
        if (status->packets_down > 0) {
            packets_down_found = true;
        }
        if (status->packets_up > 0) {
            packets_up_found = true;
        }
    }

    ASSERT_TRUE(bytes_down_found);
    ASSERT_TRUE(bytes_up_found);
    ASSERT_TRUE(packets_down_found);
    ASSERT_TRUE(packets_up_found);
}
