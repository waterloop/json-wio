#include <wlib/Json/JsonCompare.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

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

bool wlp::operator==(const json_element &je1, const json_element &je2) {
    unsigned char cls = je1.type() << 4;
    if (cls != je2.type() << 4) { return false; }
    return element_equals::comparators[cls](je1, je2);
}

namespace element_less {
    typedef bool (*less_function)(const json_element &, const json_element &);
    static bool of_null(const json_element &, const json_element &) { return false; }
    static bool of_int(const json_element &je1, const json_element &je2) {
        return je1.integer() < je2.integer();
    }
    static bool of_float(const json_element &je1, const json_element &je2) {
        return je1.floating() < je2.floating();
    }
    static bool of_str(const json_element &je1, const json_element &je2) {
        return je1.str() < je2.str();
    }
    static bool of_arr(const json_element &je1, const json_element &je2) {
        return false;
    }
    static bool of_obj(const json_element &je1, const json_element &je2) {
        return false;
    }
    static less_function comparators[json_type::NUM_CLASS] = {
        of_null, of_int, of_int,
        of_float, of_str,
        of_arr, of_obj
    };
}

bool wlp::operator<(const json_element &je1, const json_element &je2) {
    unsigned char cls1 = je1.type() << 4;
    unsigned char cls2 = je2.type() << 4;
    if (cls1 != cls2) { return cls1 < cls2; }
    return element_less::comparators[cls1](je1, je2);
}

