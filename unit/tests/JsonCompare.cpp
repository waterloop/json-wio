#include <gtest/gtest.h>

#include <wlib/Json/JsonCompare.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

#define same_type(je1, je2) ASSERT_EQ(je1.type(), je2.type())
#define val_eq(type_t, je1, je2) ASSERT_EQ(je1.as<type_t>(), je2.as<type_t>())
#define val_ne(type_t, je1, je2) ASSERT_NE(je1.as<type_t>(), je2.as<type_t>())

TEST(json_compare, json_element_equality_null) {
    json_element a;
    json_element b;

    same_type(a, b);
    ASSERT_EQ(a, b);

    json_element x(nullptr);
    json_element y(nullptr);

    same_type(a, x);
    same_type(b, y);
    ASSERT_EQ(a, x);
    ASSERT_EQ(b, y);
}

#include <stdio.h>

TEST(json_compare, json_element_equality_bool) {
    json_element a(true);
    json_element b(false);

    same_type(a, b);
    val_ne(bool, a, b);
    ASSERT_NE(a, b);

    json_element x(true);
    json_element y(false);

    same_type(a, x);
    same_type(b, y);

    val_eq(bool, a, x);
    val_eq(bool, b, y);

    printf("%lli, %lli\n", a.integer(), x.integer());

    ASSERT_TRUE(a == x);

    ASSERT_EQ(a, x);
    ASSERT_EQ(b, y);
}

