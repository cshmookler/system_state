// External includes
#include <gtest/gtest.h>

// Local includes
#include "../system_state/system_state.hpp"

TEST(user_test, username) {
    auto username = syst::get_username();
    ASSERT_TRUE(username.has_value()) << RES_TRACE(username.error());
    // Usernames must be at least one character long.
    ASSERT_GE(username->size(), 1);
}
