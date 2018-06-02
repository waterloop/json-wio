#ifndef __WLIB_JSON_JSONSTRING_H__
#define __WLIB_JSON_JSONSTRING_H__

#include <wlib/Json/JsonElement.h>

namespace wlp {
    
    class json_string : public json_element {
    public:
        bool contents_are_null();
        bool contents_are_bool();
        bool contents_are_int();
        bool contents_are_float();
        bool contents_are_array();
        bool contents_are_object();
    };

}

#endif
