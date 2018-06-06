#include <stdlib.h>

#include <wlib/hash>

#include <wlib/Json/JsonElement.h>
#include <wlib/Json/JsonUtil.h>

using namespace wlp;

bool wlp::string_is_int(const char *str) {
    char *end = nullptr;
    //strtoll
    strtol(str, &end, 10);
    return *end == '\0';
}

bool wlp::string_is_float(const char *str) {
    char *end = nullptr;
    //strtold
    strtod(str, &end);
    return *end == '\0';
}

// functors
namespace element_hash {
    typedef size_type (*hash_function)(const json_element &);
    static size_type of_null(const json_element &) { return 0; }
    static size_type of_int(const json_element &je) {
        return static_cast<size_type>(je.integer());
    }
    static size_type of_float(const json_element &je) {
        static_assert(
            sizeof(long double) >= sizeof(size_type),
            "Expecting sizeof long double to be at least sizeof wlp::size_type");

        long double floating = je.floating();
        return *reinterpret_cast<size_type *>(&floating);
    }
    static size_type of_str(const json_element &je) {
        return hash_string<size_type>(je.str().c_str());
    }
    static size_type of_arr(const json_element &) {
        return 0;
    }
    static size_type of_obj(const json_element &) {
        return 0;
    }
    static hash_function hashers[json_type::NUM_CLASS] = {
        of_null, of_int, of_int, 
        of_float, of_str,
        of_arr, of_obj
    };
}

size_type json_element_hash::operator()(const json_element &je) const {
    return element_hash::hashers[je.type() >> 4](je);
}

namespace element_equals {
    typedef bool (*equals_function)(const json_element &, const json_element &);
    static bool of_null(const json_element &, const json_element &) { return true; }
    static bool of_int(const json_element &je1, const json_element &je2) {
        return je1.integer() == je2.integer();
    }
    static bool of_float(const json_element &je1, const json_element &je2) {
        return je1.floating() == je2.floating();
    }
    static bool of_str(const json_element &je1, const json_element &je2) {
        return je1.str() == je2.str();
    }
    static bool of_arr(const json_element &, const json_element &) {
        return false;
    }
    static bool of_obj(const json_element &, const json_element &) {
        return false;
    }
    static equals_function comparators[json_type::NUM_CLASS] = {
        of_null, of_int, of_int,
        of_float, of_str,
        of_arr, of_obj
    };
}

bool json_element_equals::operator()(const json_element &je1, const json_element &je2) const {
    unsigned char cls = je1.type() << 4;
    if (cls != je2.type() << 4) { return false; }
    return element_equals::comparators[cls](je1, je2);
}
