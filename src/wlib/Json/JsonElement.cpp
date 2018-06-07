#include <stdio.h>
#include <float.h>

#include <wlib/Json/JsonElement.h>

using namespace wlp;

// null constructor
json_element::json_element() :
    m_type(TYPE_NULL) {}
json_element::json_element(nullptr_t) :
    m_type(TYPE_NULL) {}

// string type constructors
json_element::json_element(char *str) : json_element(
        static_cast<const char *>(str),
        static_cast<size_type>(strlen(str))) {}
json_element::json_element(char *str, size_type size) :
    json_element(static_cast<const char *>(str), size) {}
json_element::json_element(const char *str) :
    json_element(str, static_cast<size_type>(strlen(str))) {}

json_element::json_element(const char *str, size_type size) :
    m_string(str, size),
    m_type(TYPE_JSON_STRING) {}

json_element::json_element(const dynamic_string &str) :
    m_string(str),
    m_type(TYPE_JSON_STRING) {}

json_element::json_element(dynamic_string &&str) :
    m_string(move(str)),
    m_type(TYPE_JSON_STRING) {}

// destruction jump tables
namespace destroy {
    typedef void (*data_destroyer)(json_element *);
    static void destroy_null(json_element *) {}
    static void destroy_string(json_element *je) {
        je->string().~dynamic_string();
    }
    static void destroy_array(json_element *je) {
        je->array().~json_array();
    }
    static void destroy_object(json_element *je) {
        je->object().~json_object();
    }
    static data_destroyer functions[json_type::NUM_CLASS] = {
        destroy_null, destroy_null, destroy_null,
        destroy_null, destroy_string,
        destroy_array, destroy_object
    };
}

// assignment jump tables
namespace copy_assign {
    typedef void (*data_copier)(json_element *, const json_element &);
    static void copy_null(json_element *, const json_element &) {}
    static void copy_int(json_element *dst, const json_elememt &src) {
        dst->integer() = src.integer();
    }
    static void copy_float(json_element *dst, const json_element &src) {
        dst->floating() = src.floating();
    }
    static void copy_string(json_element *dst, const json_element &src) {
        dst->string() = src.string();
    }
    static void copy_array(json_element *dst, const json_element &src) {
        dst->array() = src.array();
    }
    static void copy_object(json_element *dst, const json_element &src) {
        dst->object() = src.object();
    }
    static data_copier functions[json_type::NUM_CLASS] = {
        copy_null, copy_int, copy_int,
        copy_float, copy_string,
        copy_array, copy_object
    };
}
namespace move_assign {
    typedef void (*data_mover)(json_element *, json_element &&);
    static void move_null(json_element *dst, json_element &&src) {}
    static void move_int(json_element *dst, json_element &&src) {
        dst->integer() = move(src.integer());
    }
    static void move_float(json_element *dst, json_element &&src) {
        dst->floating() = move(src.floating());
    }
    static void move_string(json_element *dst, json_element &&src) {
        dst->string() = move(src.string());
    }
    static void move_array(json_element *dst, json_element &&src) {
        dst->array() = move(src.array());
    }
    static void move_object(json_element *dst, json_element &&src) {
        dst->object() = move(src.object());
    }
    static data_mover functions[json_type::NUM_CLASS] = {
        move_null, move_int, move_int,
        move_float, move_string,
        move_array, move_object
    };
}

// copy and move constructor
json_element::json_element(const json_element &je) :
    m_type(je.m_type) {
    copy_assign::functions[m_type >> 4](this, je);
}
json_element::json_element(json_element &&je) :
    m_type(move(je.m_type)) {
    move_assign::functions[m_type >> 4](this, move(je));
    je.m_type = TYPE_NULL;
}

// copy and move operator
json_element &json_element::operator=(const json_element &je) {
    destroy::functions[m_type >> 4](this);
    m_type = je.m_type;
    copy_assign::functions[m_type >> 4](this, je);
    return *this;
}
json_element &json_element::operator=(json_element &&je) {
    destroy::functions[m_type >> 4](this);
    m_type = move(je.m_type);
    move_assign::functions[m_type >> 4](this, move(je));
    return *this;
}

// null assignment
json_element &json_element::operator=(nullptr_t) {
    destroy::functions[m_type >> 4](this);
    m_type = TYPE_NULL;
    return *this;
}
// integer assignment
json_element &json_element::assign(json_int i, json_type type) {
    destroy::functions[m_type >> 4](this);
    m_type = type;
    m_integer = i;
    return *this;
}
// float assignment
json_element &json_element::assign(json_float f, json_type type) {
    destroy::functions[m_type >> 4](this);
    m_type = type;
    m_floating = f;
    return *this;
}
// string assignment
json_element &json_element::assign(const char *str, size_type len) {
    destroy::functions[m_type >> 4](this);
    m_string.set_value(str, len);
    m_type = TYPE_JSON_STRING;
    return *this;
}

json_element &json_element::operator=(char *str)
{ return assign(str, static_cast<size_type>(strlen(str))); }

json_element &json_element::operator=(const char *str)
{ return assign(str, static_cast<size_type>(strlen(str))); }

json_element &json_element::operator=(const dynamic_string &str)
{ return assign(str.c_str(), str.length()); }

json_element &json_element::operator=(dynamic_string &&str) {
    destroy::functions[m_type >> 4](this);
    m_string = move(str);
    m_type = TYPE_JSON_STRING;
    return *this;
}

// json_type checks
template<json_type lo, json_type hi>
static bool between(json_type i)
{ return lo <= i && i <= hi; }

bool json_element::is_null() { return m_type == TYPE_NULL; }
bool json_element::is_bool() { return m_type == TYPE_BOOL; }
bool json_element::is_int()
{ return between<TYPE_CHAR, TYPE_UNSIGNED_LONG_LONG>(m_type); }
bool json_element::is_float()
{ return between<TYPE_FLOAT, TYPE_LONG_DOUBLE>(m_type); }
bool json_element::is_number()
{ return between<TYPE_CHAR, TYPE_LONG_DOUBLE>(m_type); }
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
    return m_type <= TYPE_LONG_DOUBLE ||
    (is_string() && (!m_str.length() || is_string_bool()));
}
bool json_element::convertible_to_int() {
    return m_type <= TYPE_UNSIGNED_LONG_LONG ||
    (is_string() && is_string_int());
}
bool json_element::convertible_to_float() {
    return m_type <= TYPE_LONG_DOUBLE ||
    (is_string() && is_string_float());
}
bool json_element::convertible_to_string() {
    return is_string();
}

// type conversions
namespace convert_bool {
    typedef bool (*type)(json_element *);
    static bool from_null(json_element *) { return false; }
    static bool from_int(json_element *je) { return static_cast<bool>(je->integer()); }
    static bool from_float(json_element *je) { return static_cast<bool>(je->floating()); }
    static bool from_str(json_element *je) { return je->is_string_true(); }
    static bool from_arr(json_element *je) { return !je->array().empty(); }
    static bool from_obj(json_element *je) { return !je->object().empty(); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace convert_int {
    typedef json_int (*type)(json_element *);
    static json_int from_null(json_element *) { return 0; }
    static json_int from_int(json_element *je)
    { return je->integer(); }
    static json_int from_float(json_element *je)
    { return static_cast<json_int>(je->floating()); }
    static json_int from_str(json_element *je)
    { return strtoll(je->str().c_str(), nullptr, 10); }
    static json_int from_arr(json_element *je)
    { return je->array().size(); }
    static json_int from_obj(json_element *je)
    { return je->object().size(); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace convert_float {
    typedef json_float (*type)(json_element *);
    static json_float from_null(json_element *) { return 0; }
    static json_float from_int(json_element *je)
    { return static_cast<json_float>(je->integer()); }
    static json_float from_float(json_element *je)
    { return je->floating(); }
    static json_float from_str(json_element *je)
    { return strtold(je->str().c_str(), nullptr); }
    static json_float from_arr(json_element *je)
    { return static_cast<json_float>(je->array().size()); }
    static json_float from_obj(json_element *je)
    { return static_cast<json_float>(je->object().size()); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}

nullptr_t json_element::convert_to_null() {
    return nullptr;
}
bool json_element::convert_to_bool() {
    return convert_bool::converter[m_type >> 4](this);
}
json_int json_element::convert_to_int() {
    return convert_int::converter[m_type >> 4](this);
}
json_float json_element::convert_to_float() {
    return convert_float::converter[m_type >> 4](this);
}
const char *json_element::convert_to_string() {
    return is_string() ? m_string.c_str() : nullptr;
}

// handle conversion to dynamic_string
dynamic_string json_element::convert_to_dynamic_string()
{ return m_string; }

// getters
const json_int &json_element::integer() const
{ return m_integer; }
const json_float &json_element::floating() const
{ return m_floating; }
const dynamic_string &json_element::string() const
{ return m_string; }
const json_array &json_element::array() const
{ return m_array; }
const json_object &json_element::object() const
{ return m_object; }

json_int &json_element::integer()
{ return m_integer; }
json_float &json_element::floating()
{ return m_floating; }
dynamic_string &json_element::string()
{ return m_string; }
json_array &json_element::array()
{ return m_array; }
json_object &json_element::object()
{ return m_object; }

json_type json_element::type() { return m_type; }
