#ifndef __WLIB_JSON_JSONPARSE_H__
#define __WLIB_JSON_JSONPARSE_H__

#include <wlib/types>

namespace wlp {

    class json_element;

    namespace json {
        extern json_element parse(const char *str);
    }

}

#endif
