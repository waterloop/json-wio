#include "Placement.h"

#include <wlib/memory>
#include <wlib/wjson/JsonNull.h>

namespace wlp {
    namespace json {
        static json_element *pNull = nullptr;
        namespace array {
            static json_array *pNull = nullptr;
        }
        namespace object {
            static json_object *pNull = nullptr;
        }
    }
}

using namespace wlp;

json_element *json::null() {
    if (nullptr == json::pNull) {
        pNull = create<json_element>();
    }
    return pNull;
}

json_array *json::array::null() {
    if (nullptr == json::array::pNull) {
        pNull = create<json_array>();
    }
    return pNull;
}

json_object *json::object::null() {
    if (nullptr == json::object::pNull) {
        pNull = create<json_object>();
    }
    return pNull;
}
