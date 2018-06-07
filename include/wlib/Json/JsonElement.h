#ifndef __WLIB_JSON_JSONELEMENT_H__
#define __WLIB_JSON_JSONELEMENT_H__

#include <wlib/string>
#include <wlib/utility>

#include <wlib/Json/JsonDefine.h>
#include <wlib/Json/JsonType.h>
#include <wlib/Json/JsonObject.h>
#include <wlib/Json/JsonArray.h>

namespace wlp {

    class json_element {
    public:
        // destructor
        ~json_element();

        // null constructors
        json_element();
        json_element(nullptr_t);

        // bool and integer type constructors
        template<typename number_t, typename =
        typename enable_if<is_integral<number_t>::value
        >::type>
        json_element(number_t integer, bool = 0) :
            m_integer(static_cast<json_int>(integer)),
            m_type(type_info<number_t>::value) {}
        // floating point type constructors
        template<typename number_t, typename =
        typename enable_if<is_floating_point<number_t>::value
        >::type>
        json_element(number_t floating, char = 0) :
            m_floating(static_cast<json_float>(floating)),
            m_type(type_info<number_t>::value) {}

        // string type constructors
        json_element(char *str);
        json_element(char *str, size_type size);
        json_element(const char *str);
        json_element(const char *str, size_type size);
        json_element(const dynamic_string &str);
        json_element(dynamic_string &&str);

        // static string
        template<size_type string_size>
        json_element(const static_string<string_size> &str) :
            json_element(static_cast<const char*>(str.c_str()), str.length()) {}

        // copy constructor and operator
        json_element(const json_element &je);
        json_element &operator=(const json_element &je);

        // move constructor and operator
        json_element(json_element &&je) noexcept;
        json_element &operator=(json_element &&je) noexcept;

        // null assignment
        json_element &operator=(nullptr_t);

        // bool and integer type assignment
        template<typename number_t>
        typename enable_type_if<
            is_integral<number_t>::value &&
            !is_same<json_int, number_t>::value,
            json_element &>::type
        operator=(number_t i)
        { return assign(static_cast<json_int>(i), type_info<number_t>::value); }
        // floating point types
        template<typename number_t>
        typename enable_type_if<
            is_floating_point<number_t>::value &&
            !is_same<json_float, number_t>::value,
            json_element &>::type
        operator=(number_t f)
        { return assign(static_cast<json_float>(f), type_info<number_t>::value); }

    private:
        json_element &assign(json_int i, json_type type);
        json_element &assign(json_float f, json_type type);
        json_element &assign(const char *str, size_type len);

    public:
        // string types
        json_element &operator=(char *str);
        json_element &operator=(const char *str);
        json_element &operator=(const dynamic_string &str);
        json_element &operator=(dynamic_string &&str);

        template<size_type string_size>
        json_element &operator=(const static_string<string_size> &str)
        { return assign(str.c_str(), str.length()); }

        // type checks
        bool is_null();
        bool is_bool();
        bool is_int();
        bool is_float();
        bool is_number();
        bool is_string();
        bool is_array();
        bool is_object();

        // string content checks
        bool is_string_null();
        bool is_string_true();
        bool is_string_false();
        bool is_string_bool();
        bool is_string_int();
        bool is_string_float();
        bool is_string_array();
        bool is_string_object();

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
            bool>::type convertible_to()
        { return convertible_to_null(); }
        // convertible to bool
        template<typename target_t>
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type convertible_to()
        { return convertible_to_bool(); }
        // convertible to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            bool>::type convertible_to()
        { return convertible_to_int(); }
        // convertible to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            bool>::type convertible_to()
        { return convertible_to_float(); }
        template<typename target_t>
        // convertible to string
        typename enable_type_if<
            is_string_type<target_t>(),
            bool>::type convertible_to()
        { return convertible_to_string(); }

    public:
        // convert to null
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            nullptr_t>::type as()
        { return convert_to_null(); }
        template<typename target_t>
        // convert to bool
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type as()
        { return convert_to_bool(); }
        // convert to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            target_t>::type as()
        { return static_cast<target_t>(convert_to_int()); }
        // convert to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            target_t>::type as()
        { return static_cast<target_t>(convert_to_float()); }
        // convert to string
        template<typename target_t>
        typename enable_type_if<
            is_same<const char *, target_t>::value ||
            is_same<char *, target_t>::value,
            target_t>::type as()
        { return const_cast<target_t>(convert_to_string()); }

    public:
        // convert to dynamic string
        template<typename target_t>
        typename enable_type_if<
            is_same<dynamic_string, target_t>::value,
            dynamic_string>::type as()
        { return convert_to_dynamic_string(); }

    private:
        nullptr_t convert_to_null();
        bool convert_to_bool();
        json_int convert_to_int();
        json_float convert_to_float();
        const char *convert_to_string();

    private:
        dynamic_string convert_to_dynamic_string();

    private:
        void assign_null();
        void assign_bool(bool b);
        void assign_int(json_int i);
        void assign_float(json_float f);
        void assign_str(const char *str, size_type len);
        void assign_array(json_array &&arr);
        void assign_object(json_array &&obj);

    public:
        const json_int &integer() const;
        const json_float &floating() const;
        const dynamic_string &string() const;
        const json_array &array() const;
        const json_object &object() const;

    public:
        json_int &integer();
        json_float &floating();
        dynamic_string &string();
        json_array &array();
        json_object &object();

    public:
        json_type type() const;

    public:
        bool operator==(const json_element &o) const;
        bool operator!=(const json_element &o) const;
        bool operator< (const json_element &o) const;
        bool operator> (const json_element &o) const;
        bool operator<=(const json_element &o) const;
        bool operator>=(const json_element &o) const;

    private:
        union {
            json_int m_integer;
            json_float m_floating;
            dynamic_string m_string;
            json_array m_array;
            json_object m_object;
        };
        json_type m_type;
    };

}

#endif
