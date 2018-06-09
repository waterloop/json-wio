#ifndef __WLIB_JSON_JSONNULL_H__
#define __WLIB_JSON_JSONNULL_H__

#include <wlib/Json/JsonElement.h>

namespace wlp {
    namespace json {
        extern const json_element null;
        namespace array {
            extern const json_array null;
        }
        namespace object {
            extern const json_object null;
        }
    }
}

#endif

