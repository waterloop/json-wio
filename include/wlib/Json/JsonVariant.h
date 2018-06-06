#ifndef __WLIB_JSON_JSONVARIANT_H__
#define __WLIB_JSON_JSONVARIANT_H__

#include <float.h>

#include <wlib/string>
#include <wlib/utility>

#include <wlib/Json/JsonUtil.h>
#include <wlib/Json/JsonType.h>

namespace wlp {

    class json_variant {
    public:
        // null
        json_variant();
        explicit json_variant(nullptr_t);

        // bool and integer types
        template<typename number_t, typename =
        typename enable_if<is_integral<number_t>::value
        >::type>
        explicit json_variant(number_t integer, char = 0) :
            m_data(static_cast<long long>(integer)),
            m_type(type_info<number_t>::value) {}
        // floating point types
        template<typename number_t, typename =
        typename enable_if<is_floating_point<number_t>::value
        >::type>
        explicit json_variant(number_t floating, float = 0) :
            m_data(static_cast<long double>(floating)),
            m_type(type_info<number_t>::value) {}

        // string types
        explicit json_variant(char *str);
        explicit json_variant(char *str, size_type size);
        explicit json_variant(const char *str);
        explicit json_variant(const char *str, size_type size);
        explicit json_variant(const dynamic_string &str);
        template<size_type string_size>
        explicit json_variant(const static_string<string_size> &str) :
            json_variant(static_cast<const char *>(str.c_str()), str.length()) {}

        // copy and move constructors
        json_variant(const json_variant &je);
        json_variant(json_variant &&je);

        // null
        json_variant &operator=(nullptr_t);

        // bool and integer types
        template<typename number_t>
        typename enable_type_if<
            is_integral<number_t>::value,
            json_variant &>::type
        operator=(number_t integer) {
            m_data.integer = integer;
            m_type = type_info<number_t>::value;
            return *this;
        }
        // floating point types
        template<typename number_t>
        typename enable_type_if<
            is_floating_point<number_t>::value,
            json_variant &>::type
        operator=(number_t floating) {
            m_data.floating = floating;
            m_type = type_info<number_t>::value;
            return *this;
        }

        // string types
        json_variant &operator=(char *str);
        json_variant &operator=(const char *str);
        json_variant &operator=(const dynamic_string &str);
        template<size_type string_size>
        json_variant &operator=(const static_string<string_size> &str) { m_str = str; }

        // copy and move operators
        json_variant &operator=(const json_variant &je);
        json_variant &operator=(json_variant &&je);

        // type checks
        bool is_primitive();
        bool is_null();
        bool is_bool();
        bool is_int();
        bool is_signed_int();
        bool is_unsigned_int();
        bool is_float();
        bool is_number();
        bool is_string();
        bool is_array();
        bool is_object();

        // type conversion
        bool convertible_to_null();
        bool convertible_to_bool();
        bool convertible_to_int();
        bool convertible_to_float();
        bool convertible_to_string();

        // convertible to null
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            bool>::type convertible_to() { return convertible_to_null(); }
        // convertible to bool
        template<typename target_t>
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type convertible_to() { return convertible_to_bool(); }
        // convertible to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            bool>::type convertible_to() { return convertible_to_int(); }
        // convertible to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            bool>::type convertible_to() { return convertible_to_float(); }
        template<typename target_t>
        // convertible to string
        typename enable_type_if<
            is_string_type<target_t>(),
            bool>::type convertible_to() { return convertible_to_string(); }

        // convert to null
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            nullptr_t>::type as() {
            return convert_to_null();
        }
        template<typename target_t>
        // convert to bool
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type as() {
            return convert_to_bool();
        }
        // convert to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            target_t>::type as() {
            return static_cast<target_t>(convert_to_int());
        }
        // convert to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            target_t>::type as() {
            return static_cast<target_t>(convert_to_float());
        }
        // convert to C string
        template<typename target_t>
        typename enable_type_if<
            is_same<const char *, target_t>::value ||
            is_same<char *, target_t>::value,
            target_t>::type as() {
            return const_cast<target_t>(convert_to_string());
        }
        // convert to dynamic string
        template<typename target_t>
        typename enable_type_if<
            is_same<dynamic_string, target_t>::value,
            dynamic_string>::type as() {
            return convert_to_dynamic_string();
        }

        nullptr_t convert_to_null();
        bool convert_to_bool();
        long long convert_to_int();
        long double convert_to_float();
        const char *convert_to_string();
        dynamic_string convert_to_dynamic_string();

    public:
        bool is_string_null();
        bool is_string_true();
        bool is_string_false();
        bool is_string_bool();
        bool is_string_int();
        bool is_string_float();
        bool is_string_array();
        bool is_string_object();

    public:
        long long integer() const;
        long double floating() const;
        dynamic_string &str();
        const dynamic_string &str() const;
        json_type type() const;

    private:
        union data {
            data();
            explicit data(long long i);
            explicit data(long double f);
            long long integer;
            long double floating;
        } m_data;
        dynamic_string m_str;
        json_type m_type;
    };

}

#endif
