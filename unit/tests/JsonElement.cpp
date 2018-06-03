#include <gtest/gtest.h>
#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonElement.h>

#define decl_je(name, type, val) \
static json_element name(static_cast<type>(val))
#define assert_je_type(var, type_v) \
ASSERT_EQ(var.type(), type_v)
#define assert_je_size(var, type_v) \
ASSERT_EQ(var.size(), sizeof(type_of<type_v>::type))
#define assert_je_data(var, type_v, val)                    \
{ type_of<type_v>::type *ptr =                              \
  reinterpret_cast<type_of<type_v>::type *>(var.data());    \
  ASSERT_EQ(*ptr, val); }
#define assert_je_dataf(var, type_v, asserter, fval)        \
{ type_of<type_v>::type *ptr =                              \
  reinterpret_cast<type_of<type_v>::type *>(var.data());    \
  asserter(*ptr, fval); }
#define assert_je_datastr(var, strval)          \
{ const char *str =                             \
  reinterpret_cast<const char *>(var.data());   \
  ASSERT_STREQ(str, strval); }
#define assert_je_typecheck(var,                \
    isnull, isbool, isint, isfloat,             \
    isnumber, isstring, isarray, isobject)      \
ASSERT_EQ(var.is_null(), isnull);               \
ASSERT_EQ(var.is_bool(), isbool);               \
ASSERT_EQ(var.is_int(), isint);                 \
ASSERT_EQ(var.is_float(), isfloat);             \
ASSERT_EQ(var.is_number(), isnumber);           \
ASSERT_EQ(var.is_string(), isstring);           \
ASSERT_EQ(var.is_array(), isarray);             \
ASSERT_EQ(var.is_object(), isobject);       
#define assert_je_typecheck_int(ivar)           \
assert_je_typecheck(ivar, false, false, true,   \
    false, true, false, false, false)
#define assert_je_typecheck_float(fvar)         \
assert_je_typecheck(fvar, false, false, false,  \
    true, true, false, false, false)
#define assert_je_typecheck_str(strvar)             \
assert_je_typecheck(strvar, false, false, false,    \
    false, false, true, false, false)
#define assert_je_ct(var, tonull, tobool,               \
    toint, tofloat, tostr)                              \
ASSERT_EQ(var.convertible_to<nullptr_t>(), tonull);     \
ASSERT_EQ(var.convertible_to<bool>(), tobool);          \
ASSERT_EQ(var.convertible_to<int>(), toint);            \
ASSERT_EQ(var.convertible_to<unsigned lont>(), toint);  \
ASSERT_EQ(var.convertible_to<float>(), tofloat);        \
ASSERT_EQ(var.convertible_to<double>(), tofloat);       \
ASSERT_EQ(var.convertible_to<const char *>(), tostr);   \
ASSERT_EQ(var.convertible_to<char *>(), tostr);         \
ASSERT_EQ(var.convertible_to<dynamic_string>(), tostr); 

using namespace wlp;

#define TEST_STR1   "hello my name is jeff"
#define TEST_STR2   "war of the worlds"
#define TEST_STR3   "your empire needs you"
#define TEST_STR4   "null"
#define TEST_STR5   "true"
#define TEST_STR6   "false"

decl_je(null_e, nullptr_t, nullptr);
decl_je(bool_e, bool, true);
decl_je(char_e, char, 'g');
decl_je(s_char_e, signed char, -10);
decl_je(s_short_e, short, -11);
decl_je(s_int_e, int, -12);
decl_je(s_long_e, long, -13);
decl_je(s_lli_e, long long, -14);
decl_je(u_char_e, unsigned char, 10);
decl_je(u_short_e, unsigned short, 11);
decl_je(u_int_e, unsigned int, 12);
decl_je(u_long_e, unsigned long, 13);
decl_je(u_lli_e, unsigned long long, 14);
decl_je(float_e, float, 12.12f);
decl_je(double_e, double, 13.13);
decl_je(longdbl_e, long double, 14.14);
dynamic_string d_str(TEST_STR2);
static_string<128> s_str(TEST_STR3);
json_element c_str_e(TEST_STR1);
json_element d_str_e(d_str);
json_element s_str_e(s_str);

/*
null_e
bool_e
char_e
s_char_e
s_short_e
s_int_e
s_long_e
s_lli_e
u_char_e
u_short_e
u_int_e
u_long_e
u_lli_e
float_e
double_e
longdbl_e
c_str_e
d_str_e
s_str_e
*/

TEST(json_element, constructor_json_type) {
    assert_je_type(null_e, TYPE_NULL);
    assert_je_type(bool_e, TYPE_BOOL);
    assert_je_type(char_e, TYPE_CHAR);
    assert_je_type(s_char_e, TYPE_SIGNED_CHAR);
    assert_je_type(s_short_e, TYPE_SIGNED_SHORT);
    assert_je_type(s_int_e, TYPE_SIGNED_INT);
    assert_je_type(s_long_e, TYPE_SIGNED_LONG);
    assert_je_type(s_lli_e, TYPE_SIGNED_LONG_LONG);
    assert_je_type(u_char_e, TYPE_UNSIGNED_CHAR);
    assert_je_type(u_short_e, TYPE_UNSIGNED_SHORT);
    assert_je_type(u_int_e, TYPE_UNSIGNED_INT);
    assert_je_type(u_long_e, TYPE_UNSIGNED_LONG);
    assert_je_type(u_lli_e, TYPE_UNSIGNED_LONG_LONG);
    assert_je_type(float_e, TYPE_FLOAT);
    assert_je_type(double_e, TYPE_DOUBLE);
    assert_je_type(longdbl_e, TYPE_LONG_DOUBLE);
    assert_je_type(c_str_e, TYPE_JSON_STRING);
    assert_je_type(d_str_e, TYPE_JSON_STRING);
    assert_je_type(s_str_e, TYPE_JSON_STRING);
}

TEST(json_element, constructor_size) {
    assert_je_size(null_e, TYPE_NULL);
    assert_je_size(bool_e, TYPE_BOOL);
    assert_je_size(char_e, TYPE_CHAR);
    assert_je_size(s_char_e, TYPE_SIGNED_CHAR);
    assert_je_size(s_short_e, TYPE_SIGNED_SHORT);
    assert_je_size(s_int_e, TYPE_SIGNED_INT);
    assert_je_size(s_long_e, TYPE_SIGNED_LONG);
    assert_je_size(s_lli_e, TYPE_SIGNED_LONG_LONG);
    assert_je_size(u_char_e, TYPE_UNSIGNED_CHAR);
    assert_je_size(u_short_e, TYPE_UNSIGNED_SHORT);
    assert_je_size(u_int_e, TYPE_UNSIGNED_INT);
    assert_je_size(u_long_e, TYPE_UNSIGNED_LONG);
    assert_je_size(u_lli_e, TYPE_UNSIGNED_LONG_LONG);
    assert_je_size(float_e, TYPE_FLOAT);
    assert_je_size(double_e, TYPE_DOUBLE);
    assert_je_size(longdbl_e, TYPE_LONG_DOUBLE);
    ASSERT_EQ(c_str_e.size(), strlen(TEST_STR1));
    ASSERT_EQ(d_str_e.size(), strlen(TEST_STR2));
    ASSERT_EQ(s_str_e.size(), strlen(TEST_STR3));
}

TEST(json_element, constructor_data) {
    assert_je_data(null_e, TYPE_NULL, nullptr);
    assert_je_data(bool_e, TYPE_BOOL, true);
    assert_je_data(char_e, TYPE_CHAR, 'g');
    assert_je_data(s_char_e, TYPE_SIGNED_CHAR, -10);
    assert_je_data(s_short_e, TYPE_SIGNED_SHORT, -11);
    assert_je_data(s_int_e, TYPE_SIGNED_INT, -12);
    assert_je_data(s_long_e, TYPE_SIGNED_LONG, -13);
    assert_je_data(s_lli_e, TYPE_SIGNED_LONG_LONG, -14);
    assert_je_data(u_char_e, TYPE_UNSIGNED_CHAR, 10);
    assert_je_data(u_short_e, TYPE_UNSIGNED_SHORT, 11);
    assert_je_data(u_int_e, TYPE_UNSIGNED_INT, 12);
    assert_je_data(u_long_e, TYPE_UNSIGNED_LONG, 13);
    assert_je_data(u_lli_e, TYPE_UNSIGNED_LONG_LONG, 14);
    assert_je_dataf(float_e, TYPE_FLOAT, ASSERT_FLOAT_EQ, 12.12f);
    assert_je_dataf(double_e, TYPE_DOUBLE, ASSERT_DOUBLE_EQ, 13.13);
    assert_je_dataf(longdbl_e, TYPE_LONG_DOUBLE, ASSERT_DOUBLE_EQ, 14.14);
    assert_je_datastr(c_str_e, TEST_STR1);
    assert_je_datastr(d_str_e, TEST_STR2);
    assert_je_datastr(s_str_e, TEST_STR3);
}

TEST(json_element, json_type_checks) {
    assert_je_typecheck(null_e, true, false, false, 
        false, false, false, false, false);
    assert_je_typecheck(bool_e, false, true, false,
        false, false, false, false, false);
    assert_je_typecheck_int(char_e);
    assert_je_typecheck_int(s_char_e);
    assert_je_typecheck_int(s_short_e);
    assert_je_typecheck_int(s_int_e);
    assert_je_typecheck_int(s_long_e);
    assert_je_typecheck_int(s_lli_e);
    assert_je_typecheck_int(u_char_e);
    assert_je_typecheck_int(u_short_e);
    assert_je_typecheck_int(u_int_e);
    assert_je_typecheck_int(u_long_e);
    assert_je_typecheck_int(u_lli_e);
    assert_je_typecheck_float(float_e);
    assert_je_typecheck_float(double_e);
    assert_je_typecheck_float(longdbl_e);
    assert_je_typecheck_str(c_str_e)
    assert_je_typecheck_str(d_str_e)
    assert_je_typecheck_str(s_str_e)
}

TEST(json_element, convertible_to) {

}
