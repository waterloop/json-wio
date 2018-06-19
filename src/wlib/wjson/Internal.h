#ifndef __WLIB_JSON_INTERNAL_H__
#define __WLIB_JSON_INTERNAL_H__

#include <wlib/wjson/JsonObject.h>
#include <wlib/wjson/JsonArray.h>

struct json_union {
    typedef wlp::json_object::const_iterator obj_it;
    typedef wlp::json_array::const_iterator arr_it;
    typedef void *json_ref;
    union {
        obj_it obj;
        arr_it arr;
        json_ref ref;
    };
    json_union();
    explicit json_union(const obj_it &it);
    explicit json_union(const arr_it &it);
    explicit json_union(json_ref r);
    json_union &operator=(json_union &&o) noexcept;
};

#endif
