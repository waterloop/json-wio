#include <stdio.h>
#include <stdarg.h>

#include <wlib/Json/JsonType.h>

#define DEFN_PRINTER(name, format)          \
static int name(char *buf, ...) {           \
    va_list args;                           \
    va_start(args, buf);                    \
    int ret = vsprintf(buf, format, args);  \
    va_end(args);                           \
    return ret; }

static int null_printer(char *buf, ...) {
    return sprintf(buf, "null");
}

static int bool_printer(char *buf, ...) {
    va_list args;
    va_start(args, buf);

    bool val = va_arg(args, int) > 0;
    int ret = sprintf(buf, val ? "true" : "false");

    va_end(args);
    return ret;
}

DEFN_PRINTER(char_printer, "%hhi")
DEFN_PRINTER(signed_char_printer, "%hhi")
DEFN_PRINTER(signed_short_printer, "%hi")
DEFN_PRINTER(signed_int_printer, "%i")
DEFN_PRINTER(signed_long_printer, "%li")
DEFN_PRINTER(signed_long_long_printer, "%lli")
DEFN_PRINTER(unsigned_char_printer, "%hhu")
DEFN_PRINTER(unsigned_short_printer, "%hu")
DEFN_PRINTER(unsigned_int_printer, "%u")
DEFN_PRINTER(unsigned_long_printer, "%lu")
DEFN_PRINTER(unsigned_long_long_printer, "%llu")
DEFN_PRINTER(float_printer, "%f")
DEFN_PRINTER(double_printer, "%f")
DEFN_PRINTER(long_double_printer, "%Lf")
DEFN_PRINTER(string_printer, "%s")

wlp::fprintf_t wlp::s_type_printer[wlp::json_type::NUM_TYPES] = {
    null_printer,
    bool_printer,
    char_printer,
    signed_char_printer,
    signed_short_printer,
    signed_int_printer,
    signed_long_printer,
    signed_long_long_printer,
    unsigned_char_printer,
    unsigned_short_printer,
    unsigned_int_printer,
    unsigned_long_printer,
    unsigned_long_long_printer,
    float_printer,
    double_printer,
    long_double_printer,
    string_printer,
    nullptr,
    nullptr
};
