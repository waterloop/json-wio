#ifndef __WLIB_JSON_JSONELEMENT_H__
#define __WLIB_JSON_JSONELEMENT_H__

#include <wlib/string>
#include <wlib/utility>

#include <wlib/wjson/JsonDefine.h>
#include <wlib/wjson/JsonType.h>
#include <wlib/wjson/JsonObject.h>
#include <wlib/wjson/JsonArray.h>

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
        explicit json_element(number_t integer, bool = 0) :
            m_integer(static_cast<json_int>(integer)),
            m_type(type_info<number_t>::value) {}
        // floating point type constructors
        template<typename number_t, typename =
        typename enable_if<is_floating_point<number_t>::value
        >::type>
        explicit json_element(number_t floating, char = 0) :
            m_floating(static_cast<json_float>(floating)),
            m_type(type_info<number_t>::value) {}

        // string type constructors
        json_element(char *str);
        json_element(char *str, size_t size);
        json_element(const char *str);
        json_element(const char *str, size_t size);
        json_element(const dynamic_string &str);
        json_element(dynamic_string &&str);

        // static string
        template<size_t string_size>
        json_element(const static_string<string_size> &str) :
            json_element(static_cast<const char*>(str.c_str()), str.length()) {}

        // json array and json object
        json_element(json_array &&arr);
        json_element(json_object &&arr);

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
            is_integral<number_t>::value,
            json_element &>::type
        operator=(number_t i)
        { return assign(static_cast<json_int>(i), type_info<number_t>::value); }
        // floating point types
        template<typename number_t>
        typename enable_type_if<
            is_floating_point<number_t>::value,
            json_element &>::type
        operator=(number_t f)
        { return assign(static_cast<json_float>(f), type_info<number_t>::value); }

    private:
        json_element &assign(json_int i, json_type type);
        json_element &assign(json_float f, json_type type);
        json_element &assign(const char *str, size_t len);

    public:
        // string types
        json_element &operator=(char *str);
        json_element &operator=(const char *str);
        json_element &operator=(const dynamic_string &str);
        json_element &operator=(dynamic_string &&str);

        template<size_t string_size>
        json_element &operator=(const static_string<string_size> &str)
        { return assign(str.c_str(), str.length()); }

        // json array and json object
        json_element &operator=(json_array &&arr);
        json_element &operator=(json_object &&arr);

        // type checks
        bool is_null() const;
        bool is_bool() const;
        bool is_int() const;
        bool is_float() const;
        bool is_number() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        // string content checks
        bool is_string_null() const;
        bool is_string_true() const;
        bool is_string_false() const;
        bool is_string_bool() const;
        bool is_string_int() const;
        bool is_string_float() const;
        bool is_string_array() const;
        bool is_string_object() const;

        // type conversion
        bool convertible_to_null() const;
        bool convertible_to_bool() const;
        bool convertible_to_int() const;
        bool convertible_to_float() const;
        bool convertible_to_string() const;

        // convertible to null
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            bool>::type convertible_to() const
        { return convertible_to_null(); }
        // convertible to bool
        template<typename target_t>
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type convertible_to() const
        { return convertible_to_bool(); }
        // convertible to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            bool>::type convertible_to() const
        { return convertible_to_int(); }
        // convertible to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            bool>::type convertible_to() const
        { return convertible_to_float(); }
        // convertible to string
        template<typename target_t>
        typename enable_type_if<
            is_string_type<target_t>(),
            bool>::type convertible_to() const
        { return convertible_to_string(); }
        // convertible to array or object
        template<typename target_t>
        typename enable_type_if<
            is_same<json_array, target_t>::value ||
            is_same<json_object, target_t>::value,
            bool>::type convertible_to() const
        { return false; }

    public:
        // convert to null
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            nullptr_t>::type as() const
        { return convert_to_null(); }
        template<typename target_t>
        // convert to bool
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type as() const
        { return convert_to_bool(); }
        // convert to int
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            target_t>::type as() const
        { return static_cast<target_t>(convert_to_int()); }
        // convert to float
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            target_t>::type as() const
        { return static_cast<target_t>(convert_to_float()); }
        // convert to string
        template<typename target_t>
        typename enable_type_if<
            is_same<const char *, target_t>::value ||
            is_same<char *, target_t>::value,
            target_t>::type as() const
        { return const_cast<target_t>(convert_to_string()); }
        // convert to json array
        template<typename target_t>
        typename enable_type_if<
            is_same<json_array, target_t>::value,
            const json_array &>::type as() const
        { return convert_to_array(); }
        // convert to json object
        template<typename target_t>
        typename enable_type_if<
            is_same<json_object, target_t>::value,
            const json_object &>::type as() const
        { return convert_to_object(); }

    public:
        // convert to dynamic string
        template<typename target_t>
        typename enable_type_if<
            is_same<dynamic_string, target_t>::value,
            dynamic_string>::type as()
        { return convert_to_dynamic_string(); }

    private:
        nullptr_t convert_to_null() const;
        bool convert_to_bool() const;
        json_int convert_to_int() const;
        json_float convert_to_float() const;
        const char *convert_to_string() const;
        const json_array &convert_to_array() const;
        const json_object &convert_to_object() const;

    private:
        dynamic_string convert_to_dynamic_string() const;

    public:
        // implicit conversions
        template<typename target_t>
        operator target_t() const
        { return as<target_t>(); }

    private:
        json_element &access(json_int i);
        const json_element &access(json_int i) const;
        json_element &access(json_float f);
        const json_element &access(json_float f) const;
        json_element &access(const char *str);
        const json_element &access(const char *str) const;

    public:
        // null access
        json_element &operator[](nullptr_t);
        const json_element &operator[](nullptr_t) const;

        // bool and integral access
        template<typename int_t>
        typename enable_type_if<
            is_integral<int_t>::value,
            json_element &>::type
        operator[](int_t i)
        { return access(static_cast<json_int>(i)); }
        template<typename int_t>
        typename enable_type_if<
            is_integral<int_t>::value,
            const json_element &>::type
        operator[](int_t i) const
        { return access(static_cast<json_int>(i)); }

        // float access
        template<typename float_t>
        typename enable_type_if<
            is_floating_point<float_t>::value,
            json_element &>::type
        operator[](float_t f)
        { return access(static_cast<json_float>(f)); }
        template<typename float_t>
        typename enable_type_if<
            is_floating_point<float_t>::value,
            const json_element &>::type
        operator[](float_t f) const
        { return access(static_cast<json_float>(f)); }

        // string access
        template<typename c_str_t>
        typename enable_type_if<
            is_same<char *, c_str_t>::value ||
            is_same<const char *, c_str_t>::value,
            json_element &>::type
        operator[](c_str_t *str)
        { return access(static_cast<const char *>(str)); }
        template<typename c_str_t>
        typename enable_type_if<
            is_same<char *, c_str_t>::value ||
            is_same<const char *, c_str_t>::value,
            const json_element &>::type
        operator[](c_str_t *str) const
        { return access(static_cast<const char *>(str)); }

        json_element &operator[](const char *str);
        json_element &operator[](const dynamic_string &str);
        const json_element &operator[](const dynamic_string &str) const;

        // regular element access
        json_element &operator[](const json_element &je);
        const json_element &operator[](const json_element &je) const;

        // size query for array and object types
        size_t size() const;
        size_t length() const;

    public:
        const json_int &integer() const;
        const json_float &floating() const;
        const dynamic_string &string() const;
        const json_array &array() const;
        const json_object &object() const;

        json_int &integer();
        json_float &floating();
        dynamic_string &string();
        json_array &array();
        json_object &object();

    public:
        template<typename access_t>
        typename enable_type_if<
            is_same<nullptr_t, access_t>::value,
            nullptr_t>::type ref_access() const
        { return nullptr; }
        template<typename access_t>
        typename enable_type_if<
            is_integral<access_t>::value,
            const json_int &>::type ref_access() const
        { return m_integer; }
        template<typename access_t>
        typename enable_type_if<
            is_floating_point<access_t>::value,
            const json_float &>::type ref_access() const
        { return m_floating; }
        template<typename access_t>
        typename enable_type_if<
            is_string_type<access_t>(),
            const dynamic_string &>::type ref_access() const
        { return m_string; }

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

    template<typename cmp_t,
        typename access_t = typename remove_cv<
        typename remove_reference<cmp_t>::type>::type,
        json_type type = type_info<cmp_t>::value>
    bool operator==(const json_element &je, const cmp_t &v)
    { return je.type() >> 4 == type >> 4 && v == je.ref_access<access_t>(); }

    template<typename cmp_t,
        typename access_t = typename remove_cv<
        typename remove_reference<cmp_t>::type>::type,
        json_type type = type_info<cmp_t>::value>
    bool operator< (const json_element &je, const cmp_t &v) {
        json_type_t cls1 = je.type() >> 4;
        json_type_t cls2 = type >> 4;
        return cls1 < cls2 || (cls1 == cls2 &&
            je.ref_access<access_t>() < v);
    }
    template<typename cmp_t,
        typename access_t = typename remove_cv<
        typename remove_reference<cmp_t>::type>::type,
        json_type type = type_info<cmp_t>::value>
    bool operator< (const cmp_t &v, const json_element &je) {
        json_type_t cls1 = je.type() >> 4;
        json_type_t cls2 = type >> 4;
        return cls2 < cls1 || (cls2 == cls1 &&
            v < je.ref_access<access_t>());
    }

    template<typename cmp_t>
    bool operator!=(const json_element &je, const cmp_t &v)
    { return !operator==<cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator> (const json_element &je, const cmp_t &v)
    { return operator< <cmp_t>(v, je); }
    template<typename cmp_t>
    bool operator<=(const json_element &je, const cmp_t &v)
    { return !operator< <cmp_t>(v, je); }
    template<typename cmp_t>
    bool operator>=(const json_element &je, const cmp_t &v)
    { return !operator< <cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator==(const cmp_t &v, const json_element &je)
    { return operator==<cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator!=(const cmp_t &v, const json_element &je)
    { return !operator==<cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator> (const cmp_t &v, const json_element &je)
    { return operator< <cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator<=(const cmp_t &v, const json_element &je)
    { return !operator< <cmp_t>(je, v); }
    template<typename cmp_t>
    bool operator>=(const cmp_t &v, const json_element &je)
    { return !operator< <cmp_t>(v, je); }

}

#endif
