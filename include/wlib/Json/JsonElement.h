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
        bool is_null() const;
        bool is_bool() const;
        bool is_int() const;
        bool is_float() const;
        bool is_number() const;
        bool is_string() const;
        bool is_array() const;
        bool is_object() const;

        // string content checks
        bool is_string_null() const ;
        bool is_string_true() const ;
        bool is_string_false() const ;
        bool is_string_bool() const ;
        bool is_string_int() const ;
        bool is_string_float() const ;
        bool is_string_array() const ;
        bool is_string_object() const ;

        // type conversion
        bool convertible_to_null() const ;
        bool convertible_to_bool() const ;
        bool convertible_to_int() const ;
        bool convertible_to_float() const ;
        bool convertible_to_string() const ;

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
        template<typename target_t>
        // convertible to string
        typename enable_type_if<
            is_string_type<target_t>(),
            bool>::type convertible_to() const
        { return convertible_to_string(); }

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

    private:
        dynamic_string convert_to_dynamic_string() const;

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
