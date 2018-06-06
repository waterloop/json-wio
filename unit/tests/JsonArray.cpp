#include <gtest/gtest.h>

#include <wlib/Json/JsonArray.h>
#include <wlib/Json/JsonElement.h>
#include <wlib/Json/JsonNull.h>

using namespace wlp;

TEST(json_array, json_array_null_elements) {
    json_array arr;

    constexpr size_type size = 10;
    for (size_type i = 0; i < size; ++i) {
        arr.push_back(json::null);
    }

    ASSERT_EQ(size, arr.size());
    ASSERT_FALSE(arr.empty());

    for (size_type i = 0; i < size; ++i) {
        ASSERT_EQ(json::null, arr[i]);
    }
}

TEST(json_array, json_array_bool_elements) {
    json_array arr;

    constexpr size_type size = 10;
    for (size_type i = 0; i < size; ++i) {
        arr.push_back(json_element(!(i % 2)));
    }

    ASSERT_EQ(size, arr.size());

    json_element true_e(true);
    json_element false_e(false);
    for (size_type i = 0; i < size; ++i) {
        ASSERT_EQ(!(i % 2) ? true_e : false_e, arr[i]);
    }
}

TEST(json_array, json_array_int_elements) {
    json_array arr;

    for (int i = 0; i < 10; ++i) {
        arr.push_back(json_element(i + 1));
    }

    ASSERT_EQ(10, arr.size());

    int sum = 0;
    for (int i = 0; i < 10; ++i) {
        sum += arr[i].as<int>();
    }

    ASSERT_EQ(55, sum);
}

#include <wlib/Json/JsonObject.h>
TEST(json_array, size) {
    printf("sizeof(json_array): %lu\n", sizeof(json_array));
    printf("sizeof(json_object): %lu\n", sizeof(json_object));
}

