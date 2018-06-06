#include <gtest/gtest.h>

#include <wlib/Json/JsonArray.h>
#include <wlib/Json/JsonElement.h>
#include <wlib/Json/JsonNull.h>

using namespace wlp;

TEST(json_array, null_elements) {
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

TEST(json_array, bool_elements) {
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

TEST(json_array, int_elements) {
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

TEST(json_array, float_elements) {
    json_array arr;

    json_element e1(345.55);
    json_element e2(4433.23f);
    json_element e3(-2323.353e-3);
    json_element e4(-3424.23f);

    arr.push_back(e1);
    arr.push_back(e2);
    arr.push_back(e3);
    arr.push_back(e4);

    ASSERT_EQ(4, arr.size());

    ASSERT_NE(arr.end(), arr.find(e1));
    ASSERT_NE(arr.end(), arr.find(e2));
    ASSERT_NE(arr.end(), arr.find(e3));
    ASSERT_NE(arr.end(), arr.find(e4));

    json_element e5(44423.4);
    ASSERT_EQ(arr.end(), arr.find(e5));

    ASSERT_DOUBLE_EQ(345.55, arr[0].as<double>());
    ASSERT_FLOAT_EQ(-3424.23f, arr[3].as<float>());
}

TEST(json_array, string_elements) {
    json_array arr;

    constexpr auto str1 = "'Power of the Gods?' exclaimed Lieutenant Gonarph";
    constexpr auto str2 = "'Impossible' responded the minister";
    constexpr auto str3 = "'Yet they are lifting the siege of the city as we speak'";
    constexpr auto str4 = "We had no possibility of breaking siege";
    constexpr auto str5 = "So then why would they withdraw their troops?";
    constexpr auto str6 = "We may be in danger; we must leave now";

    json_element e1(str1);
    json_element e2(str2);
    json_element e3(str3);
    json_element e4(str4);
    json_element e5(str5);
    json_element e6(str6);

    arr.push_back(e1);
    arr.push_back(e2);
    arr.push_back(e3);
    arr.push_back(e4);
    arr.push_back(e5);
    arr.push_back(e6);

    ASSERT_EQ(6, arr.size());

    json_element test("The special convoy from the Tria Alliance prepares..");

    ASSERT_EQ(arr.end(), arr.find(test));
    ASSERT_NE(arr.end(), arr.find(e1));
    ASSERT_NE(arr.end(), arr.find(e5));
    ASSERT_NE(arr.end(), arr.find(e6));

    ASSERT_EQ(arr[2], e3);
}

