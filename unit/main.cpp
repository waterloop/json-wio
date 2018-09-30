#include <gtest/gtest.h>

namespace wlp {
    namespace mem {
        void *alloc(size_t bytes)
        { return ::malloc(bytes); }
        void free(void *ptr)
        { return ::free(ptr); }
        void *realloc(void *ptr, size_t bytes)
        { return ::realloc(ptr, bytes); }
    }
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    int status_code = RUN_ALL_TESTS();
    return status_code;
}
