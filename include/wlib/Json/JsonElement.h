#ifndef __WLIB_JSON_JSONELEMENT_H__
#define __WLIB_JSON_JSONELEMENT_H__

#include <float.h>

#include <wlib/string>
#include <wlib/utility>

#include <wlib/Json/JsonUtil.h>
#include <wlib/Json/JsonType.h>

#define data_assign(type, dat) (*reinterpret_cast<type *>((dat)))

#define JSON_ELEMENT_CONVERTIBLE_TO_NULL()  \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<nullptr_t, target_t>::value,    \
    bool>::type convertible_to() {          \
    return is_null() || (is_string() &&     \
        is_string_null()); }           

#define JSON_ELEMENT_CONVERTIBLE_TO_BOOL()  \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<bool, target_t>::value,         \
    bool>::type convertible_to() {          \
    return is_null() || is_bool() ||        \
        is_number() || (is_string() &&      \
        (!m_size || is_string_bool())); }     

#define JSON_ELEMENT_CONVERTIBLE_TO_INT()   \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_integral<target_t>::value &&         \
    !is_same<bool, target_t>::value,        \
    bool>::type convertible_to() {          \
    return is_null() || is_bool() ||        \
        is_int() || (is_string() &&         \
        is_string_int()); }

#define JSON_ELEMENT_CONVERTIBLE_TO_FLOAT() \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_floating_point<target_t>::value,     \
    bool>::type convertible_to() {          \
    return is_null() || is_bool() ||        \
        is_number() || (is_string() && (    \
        is_string_int() ||                  \
        is_string_float())); }               

#define JSON_ELEMENT_CONVERTIBLE_TO_STRING()    \
template<typename target_t>                     \
typename enable_type_if<                        \
    is_string_type<target_t>(),                 \
    bool>::type convertible_to() {              \
    return true; }      

#define JSON_ELEMENT_AS_NULL()              \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<nullptr_t, target_t>::value,    \
    nullptr_t>::type as() {                 \
    return convert_to_null(); }               

#define JSON_ELEMENT_AS_BOOL()              \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<bool, target_t>::value,         \
    bool>::type as() {                      \
    return convert_to_bool(); }         

#define JSON_ELEMENT_AS_INT()               \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_integral<target_t>::value &&         \
    !is_same<bool, target_t>::value,        \
    target_t>::type as() {                  \
    return convert_to_int<target_t>(); }    

#define JSON_ELEMENT_AS_FLOAT()             \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_floating_point<target_t>::value,     \
    target_t>::type as() {                  \
    return convert_to_float<target_t>(); }

#define JSON_ELEMENT_AS_C_STR(c_str_t)      \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<c_str_t, target_t>::value,      \
    c_str_t>::type as() {                   \
    return convert_to_c_str<c_str_t>(); }   
    
#define JSON_ELEMENT_AS_DYNAMIC_STRING()        \
template<typename target_t>                     \
typename enable_type_if<                        \
    is_same<dynamic_string, target_t>::value,   \
    dynamic_string>::type as() {                \
    return convert_to_dynamic_string(); }

#define JSON_ELEMENT_CONVERT_TO_INT(int_t)  \
template<typename target_t>                 \
typename enable_type_if<                    \
    is_same<int_t, target_t>::value,        \
    int_t>::type convert_to_int() {         \
    return *reinterpret_cast<int_t *>(      \
        m_data); }

#define JSON_ELEMENT_CONVERT_TO_FLOAT(float_t)  \
template<typename target_t>                     \
typename enable_type_if<                        \
    is_same<float_t, target_t>::value,          \
    float_t>::type convert_to_float() {         \
    return *reinterpret_cast<float_t *>(        \
        m_data); }


namespace wlp {

    class json_element {
    public:
        ~json_element();

        // null
        explicit json_element(nullptr_t);
        
        // true, false
        explicit json_element(bool b);

        // number types
        explicit json_element(char i);

        // signed integer types
        explicit json_element(signed char i);
        explicit json_element(signed short i);
        explicit json_element(signed int i);
        explicit json_element(signed long i);
        explicit json_element(signed long long i);

        // unsigned integer types
        explicit json_element(unsigned char i);
        explicit json_element(unsigned short i);
        explicit json_element(unsigned int i);
        explicit json_element(unsigned long i);
        explicit json_element(unsigned long long i);

        // float types
        explicit json_element(float r);
        explicit json_element(double r);
        explicit json_element(long double r);

        // string types
        explicit json_element(char *str);
        explicit json_element(char *str, size_type size);
        explicit json_element(const char *str);
        explicit json_element(const char *str, size_type size);
        explicit json_element(const dynamic_string &str);
        template<size_type string_size>
        explicit json_element(const static_string<string_size> &str) :
            json_element(static_cast<const char *>(str.c_str()), str.length()) {}

        // type conversion
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

        bool convertible_to(json_type type);
        JSON_ELEMENT_CONVERTIBLE_TO_NULL()
        JSON_ELEMENT_CONVERTIBLE_TO_BOOL()
        JSON_ELEMENT_CONVERTIBLE_TO_INT()
        JSON_ELEMENT_CONVERTIBLE_TO_FLOAT()
        JSON_ELEMENT_CONVERTIBLE_TO_STRING()

        JSON_ELEMENT_AS_NULL()
        JSON_ELEMENT_AS_BOOL()
        JSON_ELEMENT_AS_INT()
        JSON_ELEMENT_AS_FLOAT()

        JSON_ELEMENT_AS_C_STR(char *)
        JSON_ELEMENT_AS_C_STR(const char *)
        JSON_ELEMENT_AS_DYNAMIC_STRING()

        JSON_ELEMENT_CONVERT_TO_INT(char)
        JSON_ELEMENT_CONVERT_TO_INT(signed char)
        JSON_ELEMENT_CONVERT_TO_INT(signed short)
        JSON_ELEMENT_CONVERT_TO_INT(signed int)
        JSON_ELEMENT_CONVERT_TO_INT(signed long)
        JSON_ELEMENT_CONVERT_TO_INT(signed long long)
        JSON_ELEMENT_CONVERT_TO_INT(unsigned char)
        JSON_ELEMENT_CONVERT_TO_INT(unsigned short)
        JSON_ELEMENT_CONVERT_TO_INT(unsigned int)
        JSON_ELEMENT_CONVERT_TO_INT(unsigned long)
        JSON_ELEMENT_CONVERT_TO_INT(unsigned long long)
        JSON_ELEMENT_CONVERT_TO_FLOAT(float)
        JSON_ELEMENT_CONVERT_TO_FLOAT(double)
        JSON_ELEMENT_CONVERT_TO_FLOAT(long double)

        nullptr_t convert_to_null();
        bool convert_to_bool();
        template<typename c_str_t>
        c_str_t convert_to_c_str();
        dynamic_string convert_to_dynamic_string();

        void *data();
        json_type type();
        int size();

    protected:
        bool is_string_null();
        bool is_string_bool();
        bool is_string_int();
        bool is_string_float();
        bool is_string_array();
        bool is_string_object();

        static char s_str_null[STR_SIZE_NULL + 1];
        static char s_str_true[STR_SIZE_TRUE + 1];
        static char s_str_false[STR_SIZE_FALSE + 1];

        void *m_data;
        dynamic_string m_str;
        json_type m_type;
        int m_size;
    };

    template<typename c_str_t>
    c_str_t json_element::convert_to_c_str() {       
        if (is_null()) { return s_str_null; } 
        else if (is_bool()) { 
            return static_cast<c_str_t>(
                data_assign(bool, m_data)
                ? STR_TRUE : STR_FALSE);
        } else if (is_signed_int()) {
            static char strbuf[(8 * sizeof(long long) / 3) + 3]; 
            int len = 0;
            fprintf_t printer = s_type_printer[m_type];
            switch (m_type) {
            case TYPE_CHAR: 
                len = printer(strbuf, data_assign(char, m_data));
                break;
            case TYPE_SIGNED_CHAR:
                len = printer(strbuf, data_assign(signed char, m_data));
                break;
            case TYPE_SIGNED_SHORT:
                len = printer(strbuf, data_assign(signed short, m_data));
                break;
            case TYPE_SIGNED_INT:
                len = printer(strbuf, data_assign(signed int, m_data));
                break;
            case TYPE_SIGNED_LONG:
                len = printer(strbuf, data_assign(signed long, m_data));
                break;
            case TYPE_SIGNED_LONG_LONG:
                len = printer(strbuf, data_assign(signed long long, m_data));
                break;
            default:
                break;
            }
            m_str.clear();
            m_str.append(strbuf, len);
            return static_cast<c_str_t>(m_str.c_str());
        } else if (is_unsigned_int()) {
            static char strbuf[(8 * sizeof(long long) / 3) + 3]; 
            int len = 0;
            fprintf_t printer = s_type_printer[m_type];
            switch (m_type) {
            case TYPE_UNSIGNED_CHAR:
                len = printer(strbuf, data_assign(unsigned char, m_data));
                break;
            case TYPE_UNSIGNED_SHORT:
                len = printer(strbuf, data_assign(unsigned short, m_data));
                break;
            case TYPE_UNSIGNED_INT:
                len = printer(strbuf, data_assign(unsigned int, m_data));
                break;
            case TYPE_UNSIGNED_LONG:
                len = printer(strbuf, data_assign(unsigned long, m_data));
                break;
            case TYPE_UNSIGNED_LONG_LONG:
                len = printer(strbuf, data_assign(unsigned long long, m_data));
                break;
            default:
                break;
            }
            m_str.clear();
            m_str.append(strbuf, len);
            return static_cast<c_str_t>(m_str.c_str());
        } else if (is_float()) {
            static char strbuf[10 + DBL_MAX_10_EXP];
            int len = 0;
            fprintf_t printer = s_type_printer[m_type];
            switch (m_type) {
            case TYPE_FLOAT:
                len = printer(strbuf, data_assign(float, m_data));
                break;
            case TYPE_DOUBLE:
                len = printer(strbuf, data_assign(double, m_data));
                break;
            case TYPE_LONG_DOUBLE:
                len = printer(strbuf, data_assign(long double, m_data));
                break;
            default:
                break;
            }
            m_str.clear();
            m_str.append(strbuf, len);
            return static_cast<c_str_t>(m_str.c_str());
        } else if (is_string()) {
            return reinterpret_cast<c_str_t>(m_data);
        } else {
            return nullptr;
        }
    }

}

#endif
