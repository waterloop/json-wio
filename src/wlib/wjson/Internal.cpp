#include "Internal.h"

#include <wlib/new>
#include <string.h>

json_union::json_union() {}
json_union::json_union(const json_union::obj_it &it) : obj(it) {}
json_union::json_union(const json_union::arr_it &it) : arr(it) {}
json_union::json_union(json_union::json_ref r) : ref(r) {}
json_union &json_union::operator=(json_union &&o) noexcept {
    memcpy(this, &o, sizeof(json_union));
    return *this;
}
