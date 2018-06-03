#include <wlib/Json/JsonUtil.h>
#include <wlib/Json/JsonString.h>

using namespace wlp;

bool json_string::contents_are_null() {
    const char *str = static_cast<const char *>(m_data);
    return 
        m_size == STR_SIZE_NULL &&
        strcmp(str, STR_NULL) == 0;
}

bool json_string::contents_are_bool() {
    const char *str = static_cast<const char *>(m_data);
    return (
        m_size == STR_SIZE_TRUE &&
        strcmp(str, STR_TRUE) == 0
    ) || (
        m_size == STR_SIZE_FALSE &&
        strcmp(str, STR_FALSE) == 0
    );
}

bool json_string::contents_are_int() {
    return string_is_int(static_cast<const char *>(m_data));
}

bool json_string::contents_are_float() {
    return string_is_float(static_cast<const char *>(m_data));
}

bool json_string::contents_are_true() {
    const char *str = static_cast<const char *>(m_data);
    return 
        m_size == STR_SIZE_TRUE &&
        strcmp(str, STR_TRUE) == 0;
}

bool json_string::contents_are_false() {
    const char *str = static_cast<const char *>(m_data);
    return
        m_size == STR_SIZE_FALSE &&
        strcmp(str, STR_FALSE) == 0;
}

bool json_string::contents_are_array() {
    // TODO check if it's a valid JSON array
    return false;
}

bool json_string::contents_are_object() {
    // TODO check if it's a valid JSON object
    return false;
}
