#include <stdlib.h>
#include <wlib/hash>

#include <wlib/wjson/JsonDefine.h>
#include <wlib/wjson/JsonElement.h>

using namespace wlp;

bool wlp::string_is_int(const char *str) {
    char *end = nullptr;
    (void)(WLIB_JSON_STRTOINT(str, &end));
    return *end == '\0';
}

bool wlp::string_is_float(const char *str) {
    char *end = nullptr;
    (void)(WLIB_JSON_STRTOFLOAT(str, &end));
    return *end == '\0';
}

// functors
namespace element_hash {
    typedef size_t (*hash_function)(const json_element &);
    static size_t of_null(const json_element &) { return 0; }
    static size_t of_int(const json_element &je) {
        return static_cast<size_t>(je.integer());
    }
    static size_t of_float(const json_element &je) {
        json_float floating = je.floating();
        return *reinterpret_cast<size_t *>(&floating);
    }
    static size_t of_str(const json_element &je) {
        return hash_string<size_t>(je.string().c_str());
    }
    static size_t of_arr(const json_element &) {
        return 0;
    }
    static size_t of_obj(const json_element &) {
        return 0;
    }
    static hash_function hashers[json_type::NUM_CLASS] = {
        of_null, of_int, of_int,
        of_float, of_str,
        of_arr, of_obj
    };
}

size_t json_element_hash::operator()(const json_element &je) const {
    return element_hash::hashers[je.type() >> 4](je);
}

bool json_element_equals::operator()(
    const json_element &je1,
    const json_element &je2) const {
    return je1 == je2;
}

