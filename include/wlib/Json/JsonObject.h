#ifndef __WLIB_JSON_JSONOBJECT_H__
#define __WLIB_JSON_JSONOBJECT_H__

#include <wlib/hash_map>

namespace wlp {

    class json_element;
    struct json_element_hash;
    struct json_element_equals;

    typedef hash_map<
        json_element,
        json_element,
        json_element_hash,
        json_element_equals
    > json_object;

}

#endif
