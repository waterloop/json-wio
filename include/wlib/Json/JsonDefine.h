#ifndef __WLIB_JSON_JSONDEFINE_H__
#define __WLIB_JSON_JSONDEFINE_H__

#define UNUSED(x) ((void) (x))

#if defined(WLIB_JSON_LONG_LONG)
    #define WLIB_JSON_INTFMT "%lli"
    #define WLIB_JSON_STRTOINT(str, end) static_cast<json_int>(strtoll((str), (end), 10))
#elif defined(WLIB_JSON_LONG)
    #define WLIB_JSON_INTFMT "%li"
    #define WLIB_JSON_STRTOINT(str, end) static_cast<json_int>(strtol ((str), (end), 10))
#else
    #define WLIB_JSON_INTFMT "%i"
    #if defined(WLIB_JSON_STOI)
        #define WLIB_JSON_STRTOINT(str, end) static_cast<json_int>(stoi  ((str), (end), 10))
    #else
        #define WLIB_JSON_STRTOINT(str, end) static_cast<json_int>(strtol((str), (end), 10))
    #endif
#endif

#if defined(WLIB_JSON_LONG_DOUBLE)
    #define WLIB_JSON_FLOATFMT "%.16Lf"
    #define WLIB_JSON_STRTOFLOAT(str, end) static_cast<json_float>(strtold((str), (end)))
#elif defined(WLIB_JSON_DOUBLE)
    #define WLIB_JSON_FLOATFMT "%.12f"
    #define WLIB_JSON_STRTOFLOAT(str, end) static_cast<json_float>(strtod ((str), (end)))
#else
    #define WLIB_JSON_FLOATFMT "%.7f"
    #if defined(WLIB_JSON_STRTOF)
        #define WLIB_JSON_STRTOFLOAT(str, end) static_cast<json_float>(strtof((str), (end)))
    #else
        #define WLIB_JSON_STRTOFLOAT(str, end) static_cast<json_float>(strtod((str), (end)))
    #endif
#endif

namespace wlp {

    typedef
#if defined(WLIB_JSON_LONG_LONG)
    long long
#elif defined(WLIB_JSON_LONG)
    long
#else
    int
#endif
    json_int;

    typedef
#if defined(WLIB_JSON_LONG_DOUBLE)
    long double
#elif defined(WLIB_JSON_DOUBLE)
    double
#else
    float
#endif
    json_float;

}

#endif
