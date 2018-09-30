#include <wlib/wjson/JsonObject.h>
#include <wlib/wjson/JsonElement.h>

using namespace wlp;

json_object::json_object(
    size_t n,
    json_object::percent_type load) :
    parent_t(n, load) {}

json_object::json_object(json_object &&obj) :
    parent_t(move(obj)) {}

json_object &json_object::operator=(json_object &&obj) {
    parent_t::operator=(move(obj));
    return *this;
}

