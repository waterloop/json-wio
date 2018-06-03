#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    int status_code = RUN_ALL_TESTS();
    return status_code;
}
