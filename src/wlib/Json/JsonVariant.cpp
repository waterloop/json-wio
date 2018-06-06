#include <stdio.h>
#include <string.h>

#include <wlib/memory>

#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonVariant.h>

using namespace wlp;

// union constructors
json_variant::data::data() {}
json_variant::data::data(long long i) : integer(i) {}
json_variant::data::data(long double f) : floating(f) {}

// null constructor
json_variant::json_variant() :
    m_type(TYPE_NULL) {}
json_variant::json_variant(nullptr_t) :
    m_type(TYPE_NULL) {}

// string type constructors
json_variant::json_variant(char *str) :
    json_variant(
        static_cast<const char *>(str),
        static_cast<size_type>(strlen(str))) {}
json_variant::json_variant(char *str, size_type size) :
    json_variant(static_cast<const char *>(str), size) {}
json_variant::json_variant(const char *str) :
    json_variant(str, static_cast<size_type>(strlen(str))) {}
json_variant::json_variant(const char *str, size_type size) :
    m_str(str, size),
    m_type(TYPE_JSON_STRING) {}
json_variant::json_variant(const dynamic_string &str) :
    m_str(str),
    m_type(TYPE_JSON_STRING) {}

// copy constructor and operator
json_variant::json_variant(const json_variant &) = default;
json_variant &json_variant::operator=(const json_variant &) = default;

// move constructor and operator
json_variant::json_variant(json_variant &&je) :
    m_data(move(je.m_data)),
    m_str(move(je.m_str)),
    m_type(move(je.m_type)) {
    je.m_type = TYPE_NULL;
}
json_variant &json_variant::operator=(json_variant &&je) {
    m_data = move(je.m_data);
    m_str = move(je.m_str);
    m_type = move(je.m_type);
    je.m_type = TYPE_NULL;
}

// assignment operators
json_variant &json_variant::operator=(nullptr_t) {
    m_str.clear();
    m_type = TYPE_NULL;
}
json_variant &json_variant::operator=(char *str) {
    m_str = str;
    m_type = TYPE_JSON_STRING;
}
json_variant &json_variant::operator=(const char *str) {
    m_str = str;
    m_type = TYPE_JSON_STRING;
}
json_variant &json_variant::operator=(const dynamic_string &str) {
    m_str = str;
    m_type = TYPE_JSON_STRING;
}

// json_type checks
bool json_variant::is_primitive() { return m_type < TYPE_JSON_STRING; }
bool json_variant::is_null() { return m_type == TYPE_NULL; }
bool json_variant::is_bool() { return m_type == TYPE_BOOL; }
bool json_variant::is_int() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_UNSIGNED_LONG_LONG;
}
bool json_variant::is_signed_int() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_SIGNED_LONG_LONG;
}
bool json_variant::is_unsigned_int() {
    return TYPE_UNSIGNED_CHAR <= m_type &&
           m_type <= TYPE_UNSIGNED_LONG_LONG;
}
bool json_variant::is_float() {
    return TYPE_FLOAT <= m_type &&
           m_type <= TYPE_LONG_DOUBLE;
}
bool json_variant::is_number() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_LONG_DOUBLE;
}
bool json_variant::is_string() { return m_type == TYPE_JSON_STRING; }
bool json_variant::is_array() { return m_type == TYPE_JSON_ARRAY; }
bool json_variant::is_object() { return m_type == TYPE_JSON_OBJECT; }

// string content checking
bool json_variant::is_string_null() {
    return m_str.length() == STR_SIZE_NULL &&
           m_str == STR_NULL;
}
bool json_variant::is_string_true() {
    return m_str.length() == STR_SIZE_TRUE &&
           m_str == STR_TRUE;
}
bool json_variant::is_string_false() {
    return m_str.length() == STR_SIZE_FALSE &&
           m_str == STR_FALSE;
}
bool json_variant::is_string_bool() {
    return is_string_true() || is_string_false();
}
bool json_variant::is_string_int() {
    return string_is_int(m_str.c_str());
}
bool json_variant::is_string_float() {
    return string_is_float(m_str.c_str());
}
bool json_variant::is_string_array() {
    return false;
}
bool json_variant::is_string_object() {
    return false;
}

// conversion checks
bool json_variant::convertible_to_null() {
    return is_null() || (is_string() && is_string_null());
}
bool json_variant::convertible_to_bool() {
    return m_type <= TYPE_LONG_DOUBLE || (is_string() && (!m_str.length() || is_string_bool()));
}
bool json_variant::convertible_to_int() {
    return m_type <= TYPE_UNSIGNED_LONG_LONG || (is_string() && is_string_int());
}
bool json_variant::convertible_to_float() {
    return m_type <= TYPE_LONG_DOUBLE || (is_string() && is_string_float());
}
bool json_variant::convertible_to_string() {
    return true;
}

// type conversions
namespace bool_convert {
    typedef bool (*type)(json_variant *);
    static bool from_null(json_variant *) { return false; }
    static bool from_int(json_variant *je) { return static_cast<bool>(je->integer()); }
    static bool from_float(json_variant *je) { return static_cast<bool>(je->floating()); }
    static bool from_str(json_variant *je) { return je->is_string_true(); }
    static bool from_arr(json_variant *) { return false; }
    static bool from_obj(json_variant *) { return false; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace long_convert {
    typedef long long (*type)(json_variant *);
    static long long from_null(json_variant *) { return 0; }
    static long long from_int(json_variant *je) { return je->integer(); }
    static long long from_float(json_variant *je) { return static_cast<long long>(je->floating()); }
    static long long from_str(json_variant *je) { return strtoll(je->str().c_str(), nullptr, 10); }
    static long long from_arr(json_variant *) { return 0; }
    static long long from_obj(json_variant *) { return 0; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace double_convert {
    typedef long double (*type)(json_variant *);
    static long double from_null(json_variant *) { return 0; }
    static long double from_int(json_variant *je) { return static_cast<long double>(je->integer()); }
    static long double from_float(json_variant *je) { return je->floating(); }
    static long double from_str(json_variant *je) { return strtold(je->str().c_str(), nullptr); }
    static long double from_arr(json_variant *) { return 0.0; }
    static long double from_obj(json_variant *) { return 0.0; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace string_convert {
    typedef void (*type)(json_variant *je);
    static void from_null(json_variant *je) {
        je->str().set_value(STR_NULL, STR_SIZE_NULL);
    }
    static void from_bool(json_variant *je) {
        if (static_cast<bool>(je->integer())) {
            je->str().set_value(STR_TRUE, STR_SIZE_TRUE);
        } else { je->str().set_value(STR_FALSE, STR_SIZE_FALSE); }
    }
    static void from_int(json_variant *je) {
        constexpr int bufsize = (8 * sizeof(long long) / 3) + 4;
        je->str().resize(bufsize);
        je->str().length_set(static_cast<size_type>(sprintf(je->str().c_str(), "%lli", je->integer())));
    }
    static void from_float(json_variant *je) {
        constexpr int bufsize = 11 + LDBL_MAX_10_EXP;
        je->str().resize(bufsize);
        je->str().length_set(static_cast<size_type>(sprintf(je->str().c_str(), "%Lf", je->floating())));
    }
    static void from_str(json_variant *) {}
    static void from_arr(json_variant *) {}
    static void from_obj(json_variant *) {}
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_bool, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}

nullptr_t json_variant::convert_to_null() {
    return nullptr;
}
bool json_variant::convert_to_bool() {
    return bool_convert::converter[m_type >> 4](this);
}
long long json_variant::convert_to_int() {
    return long_convert::converter[m_type >> 4](this);
}
long double json_variant::convert_to_float() {
    return double_convert::converter[m_type >> 4](this);
}
const char *json_variant::convert_to_string() {
    string_convert::converter[m_type >> 4](this);
    return m_str.c_str();
}

// handle conversion to dynamic_string
dynamic_string json_variant::convert_to_dynamic_string() {
    convert_to_string();
    return m_str;
}

long long json_variant::integer() const { return m_data.integer; }
long double json_variant::floating() const { return m_data.floating; }
dynamic_string &json_variant::str() { return m_str; }
const dynamic_string &json_variant::str() const { return m_str; }
json_type json_variant::type() const { return m_type; }

