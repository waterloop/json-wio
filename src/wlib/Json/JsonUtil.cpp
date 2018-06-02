#include <stdlib.h>

#include <wlib/Json/JsonUtil.h>

bool wlp::string_is_int(const char *str) {
    char *end = '\0';
    //strtoll
    strtol(str, &end, 10);
    return *end == '\0';
}

bool wlp::string_is_float(const char *str) {
    char *end = '\0';
    //strtold
    strtod(str, &end);
    return *end == '\0';
}
