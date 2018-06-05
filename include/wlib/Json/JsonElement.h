#ifndef __WLIB_JSON_JSONELEMENT_H__
#define __WLIB_JSON_JSONELEMENT_H__

#include <float.h>

#include <wlib/string>
#include <wlib/utility>

#include <wlib/Json/JsonUtil.h>
#include <wlib/Json/JsonType.h>

#define data_assign(type, dat) (*reinterpret_cast<type *>((dat)))

namespace wlp {

    class json_element {
    public:
        ~json_element();

        // null
        explicit json_element(nullptr_t);

        // number types and bool
        template<typename number_t, typename =
            typename enable_if<is_integral<number_t>::value ||
            is_floating_point<number_t>::value>::type>
        json_element(number_t i) :
            m_data(malloc<number_t>()),
            m_type(type_info<number_t>::value),
            m_size(sizeof(number_t)) {
            data_assign(number_t, m_data) = i; }

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
        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            bool>::type convertible_to() {
            return is_null() || (is_string() &&
            is_string_null()); }
        template<typename target_t>
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type convertible_to() {
            return is_null() || is_bool() ||
                is_number() || (is_string() &&
                (!m_size || is_string_bool())); }
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            bool>::type convertible_to() {
            return is_null() || is_bool() ||
                is_int() || (is_string() &&
                is_string_int()); }
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            bool>::type convertible_to() {
            return is_null() || is_bool() ||
                is_number() || (is_string() && (
                is_string_int() ||
                is_string_float())); }
        template<typename target_t>
        typename enable_type_if<
            is_string_type<target_t>(),
            bool>::type convertible_to() {
            return true; }

        template<typename target_t>
        typename enable_type_if<
            is_same<nullptr_t, target_t>::value,
            nullptr_t>::type as() {
            return convert_to_null(); }
        template<typename target_t>
        typename enable_type_if<
            is_same<bool, target_t>::value,
            bool>::type as() {
            return convert_to_bool(); }
        template<typename target_t>
        typename enable_type_if<
            is_integral<target_t>::value &&
            !is_same<bool, target_t>::value,
            target_t>::type as() {
            return convert_to_number<target_t>(); }
        template<typename target_t>
        typename enable_type_if<
            is_floating_point<target_t>::value,
            target_t>::type as() {
            return convert_to_number<target_t>(); }
        template<typename target_t>
        typename enable_type_if<
            is_same<const char *, target_t>::value ||
            is_same<char *, target_t>::value,
            target_t>::type as() {
                return convert_to_c_str<target_t>(); }
        template<typename target_t>
        typename enable_type_if<
            is_same<dynamic_string, target_t>::value,
            dynamic_string>::type as() {
            return convert_to_dynamic_string(); }


        nullptr_t convert_to_null();
        bool convert_to_bool();
        template<typename target_t>
        typename enable_type_if<
            (is_integral<target_t>::value &&
            !is_same<bool, target_t>::value) ||
            is_floating_point<target_t>::value,
            target_t>::type convert_to_number();
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

        void *m_data;
        dynamic_string m_str;
        json_type m_type;
        int m_size;
    };

    template<typename c_str_t>
    c_str_t json_element::convert_to_c_str() {
        if (is_null()) {
            m_str = dynamic_string(STR_NULL, STR_SIZE_NULL);
            return const_cast<c_str_t>(m_str.c_str());
        }
        else if (is_bool()) {
            bool b = data_assign(bool, m_data);
            m_str = dynamic_string(
                b ? STR_TRUE : STR_FALSE,
                b ? STR_SIZE_TRUE : STR_SIZE_FALSE);
            return const_cast<c_str_t>(m_str.c_str());
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
            m_str = dynamic_string(strbuf, len);
            return const_cast<c_str_t>(m_str.c_str());
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
            m_str = dynamic_string(strbuf, len);
            return const_cast<c_str_t>(m_str.c_str());
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
            m_str = dynamic_string(strbuf, len);
            return const_cast<c_str_t>(m_str.c_str());
        } else if (is_string()) {
            return reinterpret_cast<c_str_t>(m_data);
        } else {
            return nullptr;
        }
    }

    template<typename target_t>
    typename enable_type_if<
        (is_integral<target_t>::value &&
        !is_same<bool, target_t>::value) ||
        is_floating_point<target_t>::value,
        target_t>::type json_element::convert_to_number() {
        if (is_bool()) {
            return static_cast<target_t>(
                data_assign(bool, m_data));
        } else if (is_int()) {
            switch (m_size) {
            case sizeof(char):
                return static_cast<target_t>(
                    data_assign(char, m_data));
            case sizeof(short):
                return static_cast<target_t>(
                    data_assign(short, m_data));
            case sizeof(int):
                return static_cast<target_t>(
                    data_assign(int, m_data));
            case sizeof(long):
                return static_cast<target_t>(
                    data_assign(long, m_data));
#if __WLIB_LONG_LONG__
            case sizeof(long long):
                return static_cast<target_t>(
                    data_assign(long long, m_data));
#endif
            default:
                return static_cast<target_t>(0);
            }
        } else if (is_float()) {
            switch (m_size) {
            case sizeof(float):
                return static_cast<target_t>(
                    data_assign(float, m_data));
            case sizeof(double):
                return static_cast<target_t>(
                    data_assign(double, m_data));
#if __WLIB_LONG_DOUBLE__
            case sizeof(long double):
                return static_cast<target_t>(
                    data_assign(long double, m_data));
#endif
            default:
                return static_cast<target_t>(0);
            }
        } else if (is_string()) {
            return parser<target_t>()(
                static_cast<const char *>(m_data));
        } else {
            return static_cast<target_t>(0);
        }
    }

}

#endif
