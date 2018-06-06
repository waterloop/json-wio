#include <stdio.h>
#include <string.h>

#include <wlib/memory>

#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

// union constructors
json_element::data::data() {}
json_element::data::data(long long i) : integer(i) {}
json_element::data::data(long double f) : floating(f) {}

// null constructor
json_element::json_element() :
    m_type(TYPE_NULL) {}
json_element::json_element(nullptr_t) :
    m_type(TYPE_NULL) {}

// string type constructors
json_element::json_element(char *str) :
    json_element(
        static_cast<const char *>(str),
        static_cast<size_type>(strlen(str))) {}
json_element::json_element(char *str, size_type size) :
    json_element(static_cast<const char *>(str), size) {}
json_element::json_element(const char *str) :
    json_element(str, static_cast<size_type>(strlen(str))) {}
json_element::json_element(const char *str, size_type size) :
    m_str(str, size),
    m_type(TYPE_JSON_STRING) {}
json_element::json_element(const dynamic_string &str) :
    m_str(str),
    m_type(TYPE_JSON_STRING) {}

// copy constructor and operator
json_element::json_element(const json_element &) = default;
json_element &json_element::operator=(const json_element &) = default;

// move constructor and operator
json_element::json_element(json_element &&je) :
    m_data(move(je.m_data)),
    m_str(move(je.m_str)),
    m_type(move(je.m_type)) {
    je.m_type = TYPE_NULL;
}
json_element &json_element::operator=(json_element &&je) {
    m_data = move(je.m_data);
    m_str = move(je.m_str);
    m_type = move(je.m_type);
    je.m_type = TYPE_NULL;
}


// json_type checks
bool json_element::is_primitive() { return m_type < TYPE_JSON_STRING; }
bool json_element::is_null() { return m_type == TYPE_NULL; }
bool json_element::is_bool() { return m_type == TYPE_BOOL; }
bool json_element::is_int() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_UNSIGNED_LONG_LONG;
}
bool json_element::is_signed_int() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_SIGNED_LONG_LONG;
}
bool json_element::is_unsigned_int() {
    return TYPE_UNSIGNED_CHAR <= m_type &&
           m_type <= TYPE_UNSIGNED_LONG_LONG;
}
bool json_element::is_float() {
    return TYPE_FLOAT <= m_type &&
           m_type <= TYPE_LONG_DOUBLE;
}
bool json_element::is_number() {
    return TYPE_CHAR <= m_type &&
           m_type <= TYPE_LONG_DOUBLE;
}
bool json_element::is_string() { return m_type == TYPE_JSON_STRING; }
bool json_element::is_array() { return m_type == TYPE_JSON_ARRAY; }
bool json_element::is_object() { return m_type == TYPE_JSON_OBJECT; }

// string content checking
bool json_element::is_string_null() {
    return m_str.length() == STR_SIZE_NULL &&
           m_str == STR_NULL;
}
bool json_element::is_string_true() {
    return m_str.length() == STR_SIZE_TRUE &&
           m_str == STR_TRUE;
}
bool json_element::is_string_false() {
    return m_str.length() == STR_SIZE_FALSE &&
           m_str == STR_FALSE;
}
bool json_element::is_string_bool() {
    return is_string_true() || is_string_false();
}
bool json_element::is_string_int() {
    return string_is_int(m_str.c_str());
}
bool json_element::is_string_float() {
    return string_is_float(m_str.c_str());
}
bool json_element::is_string_array() {
    return false;
}
bool json_element::is_string_object() {
    return false;
}

// conversion checks
bool json_element::convertible_to_null() {
    return is_null() || (is_string() && is_string_null());
}
bool json_element::convertible_to_bool() {
    return m_type <= TYPE_LONG_DOUBLE || (is_string() && (!m_str.length() || is_string_bool()));
}
bool json_element::convertible_to_int() {
    return m_type <= TYPE_UNSIGNED_LONG_LONG || (is_string() && is_string_int());
}
bool json_element::convertible_to_float() {
    return m_type <= TYPE_LONG_DOUBLE || (is_string() && is_string_float());
}
bool json_element::convertible_to_string() {
    return true;
}

// type conversions
namespace bool_convert {
    typedef bool (*type)(json_element *);
    static bool from_null(json_element *) { return false; }
    static bool from_int(json_element *je) { return static_cast<bool>(je->integer()); }
    static bool from_float(json_element *je) { return static_cast<bool>(je->floating()); }
    static bool from_str(json_element *je) { return je->is_string_true(); }
    static bool from_arr(json_element *) { return false; }
    static bool from_obj(json_element *) { return false; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace long_convert {
    typedef long long (*type)(json_element *);
    static long long from_null(json_element *) { return 0; }
    static long long from_int(json_element *je) { return je->integer(); }
    static long long from_float(json_element *je) { return static_cast<long long>(je->floating()); }
    static long long from_str(json_element *je) { return strtoll(je->str().c_str(), nullptr, 10); }
    static long long from_arr(json_element *) { return 0; }
    static long long from_obj(json_element *) { return 0; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace double_convert {
    typedef long double (*type)(json_element *);
    static long double from_null(json_element *) { return 0; }
    static long double from_int(json_element *je) { return static_cast<long double>(je->integer()); }
    static long double from_float(json_element *je) { return je->floating(); }
    static long double from_str(json_element *je) { return strtold(je->str().c_str(), nullptr); }
    static long double from_arr(json_element *) { return 0.0; }
    static long double from_obj(json_element *) { return 0.0; }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace string_convert {
    typedef void (*type)(json_element *je);
    static void from_null(json_element *je) {
        je->str().set_value(STR_NULL, STR_SIZE_NULL);
    }
    static void from_bool(json_element *je) {
        if (static_cast<bool>(je->integer())) {
            je->str().set_value(STR_TRUE, STR_SIZE_TRUE);
        } else { je->str().set_value(STR_FALSE, STR_SIZE_FALSE); }
    }
    static void from_int(json_element *je) {
        constexpr int bufsize = (8 * sizeof(long long) / 3) + 4;
        je->str().resize(bufsize);
        je->str().length_set(static_cast<size_type>(sprintf(je->str().c_str(), "%lli", je->integer())));
    }
    static void from_float(json_element *je) {
        constexpr int bufsize = 11 + LDBL_MAX_10_EXP;
        je->str().resize(bufsize);
        je->str().length_set(static_cast<size_type>(sprintf(je->str().c_str(), "%Lf", je->floating())));
    }
    static void from_str(json_element *) {}
    static void from_arr(json_element *) {}
    static void from_obj(json_element *) {}
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_bool, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}

nullptr_t json_element::convert_to_null() {
    return nullptr;
}
bool json_element::convert_to_bool() {
    return bool_convert::converter[m_type >> 4](this);
}
long long json_element::convert_to_int() {
    return long_convert::converter[m_type >> 4](this);
}
long double json_element::convert_to_float() {
    return double_convert::converter[m_type >> 4](this);
}
const char *json_element::convert_to_string() {
    string_convert::converter[m_type >> 4](this);
    return m_str.c_str();
}

// handle conversion to dynamic_string
dynamic_string json_element::convert_to_dynamic_string() {
    convert_to_string();
    return m_str;
}

long long json_element::integer() const { return m_data.integer; }
long double json_element::floating() const { return m_data.floating; }
dynamic_string &json_element::str() { return m_str; }
const dynamic_string &json_element::str() const { return m_str; }
json_type json_element::type() const { return m_type; }

