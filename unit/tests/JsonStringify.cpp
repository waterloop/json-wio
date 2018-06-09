#include <gtest/gtest.h>

#include <wlib/Json/JsonStringify.h>
#include <wlib/Json/JsonElement.h>

using namespace wlp;

TEST(json_stringify, primitives) {
    static char buf[256];
    int wrt;

    json_element el_null;
    wrt = json::stringify(buf, el_null);
    ASSERT_STREQ("null", buf);
    ASSERT_EQ(4, wrt);
    ASSERT_EQ('\0', buf[4]);

    json_element el_false(false);
    wrt = json::stringify(buf, el_false);
    ASSERT_STREQ("false", buf);
    ASSERT_EQ(5, wrt);

    json_element el_true(true);
    wrt = json::stringify(buf, el_true);
    ASSERT_STREQ("true", buf);
    ASSERT_EQ(4, wrt);

    json_element el_int(-5848384L);
    wrt = json::stringify(buf, el_int);
    ASSERT_STREQ("-5848384", buf);
    ASSERT_EQ(8, wrt);

    json_element el_float(-664.242342552e-4);
    wrt = json::stringify(buf, el_float);
    ASSERT_STREQ("-0.066424234255200", buf);
    ASSERT_EQ(18, wrt);

    constexpr auto str = "fat pigs and skinny digs";
    json_element el_string(str);
    wrt = json::stringify(buf, el_string);
    ASSERT_STREQ("\"fat pigs and skinny digs\"", buf);
    ASSERT_EQ(strlen(str) + 2, static_cast<unsigned int>(wrt));

}

TEST(json_stringify, empty_array) {
    json_array arr;
    json_element el(move(arr));

    static char buf[16];
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ("[]", buf);
    ASSERT_EQ(2, wrt);    
}

TEST(json_stringify, singleton_array) {
    json_array arr = {"people"};
    ASSERT_EQ("people", arr[0]);
    json_element el(move(arr));

    static char buf[16];
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ("[\"people\"]", buf);
    ASSERT_EQ(10, wrt);
}

TEST(json_stringify, primitive_array) {
    json_array arr = {5, 4, "three", "two", true, false};
    json_element el(move(arr));

    static char buf[32];
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ("[false,true,\"two\",\"three\",4,5]", buf);
    ASSERT_EQ(30, wrt);
}

TEST(json_stringify, empty_object) {
    json_object obj;
    json_element el(move(obj));

    static char buf[4];
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ("{}", buf);
    ASSERT_EQ(2, wrt);
}

TEST(json_stringify, singleton_object) {
    json_object obj;
    obj["key"] = 156;
    json_element el(move(obj));

    static char buf[32];
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ("{\"key\":156}", buf);
    ASSERT_EQ(11, wrt);
}

TEST(json_stringify, primitive_object) {
    json_object obj;
    obj["first"] = false;
    obj["second"] = true;
    obj["third"] = 3;
    obj["fourth"] = -456;
    obj["fifth"] = "hello";
    obj["sixth"] = "goodbye";
    json_element el(move(obj));

    static char buf[512];
    constexpr auto expected = "{\"first\":false,\"second\":true,\"third\":3,\"fourth\":-456,\"fifth\":\"hello\",\"sixth\":\"goodbye\"}";
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ(expected, buf);
}
