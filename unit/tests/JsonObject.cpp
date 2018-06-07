#include <gtest/gtest.h>

#include <wlib/Json/JsonObject.h>
#include <wlib/Json/JsonElement.h>
#include <wlib/Json/JsonNull.h>

using namespace wlp;

TEST(json_object, move_constructor) {
    json_object obj;

    json_element k1("key1");
    json_element k2("key2");
    json_element v1(24);
    json_element v2(75);

    obj.insert(k1, v1);
    obj.insert(k2, v2);

    ASSERT_EQ(2, obj.size());
    ASSERT_NE(obj.end(), obj.find(k1));
    ASSERT_NE(obj.end(), obj.find(k2));
    ASSERT_EQ(v1, *obj.find(k1));
    ASSERT_EQ(v2, *obj.find(k2));

    json_object moved(move(obj));

    ASSERT_EQ(0, obj.size());
    ASSERT_EQ(0, obj.capacity());
    ASSERT_EQ(obj.begin(), obj.end());

    ASSERT_EQ(2, moved.size());
    ASSERT_NE(moved.end(), moved.find(k1));
    ASSERT_NE(moved.end(), moved.find(k2));
    ASSERT_EQ(v1, *moved.find(k1));
    ASSERT_EQ(v2, *moved.find(k2));
}

TEST(json_object, move_operator) {
    json_element k1("key1");
    json_element k2("key2");
    json_element v1("val1");
    json_element v2("val2");

    json_object o1;
    json_object o2;

    o1.insert(k1, v1);
    o2.insert(k2, v2);

    ASSERT_EQ(1, o1.size());
    ASSERT_EQ(1, o2.size());

    ASSERT_NE(o1.end(), o1.find(k1));
    ASSERT_EQ(o1.end(), o1.find(k2));
    ASSERT_NE(o2.end(), o2.find(k2));
    ASSERT_EQ(o2.end(), o2.find(k1));

    o1 = move(o2);

    ASSERT_EQ(0, o2.size());
    ASSERT_EQ(0, o2.capacity());
    ASSERT_EQ(o2.begin(), o2.end());

    ASSERT_EQ(1, o1.size());
    ASSERT_EQ(o1.end(), o1.find(k1));
    ASSERT_NE(o1.end(), o1.find(k2));
}

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
    ASSERT_EQ(*ret.first(), val1);
}

TEST(json_object, integer_keys) {
    json_element null_e;
    json_element bool_e(true);
    json_element int_e(-4923);
    json_element float_e(-2324.323f);
    json_element str_e("hello");

    json_element k1(25);
    json_element k2(35);
    json_element k3(45);
    json_element k4(55);

    json_object obj;

    obj.insert(k1, null_e);
    obj.insert(k2, bool_e);
    obj.insert(k3, int_e);
    obj.insert(k4, float_e);

    ASSERT_EQ(*obj.find(k1), null_e);
    ASSERT_EQ(*obj.find(k2), bool_e);
    ASSERT_EQ(*obj.find(k3), int_e);
    ASSERT_EQ(*obj.find(k4), float_e);

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));

    auto ret = obj.insert(k3, str_e);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(*ret.first(), int_e);

    obj.insert(k1, str_e);
    obj.insert(k2, str_e);

    ASSERT_EQ(obj[k1], str_e);
    ASSERT_EQ(obj[k2], str_e);

    json_element k5(100);
    ASSERT_EQ(obj[k5], json::null);
}

TEST(json_object, float_keys) {
    json_element str1("Their metal birds rained messages from the sky");
    json_element str2("They read: CITIZENS OF MITTENHEID,");
    json_element str3("THE LOSTLANDS CONFEDERACY HAS OBTAINED POWER OF THE GODS");
    json_element str4("SURRENDER OR FACE PROMPT AND UTTER DESTRUCTION");

    json_element k1(12.3);
    json_element k2(-23.12f);
    json_element k3(55.56);
    json_element k4(55.56);
    json_element k5(-232.23);

    json_object obj;

    obj.insert(k1, str1);
    obj.insert(k2, str2);
    obj.insert(k3, str3);
    obj.insert(k5, str4);

    ASSERT_EQ(4, obj.size());

    ASSERT_EQ(str1, *obj.find(k1));
    ASSERT_EQ(str2, *obj.find(k2));
    ASSERT_EQ(str3, *obj.find(k3));
    ASSERT_EQ(str4, *obj.find(k5));

    auto ret = obj.insert(k4, str1);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(*ret.first(), str3);

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));
    ASSERT_TRUE(obj.erase(k3));
    ASSERT_FALSE(obj.erase(k4));
    ASSERT_TRUE(obj.erase(k5));

    ASSERT_EQ(0, obj.size());
}

TEST(json_object, string_keys) {
    json_element e1(55.6);
    json_element e2(false);
    json_element e3("TYPE_LONG_DOUBLE");

    json_element k1("key1");
    json_element k2("welcome");
    json_element k3("goodbye");

    json_object obj;

    obj.insert(k1, e1);
    obj.insert(k2, e2);
    obj.insert(k3, e3);

    ASSERT_EQ(3, obj.size());

    ASSERT_EQ(e1, *obj.find(k1));
    ASSERT_EQ(e2, *obj.find(k2));
    ASSERT_EQ(e3, *obj.find(k3));

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));
    ASSERT_TRUE(obj.erase(k3));

    ASSERT_EQ(0, obj.size());

    obj.insert(k1, e1);
    obj.insert(k2, e2);

    ASSERT_EQ(2, obj.size());

    auto ret = obj.insert(k1, e3);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(e1, *ret.first());
}

TEST(json_object, move_insert) {
    constexpr auto keystr = "another string filled with numbers";
    constexpr auto valstr = "a string filled with characters";

    json_element val(valstr);
    json_element key(keystr);

    json_object obj;

    ASSERT_TRUE(obj.empty());

    obj.insert(move(key), move(val));

    ASSERT_EQ(json::null, key);
    ASSERT_EQ(json::null, val);

    ASSERT_EQ(1, obj.size());

    key = keystr;
    val = valstr;

    ASSERT_EQ(val, *obj.find(key));
}

