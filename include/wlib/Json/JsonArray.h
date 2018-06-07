#ifndef __WLIB_JSON_JSONARRAY_H__
#define __WLIB_JSON_JSONARRAY_H__

#include <wlib/array_list>

namespace wlp {

    class json_element;

    class json_array :
        public array_list<json_element> {

        enum {
            JSON_ARRAY_DEFAULT_SIZE = 15
        };

    public:
        explicit json_array(
            size_type n = JSON_ARRAY_DEFAULT_SIZE);

        json_array(json_array &&arr);
        json_array &operator=(json_array &&arr);

        json_array(const json_array &) = delete;
        json_array &operator=(const json_array &) = delete;
    };

}

#endif
