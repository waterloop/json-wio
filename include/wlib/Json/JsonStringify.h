#ifndef __WLIB_JSON_JSONSTRINGIFY_H__
#define __WLIB_JSON_JSONSTRINGIFY_H__

#include <wlib/string>

namespace wlp {

    class json_element;

    namespace json {
        extern int buff_size(const json_element &je);
        extern int stringify(char *buf, const json_element &je);
    }

}

#endif
