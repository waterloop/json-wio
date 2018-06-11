#include <wlib/wjson/JsonArray.h>
#include <wlib/wjson/JsonElement.h>

using namespace wlp;

json_array::json_array(size_type n) : parent_t(n) {}
json_array::json_array(json_array &&arr) : parent_t(move(arr)) {}

json_array::json_array(initializer_list<json_element> l) :
    parent_t(l.size()) {
    for (auto it = l.begin(); it != l.end(); ++it)
    { push_back(*it); }
}

json_array &json_array::operator=(json_array &&arr) {
    parent_t::operator=(move(arr));
    return *this;
}
