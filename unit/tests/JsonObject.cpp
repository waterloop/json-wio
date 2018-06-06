#include <gtest/gtest.h>

#include <wlib/Json/JsonObject.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

TEST(json_object, null_keys) {
    json_object obj;
    
    json_element null_a;
    json_element null_b;

    json_element val1(10);
    json_element val2(20);

    obj.insert(null_a, val1);

    ASSERT_EQ(1, obj.size());
    ASSERT_TRUE(obj.contains(null_a));
    ASSERT_TRUE(obj.contains(null_b));

    auto ret = obj.insert(null_b, val2);

    ASSERT_FALSE(ret.second());
}
