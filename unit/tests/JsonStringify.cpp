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
    constexpr auto expected = "{\"first\":false,\"fourth\":-456,\"sixth\":\"goodbye\",\"fifth\":\"hello\",\"second\":true,\"third\":3}";
    int wrt = json::stringify(buf, el);
    ASSERT_STREQ(expected, buf);
    ASSERT_EQ(strlen(expected), static_cast<unsigned int>(wrt));
}

TEST(json_stringify, invalid_key) {
    json_object obj;
    obj[4] = 4;
    json_element el(move(obj));
    static char buf[32];
    ASSERT_EQ(-1, json::stringify(buf, el));
}

TEST(json_stringify, very_nested) {
    json_array random = {1, 2, 3, 4, "hello", "bye"};
    json_array colors = {"blue", "green", "red"};
    json_array pair3 = {3, "three"};
    json_array pair2 = {2, "two"};
    json_array pair1 = {1, "one", true};
    json_array pair0 = {0, "zero", false};
    json_object birth = {
        "day", 25,
        "month", "jan",
        "year", 1998
    };
    json_object desc = {
        "firstname", "jeff",
        "lastname", "niu",
        "birthdate", move(birth)
    };
    json_array pairarr = {
        move(pair3), move(pair2), move(pair1), move(pair0)
    };
    json_object stuff = {
        "pairs", move(pairarr),
        "desc", move(desc),
        "random", move(random),
        "colors", move(colors)
    };
    json_array count0 = {move(stuff), 3, 2, 1};
    json_array count1 = {5, 4, 3, 2, 1};
    json_array count2 = {5, 4, 3, 2, 1};
    json_array count3 = {"five", "three", "one"};
    
    json_object obj1 = {"fire", "hot"};
    json_object obj2 = {"ice", "cold"};
    json_object obj3 = {"water", "lukewarm"};
    json_array objarr = {move(obj3), move(obj2), move(obj1)};
    json_array counts = {move(count0), move(count1), move(count2), move(count3)};

    json_object sigma = {
        "objarr", move(objarr),
        "counts", move(counts)
    };
    json_array single = {move(sigma)};

    json_element element(move(single));

    static char buf[2048];
    int wrt = json::stringify(buf, element);
    constexpr auto expected =
    "[{\"counts\":[[\"one\",\"three\",\"five\"],[1,2,3,4,5],[1,2,3,4,5],"
    "[1,2,3,{\"colors\":[\"red\",\"green\",\"blue\"],\"desc\":{\"birthda"
    "te\":{\"month\":\"jan\",\"day\":25,\"year\":1998},\"lastname\":\"ni"
    "u\",\"firstname\":\"jeff\"},\"pairs\":[[false,\"zero\",0],[true,\"o"
    "ne\",1],[\"two\",2],[\"three\",3]],\"random\":[\"bye\",\"hello\",4,"
    "3,2,1]}]],\"objarr\":[{\"fire\":\"hot\"},{\"ice\":\"cold\"},{\"wate"
    "r\":\"lukewarm\"}]}]";
    ASSERT_STREQ(expected, buf);
    ASSERT_EQ(wrt, json::buff_size(element));
}
