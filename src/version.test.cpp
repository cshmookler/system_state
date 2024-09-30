// External includes
#include <gtest/gtest.h>

// Local includes
#include "../build/version.hpp"

TEST(version_test, runtime_version_matches_compiletime_version) {
    ASSERT_STREQ(
            syst::get_runtime_version(),
            syst::compiletime_version);
}