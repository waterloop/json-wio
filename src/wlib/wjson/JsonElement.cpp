#include <stdio.h>
#include <float.h>

#include <wlib/wjson/JsonElement.h>
#include <wlib/wjson/JsonNull.h>

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

// json array and json object
json_element::json_element(json_array &&arr) :
    m_array(move(arr)),
    m_type(TYPE_JSON_ARRAY) {}

json_element::json_element(json_object &&obj) :
    m_object(move(obj)),
    m_type(TYPE_JSON_OBJECT) {}

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

// destructor
json_element::~json_element()
{ destroy::functions[m_type >> 4](this); }

// assignment jump tables
namespace copy_assign {
    typedef void (*data_copier)(json_element *, const json_element &);
    static void copy_null(json_element *, const json_element &) {}
    static void copy_int(json_element *dst, const json_element &src) {
        dst->integer() = src.integer();
    }
    static void copy_float(json_element *dst, const json_element &src) {
        dst->floating() = src.floating();
    }
    static void copy_string(json_element *dst, const json_element &src) {
        ::new(&dst->string()) dynamic_string(src.string());
    }
    static void copy_array(json_element * /*dst*/, const json_element &src) {
        //::new(&dst->array()) json_array(src.array());
    }
    static void copy_object(json_element * /*dst*/, const json_element &src) {
        //::new(&dst->object()) json_object(src.object());
    }
    static data_copier functions[json_type::NUM_CLASS] = {
        copy_null, copy_int, copy_int,
        copy_float, copy_string,
        copy_array, copy_object
    };
}
namespace move_assign {
    typedef void (*data_mover)(json_element *, json_element &&);
    static void move_null(json_element *, json_element &&) {}
    static void move_int(json_element *dst, json_element &&src) {
        dst->integer() = move(src.integer());
    }
    static void move_float(json_element *dst, json_element &&src) {
        dst->floating() = move(src.floating());
    }
    static void move_string(json_element *dst, json_element &&src) {
        ::new(&dst->string()) dynamic_string(move(src.string()));
        src.string().~dynamic_string();
    }
    static void move_array(json_element *dst, json_element &&src) {
        ::new(&dst->array()) json_array(move(src.array()));
    }
    static void move_object(json_element *dst, json_element &&src) {
        ::new(&dst->object()) json_object(move(src.object()));
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
json_element::json_element(json_element &&je) noexcept :
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
json_element &json_element::operator=(json_element &&je) noexcept {
    destroy::functions[m_type >> 4](this);
    m_type = move(je.m_type);
    move_assign::functions[m_type >> 4](this, move(je));
    je.m_type = TYPE_NULL;
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
    ::new(&m_string) dynamic_string(str, len);
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
    ::new(&m_string) dynamic_string(move(str));
    m_type = TYPE_JSON_STRING;
    return *this;
}

// json array and object
json_element &json_element::operator=(json_array &&arr) {
    destroy::functions[m_type >> 4](this);
    ::new(&m_array) json_array(move(arr));
    m_type = TYPE_JSON_ARRAY;
    return *this;
}
json_element &json_element::operator=(json_object &&obj) {
    destroy::functions[m_type >> 4](this);
    ::new(&m_object) json_object(move(obj));
    m_type = TYPE_JSON_OBJECT;
    return *this;
}

// json_type checks
template<json_type lo, json_type hi>
static bool between(json_type i)
{ return lo <= i && i <= hi; }

bool json_element::is_null() const { return m_type == TYPE_NULL; }
bool json_element::is_bool() const { return m_type == TYPE_BOOL; }
bool json_element::is_int() const { return m_type >> 4 == CLASS_INT; }
bool json_element::is_float() const { return m_type >> 4 == CLASS_FLOAT; }
bool json_element::is_number() const
{ return between<TYPE_CHAR, TYPE_LONG_DOUBLE>(m_type); }
bool json_element::is_string() const { return m_type == TYPE_JSON_STRING; }
bool json_element::is_array() const { return m_type == TYPE_JSON_ARRAY; }
bool json_element::is_object() const { return m_type == TYPE_JSON_OBJECT; }

// string content checking
bool json_element::is_string_null() const
{ return m_string.length() == STR_SIZE_NULL && m_string == STR_NULL; }
bool json_element::is_string_true() const
{ return m_string.length() == STR_SIZE_TRUE && m_string == STR_TRUE; }
bool json_element::is_string_false() const
{ return m_string.length() == STR_SIZE_FALSE && m_string == STR_FALSE; }
bool json_element::is_string_bool() const
{ return is_string_true() || is_string_false(); }
bool json_element::is_string_int() const
{ return string_is_int(m_string.c_str()); }
bool json_element::is_string_float() const
{ return string_is_float(m_string.c_str()); }
bool json_element::is_string_array() const
{ return false; }
bool json_element::is_string_object() const
{ return false; }

// conversion checks
bool json_element::convertible_to_null() const
{ return is_null() || (is_string() && is_string_null()); }
bool json_element::convertible_to_bool() const {
    return m_type <= TYPE_LONG_DOUBLE ||
    (is_string() && (!m_string.length() || is_string_bool()));
}
bool json_element::convertible_to_int() const {
    return m_type <= TYPE_UNSIGNED_LONG_LONG ||
    (is_string() && is_string_int());
}
bool json_element::convertible_to_float() const {
    return m_type <= TYPE_LONG_DOUBLE ||
    (is_string() && is_string_float());
}
bool json_element::convertible_to_string() const
{ return is_string(); }

// type conversions
namespace convert_bool {
    typedef bool (*type)(const json_element *);
    static bool from_null(const json_element *)
    { return false; }
    static bool from_int(const json_element *je)
    { return static_cast<bool>(je->integer()); }
    static bool from_float(const json_element *je)
    { return static_cast<bool>(je->floating()); }
    static bool from_str(const json_element *je)
    { return je->is_string_true(); }
    static bool from_arr(const json_element *je)
    { return !je->array().empty(); }
    static bool from_obj(const json_element *je)
    { return !je->object().empty(); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace convert_int {
    typedef json_int (*type)(const json_element *);
    static json_int from_null(const json_element *)
    { return 0; }
    static json_int from_int(const json_element *je)
    { return je->integer(); }
    static json_int from_float(const json_element *je)
    { return static_cast<json_int>(je->floating()); }
    static json_int from_str(const json_element *je)
    { return WLIB_JSON_STRTOINT(je->string().c_str(), nullptr); }
    static json_int from_arr(const json_element *je)
    { return je->array().size(); }
    static json_int from_obj(const json_element *je)
    { return je->object().size(); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}
namespace convert_float {
    typedef json_float (*type)(const json_element *);
    static json_float from_null(const json_element *)
    { return 0; }
    static json_float from_int(const json_element *je)
    { return static_cast<json_float>(je->integer()); }
    static json_float from_float(const json_element *je)
    { return je->floating(); }
    static json_float from_str(const json_element *je)
    { return WLIB_JSON_STRTOFLOAT(je->string().c_str(), nullptr); }
    static json_float from_arr(const json_element *je)
    { return static_cast<json_float>(je->array().size()); }
    static json_float from_obj(const json_element *je)
    { return static_cast<json_float>(je->object().size()); }
    static type converter[json_type::NUM_CLASS] = {
        from_null, from_int, from_int,
        from_float, from_str,
        from_arr, from_obj
    };
}

nullptr_t json_element::convert_to_null() const
{ return nullptr; }
bool json_element::convert_to_bool() const
{ return convert_bool::converter[m_type >> 4](this); }
json_int json_element::convert_to_int() const
{ return convert_int::converter[m_type >> 4](this); }
json_float json_element::convert_to_float() const
{ return convert_float::converter[m_type >> 4](this); }
const char *json_element::convert_to_string() const 
{ return is_string() ? m_string.c_str() : nullptr; }
const json_array &json_element::convert_to_array() const
{ return is_array() ? m_array : json::array::null; }
const json_object &json_element::convert_to_object() const
{ return is_object() ? m_object : json::object::null; }

// handle conversion to dynamic_string
dynamic_string json_element::convert_to_dynamic_string() const
{ return is_string() ? m_string : dynamic_string(nullptr); }

// generic access
json_element &json_element::operator[](nullptr_t) {
    if (!is_object()) 
    { return const_cast<json_element &>(json::null); }
    return m_object[nullptr];
}
const json_element &json_element::operator[](nullptr_t) const {
    if (!is_object()) { return json::null; }
    return m_object.at(nullptr);
}

namespace access_int {
    typedef json_element &(*fun_t)(json_element *, json_int);
    typedef const json_element &(*const_t)(const json_element *, json_int);
    static json_element &access_array(json_element *je, json_int i)
    { return je->array()[i]; }
    static const json_element &const_array(const json_element *je, json_int i)
    { return je->array()[i]; }
    static json_element &access_object(json_element *je, json_int i)
    { return je->object()[i]; }
    static const json_element &const_object(const json_element *je, json_int i) 
    { return je->object().at(i); }
    static fun_t functions[2] = 
    { access_array, access_object };
    static const_t const_functions[2] =
    { const_array, const_object };
}
json_element &json_element::access(json_int i) {
    if (m_type < TYPE_JSON_ARRAY) 
    { return const_cast<json_element &>(json::null); }
    return access_int::functions[m_type - TYPE_JSON_ARRAY](this, i);
}
const json_element &json_element::access(json_int i) const {
    if (m_type < TYPE_JSON_ARRAY) { return json::null; }
    return access_int::const_functions[m_type - TYPE_JSON_ARRAY](this, i);
}

json_element &json_element::access(json_float f) {
    if (!is_object()) 
    { return const_cast<json_element &>(json::null); }
    return m_object[f];
}
const json_element &json_element::access(json_float f) const {
    if (!is_object()) { return json::null; }
    return m_object.at(f);
}

json_element &json_element::access(const char *str) {
    if (!is_object())
    { return const_cast<json_element &>(json::null); }
    return m_object[str];
}
const json_element &json_element::access(const char *str) const {
    if (!is_object()) { return json::null; }
    return m_object.at(str);
}

json_element &json_element::operator[](const dynamic_string &str) { 
    if (!is_object())
    { return const_cast<json_element &>(json::null); }
    return m_object[str];
}
const json_element &json_element::operator[](const dynamic_string &str) const {
    if (!is_object()) { return json::null; }
    return m_object.at(str);
}

json_element &json_element::operator[](const json_element &je) {
    if (je.is_int()) { return operator[](je.integer()); }
    if (!is_object()) 
    { return const_cast<json_element &>(json::null); }
    return m_object[je];
}
const json_element &json_element::operator[](const json_element &je) const {
    if (je.is_int()) { return operator[](je.integer()); }
    if (!is_object()) { return json::null; }
    return m_object.at(je);
}

// size query
namespace size_query {
    typedef size_type (*size_get_func_t)(const json_element *);

    static size_type from_string(const json_element *je)
    { return je->string().length(); }
    static size_type from_array(const json_element *je)
    { return je->array().size(); }
    static size_type from_object(const json_element *je) 
    { return je->object().size(); }
    static size_get_func_t functions[3] = 
    { from_string, from_array, from_object };

    static size_type jump_true(const json_element *je) 
    { return functions[(je->type() >> 4) - CLASS_STRING](je); }
    static size_type jump_false(const json_element *je)
    { return 0; }
    static size_get_func_t jump[2] = 
    { jump_false, jump_true };
}
size_type json_element::size() const 
{ return size_query::jump[m_type >> 6](this); }
size_type json_element::length() const
{ return size(); }

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

json_type json_element::type() const
{ return m_type; }
