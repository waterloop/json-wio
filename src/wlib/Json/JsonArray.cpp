#include <wlib/Json/JsonArray.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

json_array::json_array(size_type n) : array_list(n) {}
json_array::json_array(json_array &&arr) : array_list(move(arr)) {}

json_array &json_array::operator=(json_array &&arr) {
    array_list::operator=(move(arr));
    return *this;
}

