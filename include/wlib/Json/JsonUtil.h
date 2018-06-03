#ifndef __WLIB_JSON_JSONUTIL_H__
#define __WLIB_JSON_JSONUTIL_H__

#define STR_NULL    "null"
#define STR_TRUE    "true"
#define STR_FALSE   "false"

#define STR_SIZE_NULL   4
#define STR_SIZE_TRUE   4
#define STR_SIZE_FALSE  5

namespace wlp {
    bool string_is_int(const char *str);
    bool string_is_float(const char *str);
}

#endif
