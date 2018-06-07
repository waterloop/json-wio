#ifndef __WLIB_JSON_JSONTYPE_H__
#define __WLIB_JSON_JSONTYPE_H__

#include <stdlib.h>

#include <wlib/string>
#include <wlib/utility>

#define TYPE_DECL(type_v, type_t)               \
template<>                                      \
struct type_of<type_v> {                        \
    typedef type_t type;                        \
};                                              \
template<>                                      \
struct type_info<type_t> {                      \
    static constexpr json_type value = type_v;  \
};

#define TYPE_INFO(type_v, type_t)               \
template<>                                      \
struct type_info<type_t> {                      \
    static constexpr json_type value = type_v;  \
};

namespace wlp {

    typedef unsigned char json_type_t;

    enum json_type : json_type_t {
        TYPE_NULL = 0x00,

        TYPE_BOOL = 0x11,

        TYPE_CHAR = 0x22,
        TYPE_SIGNED_CHAR = 0x23,
        TYPE_SIGNED_SHORT = 0x24,
        TYPE_SIGNED_INT = 0x25,
        TYPE_SIGNED_LONG = 0x26,
        TYPE_SIGNED_LONG_LONG = 0x27,
        TYPE_UNSIGNED_CHAR = 0x28,
        TYPE_UNSIGNED_SHORT = 0x29,
        TYPE_UNSIGNED_INT = 0x2a,
        TYPE_UNSIGNED_LONG = 0x2b,
        TYPE_UNSIGNED_LONG_LONG = 0x2c,

        TYPE_FLOAT = 0x3d,
        TYPE_DOUBLE = 0x3e,
        TYPE_LONG_DOUBLE = 0x3f,

        TYPE_JSON_STRING = 0x40,

        TYPE_JSON_ARRAY = 0x50,

        TYPE_JSON_OBJECT = 0x60,

        NUM_TYPES = 19,
        NUM_CLASS = 7,
        NUM_NUMERICAL = 14,
        NUM_PRIMITIVE = 16
    };

    template<json_type type>
    struct type_of;

    template<typename type>
    struct type_info;

    TYPE_DECL(TYPE_NULL, nullptr_t)
    TYPE_DECL(TYPE_BOOL, bool)
    TYPE_DECL(TYPE_CHAR, char)
    TYPE_DECL(TYPE_SIGNED_CHAR, signed char)
    TYPE_DECL(TYPE_SIGNED_SHORT, signed short)
    TYPE_DECL(TYPE_SIGNED_INT, signed int)
    TYPE_DECL(TYPE_SIGNED_LONG, signed long)
    TYPE_DECL(TYPE_SIGNED_LONG_LONG, signed long long)
    TYPE_DECL(TYPE_UNSIGNED_CHAR, unsigned char)
    TYPE_DECL(TYPE_UNSIGNED_SHORT, unsigned short)
    TYPE_DECL(TYPE_UNSIGNED_INT, unsigned int)
    TYPE_DECL(TYPE_UNSIGNED_LONG, unsigned long)
    TYPE_DECL(TYPE_UNSIGNED_LONG_LONG, unsigned long long)
    TYPE_DECL(TYPE_FLOAT, float)
    TYPE_DECL(TYPE_DOUBLE, double)
    TYPE_DECL(TYPE_LONG_DOUBLE, long double)
    TYPE_DECL(TYPE_JSON_STRING, const char *)
    TYPE_DECL(TYPE_JSON_ARRAY, short *)
    TYPE_DECL(TYPE_JSON_OBJECT, int *)

    TYPE_INFO(TYPE_JSON_STRING, char *)
    TYPE_INFO(TYPE_JSON_STRING, dynamic_string)

    template<bool cond, typename target_t = void>
    struct enable_type_if {};
    template<typename target_t>
    struct enable_type_if<true, target_t> {
        typedef target_t type;
    };

    template<typename target_t>
    constexpr bool is_string_type() {
        return
            is_same<char *, target_t>::value ||
            is_same<const char *, target_t>::value ||
            is_same<dynamic_string, target_t>::value;
    }

}

#endif

