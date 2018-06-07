#ifndef __WLIB_JSON_JSONUTIL_H__
#define __WLIB_JSON_JSONUTIL_H__

#include <wlib/types>

#define STR_NULL    "null"
#define STR_TRUE    "true"
#define STR_FALSE   "false"

#define STR_SIZE_NULL   4
#define STR_SIZE_TRUE   4
#define STR_SIZE_FALSE  5

namespace wlp {

    extern bool string_is_int(const char *str);
    extern bool string_is_float(const char *str);

    class json_element;

    struct json_element_hash {
        size_type operator()(const json_element &je) const;
    };

    struct json_element_equals {
        bool operator()(const json_element &je1, const json_element &je2) const;
    };

}

#endif
