#ifndef __WLIB_JSON_JSONOBJECT_H__
#define __WLIB_JSON_JSONOBJECT_H__

#include <wlib/hash_map>

#include <wlib/Json/JsonUtil.h>

namespace wlp {

    class json_element;

    class json_object :
        public hash_map<
            json_element,
            json_element,
            json_element_hash,
            json_element_equals> {

        enum {
            JSON_OBJECT_DEFAULT_SIZE = 12,
            JSON_OBJECT_DEFAULT_LOAD = 75
        };

    public:
        explicit json_object(
            size_type n = JSON_OBJECT_DEFAULT_SIZE,
            percent_type load = JSON_OBJECT_DEFAULT_LOAD
        );

        json_object(json_object &&obj);
        json_object(const json_object &) = delete;

        json_object &operator=(json_object &&obj);
        json_object &operator=(const json_object &) = delete;
    };

}

#endif
