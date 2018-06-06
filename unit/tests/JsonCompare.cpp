#include <gtest/gtest.h>

#include <wlib/Json/JsonElement.h>

using namespace wlp;

#define same_type(je1, je2) ASSERT_EQ(je1.type(), je2.type())
#define same_class(je1, je2) ASSERT_EQ(je1.type() >> 4, je2.type() >> 4)
#define val_eq(type_t, je1, je2) ASSERT_EQ(je1.as<type_t>(), je2.as<type_t>())
#define val_ne(type_t, je1, je2) ASSERT_NE(je1.as<type_t>(), je2.as<type_t>())
#define type_is(je1, type_t)                \
{ json_type jt = type_info<type_t>::value;  \
  ASSERT_EQ(jt, je1.type()); }

TEST(json_compare, equality_null) {
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

TEST(json_compare, equality_bool) {
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

    ASSERT_EQ(a, x);
    ASSERT_EQ(b, y);
    ASSERT_NE(x, y);
    ASSERT_NE(x, b);
    ASSERT_NE(y, a);
}

TEST(json_compare, equality_int) {
    constexpr short val1 = -523;
    constexpr int val2 = -523;
    constexpr int val3 = 6642;
    constexpr long val4 = 3323L;

    json_element a(val1);
    json_element b(val2);
    json_element c(val3);
    json_element d(val4);

    type_is(a, short);
    type_is(b, int);
    type_is(c, int);
    type_is(d, long);

    val_eq(short, a, b);
    val_eq(int, a, b);
    val_eq(long, a, b);

    ASSERT_EQ(val1, a.as<int>());
    ASSERT_EQ(val2, b.as<int>());
    ASSERT_EQ(val3, c.as<int>());
    ASSERT_EQ(val3, c.as<unsigned int>());
    ASSERT_EQ(val4, d.as<int>());
    ASSERT_EQ(val4, d.as<long>());
    ASSERT_EQ(val4, d.as<unsigned short>());

    ASSERT_EQ(a, b);
    ASSERT_NE(b, c);
    ASSERT_NE(b, d);
    ASSERT_NE(a, d);

    json_element e(val4);

    ASSERT_EQ(e, d);
    ASSERT_NE(e, a);
}

TEST(json_compare, equality_float) {
    constexpr float f1 = -49293.424f;
    constexpr float f2 = f1;
    constexpr float f3 = 234.12f;

    constexpr double d1 = 5828328.24353e12;
    constexpr double d2 = d1;
    constexpr double d3 = -3412531.232;

    json_element ef1(f1);
    json_element ef2(f2);
    json_element ef3(f3);

    json_element ed1(d1);
    json_element ed2(d2);
    json_element ed3(d3);

    type_is(ef1, float);
    type_is(ef2, float);
    type_is(ef3, float);

    type_is(ed1, double);
    type_is(ed2, double);
    type_is(ed3, double);

    ASSERT_FLOAT_EQ(f1, ef1.as<float>());
    ASSERT_FLOAT_EQ(f2, ef2.as<float>());
    ASSERT_FLOAT_EQ(f3, ef3.as<float>());

    ASSERT_EQ(ef1, ef2);
    ASSERT_NE(ef1, ef3);
    ASSERT_NE(ef2, ef3);

    ASSERT_DOUBLE_EQ(d1, ed1.as<double>());
    ASSERT_DOUBLE_EQ(d2, ed2.as<double>());
    ASSERT_DOUBLE_EQ(d3, ed3.as<double>());

    ASSERT_EQ(ed1, ed2);
    ASSERT_NE(ed1, ed3);
    ASSERT_NE(ed2, ed3);
}

TEST(json_compare, equality_string) {
    constexpr auto str1 = "Confederation forces continue to march south towards Mittenheid";
    constexpr auto str2 = "The unstoppable war machine annihilates our armies and destroys are resources";
    constexpr auto str3 = "Soon they will lay siege to our capital and threaten us with destruction";

    json_element es1(str1);
    json_element es2(str1);
    json_element es3(str2);
    json_element es4(str2);
    json_element es5(str3);
    json_element es6(str3);

    type_is(es1, const char *);
    type_is(es3, const char *);
    type_is(es5, const char *);

    ASSERT_STREQ(str1, es1.as<const char *>());
    ASSERT_STREQ(str2, es3.as<const char *>());
    ASSERT_STREQ(str3, es5.as<const char *>());

    ASSERT_EQ(es1, es2);
    ASSERT_EQ(es3, es4);
    ASSERT_EQ(es5, es6);

    ASSERT_NE(es1, es3);
    ASSERT_NE(es2, es4);
    ASSERT_NE(es3, es5);
    ASSERT_NE(es4, es6);
}

TEST(json_compare, different_types) {
    json_element null_e(nullptr);
    json_element bool_e(true);
    json_element int_e(1);
    json_element float_e(1.0f);
    json_element str_e("1");

    type_is(null_e, nullptr_t);
    type_is(bool_e, bool);
    type_is(int_e, int);
    type_is(float_e, float);
    type_is(str_e, const char *);

    ASSERT_NE(null_e, bool_e);
    ASSERT_NE(bool_e, int_e);
    ASSERT_NE(int_e, float_e);
    ASSERT_NE(float_e, str_e);
    ASSERT_NE(str_e, null_e);
}

TEST(json_compare, less_null) {
    json_element null_a(nullptr);
    json_element null_b(nullptr);

    type_is(null_a, nullptr_t);
    type_is(null_b, nullptr_t);

    ASSERT_FALSE(null_a < null_b);
    ASSERT_FALSE(null_a > null_b);
    ASSERT_TRUE(null_a == null_b);

    ASSERT_GE(null_a, null_b);
    ASSERT_LE(null_a, null_b);
}

TEST(json_compare, less_bool) {
    json_element true_e(true);
    json_element false_e(false);

    type_is(true_e, bool);
    type_is(false_e, bool);

    ASSERT_LT(false_e, true_e);
    ASSERT_GT(true_e, false_e);
    ASSERT_LE(false_e, true_e);
    ASSERT_GE(true_e, false_e);

    json_element other_e(true);

    ASSERT_GE(other_e, true_e);
    ASSERT_LE(other_e, true_e);
}

TEST(json_compare, less_int) {
    json_element low(25);
    json_element mid1(static_cast<short>(50));
    json_element mid2(50);
    json_element high(75);

    type_is(low, int);
    type_is(mid1, short);
    type_is(mid2, int);
    type_is(high, int);

    ASSERT_LT(low, mid1);
    ASSERT_LT(mid1, high);
    ASSERT_GT(high, mid2);
    ASSERT_GT(high, low);
    ASSERT_LE(low, high);
    ASSERT_GE(high, low);
    ASSERT_LE(mid1, mid2);
    ASSERT_GE(mid1, mid2);
}

TEST(json_compare, less_float) {
    json_element lower(-423.23f);
    json_element low(-423.23f);
    json_element mid(-10.0);
    json_element high(23.323);
    json_element higher(500.0f);

    type_is(lower, float);
    type_is(low, float);
    type_is(mid, double);
    type_is(high, double);
    type_is(higher, float);

    ASSERT_LT(low, mid);
    ASSERT_LT(mid, high);
    ASSERT_LT(high, higher);

    ASSERT_GT(higher, high);
    ASSERT_GT(high, mid);
    ASSERT_GT(mid, low);

    ASSERT_LE(lower, low);
    ASSERT_LE(low, mid);

    ASSERT_GE(low, lower);
    ASSERT_GE(higher, high);
}

TEST(json_compare, element_less_str) {
    constexpr auto low = "A brown fox jumped over the lazy dog";
    constexpr auto mid = "aaaaaaaaabbbbbbbbbbbbbbbbcccccccccc";
    constexpr auto high = "zebras are horses with white and black stripes";

    json_element lo(low);
    json_element mi(mid);
    json_element hi(high);

    json_element o(mid);

    type_is(lo, const char *);
    type_is(mi, const char *);
    type_is(hi, const char *);
    type_is(o, const char *);

    ASSERT_LT(lo, mi);
    ASSERT_LT(lo, hi);

    ASSERT_GT(hi, mi);
    ASSERT_GT(hi, lo);

    ASSERT_LE(mi, o);
    ASSERT_LE(lo, mi);

    ASSERT_GE(mi, o);
    ASSERT_GE(hi, mi);
}

