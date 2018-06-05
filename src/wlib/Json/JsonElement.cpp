#include <string.h>
#include <wlib/memory>

#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonString.h>
#include <wlib/Json/JsonElement.h>

#define switch_size_cast(target_t, fill_t, data)    \
case sizeof(fill_t):                                \
    return static_cast<target_t>(*reinterpret_cast<fill_t *>(data));

using namespace wlp;

json_element::~json_element() {
    if (m_type == TYPE_JSON_STRING) {
        free<char>(static_cast<char *>(m_data));
    } else {
        // HACK need to cast to something else than void
        free(static_cast<int *>(m_data));
    }
}

json_element::json_element(nullptr_t) :
    m_data(nullptr),
    m_type(TYPE_NULL),
    m_size(sizeof(nullptr_t)) {}

json_element::json_element(char *str) :
    json_element(static_cast<const char *>(str), strlen(str)) {}

json_element::json_element(char *str, size_type size) :
    json_element(static_cast<const char *>(str), size) {}

json_element::json_element(const char *str) :
    json_element(str, strlen(str)) {}

json_element::json_element(const char *str, size_type size) :
    m_data(malloc<char[]>(size + 1)),
    m_type(TYPE_JSON_STRING),
    m_size(size) {
    memcpy(m_data, str, size);
    static_cast<char *>(m_data)[size] = '\0';
}

json_element::json_element(const dynamic_string &str) :
    json_element(static_cast<const char *>(str.c_str()), str.length()) {}

bool json_element::is_null() {
    return m_type == TYPE_NULL;
}

bool json_element::is_bool() {
    return m_type == TYPE_BOOL;
}

bool json_element::is_int() {
    return TYPE_CHAR <= m_type && m_type <= TYPE_UNSIGNED_LONG_LONG;
}

bool json_element::is_signed_int() {
    return TYPE_CHAR <= m_type && m_type <= TYPE_SIGNED_LONG_LONG;
}

bool json_element::is_unsigned_int() {
    return TYPE_UNSIGNED_CHAR <= m_type && m_type <= TYPE_UNSIGNED_LONG_LONG;
}

bool json_element::is_float() {
    return TYPE_FLOAT <= m_type && m_type <= TYPE_LONG_DOUBLE;
}

bool json_element::is_number() {
    return TYPE_CHAR <= m_type && m_type <= TYPE_LONG_DOUBLE;
}

bool json_element::is_string() {
    return m_type == TYPE_JSON_STRING;
}

bool json_element::is_array() {
    return m_type == TYPE_JSON_ARRAY;
}

bool json_element::is_object() {
    return m_type == TYPE_JSON_OBJECT;
}

// Type convertibility to another json_type
// We let `null` convert to any other type as the equilvalent false-y value
// We let `true` and `false` convert to number as `1` and `0`
// We let array and object convert to string to stringify
bool json_element::convertible_to(json_type type) {
    if (is_null()) {
        return true;
    } else if (is_bool()) {
        return TYPE_BOOL <= type && type <= TYPE_JSON_STRING;
    } else if (is_number()) {
        return TYPE_CHAR <= type <= TYPE_JSON_STRING;
    } else if (is_array()) {
        return
            type == TYPE_JSON_STRING ||
            type == TYPE_JSON_ARRAY;
    } else if (is_object()) {
        return
            type == TYPE_JSON_STRING ||
            type == TYPE_JSON_OBJECT;
    } else {
        return false;
    }
}

bool json_element::is_string_null() {
    return reinterpret_cast<json_string *>(this)->contents_are_null();
}
bool json_element::is_string_bool() {
    return reinterpret_cast<json_string *>(this)->contents_are_bool();
}
bool json_element::is_string_int() {
    return reinterpret_cast<json_string *>(this)->contents_are_int();
}
bool json_element::is_string_float() {
    return reinterpret_cast<json_string *>(this)->contents_are_float();
}
bool json_element::is_string_array() {
    return reinterpret_cast<json_string *>(this)->contents_are_array();
}
bool json_element::is_string_object() {
    return reinterpret_cast<json_string *>(this)->contents_are_object();
}

nullptr_t json_element::convert_to_null() {
    return nullptr;
}

bool json_element::convert_to_bool() {
    if (is_null()) {
        return false;
    } else if (is_bool()) {
        return data_assign(bool, m_data);
    } else if (is_signed_int()) {
        switch (m_size) {
            switch_size_cast(bool, char, m_data)
            switch_size_cast(bool, short, m_data)
            switch_size_cast(bool, int, m_data)
            switch_size_cast(bool, long, m_data)
#if __WLIB_LONG_LONG__
            switch_size_cast(bool, long long, m_data)
#endif
        default:
            return false;
        }
    } else if (is_unsigned_int()) {
        switch (m_size) {
            switch_size_cast(bool, unsigned char, m_data)
            switch_size_cast(bool, unsigned short, m_data)
            switch_size_cast(bool, unsigned int, m_data)
            switch_size_cast(bool, unsigned long, m_data)
#if __WLIB_LONG_LONG__
            switch_size_cast(bool, unsigned long long, m_data)
#endif
        default:
            return false;
        }
    } else if (is_float()) {
        switch (m_size) {
            switch_size_cast(bool, float, m_data)
            switch_size_cast(bool, double, m_data)
#if __WLIB_LONG_DOUBLE__
            switch_size_cast(bool, long double, m_data)
#endif
        default:
            return false;
        }
    } else if (is_string()) {
        return reinterpret_cast<json_string *>(this)->contents_are_true();
    } else {
        return false;
    }
}

dynamic_string json_element::convert_to_dynamic_string() {
    convert_to_c_str<const char *>();
    return m_str;
}

void *json_element::data() { return m_data; }
json_type json_element::type() { return m_type; }
int json_element::size() { return m_size; }
