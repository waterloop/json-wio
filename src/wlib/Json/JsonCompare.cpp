#include <wlib/Json/JsonElement.h>

#include <stdio.h>
using namespace wlp;

namespace element_equals {
    typedef bool (*equals_function)(const json_element &, const json_element &);
    static bool of_null(const json_element &, const json_element &) 
    { return true; }
    static bool of_int(const json_element &je1, const json_element &je2) 
    { return je1.integer() == je2.integer(); }
    static bool of_float(const json_element &je1, const json_element &je2) 
    { return je1.floating() == je2.floating(); }
    static bool of_str(const json_element &je1, const json_element &je2) 
    { return je1.string() == je2.string(); }
    static bool of_arr(const json_element &, const json_element &) 
    { return false; }
    static bool of_obj(const json_element &, const json_element &) 
    { return false; }
    static equals_function comparators[json_type::NUM_CLASS] = {
        of_null, of_int, of_int,
        of_float, of_str,
        of_arr, of_obj
    };
    static bool compare(const json_element &je1, const json_element &je2) {
        json_type_t cls = je1.type() >> 4;
        return cls == je2.type() >> 4 &&
            element_equals::comparators[cls](je1, je2);
    }
}

bool json_element::operator==(const json_element &je) const 
{ return  element_equals::compare(*this, je); }
bool json_element::operator!=(const json_element &je) const 
{ return !element_equals::compare(*this, je); }

namespace element_less {
    typedef bool (*less_function)(const json_element &, const json_element &);
    static bool of_null(const json_element &, const json_element &)
    { return false; }
    static bool of_int(const json_element &je1, const json_element &je2) 
    { return je1.integer() < je2.integer(); }
    static bool of_float(const json_element &je1, const json_element &je2) 
    { return je1.floating() < je2.floating(); }
    static bool of_str(const json_element &je1, const json_element &je2) 
    { return je1.string() < je2.string(); }
    static bool of_arr(const json_element &, const json_element &) 
    { return false; }
    static bool of_obj(const json_element &, const json_element &) 
    { return false; }
    static less_function comparators[json_type::NUM_CLASS] = {
        of_null, of_int, of_int,
        of_float, of_str,
        of_arr, of_obj
    };
    static bool compare(const json_element &je1, const json_element &je2) {
        json_type_t cls1 = je1.type() >> 4;
        json_type_t cls2 = je2.type() >> 4;
        return cls1 < cls2 || (cls1 == cls2 &&
            element_less::comparators[cls1](je1, je2));
    }
}

bool json_element::operator< (const json_element &je) const 
{ return  element_less::compare(*this, je); }
bool json_element::operator> (const json_element &je) const 
{ return  element_less::compare(je, *this); }
bool json_element::operator<=(const json_element &je) const 
{ return !element_less::compare(je, *this); }
bool json_element::operator>=(const json_element &je) const 
{ return !element_less::compare(*this, je); }
