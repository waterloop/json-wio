/***************************************************
 * Includes
 **************************************************/
#include <gtest/gtest.h>

#include <wlib/utility>

#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonElement.h>

/***************************************************
 * Macro definitions
 **************************************************/
#define cast(type, v) static_cast<type>(v)

#define assert_type(var, type_t)                \
{ json_type _type_ = type_info<type_t>::value;  \
  ASSERT_EQ(_type_, var.type()); }

#define decl_je(name, type, val) \
static json_element name(static_cast<type>(val))

#define assert_je_type(var, type_v) \
ASSERT_EQ(var.type(), type_v)

#define assert_je_datai(var, type_v, val)                       \
{ auto a = static_cast<type_of<type_v>::type>(var.integer());   \
  auto b = static_cast<type_of<type_v>::type>(val);             \
  ASSERT_EQ(a, b); }

#define assert_je_dataf(var, type_v, asfunc, val)               \
{ auto a = static_cast<type_of<type_v>::type>(var.floating());  \
  auto b = static_cast<type_of<type_v>::type>(val);             \
  asfunc(a, b); }

#define assert_je_datastr(var, val) \
{ auto a = var.string().c_str();    \
  ASSERT_STREQ(a, val); }

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
ASSERT_EQ(var.convertible_to<unsigned long>(), toint);  \
ASSERT_EQ(var.convertible_to<float>(), tofloat);        \
ASSERT_EQ(var.convertible_to<double>(), tofloat);       \
ASSERT_EQ(var.convertible_to<const char *>(), tostr);   \
ASSERT_EQ(var.convertible_to<char *>(), tostr);         \
ASSERT_EQ(var.convertible_to<dynamic_string>(), tostr);

#define assert_je_ct_int(nvar) \
assert_je_ct(nvar, false, true, true, true, false)

#define assert_je_ct_float(nvar) \
assert_je_ct(nvar, false, true, false, true, false)

#define assert_je_ct_str(strvar) \
assert_je_ct(strvar, false, false, false, false, true)

#define assert_je_as(var, asnull, asbool, asint,    \
    asuint, asfloat, asstr)                         \
ASSERT_EQ(var.as<nullptr_t>(), asnull);             \
ASSERT_EQ(var.as<bool>(), asbool);                  \
ASSERT_EQ(var.as<char>(), cast(char, asint));       \
ASSERT_EQ(var.as<short>(), cast(short, asint));     \
ASSERT_EQ(var.as<int>(), cast(int, asint));         \
ASSERT_EQ(var.as<long>(), cast(long, asint));       \
ASSERT_EQ(var.as<long long>(), cast(lli, asint));   \
ASSERT_EQ(var.as<unsigned char>(), cast(uchar, asuint));    \
ASSERT_EQ(var.as<unsigned short>(), cast(ushort, asuint));  \
ASSERT_EQ(var.as<unsigned int>(), cast(uint, asuint));      \
ASSERT_EQ(var.as<unsigned long>(), cast(ulong, asuint));    \
ASSERT_EQ(var.as<unsigned long long>(), cast(ull, asuint)); \
ASSERT_FLOAT_EQ(var.as<float>(), asfloat);          \
ASSERT_DOUBLE_EQ(var.as<double>(), asfloat);        \
ASSERT_DOUBLE_EQ(var.as<long double>(), asfloat);   \
ASSERT_STREQ(var.as<char *>(), asstr);              \
ASSERT_STREQ(var.as<const char *>(), asstr);        \
ASSERT_EQ(var.as<dynamic_string>(), dynamic_string(asstr));

#define assert_je_as_u(var, asnull, asbool, asint,  \
    asfloat, asstr)                                 \
assert_je_as(var, asnull, asbool, asint, asint, asfloat, asstr)

#define assert_je_as_str(strvar, asstr)         \
ASSERT_EQ(strvar.as<nullptr_t>(), nullptr);     \
ASSERT_STREQ(strvar.as<char *>(), asstr);       \
ASSERT_STREQ(strvar.as<const char *>(), asstr); \
ASSERT_EQ(strvar.as<dynamic_string>(), dynamic_string(asstr))

#define assert_nullinit(var)        \
ASSERT_EQ(TYPE_NULL, var.type());

/***************************************************
 * Data types
 **************************************************/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ull;

typedef long long lli;

using namespace wlp;

/***************************************************
 * Test strings
 **************************************************/
#define TEST_STR1   "hello my name is jeff"
#define TEST_STR2   "war of the worlds"
#define TEST_STR3   "your empire needs you"
#define TEST_STR4   "null"
#define TEST_STR5   "true"
#define TEST_STR6   "false"
#define TEST_STR7   "6634"
#define TEST_STR8   "-503"
#define TEST_STR9   "-4023.8"

/***************************************************
 * Static members
 **************************************************/
decl_je(null_e, nullptr_t, nullptr);
decl_je(false_e, bool, false);
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
json_element str_null_e(TEST_STR4);
json_element str_true_e(TEST_STR5);
json_element str_false_e(TEST_STR6);
json_element str_uint_e(TEST_STR7);
json_element str_sint_e(TEST_STR8);
json_element str_float_e(TEST_STR9);

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

/***************************************************
 * Unit tests
 **************************************************/
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

TEST(json_element, constructor_data) {
    assert_je_datai(bool_e, TYPE_BOOL, true);
    assert_je_datai(char_e, TYPE_CHAR, 'g');
    assert_je_datai(s_char_e, TYPE_SIGNED_CHAR, -10);
    assert_je_datai(s_short_e, TYPE_SIGNED_SHORT, -11);
    assert_je_datai(s_int_e, TYPE_SIGNED_INT, -12);
    assert_je_datai(s_long_e, TYPE_SIGNED_LONG, -13);
    assert_je_datai(s_lli_e, TYPE_SIGNED_LONG_LONG, -14);
    assert_je_datai(u_char_e, TYPE_UNSIGNED_CHAR, 10);
    assert_je_datai(u_short_e, TYPE_UNSIGNED_SHORT, 11);
    assert_je_datai(u_int_e, TYPE_UNSIGNED_INT, 12);
    assert_je_datai(u_long_e, TYPE_UNSIGNED_LONG, 13);
    assert_je_datai(u_lli_e, TYPE_UNSIGNED_LONG_LONG, 14);
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
    assert_je_ct(null_e, true, true, true, true, false);
    assert_je_ct(bool_e, false, true, true, true, false);
    assert_je_ct_int(char_e);
    assert_je_ct_int(s_char_e);
    assert_je_ct_int(s_short_e);
    assert_je_ct_int(s_int_e);
    assert_je_ct_int(s_long_e);
    assert_je_ct_int(s_lli_e);
    assert_je_ct_int(u_char_e);
    assert_je_ct_int(u_short_e);
    assert_je_ct_int(u_int_e);
    assert_je_ct_int(u_long_e);
    assert_je_ct_int(u_lli_e);
    assert_je_ct_float(float_e);
    assert_je_ct_float(double_e);
    assert_je_ct_float(longdbl_e);
    assert_je_ct_str(c_str_e);
    assert_je_ct_str(d_str_e);
    assert_je_ct_str(s_str_e);
    assert_je_ct(str_null_e, true, false, false, false, true);
    assert_je_ct(str_true_e, false, true, false, false, true);
    assert_je_ct(str_false_e, false, true, false, false, true);
    assert_je_ct(str_uint_e, false, false, true, true, true);
    assert_je_ct(str_sint_e, false, false, true, true, true);
    assert_je_ct(str_float_e, false, false, false, true, true);
}

TEST(json_element, as) {
    constexpr int g_ord = cast(int, 'g');
    assert_je_as(null_e, nullptr, false, 0, 0, 0, nullptr);
    assert_je_as(false_e, nullptr, false, 0, 0, 0, nullptr);
    assert_je_as(bool_e, nullptr, true, 1, 1, 1.0, nullptr);
    assert_je_as_u(char_e, nullptr, true, g_ord, g_ord, nullptr);
    assert_je_as_u(s_char_e, nullptr, true, -10, -10.0f, nullptr);
    assert_je_as_u(s_short_e, nullptr, true, -11, -11.0f, nullptr);
    assert_je_as_u(s_int_e, nullptr, true, -12, -12.0f, nullptr);
    assert_je_as_u(s_long_e, nullptr, true, -13, -13.0f, nullptr);
    assert_je_as_u(s_lli_e, nullptr, true, -14, -14.0f, nullptr);
    assert_je_as_u(u_char_e, nullptr, true, 10, 10.0f, nullptr);
    assert_je_as_u(u_short_e, nullptr, true, 11, 11.0f, nullptr);
    assert_je_as_u(u_int_e, nullptr, true, 12, 12.0f, nullptr);
    assert_je_as_u(u_long_e, nullptr, true, 13, 13.0f, nullptr);
    assert_je_as_u(u_lli_e, nullptr, true, 14, 14.0f, nullptr);
    assert_je_as_u(float_e, nullptr, true, 12, 12.12f, nullptr);
    assert_je_as_u(double_e, nullptr, true, 13, 13.13, nullptr);
    assert_je_as_u(longdbl_e, nullptr, true, 14, 14.14, nullptr);
    assert_je_as_str(c_str_e, TEST_STR1);
    assert_je_as_str(d_str_e, TEST_STR2);
    assert_je_as_str(s_str_e, TEST_STR3);
    assert_je_as_str(str_null_e, TEST_STR4);
    assert_je_as_str(str_true_e, TEST_STR5);
    assert_je_as_str(str_false_e, TEST_STR6);
    assert_je_as_u(str_uint_e, nullptr, false, 6634, 6634.0f, "6634");
    assert_je_as_u(str_sint_e, nullptr, false, -503, -503.0f, "-503");
    assert_je_as_u(str_float_e, nullptr, false, -4023, -4023.8, "-4023.8");
}

TEST(json_element, copy_constructor) {
    json_element null_source(nullptr);
    json_element null_copy(null_source);

    ASSERT_EQ(TYPE_NULL, null_copy.type());
    ASSERT_EQ(TYPE_NULL, null_source.type());

    json_element bool_source(true);
    json_element bool_copy(bool_source);

    ASSERT_EQ(TYPE_BOOL, bool_copy.type());
    ASSERT_EQ(true, bool_copy.as<bool>());
    ASSERT_EQ(TYPE_BOOL, bool_source.type());
    ASSERT_EQ(true, bool_source.as<bool>());

    constexpr int ival = -673427572;
    json_element int_source(ival);
    json_element int_copy(int_source);

    ASSERT_EQ(TYPE_SIGNED_INT, int_copy.type());
    ASSERT_EQ(ival, int_copy.as<int>());
    ASSERT_EQ(TYPE_SIGNED_INT, int_source.type());
    ASSERT_EQ(ival, int_source.as<int>());

    constexpr uint uval = 25727344;
    json_element uint_source(uval);
    json_element uint_copy(uint_source);

    ASSERT_EQ(TYPE_UNSIGNED_INT, uint_copy.type());
    ASSERT_EQ(uval, uint_copy.as<uint>());
    ASSERT_EQ(TYPE_UNSIGNED_INT, uint_source.type());
    ASSERT_EQ(uval, uint_source.as<uint>());

    constexpr double fval = -78383.5867e-2;
    json_element double_source(fval);
    json_element double_copy(double_source);

    ASSERT_EQ(TYPE_DOUBLE, double_copy.type());
    ASSERT_DOUBLE_EQ(fval, double_copy.as<double>());
    ASSERT_EQ(TYPE_DOUBLE, double_source.type());
    ASSERT_DOUBLE_EQ(fval, double_source.as<double>());

    static char strval[] = "hello world! I am Jayson";
    json_element str_source(strval);
    json_element str_copy(str_source);

    ASSERT_EQ(TYPE_JSON_STRING, str_copy.type());
    ASSERT_EQ(TYPE_JSON_STRING, str_source.type());
    ASSERT_STREQ(strval, str_copy.as<const char *>());
    ASSERT_STREQ(strval, str_source.as<const char *>());
}

TEST(json_element, default_constructor) {
    json_element var;
    ASSERT_EQ(TYPE_NULL, var.type());
}

TEST(json_element, move_constructor) {
    constexpr int ival = -2747572;
    constexpr uint uval = 57727273;
    constexpr double fval = -48285.2e-2;
    static char teststr[] = "the birth of sigmar";

    json_element null_a(nullptr);
    json_element bool_a(true);
    json_element int_a(ival);
    json_element uint_a(uval);
    json_element double_a(fval);
    json_element str_a(teststr);

    assert_type(null_a, nullptr_t);
    assert_type(bool_a, bool);
    assert_type(int_a, int);
    assert_type(uint_a, uint);
    assert_type(double_a, double);
    assert_type(str_a, const char *);

    json_element null_b(move(null_a));
    json_element bool_b(move(bool_a));
    json_element int_b(move(int_a));
    json_element uint_b(move(uint_a));
    json_element double_b(move(double_a));
    json_element str_b(move(str_a));

    assert_type(null_a, nullptr_t);
    assert_type(bool_a, nullptr_t);
    assert_type(int_a, nullptr_t);
    assert_type(uint_a, nullptr_t);
    assert_type(double_a, nullptr_t);
    assert_type(str_a, nullptr_t);

    assert_type(null_b, nullptr_t);
    assert_type(bool_b, bool);
    assert_type(int_b, int);
    assert_type(uint_b, uint);
    assert_type(double_b, double);
    assert_type(str_b, const char *);

    ASSERT_EQ(true, bool_b.as<bool>());
    ASSERT_EQ(ival, int_b.as<int>());
    ASSERT_EQ(uval, uint_b.as<uint>());
    ASSERT_DOUBLE_EQ(fval, double_b.as<double>());
    ASSERT_STREQ(teststr, str_b.as<const char *>());

    ASSERT_STREQ(nullptr, null_a.as<const char *>());
    ASSERT_STREQ(nullptr, str_a.as<const char *>());
}

TEST(json_element, copy_operators) {
    constexpr int ival = -5828474;
    constexpr uint uval = 382478214;
    constexpr double fval = -59382.23e-4;
    static char strval[] = "testing string";

    json_element a_null(nullptr);
    json_element a_bool(true);
    json_element a_int(ival);
    json_element a_uint(uval);
    json_element a_double(fval);
    json_element a_str(strval);

    json_element b_null;
    json_element b_bool;
    json_element b_int;
    json_element b_uint;
    json_element b_double;
    json_element b_str;

    assert_nullinit(b_null);
    assert_nullinit(b_bool);
    assert_nullinit(b_int);
    assert_nullinit(b_uint);
    assert_nullinit(b_double);
    assert_nullinit(b_str);

    b_null = a_null;
    b_bool = a_bool;
    b_int = a_int;
    b_uint = a_uint;
    b_double = a_double;
    b_str = a_str;

    assert_type(a_null, nullptr_t);
    assert_type(a_bool, bool);
    assert_type(a_int, int);
    assert_type(a_uint, uint);
    assert_type(a_double, double);
    assert_type(a_str, const char *);

    assert_type(b_null, nullptr_t);
    assert_type(b_bool, bool);
    assert_type(b_int, int);
    assert_type(b_uint, uint);
    assert_type(b_double, double);
    assert_type(b_str, const char *);

    ASSERT_EQ(true, b_bool.as<bool>());
    ASSERT_EQ(ival, b_int.as<int>());
    ASSERT_EQ(uval, b_uint.as<uint>());
    ASSERT_DOUBLE_EQ(fval, b_double.as<double>());
    ASSERT_STREQ(strval, b_str.as<const char *>());

    ASSERT_EQ(true, a_bool.as<bool>());
    ASSERT_EQ(ival, a_int.as<int>());
    ASSERT_EQ(uval, a_uint.as<uint>());
    ASSERT_DOUBLE_EQ(fval, a_double.as<double>());
    ASSERT_STREQ(strval, a_str.as<const char *>());
}

TEST(json_element, move_operator) {
    constexpr int ival = -5828474;
    constexpr uint uval = 382478214;
    constexpr double fval = -59382.23e-4;
    static char strval[] = "testing string";

    json_element a_null(nullptr);
    json_element a_bool(true);
    json_element a_int(ival);
    json_element a_uint(uval);
    json_element a_double(fval);
    json_element a_str(strval);

    json_element b_null;
    json_element b_bool;
    json_element b_int;
    json_element b_uint;
    json_element b_double;
    json_element b_str;

    b_null = move(a_null);
    b_bool = move(a_bool);
    b_int = move(a_int);
    b_uint = move(a_uint);
    b_double = move(a_double);
    b_str = move(a_str);

    assert_nullinit(a_null);
    assert_nullinit(a_bool);
    assert_nullinit(a_int);
    assert_nullinit(a_uint);
    assert_nullinit(a_double);
    assert_nullinit(a_str);

    assert_type(b_null, nullptr_t);
    assert_type(b_bool, bool);
    assert_type(b_int, int);
    assert_type(b_uint, uint);
    assert_type(b_double, double);
    assert_type(b_str, const char *);

    ASSERT_EQ(true, b_bool.as<bool>());
    ASSERT_EQ(ival, b_int.as<int>());
    ASSERT_EQ(uval, b_uint.as<uint>());
    ASSERT_DOUBLE_EQ(fval, b_double.as<double>());
    ASSERT_STREQ(strval, b_str.as<const char *>());
}

TEST(json_element, operators) {
    json_element var(static_cast<int>(10));
    assert_type(var, int);
    ASSERT_EQ(10, var.as<int>());

    var = nullptr;
    assert_type(var, nullptr_t);

    var = true;
    assert_type(var, bool);
    ASSERT_EQ(true, var.as<bool>());

    var = static_cast<char>('g');
    assert_type(var, char);
    ASSERT_EQ('g', var.as<char>());

    var = static_cast<unsigned short>(43);
    assert_type(var, unsigned short);
    ASSERT_EQ(43, var.as<unsigned short>());

    var = static_cast<long>(54333);
    assert_type(var, long);
    ASSERT_EQ(54333, var.as<int>());

    constexpr float fval = -482.23f;
    constexpr double dval = 5782723.124452;

    var = fval;
    assert_type(var, float);
    ASSERT_FLOAT_EQ(fval, var.as<float>());

    var = dval;
    assert_type(var, double);
    ASSERT_DOUBLE_EQ(dval, var.as<double>());

    static char str1[] = "testing string 1";
    static char str2[] = "2 string testing";

    var = str1;
    assert_type(var, const char *);
    ASSERT_STREQ(str1, var.as<const char *>());

    var = str2;
    assert_type(var, char *);
    ASSERT_STREQ(str2, var.as<char *>());

    static char str3[] = "The tide of chaos stretches from the North";
    dynamic_string dstr(str3);

    var = dstr;
    assert_type(var, dynamic_string);
    ASSERT_STREQ(str3, var.as<const char *>());
    ASSERT_EQ(dstr, var.as<dynamic_string>());
}

TEST(json_element, implicit_equals) {
    json_element element_null;
    json_element element_bool(true);
    json_element element_int(-50);
    json_element element_float(60.70);
    json_element element_string("hello");

    ASSERT_TRUE(element_null == nullptr);
    ASSERT_FALSE(element_null == true);
    ASSERT_FALSE(element_null == 10);
    ASSERT_FALSE(element_null == 50.56);
    ASSERT_FALSE(element_null == "hello");

    ASSERT_FALSE(element_bool == nullptr);
    ASSERT_FALSE(element_bool == false);
    ASSERT_TRUE(element_bool == true);
    ASSERT_FALSE(element_bool == 24);
    ASSERT_FALSE(element_bool == 60.67);
    ASSERT_FALSE(element_bool == "55677");

    ASSERT_FALSE(element_int == nullptr);
    ASSERT_FALSE(element_int == true);
    ASSERT_FALSE(element_int == 10);
    ASSERT_TRUE(element_int == -50);
    ASSERT_FALSE(element_int == 44.44);
    ASSERT_FALSE(element_int == "hello");

    ASSERT_FALSE(element_float == nullptr);
    ASSERT_FALSE(element_float == true);
    ASSERT_FALSE(element_float == 60);
    ASSERT_FALSE(element_float == -555.555);
    ASSERT_TRUE(element_float == 60.70);
    ASSERT_FALSE(element_float == "hello");

    ASSERT_FALSE(element_string == nullptr);
    ASSERT_FALSE(element_string == true);
    ASSERT_FALSE(element_string == 10);
    ASSERT_FALSE(element_string == 10.555);
    ASSERT_FALSE(element_string == "noway");
    ASSERT_TRUE(element_string == "hello");

    ASSERT_TRUE(element_int == static_cast<short>(-50));
    ASSERT_TRUE(element_int == static_cast<long>(-50));

    json_element zero_float(0.0f);
    ASSERT_TRUE(zero_float == 0.0);
    ASSERT_TRUE(zero_float == 0.0f);

    dynamic_string hello("hello");
    ASSERT_TRUE(element_string == hello);

    ASSERT_TRUE(zero_float != 10.0);
}

TEST(json_element, implicit_less) {
    ASSERT_FALSE(null_e < nullptr);
    ASSERT_FALSE(bool_e < false);
    ASSERT_FALSE(bool_e < true);
    ASSERT_TRUE(false_e < true);
    ASSERT_TRUE(null_e < 10);
    ASSERT_TRUE(bool_e < 10);
    json_element e_int(50);
    ASSERT_FALSE(e_int < nullptr);
    ASSERT_FALSE(e_int < true);
    ASSERT_TRUE(e_int < 70);
    ASSERT_FALSE(e_int < 20);
    ASSERT_TRUE(e_int < 66.666);
    json_element e_float(45.56f);
    ASSERT_FALSE(e_float < nullptr);
    ASSERT_FALSE(e_float < true);
    ASSERT_FALSE(e_float < 10000);
    ASSERT_FALSE(e_float < 20.0);
    ASSERT_TRUE(e_float < 100.0f);
    ASSERT_TRUE(e_float < 1000.0);
    ASSERT_TRUE(e_float < "hello");
    json_element str("hohohoho");
    ASSERT_TRUE(str < "zzozozo");
    ASSERT_FALSE(str < "aasdfasd");
    ASSERT_FALSE(str < 35.3553);
}

TEST(json_element, implicit_comparisons) {
    json_element e1(4);
    json_element e2(55.55);
    json_element e3("hello");

    ASSERT_TRUE(e1 == 4);
    ASSERT_TRUE(4 == e1);
    ASSERT_FALSE(e1 == 3);
    ASSERT_FALSE(3 == e1);

    ASSERT_TRUE(e2 == 55.55);
    ASSERT_TRUE(55.55 == e2);
    ASSERT_FALSE(e2 == 44.44);
    ASSERT_FALSE(44.44 == e2);

    ASSERT_TRUE(e3 == "hello");
    ASSERT_TRUE("hello" == e3);
    ASSERT_FALSE(e3 == "womba");
    ASSERT_FALSE("womba" == e3);

    ASSERT_TRUE(e1 != 3);
    ASSERT_TRUE(3 != e1);
    ASSERT_FALSE(e1 != 4);
    ASSERT_FALSE(4 != e1);

    ASSERT_TRUE(e2 != 2.22);
    ASSERT_TRUE(2.22 != e2);
    ASSERT_FALSE(e2 != 55.55);
    ASSERT_FALSE(e2 != 55.55);

    ASSERT_TRUE(e3 != "womba");
    ASSERT_TRUE("womba" != e3);
    ASSERT_FALSE(e3 != "hello");
    ASSERT_FALSE("hello" != e3);

    ASSERT_TRUE(e1 < 5);
    ASSERT_TRUE(2 < e1);
    ASSERT_FALSE(e1 < 1);
    ASSERT_FALSE(10 < e1);

    ASSERT_TRUE(e2 < 66.66);
    ASSERT_TRUE(33.33 < e2);
    ASSERT_FALSE(e2 < -55.55);
    ASSERT_FALSE(77.77 < e2);

    ASSERT_TRUE(e3 < "zoppa");
    ASSERT_TRUE("ahaha" < e3);
    ASSERT_FALSE(e3 < "apppa");
    ASSERT_FALSE("zoppa" < e3);

    ASSERT_TRUE(e1 > 1);
    ASSERT_TRUE(10 > e1);
    ASSERT_FALSE(e1 > 1000);
    ASSERT_FALSE(-40 > e1);

    ASSERT_TRUE(e2 > -50.0);
    ASSERT_TRUE(500.00 > e2);
    ASSERT_FALSE(e2 > 5000.0);
    ASSERT_FALSE(-20.0 > e2);

    ASSERT_TRUE(e3 > "apple");
    ASSERT_TRUE("zebra" > e3);
    ASSERT_FALSE(e3 > "zebra");
    ASSERT_FALSE("apple" > e3);
    
    ASSERT_TRUE(e1 <= 5);
    ASSERT_TRUE(2 <= e1);
    ASSERT_FALSE(e1 <= 1);
    ASSERT_FALSE(10 <= e1);

    ASSERT_TRUE(e2 <= 66.66);
    ASSERT_TRUE(33.33 <= e2);
    ASSERT_FALSE(e2 <= -55.55);
    ASSERT_FALSE(77.77 <= e2);

    ASSERT_TRUE(e3 <= "zoppa");
    ASSERT_TRUE("ahaha" <= e3);
    ASSERT_FALSE(e3 <= "apppa");
    ASSERT_FALSE("zoppa" <= e3);

    ASSERT_TRUE(e1 >= 1);
    ASSERT_TRUE(10 >= e1);
    ASSERT_FALSE(e1 >= 1000);
    ASSERT_FALSE(-40 >= e1);

    ASSERT_TRUE(e2 >= -50.0);
    ASSERT_TRUE(500.00 >= e2);
    ASSERT_FALSE(e2 >= 5000.0);
    ASSERT_FALSE(-20.0 >= e2);

    ASSERT_TRUE(e3 >= "apple");
    ASSERT_TRUE("zebra" >= e3);
    ASSERT_FALSE(e3 >= "zebra");
    ASSERT_FALSE("apple" >= e3);
}
