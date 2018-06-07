#include <wlib/Json/JsonObject.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

json_object::json_object(
    json_object::size_type n,
    json_object::percent_type load) :
    hash_map(n, load) {}

json_object::json_object(json_object &&obj) :
    hash_map(move(obj)) {}

json_object &json_object::operator=(json_object &&obj) {
    hash_map::operator=(move(obj));
    return *this;
}

