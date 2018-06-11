#include <wlib/Json/JsonObject.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

json_object::json_object(
    json_object::size_type n,
    json_object::percent_type load) :
    parent_t(n, load) {}

json_object::json_object(json_object &&obj) :
    parent_t(move(obj)) {}

json_object &json_object::operator=(json_object &&obj) {
    parent_t::operator=(move(obj));
    return *this;
}

