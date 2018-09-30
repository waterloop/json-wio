#ifndef __JSON_NULL_H__
#define __JSON_NULL_H__

#include <wlib/wjson/JsonElement.h>

namespace wlp {
    namespace json {
        json_element *null();
        namespace array {
            json_array *null();
        }
        namespace object {
            json_object *null();
        }
    }
}

#endif
